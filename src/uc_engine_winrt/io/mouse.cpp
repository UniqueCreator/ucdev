#include "pch.h"

#include "mouse.h"

#include <windows.ui.core.h>
#include <wrl/client.h>
#include <winrt/base.h>

namespace UniqueCreatorEngine
{
    namespace WinRT
    {
        namespace InputOutput
        {
            template <typename T>
            T from_cx(Platform::Object^ from)
            {
                T to{ nullptr };

                winrt::check_hresult(reinterpret_cast<::IUnknown*>(from)->QueryInterface(winrt::guid_of<T>(),
                    reinterpret_cast<void**>(winrt::put_abi(to))));

                return to;
            }


            inline auto MakeMouseParams(Windows::UI::Core::ICoreWindow ^w, float dpi)
            {
                ::UniqueCreator::InputOutput::CreateMouseParams r;

                r.m_window	= from_cx< winrt::Windows::UI::Core::CoreWindow >( w );
                r.m_dpi		= dpi;

                return r;
            }

            Mouse::Mouse(Windows::UI::Core::CoreWindow ^w, float dpi) : m_native_mouse(MakeMouseParams(w,dpi))
            {

            }

            MouseState Mouse::Sample()
            {
                auto s = m_native_mouse.Sample();
                
                MouseState r;

                r.Buttons	= s.m_Buttons;
                r.X			= s.m_X;
                r.Y			= s.m_Y;
                    
                return r;
            }
        }
    }
}

