#pragma once

#if defined(UWP)

#include "winrt/Windows.UI.Core.h"

namespace UniqueCreator
{
	namespace InputOutput
	{
		struct CreateMouseParams
		{
			winrt::Windows::UI::Core::CoreWindow  m_window	= nullptr;
			float								  m_dpi		= 96.0f;
		};
	}
}


#endif
