#pragma once

#if defined(UWP)

#include "winrt/Windows.UI.Core.h"

namespace UniqueCreator
{
	namespace InputOutput
	{
		struct CreateKeyboardParams
		{
			winrt::Windows::UI::Core::CoreWindow m_window;
		};
	}
}

#endif
