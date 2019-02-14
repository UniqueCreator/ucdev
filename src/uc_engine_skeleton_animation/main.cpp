#include "pch.h"

#include <memory>
#include <ppltasks.h>
#include <ppl.h>
#include <winrt/base.h>

#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.ApplicationModel.h>
#include <winrt/Windows.ApplicationModel.Core.h>
#include <winrt/Windows.ApplicationModel.Activation.h>
#include <winrt/Windows.Graphics.Display.h>
#include <winrt/Windows.UI.Core.h>
#include <winrt/Windows.UI.ViewManagement.h>

#include <uc_public/io/io.h>
#include <uc_public/graphics/graphics.h>
#include <uc_public/graphics/geometry/geometry.h>
#include <uc_public/assets/graphics/graphics.h>
#include <uc_public/assets/animations/animations.h>
#include <uc_public/animations/animations.h>
#include <uc_public/math/graphics.h>
#include <uc_public/gpu/default_graphics_signature.h>

#include <uc_public/system/system.h>

#include <uc_public/render/pinhole_camera.h>
#include <uc_public/render/skinned_render_object.h>
#include <uc_public/render/skinned_render_object_factory.h>

#include <autogen/shaders/textured_skinned_graphics.h>




using namespace winrt::Windows::ApplicationModel;
using namespace winrt::Windows::ApplicationModel::Core;
using namespace winrt::Windows::ApplicationModel::Activation;
using namespace winrt::Windows::UI::Core;
using namespace winrt::Windows::UI::ViewManagement;
using namespace winrt::Windows::Graphics::Display;

using namespace UniqueCreator;

struct alignas(16) SkinnedModel
{
    Simd::Float4x4A                   m_Model;
    std::array<Simd::Float4x4A, 127>  m_JointsPalette;
};

struct alignas(16) FrameConstants
{
    Simd::Float4x4A                   m_view;
    Simd::Float4x4A                   m_perspective;
};

class ViewProvider : public winrt::implements<ViewProvider, IFrameworkView, IFrameworkViewSource>
{

public:

    ViewProvider() 
    {
        
    }

    IFrameworkView CreateView()
    {
        return *this;
    }

    // IFrameworkView methods
    virtual void Initialize( const CoreApplicationView& view)
    {
        m_activated = view.Activated(winrt::auto_revoke, { this, &ViewProvider::OnActivated });

#if defined(_DEBUG)
        ApplicationView::PreferredLaunchViewSize(winrt::Windows::Foundation::Size(1600, 900));
        ApplicationView::PreferredLaunchWindowingMode(ApplicationViewWindowingMode::PreferredLaunchViewSize);
#else
        ApplicationView::PreferredLaunchWindowingMode(ApplicationViewWindowingMode::FullScreen);
#endif
        m_resource_create_context = std::make_unique<Graphics::ResourceCreateContext>();
        m_geometry = std::make_unique<Graphics::SkinnedGeometryFactory>(m_resource_create_context.get(), 1000000);
        m_indices = std::make_unique<Graphics::IndexedGeometryFactory>(m_resource_create_context.get(), 1000000);
    }

    virtual void Uninitialize()
    {

    }

