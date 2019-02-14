//
// MainPage.xaml.cpp
// Implementation of the MainPage class.
//

#include "pch.h"
#include "MainPage.xaml.h"


using namespace uc_engine_xaml_hello_world;

using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;

#include <windows.ui.xaml.media.dxinterop.h>

#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.ApplicationModel.h>
#include <winrt/Windows.ApplicationModel.Core.h>
#include <winrt/Windows.ApplicationModel.Activation.h>
#include <winrt/Windows.Graphics.Display.h>
#include <winrt/Windows.UI.Core.h>
#include <winrt/Windows.UI.ViewManagement.h>
#include <winrt/Windows.UI.Xaml.Controls.h>

// The Blank Page item template is documented at https://go.microsoft.com/fwlink/?LinkId=402352&clcid=0x409

#include <autogen/shaders/full_screen_graphics.h>

MainPage::MainPage(Windows::UI::Core::CoreWindow^ win)
{
    _win = win;
    _windowClosed = false;
    InitializeComponent();

    auto w = winrt::Windows::UI::Core::CoreWindow::GetForCurrentThread();
    auto d = winrt::Windows::Graphics::Display::DisplayInformation::GetForCurrentView();

    m_resource_create_context = std::make_unique<UniqueCreator::Graphics::ResourceCreateContext>();

    {
        float compositionScaleX = m_swapChainPanel->CompositionScaleX;
        float compositionScaleY = m_swapChainPanel->CompositionScaleY;

        UniqueCreator::Graphics::Size2D size = { static_cast<float>(m_swapChainPanel->Width), static_cast<float>(m_swapChainPanel->Height) };
        m_background_swap_chain = std::make_unique<UniqueCreator::Graphics::CompositionSwapChainResources>(m_resource_create_context.get(), size, d, compositionScaleX, compositionScaleY);
        {
            Microsoft::WRL::ComPtr<ABI::Windows::UI::Xaml::Controls::ISwapChainPanel> panel = reinterpret_cast<ABI::Windows::UI::Xaml::Controls::ISwapChainPanel*>(m_swapChainPanel);
            Microsoft::WRL::ComPtr<ISwapChainPanelNative> nativescp;
            panel.As(&nativescp);
            nativescp->SetSwapChain(m_background_swap_chain->GetSwapChain());
        }
    }

    m_device_resources = std::make_unique<UniqueCreator::Graphics::DeviceResources>(m_resource_create_context.get(), m_background_swap_chain.get());

    m_depth_buffer = m_resource_create_context->CreateViewDepthBuffer(m_background_swap_chain->GetBackBuffer()->GetSize2D(), UniqueCreator::Graphics::DepthBufferFormat::Depth32Float);

    m_full_screen_main = UniqueCreator::Graphics::PipelineStates::full_screen_graphics::Create(m_resource_create_context.get());

    auto currentDisplayInformation = Windows::Graphics::Display::DisplayInformation::GetForCurrentView();

    currentDisplayInformation->DpiChanged +=
        ref new TypedEventHandler<Windows::Graphics::Display::DisplayInformation^, Object^>(this, &MainPage::OnDpiChanged);

    currentDisplayInformation->OrientationChanged +=
        ref new TypedEventHandler<Windows::Graphics::Display::DisplayInformation^, Object^>(this, &MainPage::OnOrientationChanged);
    
    Windows::Graphics::Display::DisplayInformation::DisplayContentsInvalidated +=
        ref new TypedEventHandler<Windows::Graphics::Display::DisplayInformation^, Object^>(this, &MainPage::OnDisplayContentsInvalidated);

    m_swapChainPanel->CompositionScaleChanged +=
        ref new TypedEventHandler<SwapChainPanel^, Object^>(this, &MainPage::OnCompositionScaleChanged);

    m_swapChainPanel->SizeChanged +=
            ref new SizeChangedEventHandler(this, &MainPage::OnSwapChainPanelSizeChanged);

}

void MainPage::OnSwapChainPanelSizeChanged(Object^ sender, Windows::UI::Xaml::SizeChangedEventArgs^ args)
{
    std::unique_lock<std::mutex> lock(m_render_lock);
    //stall and do not submit more work
    m_device_resources->WaitForGpu();
    m_background_swap_chain->WaitForGpu();

    //Reset view dependent heaps
    m_depth_buffer.reset();
    m_device_resources->ResetViewDependentResources();

    //Recreate the resources and buffers
    UniqueCreator::Graphics::Size2D size = { args->NewSize.Width, args->NewSize.Height };

    m_background_swap_chain->SetLogicalSize(size);
    m_depth_buffer = m_resource_create_context->CreateViewDepthBuffer(m_background_swap_chain->GetBackBuffer()->GetSize2D(), UniqueCreator::Graphics::DepthBufferFormat::Depth32Float);
}

void MainPage::OnCompositionScaleChanged(Windows::UI::Xaml::Controls::SwapChainPanel^ sender, Object^ args)
{
    std::unique_lock<std::mutex> lock(m_render_lock);
    //stall and do not submit more work
    m_device_resources->WaitForGpu();
    m_background_swap_chain->WaitForGpu();

    //Reset view dependent heaps
    m_depth_buffer.reset();
    m_device_resources->ResetViewDependentResources();

    //Recreate the resources and buffers
    m_background_swap_chain->SetCompositionScale(sender->CompositionScaleX, sender->CompositionScaleY);
    m_depth_buffer = m_resource_create_context->CreateViewDepthBuffer(m_background_swap_chain->GetBackBuffer()->GetSize2D(), UniqueCreator::Graphics::DepthBufferFormat::Depth32Float);
}

