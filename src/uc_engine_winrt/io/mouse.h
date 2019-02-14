#pragma once

#include <uc_public/io/mouse.h>

namespace UniqueCreatorEngine
{
	namespace WinRT
	{
		namespace InputOutput
		{
			public value struct MouseState sealed
			{
				uint32_t  Buttons;
				int16_t   X;
				int16_t   Y;
			};

			[Windows::Foundation::Metadata::WebHostHiddenAttribute]
			
			public ref class Mouse sealed
			{

			public:

				Mouse(Windows::UI::Core::CoreWindow ^w, float dpi);
				MouseState Sample();

			private:

				::UniqueCreator::InputOutput::Mouse m_native_mouse;
			};
		}
	}
}