    virtual void SetWindow( const CoreWindow& window)
    {
        auto w = window;
        auto d = DisplayInformation::GetForCurrentView();

        m_mouse = std::make_unique<InputOutput::Mouse>(InputOutput::CreateMouseParams{ w, 96.0f });
        m_keyboard = std::make_unique<InputOutput::Keyboard>(InputOutput::CreateKeyboardParams{ w });
        m_game_pad = std::make_unique<InputOutput::GamePad>();

        m_background_swap_chain = std::make_unique<Graphics::WindowSwapChainResources>(m_resource_create_context.get(), w, d);
        m_overlay_swap_chain = std::make_unique<Graphics::WindowOverlaySwapChainResources>(m_resource_create_context.get(), w, d);

        m_device_resources = std::make_unique<Graphics::DeviceResources>(m_resource_create_context.get(), m_background_swap_chain.get(), m_overlay_swap_chain.get());
        m_depth_buffer = m_resource_create_context->CreateViewDepthBuffer(m_background_swap_chain->GetBackBuffer()->GetSize2D(), Graphics::DepthBufferFormat::Depth32Float);

        m_camera->SetPosition(Simd::Set(0.0, 0.0f, -5.5f, 0.0f));
        m_camera->SetFarPlane(1200.0f);

        m_size_changed = window.SizeChanged(winrt::auto_revoke, { this, &ViewProvider::OnWindowSizeChanged });
        m_visibility_changed = window.VisibilityChanged(winrt::auto_revoke, { this, &ViewProvider::OnVisibilityChanged });
        m_closed = window.Closed(winrt::auto_revoke, { this, &ViewProvider::OnWindowClosed });

        DisplayInformation currentDisplayInformation = DisplayInformation::GetForCurrentView();

        m_dpi_changed = currentDisplayInformation.DpiChanged(winrt::auto_revoke, { this, &ViewProvider::OnDpiChanged });
        m_orientation_changed = currentDisplayInformation.OrientationChanged(winrt::auto_revoke, { this, &ViewProvider::OnOrientationChanged });
        m_display_contents_invalidated = currentDisplayInformation.DisplayContentsInvalidated(winrt::auto_revoke, { this, &ViewProvider::OnDisplayContentsInvalidated });
    }

    virtual void Load( winrt::hstring)
    {
        concurrency::task_group g;

        g.run([this]
        {
            m_skinned_pso = Graphics::PipelineStates::textured_skinned_graphics::Create(m_resource_create_context.get());
        });

        g.run([this]
        {
            //Geometry and materials
            auto asset_factory = std::make_unique<Assets::SkinnedModelFactory>();
            auto asset = asset_factory->CreateFromFile(L"appdata/meshes/military_mechanic.derivatives_skinned_model.model");
            auto render_object_factory = std::make_unique<Render::SkinnedRenderObjectFactory>();
            m_model = render_object_factory->Create(asset.get(), m_geometry.get(), m_indices.get(), m_device_resources->GetUploadQueue(), m_device_resources->GetResourceCreateContext());

        });

        g.run([this]
        {
            concurrency::task_group g0;

            g0.run([this]
            {
                auto animation_factory = std::make_unique<Assets::JointAnimationsFactory>();
                m_animation = animation_factory->CreateFromFile(L"appdata/animations/military_mechanic.animation");
            });

            g0.run([this]
            {
                auto skeleton_factory = std::make_unique<Assets::SkeletonFactory>();
                m_skeleton = skeleton_factory->CreateFromFile(L"appdata/skeletons/military_mechanic.skeleton");
            });

            g0.wait();

            auto skeleton_instance_factory = std::make_unique<Animations::SkeletonInstanceFactory>();
            m_skeleton_instance = skeleton_instance_factory->Create(m_skeleton.get());

            auto animation_instance_factory = std::make_unique<Animations::JointAnimationInstanceFactory>();
            m_animation_instance = animation_instance_factory->Create(m_animation.get(), m_skeleton.get(), 0.0f);
        });

        g.run([this]
        {
            auto timer_factory = std::make_unique<System::TimerFactory>();
            m_timer = timer_factory->Create();
        });

        g.wait();

    }

