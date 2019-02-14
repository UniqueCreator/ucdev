#pragma once

#include <wrl/client.h>
#include <wrl/wrappers/corewrappers.h>
#include <wrl/event.h>

#include <winrt/windows.ui.core.h>

namespace uc
{
    namespace uwp
    {
        template <typename f> inline void ui_run_sync( const winrt::Windows::UI::Core::CoreWindow& window, f update )
        {
			auto action = window.Dispatcher().RunAsync(winrt::Windows::UI::Core::CoreDispatcherPriority::Normal, winrt::Windows::UI::Core::DispatchedHandler(update));

            while ( action.Status() != winrt::Windows::Foundation::AsyncStatus::Completed )
            {
                std::this_thread::yield();
            }

        }

        template <typename f> inline void ui_run_async(const winrt::Windows::UI::Core::CoreWindow& window, f update)
        {
			window.Dispatcher().RunAsync(winrt::Windows::UI::Core::CoreDispatcherPriority::Normal, winrt::Windows::UI::Core::DispatchedHandler(update));
        }
    }
    
}