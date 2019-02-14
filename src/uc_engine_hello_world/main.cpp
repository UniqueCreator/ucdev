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

#include <autogen/shaders/full_screen_graphics.h>




using namespace winrt::Windows::ApplicationModel;
using namespace winrt::Windows::ApplicationModel::Activation;
using namespace winrt::Windows::UI::ViewManagement;
using namespace winrt::Windows::Graphics::Display;


class ViewProvider : public winrt::implements<ViewProvider, winrt::Windows::ApplicationModel::Core::IFrameworkView, winrt::Windows::ApplicationModel::Core::IFrameworkViewSource>
{

public:

    ViewProvider() 
    {
        
    }

    winrt::Windows::ApplicationModel::Core::IFrameworkView CreateView()
    {
        return *this;
    }

    // IFrameworkView methods
    virtual void Initialize( const winrt::Windows::ApplicationModel::Core::CoreApplicationView& view)
    {
        m_activated = view.Activated(winrt::auto_revoke, { this, &ViewProvider::OnActivated });

#if defined(_DEBUG)
        ApplicationView::PreferredLaunchViewSize(winrt::Windows::Foundation::Size(1600, 900));
        ApplicationView::PreferredLaunchWindowingMode(ApplicationViewWindowingMode::PreferredLaunchViewSize);
#else
        ApplicationView::PreferredLaunchWindowingMode(ApplicationViewWindowingMode::FullScreen);
#endif

        m_resource_create_context = std::make_unique<UniqueCreator::Graphics::ResourceCreateContext>();
    }

    virtual void Uninitialize()
    {

    }

    virtual void SetWindow( const winrt::Windows::UI::Core::CoreWindow& window)
    {
        auto w = window;
        auto d = DisplayInformation::GetForCurrentView();

        m_mouse = std::make_unique<UniqueCreator::InputOutput::Mouse>(UniqueCreator::InputOutput::CreateMouseParams{ w, 96.0f });
        m_keyboard = std::make_unique<UniqueCreator::InputOutput::Keyboard>(UniqueCreator::InputOutput::CreateKeyboardParams{ w });
        m_game_pad = std::make_unique<UniqueCreator::InputOutput::GamePad>();
        m_background_swap_chain = std::make_unique<UniqueCreator::Graphics::WindowSwapChainResources>(m_resource_create_context.get(), w, d);
        m_overlay_swap_chain = std::make_unique<UniqueCreator::Graphics::WindowOverlaySwapChainResources>(m_resource_create_context.get(), w, d);
        m_device_resources = std::make_unique<UniqueCreator::Graphics::DeviceResources>(m_resource_create_context.get(), m_background_swap_chain.get(), m_overlay_swap_chain.get());

        m_depth_buffer = m_resource_create_context->CreateViewDepthBuffer(m_background_swap_chain->GetBackBuffer()->GetSize2D(), UniqueCreator::Graphics::DepthBufferFormat::Depth32Float);

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
        m_full_screen_main = UniqueCreator::Graphics::PipelineStates::full_screen_graphics::Create(m_resource_create_context.get());
    }