    virtual void Run()
    {
        while (true)
        {
            CoreWindow::GetForCurrentThread().Dispatcher().ProcessEvents(CoreProcessEventsOption::ProcessAllIfPresent);

            //Simulation
            {
                //clamp the simulation time if we are resuming from the debugger
                double frame_time = std::max(0.0, std::min(m_timer->Seconds(), 1.0 / 33.0));
                m_timer->Reset();

                //play stopped animation in this way
                if ( false )
                {
                    frame_time = 0.05f;

                    //Reset the local bone transforms
                    m_skeleton_instance->Reset();
                    //Reset the animation
                    m_animation_instance->Reset();
                }

                //accumulate the animation transforms for every bone
                m_animation_instance->Accumulate(m_skeleton_instance.get(), frame_time);

                //concatenate transforms into hierarchy
                m_skeleton_instance->ConcatenateLocalTransforms(Simd::Identity());

            }

            //Sampling the mouse
            {
                using namespace InputOutput;

                auto s = m_mouse->Sample();
                if (MouseButtonStateHelper::ButtonWasPressed(m_mouse_state.m_Buttons, s.m_Buttons, MouseButton::Left))
                {

                }

                if (MouseButtonStateHelper::ButtonWasReleased(m_mouse_state.m_Buttons, s.m_Buttons, MouseButton::Left))
                {
                    //do code here with the mouse released
                }

                //remember the old state, so we can query the difference
                m_mouse_state = s;
            }

            {
                using namespace InputOutput;
                auto s = m_keyboard->Sample();

                if (KeyboardStateHelper::ButtonWasPressed(m_keyboard_state.m_State, s.m_State, KeyboardButton::Virtual_Key_A))
                {
                }

                if (KeyboardStateHelper::ButtonWasReleased(m_keyboard_state.m_State, s.m_State, KeyboardButton::Virtual_Key_A))
                {
                }

                m_keyboard_state = s;
            }

            {
                using namespace InputOutput;

                auto s = m_game_pad->Sample();
                if (GamePadStateHelper::ButtonWasPressed(m_game_pad_state.m_Buttons, s.m_Buttons, GamePadButton::Button_A))
                {
                    //do code here with the pad pressed
                }

                if (GamePadStateHelper::ButtonWasReleased(m_game_pad_state.m_Buttons, s.m_Buttons, GamePadButton::Button_A))
                {
                    //do code here with the pad released
                }
            }


            //Render
            {

                {
                    //let the rendering queue waits if any resources are uploaded, before it can use them
                    m_background_swap_chain->InsertWaitOn(m_device_resources->GetUploadQueue()->Flush().get());
                }

                {
                    std::unique_ptr<Graphics::DirectCommandContext> ctx = m_background_swap_chain->CreateCommandContext();
                    const auto backBuffer = m_background_swap_chain->GetBackBuffer();
                    const auto depthBuffer = m_depth_buffer.get();
                    ctx->TransitionResource(backBuffer, Graphics::ResourceState::Present, Graphics::ResourceState::RenderTarget);
                    ctx->TransitionResource(m_depth_buffer.get(), Graphics::ResourceState::Common, Graphics::ResourceState::DepthWrite);

                    ctx->Clear(backBuffer);
                    ctx->Clear(depthBuffer, 1.0f);

                    ctx->SetRenderTarget(backBuffer, depthBuffer);
                    ctx->SetDescriptorHeaps();

                    Graphics::ViewPort v;
                    auto size = backBuffer->GetSize2D();
                    v.m_topLeftX = 0;
                    v.m_topLeftY = 0;
                    v.m_width = size.m_width;
                    v.m_height = size.m_height;
                    v.m_maxDepth = 1.0f;
                    v.m_minDepth = 0.0f;

                    Graphics::Rectangle2D r;
                    r.m_left = 0;
                    r.m_right = size.m_width;
                    r.m_top = 0;
                    r.m_bottom = size.m_height;

                    ctx->SetViewPort(v);
                    ctx->SetScissorRectangle(r);

                    //geometry
                    ctx->SetPipelineStateObject(m_skinned_pso.get());

                    ctx->SetPrimitiveTopology(Graphics::PrimitiveTopology::TriangleList);
                    ctx->SetVertexBuffer(Graphics::VertexSlot::Zero, m_geometry->GetPositionsView());
                    ctx->SetVertexBuffer(Graphics::VertexSlot::One, m_geometry->GetUVView());
                    ctx->SetVertexBuffer(Graphics::VertexSlot::Two, m_geometry->GetBlendWeightsView());
                    ctx->SetVertexBuffer(Graphics::VertexSlot::Three, m_geometry->GetBlendIndicesView());
                    ctx->SetIndexBuffer(m_indices->GetIndicesView());

                    //Per many draw calls  -> frequency 1
                    {
                        FrameConstants frame;
                        Simd::Matrix4x4 view = Simd::Transpose(Render::PinholeCameraUtils::ViewTransform(m_camera.get()).Value());
                        Simd::Matrix4x4 perspective = Simd::Transpose(Render::PinholeCameraUtils::PerspectiveTransform(m_camera.get()).Value());

                        Simd::Store(&frame.m_view, view);
                        Simd::Store(&frame.m_perspective, perspective);
                        ctx->SetConstantBuffer(Gpu::DefaultRootSignature::ConstantBuffer0, &frame, sizeof(frame));
                    }

                    //Draw call -> frequency 2 ( nvidia take care these should be on a sub 1 ms granularity)
                    {
                        //multiply with the inverse bind pose to be ready for skinning
                        SkinnedModel m;
                        auto&& transforms = m_skeleton_instance->GetConcatenatedTransforms();
                        auto&& poses = m_skeleton->GetInverseBindPose();

                        using namespace Mathematics;

                        Store(&m.m_Model, Simd::Identity());

                        for (auto i = 0U; i < transforms.m_transforms_size; ++i)
                        {
                            using namespace Simd;
                            Matrix4x4 transform = Simd::Load(&transforms.m_transforms[i]);
                            Matrix4x4 pose = Simd::Load(&poses.m_transforms[i]);

                            Store(&m.m_JointsPalette[i], Transpose(Mul(pose, transform)));
                        }

                        ctx->SetDynamicConstantBuffer(Gpu::DefaultRootSignature::ConstantBuffer1, 0, &m, sizeof(m));
                        //Draw as one draw call for the whole model (suitable for depth prepass)

                        {
                            //auto geo = m_model->GetGeometry();
                            //auto ind = m_model->GetPrimitives();
                            //ctx->DrawIndexed(ind->GetIndexCount(), ind->GetIndexOffset(), geo->GetVertexOffset());
                        }

                        
                        auto primitives = m_model->GetPrimitiveRanges();
                        auto geo        = m_model->GetGeometry();
                        auto ind        = m_model->GetPrimitives();

                        auto&& textures = m_model->GetAlbedoTextures();

                        for (auto i = 0U; i < primitives.m_ranges_size; ++i)
                        {
                            auto range = primitives.m_ranges[i];

                            //Gpu::DefaultRootSignature::SharedResourceView1;
                            //ctx->SetDynamicDescriptor(gx::dx12::default_root_singature::slots::srv_1, t->srv());
                            auto&& t = textures.m_textures[i];
                            
                            ctx->SetDynamicDescriptor(Gpu::DefaultRootSignature::SharedResourceView1, t->GetShaderResourceView(), 0 );
                            ctx->DrawIndexed( range.m_end - range.m_begin, ind->GetIndexOffset() + range.m_begin, geo->GetVertexOffset());
                        }
                    }

                    ctx->TransitionResource(m_depth_buffer.get(), Graphics::ResourceState::DepthWrite, Graphics::ResourceState::Common);
                    ctx->TransitionResource(backBuffer, Graphics::ResourceState::RenderTarget, Graphics::ResourceState::Present);

                    //submit uploaded constants
                    m_background_swap_chain->InsertWaitOn(m_device_resources->GetUploadQueue()->Flush().get());

                    ctx->Submit();
                }

                m_background_swap_chain->Present();
                m_overlay_swap_chain->Present();

                //Sync point for all resources once per frame

                //Wait for the gpu to complete previous frames queues
                m_background_swap_chain->MoveToNextFrame();
                m_overlay_swap_chain->MoveToNextFrame();

                //flush copy and compute queues
                m_device_resources->Sync();
                
                //flush resources deleted this frame
                m_resource_create_context->Sync();
                m_geometry->Sync();
                m_indices->Sync();

                //swap buffers
                m_background_swap_chain->Sync();
                m_overlay_swap_chain->Sync();

            }
        }
    }

protected:
    // Event handlers
    void OnActivated(const CoreApplicationView&, const IActivatedEventArgs& )
    {
        /*
        concurrency::task_group g;

        g.run([this]
        {
            m_skinned_pso = Graphics::PipelineStates::textured_skinned::Create(m_resource_create_context.get());
        });

        g.run([this]
        {
            //Geometry and materials
            auto asset_factory          = std::make_unique<Assets::SkinnedModelFactory>();
            auto asset                  = asset_factory->CreateFromFile(L"appdata/meshes/military_mechanic.skinned.model");
            auto render_object_factory  = std::make_unique<Render::SkinnedRenderObjectFactory>();
            m_model                     = render_object_factory->Create(asset.get(), m_geometry.get(), m_indices.get(), m_device_resources->GetUploadQueue(), m_device_resources->GetResourceCreateContext() );

        });

        g.run([this]
        {
            concurrency::task_group g0;

            g0.run([this]
            {
                auto animation_factory = std::make_unique<Assets::JointAnimationsFactory>();
                m_animation = animation_factory->CreateFromFile(L"appdata/animations/military_mechanic.animation");
            });

            g0.run([this]
            {
                auto skeleton_factory = std::make_unique<Assets::SkeletonFactory>();
                m_skeleton = skeleton_factory->CreateFromFile(L"appdata/skeletons/military_mechanic.skeleton");
            });

            g0.wait();

            auto skeleton_instance_factory = std::make_unique<Animations::SkeletonInstanceFactory>();
            m_skeleton_instance = skeleton_instance_factory->Create(m_skeleton.get());

            auto animation_instance_factory = std::make_unique<Animations::JointAnimationInstanceFactory>();
            m_animation_instance = animation_instance_factory->Create(m_animation.get(), m_skeleton.get(), 0.0f);
        });

        g.run([this]
        {            
            auto timer_factory = std::make_unique<System::TimerFactory>();
            m_timer = timer_factory->Create();
        });

        g.wait();
        */

        auto w = CoreWindow::GetForCurrentThread();
        w.Activate();
    }

