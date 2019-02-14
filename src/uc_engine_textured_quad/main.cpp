#include "pch.h"

#include <memory>
#include <ppltasks.h>
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
#include <uc_public/imaging/imaging.h>
#include <uc_public/assets/assets.h>

#include <autogen/shaders/full_screen_graphics.h>




using namespace winrt::Windows::ApplicationModel;
using namespace winrt::Windows::ApplicationModel::Core;
using namespace winrt::Windows::ApplicationModel::Activation;
using namespace winrt::Windows::UI::Core;
using namespace winrt::Windows::UI::ViewManagement;
using namespace winrt::Windows::Graphics::Display;

using namespace UniqueCreator;


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
        m_full_screen_main = Graphics::PipelineStates::full_screen_graphics::Create(m_resource_create_context.get());


        // 1. create checkerboard procedural image
        {
            auto factory = std::make_unique<Imaging::ImageFactory>();
            auto checkerBoard = factory->CreateCheckerBoardImage(32, 32);
            auto queue = m_device_resources->GetUploadQueue();
            auto desc = checkerBoard->GetDescription();
            Graphics::SubresourceData s = { checkerBoard->GetData(), desc.m_row_pitch, desc.m_slice_pitch };
            m_texture0 = m_resource_create_context->CreateTexture2D({ static_cast<float>(desc.m_width), static_cast<float>(desc.m_height) }, Graphics::GraphicsFormat::R8G8B8A8_UNORM);
            queue->UploadTexture(m_texture0.get(), &s, 0, 1);
        }

        // 2. Load image from a known file format like png or jpg
        {
            auto factory = std::make_unique<Imaging::ImageFactory>();
            auto checkerBoard = factory->CreateFromFile(L"appdata/images/image_0.png");
            auto queue = m_device_resources->GetUploadQueue();
            auto desc = checkerBoard->GetDescription();
            Graphics::SubresourceData s = { checkerBoard->GetData(), desc.m_row_pitch, desc.m_slice_pitch };
            m_texture1 = m_resource_create_context->CreateTexture2D({ static_cast<float>(desc.m_width), static_cast<float>(desc.m_height) }, Graphics::GraphicsFormat::R8G8B8A8_UNORM);
            queue->UploadTexture(m_texture1.get(), &s, 0, 1);
        }

        // 3. Load packaged binarized compressed image 
        {
            auto factory = std::make_unique<Assets::Texture2DFactory>();
            auto checkerBoard = factory->CreateFromFile(L"appdata/textures/bear.texture");
            auto queue = m_device_resources->GetUploadQueue();
            auto desc = checkerBoard->GetDescription();
            auto graphicsFormat = static_cast<Graphics::GraphicsFormat>(desc.m_view_format);
            auto pitch = Graphics::TextureFormatUtils::GetRowSlicePitch(graphicsFormat, desc.m_width, desc.m_height);

            Graphics::SubresourceData s = { checkerBoard->GetData(), pitch.m_RowPitch, pitch.m_SlicePitch };

            m_texture2 = m_resource_create_context->CreateTexture2D({ static_cast<float>(desc.m_width), static_cast<float>(desc.m_height) }, graphicsFormat);
            queue->UploadTexture(m_texture2.get(), &s, 0, 1);
        }

        m_texture = m_texture0.get();

    }

    virtual void Run()
    {
        while (true)
        {
            CoreWindow::GetForCurrentThread().Dispatcher().ProcessEvents(CoreProcessEventsOption::ProcessAllIfPresent);


            //Sampling the mouse
            {
                using namespace InputOutput;

                auto s = m_mouse->Sample();
                if (MouseButtonStateHelper::ButtonWasPressed(m_mouse_state.m_Buttons, s.m_Buttons, MouseButton::Left))
                {
                    //do code here with the mouse pressed

                    Graphics::Texture2D* texture = nullptr;

                    if (m_texture == m_texture0.get())
                    {
                        texture = m_texture1.get();
                    }

                    if (m_texture == m_texture1.get())
                    {
                        texture = m_texture2.get();
                    }

                    if (m_texture == m_texture2.get())
                    {
                        texture = m_texture0.get();
                    }

                    m_texture = texture;
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

                std::unique_ptr<Graphics::DirectCommandContext> ctx = m_background_swap_chain->CreateCommandContext();
                const auto backBuffer = m_background_swap_chain->GetBackBuffer();
                ctx->TransitionResource(backBuffer, Graphics::ResourceState::Present, Graphics::ResourceState::RenderTarget);
                ctx->Clear(backBuffer);

                ctx->SetRenderTarget(backBuffer);
                ctx->SetDescriptorHeaps();

                Graphics::ViewPort v;
                auto size = backBuffer->GetSize2D();
                v.m_topLeftX    = 0;
                v.m_topLeftY    = 0;
                v.m_width       = size.m_width;
                v.m_height      = size.m_height;
                v.m_maxDepth    = 1.0f;
                v.m_minDepth    = 0.0f;

                Graphics::Rectangle2D r;
                r.m_left    = 0;
                r.m_right   = size.m_width;
                r.m_top     = 0;
                r.m_bottom  = size.m_height;


                ctx->SetViewPort(v);
                ctx->SetScissorRectangle(r);

                //Per many draw calls  -> frequency 1
                ctx->SetPrimitiveTopology(Graphics::PrimitiveTopology::TriangleList);
                ctx->SetPipelineStateObject(m_full_screen_main.get());
                ctx->SetDynamicDescriptor(5, m_texture->GetShaderResourceView(), 0);
                ctx->Draw(3, 0);

                ctx->TransitionResource(backBuffer, Graphics::ResourceState::RenderTarget, Graphics::ResourceState::Present);


                //submit uploaded constants
                m_background_swap_chain->InsertWaitOn(m_device_resources->GetUploadQueue()->Flush().get());

                ctx->Submit();

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

                //swap buffers
                m_background_swap_chain->Sync();
                m_overlay_swap_chain->Sync();
            }
        }
    }

protected:
    // Event handlers
    void OnActivated(const CoreApplicationView&, const IActivatedEventArgs&)
    {
        using namespace UniqueCreator;

        auto w = CoreWindow::GetForCurrentThread();
        auto d = DisplayInformation::GetForCurrentView();

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
    std::unique_ptr<Graphics::PipelineStates::full_screen_graphics::GraphicsPipelineState>      m_full_screen_main;

    std::unique_ptr<Graphics::Texture2D>                             m_texture0;
    std::unique_ptr<Graphics::Texture2D>                             m_texture1;
    std::unique_ptr<Graphics::Texture2D>                             m_texture2;

    Graphics::Texture2D*                                             m_texture;
};

#pragma warning( disable:4447 )
int32_t __stdcall wWinMain(HINSTANCE, HINSTANCE, PWSTR, int32_t)
{
    CoInitializeEx(nullptr, COINIT_MULTITHREADED);
    CoreApplication::Run(ViewProvider());
    return 0;
}




