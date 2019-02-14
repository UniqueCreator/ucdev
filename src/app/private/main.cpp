#include "pch.h"

#include <memory>
#include <ppltasks.h>
#include "winrt/base.h"

#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.ApplicationModel.h>
#include <winrt/Windows.ApplicationModel.Core.h>
#include <winrt/Windows.ApplicationModel.Activation.h>
#include <winrt/Windows.Graphics.Display.h>
#include <winrt/Windows.UI.Core.h>
#include <winrt/Windows.UI.ViewManagement.h>

#include <uc_dev/os/windows/com_initializer.h>

#include "uwp/uc_uwp_renderer.h"


using namespace winrt::Windows::ApplicationModel;
using namespace winrt::Windows::ApplicationModel::Core;
using namespace winrt::Windows::ApplicationModel::Activation;
using namespace winrt::Windows::UI::Core;
using namespace winrt::Windows::UI::ViewManagement;
using namespace winrt::Windows::Graphics::Display;


class ViewProvider : public winrt::implements<ViewProvider, IFrameworkView, IFrameworkViewSource>
{

public:

    ViewProvider() : 
    m_windowClosed(false),
    m_windowVisible(true)
    {
        
    }

    IFrameworkView CreateView()
    {
        return *this;
    }

    // IFrameworkView methods
    virtual void Initialize( const CoreApplicationView& applicationView)
    {
        m_activated		= applicationView.Activated(winrt::auto_revoke, { this, &ViewProvider::OnActivated });
        m_suspending	= CoreApplication::Suspending(winrt::auto_revoke, { this, &ViewProvider::OnSuspending } );
        m_resuming		= CoreApplication::Resuming(winrt::auto_revoke, { this, &ViewProvider::OnResuming });


#if defined(_DEBUG)

        ApplicationView::PreferredLaunchViewSize(winrt::Windows::Foundation::Size(1600, 900));
        ApplicationView::PreferredLaunchWindowingMode(ApplicationViewWindowingMode::PreferredLaunchViewSize);

#else

        //ApplicationView::PreferredLaunchViewSize = Size(1600, 900);
        ApplicationView::PreferredLaunchWindowingMode(ApplicationViewWindowingMode::FullScreen);
#endif

    }

    virtual void Uninitialize()
    {

    }

    virtual void SetWindow( const CoreWindow& window)
    {
        //todo: more events;

        m_size_changed			= window.SizeChanged( winrt::auto_revoke, { this, &ViewProvider::OnWindowSizeChanged });
        m_visibility_changed	= window.VisibilityChanged(winrt::auto_revoke, { this, &ViewProvider::OnVisibilityChanged });
        m_closed				= window.Closed(winrt::auto_revoke, { this, &ViewProvider::OnWindowClosed });

        DisplayInformation currentDisplayInformation = DisplayInformation::GetForCurrentView();

        m_dpi_changed			= currentDisplayInformation.DpiChanged(winrt::auto_revoke, { this, &ViewProvider::OnDpiChanged });
        m_orientation_changed	= currentDisplayInformation.OrientationChanged(winrt::auto_revoke, { this, &ViewProvider::OnOrientationChanged });
        m_display_contents_invalidated = currentDisplayInformation.DisplayContentsInvalidated(winrt::auto_revoke, { this, &ViewProvider::OnDisplayContentsInvalidated });

        m_core_window			= window;
        m_display_information	= currentDisplayInformation;
        
        /*
        Windows::UI::Core::CoreWindow^					w  = reinterpret_cast<Windows::UI::Core::CoreWindow^>(winrt::get(window));
        Windows::Graphics::Display::DisplayInformation^ di = reinterpret_cast<Windows::Graphics::Display::DisplayInformation^>(winrt::get(currentDisplayInformation));
        */

        m_renderer  = std::make_unique<uc::uwp::renderer>(&m_windowClosed, window, m_display_information );
        m_renderer->set_display_info(m_display_information);
        m_renderer->refresh_display_layout();
    }

    virtual void Load( winrt::hstring)
    {

    }

