#pragma once

#include <uc_public/uc_implementation.h>

namespace UniqueCreator
{
    namespace InputOutput
    {
        struct GamePadButtonsState
        {
            uint32_t m_Buttons = 0;

            GamePadButtonsState(uint32_t buttons) : m_Buttons(buttons)
            {

            }

            operator uint32_t() const
            {
                return m_Buttons;
            }
        };

        struct GamePadState
        {
            float               m_LeftTrigger = 0.0f;
            float               m_RightTrigger = 0.0f;

            float               m_ThumbLeftX    = 0.0f;
            float               m_ThumbLeftY    = 0.0f;

            float               m_ThumbRightX   = 0.0f;
            float               m_ThumbRightY   = 0.0f;

            GamePadButtonsState m_Buttons       = 0;
        };

        enum GamePadButton : uint32_t
        {
            Menu                        = (1 << 0),
            View                        = (1 << 1),
            Button_A                    = (1 << 2),
            Button_B                    = (1 << 3),
            Button_X                    = (1 << 4),
            Button_Y                    = (1 << 5),
            Button_Dpad_Up              = (1 << 6),
            Button_Dpad_Down            = (1 << 7),
            Button_Dpad_Left            = (1 << 8),
            Button_Dpad_Right           = (1 << 9),
            Button_Left_Shoulder        = (1 << 10),
            Button_Right_Shoulder       = (1 << 11),
            Button_Left_Thumbstick      = (1 << 12),
            Button_Right_Thumbstick     = (1 << 13)
        };


        class UC_ENGINE_API GamePad : private NonCopyable
        {
            class Impl;

            public:

            GamePad();
            ~GamePad();

            GamePadState Sample() const;

            GamePad(GamePad&&) = default;
            GamePad& operator=(GamePad&&);

            private:
            details::pimpl<Impl> m_impl;
        };

        class GamePadStateHelper
        {
        public:

            static inline uint32_t Difference(GamePadButtonsState prev, GamePadButtonsState current)
            {
                return static_cast<uint32_t>(prev) ^ static_cast<uint32_t>(current);
            }

            static inline bool IsButtonUp(GamePadButtonsState state, GamePadButton button)
            {
                return (static_cast<uint32_t>(state) & static_cast<uint32_t>(button)) == 0;
            }

            static inline bool IsButtonDown(GamePadButtonsState state, GamePadButton button)
            {
                return (static_cast<uint32_t>(state) & static_cast<uint32_t>(button)) != 0;
            }

            static inline bool HasDifferenceInState(GamePadButtonsState prev, GamePadButtonsState current, GamePadButton button)
            {
                auto d = Difference(prev, current);
                return (d & static_cast<uint32_t>(button)) != 0;
            }

            static inline bool ButtonWasPressed(GamePadButtonsState prev, GamePadButtonsState current, GamePadButton button)
            {
                return (HasDifferenceInState(prev, current, button) && IsButtonDown(current, button));
            }

            static inline bool ButtonWasReleased(GamePadButtonsState prev, GamePadButtonsState current, GamePadButton button)
            {
                return (HasDifferenceInState(prev, current, button) && IsButtonUp(current, button));
            }
        };
    }
}

