#include "pch.h"

#include <uc_public/io/keyboard.h>
#include <uc_public/uc_pimpl_impl.h>

#include <uc_dev/io/keyboard.h>


namespace UniqueCreator
{
	namespace InputOutput
	{
		class Keyboard::Impl
		{
		public:

			Keyboard::Impl(const CreateKeyboardParams& p)
			{
				m_native_pad.set_window(p.m_window);
			}

			KeyboardState Sample()
			{
				uc::io::keyboard_state s = m_native_pad.update(m_state);

				KeyboardState r;

				r.m_State.m_States[0] = s.m_states[0];
				r.m_State.m_States[1] = s.m_states[1];
				r.m_State.m_States[2] = s.m_states[2];
				r.m_State.m_States[3] = s.m_states[3];

				r.m_CharacterCodes = s.m_character_codes;

				return r;
			}

			uc::io::keyboard_state  m_state;
			uc::io::keyboard		m_native_pad;
		};

		Keyboard::Keyboard(const CreateKeyboardParams& p) : m_impl(p)
		{

		}

		Keyboard::~Keyboard() = default;
		Keyboard& Keyboard::operator=(Keyboard&&) = default;

		KeyboardState Keyboard::Sample()
		{
			return m_impl->Sample();
		}
	}
}