template <typename T>
T from_cx(Platform::Object^ from)
{
    T to{ nullptr };

    winrt::check_hresult(reinterpret_cast<::IUnknown*>(from)->QueryInterface(winrt::guid_of<T>(),
        reinterpret_cast<void**>(winrt::put_abi(to))));

    return to;
}

void MainPage::OnDpiChanged(Windows::Graphics::Display::DisplayInformation^ d, Object^ args)
{
    std::unique_lock<std::mutex> lock(m_render_lock);
    //stall and do not submit more work
    m_device_resources->WaitForGpu();
    m_background_swap_chain->WaitForGpu();

    //Reset view dependent heaps
    m_depth_buffer.reset();
    m_device_resources->ResetViewDependentResources();

    //Recreate the resources and buffers
    m_background_swap_chain->SetDisplayInformation(from_cx<winrt::Windows::Graphics::Display::DisplayInformation>(d));

    m_depth_buffer = m_resource_create_context->CreateViewDepthBuffer(m_background_swap_chain->GetBackBuffer()->GetSize2D(), UniqueCreator::Graphics::DepthBufferFormat::Depth32Float);
}





void MainPage::OnOrientationChanged(Windows::Graphics::Display::DisplayInformation^ d, Object^ args)
{
    std::unique_lock<std::mutex> lock(m_render_lock);
    //stall and do not submit more work
    m_device_resources->WaitForGpu();
    m_background_swap_chain->WaitForGpu();

    //Reset view dependent heaps
    m_depth_buffer.reset();
    m_device_resources->ResetViewDependentResources();

    m_depth_buffer = m_resource_create_context->CreateViewDepthBuffer(m_background_swap_chain->GetBackBuffer()->GetSize2D(), UniqueCreator::Graphics::DepthBufferFormat::Depth32Float);
}

void MainPage::OnDisplayContentsInvalidated(Windows::Graphics::Display::DisplayInformation^ sender, Object^ args)
{
    std::unique_lock<std::mutex> lock(m_render_lock);
}

void MainPage::BeginRendering()
{
    if (!m_renderer_thread)
    {
        m_renderer_thread = std::make_unique<std::thread>([this]()
        {
            while (!_windowClosed)
            {
                //_win->Dispatcher->ProcessEvents(Windows::UI::Core::CoreProcessEventsOption::ProcessAllIfPresent);
                //Sampling the mouse
                /*{
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
                }*/

                /*{
                    using namespace UniqueCreator::InputOutput;
                    auto s = m_keyboard->Sample();

                    if (KeyboardStateHelper::ButtonWasPressed(m_keyboard_state.m_State, s.m_State, KeyboardButton::Virtual_Key_A))
                    {
                    }

                    if (KeyboardStateHelper::ButtonWasReleased(m_keyboard_state.m_State, s.m_State, KeyboardButton::Virtual_Key_A))
                    {
                    }

                    m_keyboard_state = s;
                }*/

                /*{
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
                }*/

                Render();
            }
        });
    }
}

void uc_engine_xaml_hello_world::MainPage::Render()
{
    std::unique_lock<std::mutex> lock(m_render_lock);
    std::unique_ptr<UniqueCreator::Graphics::DirectCommandContext> ctx = m_background_swap_chain->CreateCommandContext();

    const auto backBuffer = m_background_swap_chain->GetBackBuffer();
    ctx->TransitionResource(backBuffer, UniqueCreator::Graphics::ResourceState::Present, UniqueCreator::Graphics::ResourceState::RenderTarget);
    ctx->Clear(backBuffer);

    ctx->SetDescriptorHeaps();
    ctx->SetRenderTarget(backBuffer);

    UniqueCreator::Graphics::ViewPort v;
    auto size = backBuffer->GetSize2D();
    v.m_topLeftX = 0;
    v.m_topLeftY = 0;
    v.m_width = size.m_width;
    v.m_height = size.m_height;
    v.m_maxDepth = 1.0f;
    v.m_minDepth = 0.0f;

    UniqueCreator::Graphics::Rectangle2D r;
    r.m_left = 0;
    r.m_right = size.m_width;
    r.m_top = 0;
    r.m_bottom = size.m_height;

    ctx->SetViewPort(v);
    ctx->SetScissorRectangle(r);

    //Per many draw calls  -> frequency 1
    ctx->SetPrimitiveTopology(UniqueCreator::Graphics::PrimitiveTopology::TriangleList);
    ctx->SetPipelineStateObject(m_full_screen_main.get());
    ctx->Draw(3, 0);

    ctx->TransitionResource(backBuffer, UniqueCreator::Graphics::ResourceState::RenderTarget, UniqueCreator::Graphics::ResourceState::Present);

    ctx->Submit();

    m_background_swap_chain->Present();

    //Sync point for all resources once per frame

    //Wait for the gpu to complete previous frames queues
    m_background_swap_chain->MoveToNextFrame();

    //flush copy and compute queues
    m_device_resources->Sync();

    //flush resources deleted this frame
    m_resource_create_context->Sync();

    //swap buffers
    m_background_swap_chain->Sync();
}

void uc_engine_xaml_hello_world::MainPage::EndRendering()
{
    _windowClosed = true;
    m_renderer_thread->join();
}