    void OnSuspending( const winrt::Windows::Foundation::IInspectable&, const SuspendingEventArgs& args)
    {
        auto deferral = args.SuspendingOperation().GetDeferral();

        Concurrency::create_task([this, deferral]()
        {
            deferral.Complete();
        });
    }

    void OnResuming(const winrt::Windows::Foundation::IInspectable&, const winrt::Windows::Foundation::IInspectable&)
    {

    }

    inline Graphics::Size2D size2d(winrt::Windows::Foundation::Size s)
    {
        return Graphics::Size2D { s.Width, s.Height };
    }

    void OnWindowSizeChanged( const CoreWindow&, const WindowSizeChangedEventArgs& args)
    {
        //stall and do not submit more work
        m_device_resources->WaitForGpu();
        m_background_swap_chain->WaitForGpu();
        m_overlay_swap_chain->WaitForGpu();

        //Reset view dependent heaps
        m_depth_buffer.reset();
        m_device_resources->ResetViewDependentResources();

        //Recreate the resources and buffers
        m_background_swap_chain->SetLogicalSize(size2d(args.Size()));
        m_overlay_swap_chain->SetLogicalSize(size2d(args.Size()));
        m_depth_buffer = m_resource_create_context->CreateViewDepthBuffer(m_background_swap_chain->GetBackBuffer()->GetSize2D(), Graphics::DepthBufferFormat::Depth32Float);
    }

