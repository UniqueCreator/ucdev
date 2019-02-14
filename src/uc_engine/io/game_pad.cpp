#include "pch.h"

#include <uc_public/io/game_pad.h>
#include <uc_public/uc_pimpl_impl.h>

#include <uc_dev/io/pad.h>

namespace UniqueCreator
{
	namespace InputOutput
	{
		class GamePad::Impl
		{
			public:

			GamePad::Impl()
			{

			}

			GamePadState Sample() const
			{
				uc::io::pad_state s = m_native_pad.update(uc::io::pad_state());

				GamePadState r = {	s.m_state.m_left_trigger, 
									s.m_state.m_right_trigger, 

									s.m_state.m_thumb_left_x,
									s.m_state.m_thumb_left_y,

									s.m_state.m_thumb_right_x,
									s.m_state.m_thumb_right_y,

									s.m_state.m_buttons
								};
				return r;
			}

			uc::io::pad m_native_pad;
		};

		GamePad::GamePad()
		{

		}

		GamePad::~GamePad() = default;
		GamePad& GamePad::operator=(GamePad&&) = default;

		GamePadState GamePad::Sample() const
		{
			return m_impl->Sample();
		}
	}
}
