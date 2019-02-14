#pragma once

#include <uc_public/uc_implementation.h>
#include <uc_public/io/create_mouse_params.h>

namespace UniqueCreator
{
    namespace InputOutput
    {
        struct MouseButtonsState
        {
            uint32_t m_Buttons = 0;

            MouseButtonsState(uint32_t buttons) : m_Buttons(buttons)
            {

            }

            operator uint32_t() const
            {
                return m_Buttons;
            }
        };

        struct MouseState
        {
            MouseButtonsState  m_Buttons = 0;
            int16_t            m_X       = 0;
            int16_t            m_Y       = 0;
        };

        enum MouseButton : uint32_t
        {
            Left    = (1 << 0),
            Right   = (1 << 1),
            Middle  = (1 << 2)
        };

        class UC_ENGINE_API Mouse : private NonCopyable
        {
            class Impl;

            public:

            Mouse( const CreateMouseParams& );
            ~Mouse();

            MouseState Sample();

            Mouse(Mouse&&) = default;
            Mouse& operator=(Mouse&&);

            private:
            details::pimpl<Impl> m_impl;
        };

        class MouseButtonStateHelper
        {
            public:

            static inline uint32_t Difference(MouseButtonsState prev, MouseButtonsState current)
            {
                return static_cast<uint32_t>(prev) ^ static_cast<uint32_t>(current);
            }

            static inline bool IsButtonUp(MouseButtonsState state, MouseButton button)
            {
                return (static_cast<uint32_t>(state) & static_cast<uint32_t>(button)) == 0;
            }

            static inline bool IsButtonDown(MouseButtonsState state, MouseButton button)
            {
                return (static_cast<uint32_t>(state) & static_cast<uint32_t>(button)) != 0;
            }

            static inline bool HasDifferenceInState(MouseButtonsState prev, MouseButtonsState current, MouseButton button)
            {
                auto d = Difference(prev, current);
                return (d & static_cast<uint32_t>(button)) != 0;
            }

            static inline bool ButtonWasPressed( MouseButtonsState prev, MouseButtonsState current, MouseButton button)
            {
                return (HasDifferenceInState(prev, current, button) && IsButtonDown(current, button));
            }

            static inline bool ButtonWasReleased(MouseButtonsState prev, MouseButtonsState current, MouseButton button)
            {
                return (HasDifferenceInState(prev, current, button) && IsButtonUp(current, button));
            }
        };
    }
}