    void OnWindowClosed(const CoreWindow&, const CoreWindowEventArgs&)
    {
        
    }

    void OnVisibilityChanged(const CoreWindow& , const VisibilityChangedEventArgs&)
    {

    }

    // DisplayInformation event handlers.
    void OnDpiChanged(const DisplayInformation& d, const winrt::Windows::Foundation::IInspectable&)
    {
        //stall and do not submit more work
        m_device_resources->WaitForGpu();
        m_background_swap_chain->WaitForGpu();
        m_overlay_swap_chain->WaitForGpu();

        //Reset view dependent heaps
        m_depth_buffer.reset();
        m_device_resources->ResetViewDependentResources();

        //Recreate the resources and buffers
        m_background_swap_chain->SetDisplayInformation(d);
        m_overlay_swap_chain->SetDisplayInformation(d);
        m_depth_buffer = m_resource_create_context->CreateViewDepthBuffer(m_background_swap_chain->GetBackBuffer()->GetSize2D(), Graphics::DepthBufferFormat::Depth32Float);
    }

    void OnOrientationChanged(const DisplayInformation& d, const winrt::Windows::Foundation::IInspectable&)
    {
        //stall and do not submit more work
        m_device_resources->WaitForGpu();
        m_background_swap_chain->WaitForGpu();
        m_overlay_swap_chain->WaitForGpu();

        //Reset view dependent heaps
        m_depth_buffer.reset();
        m_device_resources->ResetViewDependentResources();

        //Recreate the resources and buffers
        m_background_swap_chain->SetDisplayInformation(d);
        m_overlay_swap_chain->SetDisplayInformation(d);
        m_depth_buffer = m_resource_create_context->CreateViewDepthBuffer(m_background_swap_chain->GetBackBuffer()->GetSize2D(), Graphics::DepthBufferFormat::Depth32Float);
    }