    virtual void Run()
    {
        while (true)
        {
            winrt::Windows::UI::Core::CoreWindow::GetForCurrentThread().Dispatcher().ProcessEvents(winrt::Windows::UI::Core::CoreProcessEventsOption::ProcessAllIfPresent);


            //Sampling the mouse
            {
                using namespace UniqueCreator::InputOutput;

                auto s = m_mouse->Sample();
                if (MouseButtonStateHelper::ButtonWasPressed(m_mouse_state.m_Buttons, s.m_Buttons, MouseButton::Left))
                {
                    //do code here with the mouse pressed
                }

                if (MouseButtonStateHelper::ButtonWasReleased(m_mouse_state.m_Buttons, s.m_Buttons, MouseButton::Left))
                {
                    //do code here with the mouse released
                }

                //remember the old state, so we can query the difference
                m_mouse_state = s;
            }

            {
                using namespace UniqueCreator::InputOutput;
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
                using namespace UniqueCreator::InputOutput;

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

            {
                std::unique_ptr<UniqueCreator::Graphics::DirectCommandContext> ctx = m_background_swap_chain->CreateCommandContext();

                const auto backBuffer = m_background_swap_chain->GetBackBuffer();
                ctx->TransitionResource(backBuffer, UniqueCreator::Graphics::ResourceState::Present, UniqueCreator::Graphics::ResourceState::RenderTarget);
                ctx->Clear(backBuffer);

                ctx->SetRenderTarget(backBuffer);
                ctx->SetDescriptorHeaps();

                UniqueCreator::Graphics::ViewPort v;
                auto size = backBuffer->GetSize2D();
                v.m_topLeftX    = 0;
                v.m_topLeftY    = 0;
                v.m_width       = size.m_width;
                v.m_height      = size.m_height;
                v.m_maxDepth    = 1.0f;
                v.m_minDepth    = 0.0f;

                UniqueCreator::Graphics::Rectangle2D r;
                r.m_left    = 0;
                r.m_right   = size.m_width;
                r.m_top     = 0;
                r.m_bottom  = size.m_height;


                ctx->SetViewPort(v);
                ctx->SetScissorRectangle(r);

                //Per many draw calls  -> frequency 1
                ctx->SetPrimitiveTopology(UniqueCreator::Graphics::PrimitiveTopology::TriangleList);
                ctx->SetPipelineStateObject(m_full_screen_main.get());
                ctx->Draw(3, 0);

                ctx->TransitionResource(backBuffer, UniqueCreator::Graphics::ResourceState::RenderTarget, UniqueCreator::Graphics::ResourceState::Present);
                
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
    void OnActivated(const winrt::Windows::ApplicationModel::Core::CoreApplicationView&, const IActivatedEventArgs&)
    {
        auto w = winrt::Windows::UI::Core::CoreWindow::GetForCurrentThread();
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

    inline UniqueCreator::Graphics::Size2D size2d(winrt::Windows::Foundation::Size s)
    {
        return UniqueCreator::Graphics::Size2D { s.Width, s.Height };
    }

    void OnWindowSizeChanged( const winrt::Windows::UI::Core::CoreWindow&, const winrt::Windows::UI::Core::WindowSizeChangedEventArgs& args)
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
        m_depth_buffer = m_resource_create_context->CreateViewDepthBuffer(m_background_swap_chain->GetBackBuffer()->GetSize2D(), UniqueCreator::Graphics::DepthBufferFormat::Depth32Float);
    }

    void OnWindowClosed(const winrt::Windows::UI::Core::CoreWindow&, const winrt::Windows::UI::Core::CoreWindowEventArgs&)
    {
        
    }

    void OnVisibilityChanged(const winrt::Windows::UI::Core::CoreWindow& , const winrt::Windows::UI::Core::VisibilityChangedEventArgs&)
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
        m_depth_buffer = m_resource_create_context->CreateViewDepthBuffer(m_background_swap_chain->GetBackBuffer()->GetSize2D(), UniqueCreator::Graphics::DepthBufferFormat::Depth32Float);
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
        m_depth_buffer = m_resource_create_context->CreateViewDepthBuffer(m_background_swap_chain->GetBackBuffer()->GetSize2D(), UniqueCreator::Graphics::DepthBufferFormat::Depth32Float);
    }

    void OnDisplayContentsInvalidated(const DisplayInformation&, const winrt::Windows::Foundation::IInspectable&)
    {
        
    }

private:

    winrt::Windows::ApplicationModel::Core::CoreApplicationView::Activated_revoker  m_activated;

    winrt::Windows::UI::Core::CoreWindow::SizeChanged_revoker                       m_size_changed;
    winrt::Windows::UI::Core::CoreWindow::VisibilityChanged_revoker                 m_visibility_changed;
    winrt::Windows::UI::Core::CoreWindow::Closed_revoker                            m_closed;

    DisplayInformation::DpiChanged_revoker                                          m_dpi_changed;
    DisplayInformation::OrientationChanged_revoker                                  m_orientation_changed;
    DisplayInformation::DisplayContentsInvalidated_revoker                          m_display_contents_invalidated;

    std::unique_ptr<UniqueCreator::Graphics::ResourceCreateContext>                 m_resource_create_context;
    std::unique_ptr<UniqueCreator::Graphics::WindowSwapChainResources>              m_background_swap_chain;
    std::unique_ptr<UniqueCreator::Graphics::WindowOverlaySwapChainResources>       m_overlay_swap_chain;
    
    std::unique_ptr<UniqueCreator::Graphics::DeviceResources>                       m_device_resources;
   
    std::unique_ptr<UniqueCreator::InputOutput::Mouse>                              m_mouse;
    std::unique_ptr<UniqueCreator::InputOutput::GamePad>                            m_game_pad;
    std::unique_ptr<UniqueCreator::InputOutput::Keyboard>                           m_keyboard;

    UniqueCreator::InputOutput::MouseState                                          m_mouse_state;
    UniqueCreator::InputOutput::KeyboardState                                       m_keyboard_state;
    UniqueCreator::InputOutput::GamePadState                                        m_game_pad_state;

    //view dependent buffers
    std::unique_ptr<UniqueCreator::Graphics::ViewDepthBuffer>                           m_depth_buffer;

    //Pipeline state objects
    std::unique_ptr<UniqueCreator::Graphics::PipelineStates::full_screen_graphics::GraphicsPipelineState>      m_full_screen_main;
};


#pragma warning( disable:4447 )
int32_t __stdcall wWinMain(HINSTANCE, HINSTANCE, PWSTR, int32_t)
{
    CoInitializeEx(nullptr, COINIT_MULTITHREADED);
    winrt::Windows::ApplicationModel::Core::CoreApplication::Run(ViewProvider());
    return 0;
}