    virtual void Run()
    {
        //show splash screen here
        auto initialize = m_renderer->initialize_async();

        while (!m_windowClosed && !initialize.is_done() )
        {
            CoreWindow::GetForCurrentThread().Dispatcher().ProcessEvents(CoreProcessEventsOption::ProcessAllIfPresent);
        }

        if (m_windowClosed)
        {
            return;
        }

        if (!m_renderer_thread)
        {
            //split the event processing and the rendering
            m_renderer_thread = std::make_unique<std::thread>([this]()
            {
                while (!m_windowClosed)
                {
                    m_renderer->pre_render();
                    m_renderer->update();
                    m_renderer->render();
                    m_renderer->present();
                }
            });
        }

        while (!m_windowClosed)
        {
            if (m_windowVisible)
            {
                CoreWindow::GetForCurrentThread().Dispatcher().ProcessEvents(CoreProcessEventsOption::ProcessAllIfPresent);
            }
            else
            {
                CoreWindow::GetForCurrentThread().Dispatcher().ProcessEvents(CoreProcessEventsOption::ProcessOneAndAllPending);
            }
        }

        m_renderer_thread->join();
        {
            auto shut_down = m_renderer->shutdown_async();

            while (!shut_down.is_done())
            {
                CoreWindow::GetForCurrentThread().Dispatcher().ProcessEvents(CoreProcessEventsOption::ProcessAllIfPresent);
            }
        }
    }

protected:
    // Event handlers
    void OnActivated(const CoreApplicationView&, const IActivatedEventArgs&)
    {
        CoreWindow::GetForCurrentThread().Activate();
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

    void OnWindowSizeChanged( const CoreWindow&, const WindowSizeChangedEventArgs&)
    {
        //Windows::Graphics::Display::DisplayInformation^ di = reinterpret_cast<Windows::Graphics::Display::DisplayInformation^>(winrt::get(m_display_information));
        m_renderer->set_display_info(m_display_information);
        m_renderer->refresh_display_layout();
    }

    void OnWindowClosed(const CoreWindow&, const CoreWindowEventArgs&)
    {
        m_windowClosed = true;
    }

    void OnVisibilityChanged(const CoreWindow& , const VisibilityChangedEventArgs& args)
    {
        m_windowVisible = args.Visible();

        if (!m_windowVisible)
        {
            //free render targets here
        }
    }

    // DisplayInformation event handlers.
    void OnDpiChanged(const DisplayInformation& sender, const winrt::Windows::Foundation::IInspectable&)
    {
        m_display_information = sender;

        //attach events again
        m_dpi_changed					= m_display_information.DpiChanged(winrt::auto_revoke, { this, &ViewProvider::OnDpiChanged });
        m_orientation_changed			= m_display_information.OrientationChanged(winrt::auto_revoke, { this, &ViewProvider::OnOrientationChanged });
        m_display_contents_invalidated	= m_display_information.DisplayContentsInvalidated(winrt::auto_revoke, { this, &ViewProvider::OnDisplayContentsInvalidated });

        m_renderer->set_display_info(m_display_information);
        m_renderer->refresh_display_layout();
    }

    void OnOrientationChanged(const DisplayInformation& sender, const winrt::Windows::Foundation::IInspectable&)
    {
        m_display_information = sender;

        //attach events again
        m_dpi_changed = m_display_information.DpiChanged(winrt::auto_revoke, { this, &ViewProvider::OnDpiChanged });
        m_orientation_changed = m_display_information.OrientationChanged(winrt::auto_revoke, { this, &ViewProvider::OnOrientationChanged });
        m_display_contents_invalidated = m_display_information.DisplayContentsInvalidated(winrt::auto_revoke, { this, &ViewProvider::OnDisplayContentsInvalidated });

        m_renderer->set_display_info(m_display_information);
        m_renderer->refresh_display_layout();
    }

    void OnDisplayContentsInvalidated(const DisplayInformation& sender, const winrt::Windows::Foundation::IInspectable&)
    {
        m_display_information = sender;

        //attach events again
        m_dpi_changed = m_display_information.DpiChanged(winrt::auto_revoke, { this, &ViewProvider::OnDpiChanged });
        m_orientation_changed = m_display_information.OrientationChanged(winrt::auto_revoke, { this, &ViewProvider::OnOrientationChanged });
        m_display_contents_invalidated = m_display_information.DisplayContentsInvalidated(winrt::auto_revoke, { this, &ViewProvider::OnDisplayContentsInvalidated });

        m_renderer->set_display_info(m_display_information);
        m_renderer->refresh_display_layout();
    }

private:

    bool m_windowClosed;
    bool m_windowVisible;

    std::unique_ptr<uc::uwp::renderer>    				    m_renderer;
    std::unique_ptr<std::thread>							m_renderer_thread;

    CoreApplicationView::Activated_revoker					m_activated;
    CoreApplication::Suspending_revoker						m_suspending;
    CoreApplication::Resuming_revoker						m_resuming;


    CoreWindow::SizeChanged_revoker							m_size_changed;
    CoreWindow::VisibilityChanged_revoker					m_visibility_changed;
    CoreWindow::Closed_revoker								m_closed;

    DisplayInformation::DpiChanged_revoker					m_dpi_changed;
    DisplayInformation::OrientationChanged_revoker			m_orientation_changed;
    DisplayInformation::DisplayContentsInvalidated_revoker	m_display_contents_invalidated;
   
    CoreWindow												m_core_window			= nullptr;
    DisplayInformation										m_display_information	= nullptr;
};


#pragma warning( disable:4447 )
int32_t __stdcall wWinMain(HINSTANCE, HINSTANCE, PWSTR, int32_t)
{
    uc::os::windows::com_initializer initializer;
    //auto viewProviderFactory = ref new ViewProviderFactory();
    CoreApplication::Run(ViewProvider());
    return 0;
}