    void OnDisplayContentsInvalidated(const DisplayInformation&, const winrt::Windows::Foundation::IInspectable&)
    {
        
    }

private:

    CoreApplicationView::Activated_revoker                                          m_activated;

    CoreWindow::SizeChanged_revoker                                                 m_size_changed;
    CoreWindow::VisibilityChanged_revoker                                           m_visibility_changed;
    CoreWindow::Closed_revoker                                                      m_closed;
    
    DisplayInformation::DpiChanged_revoker                                          m_dpi_changed;
    DisplayInformation::OrientationChanged_revoker                                  m_orientation_changed;
    DisplayInformation::DisplayContentsInvalidated_revoker                          m_display_contents_invalidated;

    std::unique_ptr<Graphics::ResourceCreateContext>                 m_resource_create_context;
    std::unique_ptr<Graphics::WindowSwapChainResources>              m_background_swap_chain;
    std::unique_ptr<Graphics::WindowOverlaySwapChainResources>       m_overlay_swap_chain;
    
    std::unique_ptr<Graphics::DeviceResources>                       m_device_resources;
   
    std::unique_ptr<InputOutput::Mouse>                              m_mouse;
    std::unique_ptr<InputOutput::GamePad>                            m_game_pad;
    std::unique_ptr<InputOutput::Keyboard>                           m_keyboard;

    InputOutput::MouseState                                          m_mouse_state;
    InputOutput::KeyboardState                                       m_keyboard_state;
    InputOutput::GamePadState                                        m_game_pad_state;

    //view dependent buffers
    std::unique_ptr<Graphics::ViewDepthBuffer>                       m_depth_buffer;

    //Pipeline state objects
    std::unique_ptr<Graphics::PipelineStates::textured_skinned_graphics::GraphicsPipelineState>   m_skinned_pso;

    //Geometry and materials

    std::unique_ptr<Graphics::SkinnedGeometryFactory>   m_geometry;
    std::unique_ptr<Graphics::IndexedGeometryFactory>   m_indices;

    std::unique_ptr<Render::SkinnedRenderObject>        m_model;

    //Animations
    std::unique_ptr<System::Timer>                      m_timer;
    
    //assets
    std::unique_ptr<Assets::JointAnimations>            m_animation;
    std::unique_ptr<Assets::Skeleton>                   m_skeleton;

    //running state
    std::unique_ptr<Animations::JointAnimationInstance> m_animation_instance;
    std::unique_ptr<Animations::SkeletonInstance>       m_skeleton_instance;

    //camera

    std::unique_ptr<Render::PinholeCamera>              m_camera = std::make_unique<Render::PinholeCamera>();

};


#pragma warning( disable:4447 )
int32_t __stdcall wWinMain(HINSTANCE, HINSTANCE, PWSTR, int32_t)
{
    CoInitializeEx(nullptr, COINIT_MULTITHREADED);
    CoreApplication::Run(ViewProvider());
    return 0;
}




