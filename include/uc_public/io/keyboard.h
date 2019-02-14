#pragma once

#include <uc_public/uc_implementation.h>
#include <uc_public/io/create_keyboard_params.h>

namespace UniqueCreator
{
    namespace InputOutput
    {
        enum KeyboardButton : uint32_t
        {
            Virtual_Key_None = 0,
            Virtual_Key_Left_Button = 1,
            Virtual_Key_Right_Button = 2,
            Virtual_Key_Cancel = 3,
            Virtual_Key_Middle_Button = 4,
            Virtual_Key_X_Button_1 = 5,
            Virtual_Key_X_Button_2 = 6,
            Virtual_Key_Back = 8,
            Virtual_Key_Tab = 9,
            Virtual_Key_Clear = 12,
            Virtual_Key_Enter = 13,
            Virtual_Key_Shift = 16,
            Virtual_Key_Control = 17,
            Virtual_Key_Menu = 18,
            Virtual_Key_Pause = 19,
            Virtual_Key_Capital_Lock = 20,
            Virtual_Key_Kana = 21,
            Virtual_Key_Hangul = 21,
            Virtual_Key_Junja = 23,
            Virtual_Key_Final = 24,
            Virtual_Key_Hanja = 25,
            Virtual_Key_Kanji = 25,
            Virtual_Key_Escape = 27,
            Virtual_Key_Convert = 28,
            Virtual_Key_Non_Convert = 29,
            Virtual_Key_Accept = 30,
            Virtual_Key_Mode_Change = 31,
            Virtual_Key_Space = 32,
            Virtual_Key_Page_Up = 33,
            Virtual_Key_Page_Down = 34,
            Virtual_Key_End = 35,
            Virtual_Key_Home = 36,
            Virtual_Key_Left = 37,
            Virtual_Key_Up = 38,
            Virtual_Key_Right = 39,
            Virtual_Key_Down = 40,
            Virtual_Key_Select = 41,
            Virtual_Key_Print = 42,
            Virtual_Key_Execute = 43,
            Virtual_Key_Snapshot = 44,
            Virtual_Key_Insert = 45,
            Virtual_Key_Delete = 46,
            Virtual_Key_Help = 47,
            Virtual_Key_Number_0 = 48,
            Virtual_Key_Number_1 = 49,
            Virtual_Key_Number_2 = 50,
            Virtual_Key_Number_3 = 51,
            Virtual_Key_Number_4 = 52,
            Virtual_Key_Number_5 = 53,
            Virtual_Key_Number_6 = 54,
            Virtual_Key_Number_7 = 55,
            Virtual_Key_Number_8 = 56,
            Virtual_Key_Number_9 = 57,
            Virtual_Key_A = 65,
            Virtual_Key_B = 66,
            Virtual_Key_C = 67,
            Virtual_Key_D = 68,
            Virtual_Key_E = 69,
            Virtual_Key_F = 70,
            Virtual_Key_G = 71,
            Virtual_Key_H = 72,
            Virtual_Key_I = 73,
            Virtual_Key_J = 74,
            Virtual_Key_K = 75,
            Virtual_Key_L = 76,
            Virtual_Key_M = 77,
            Virtual_Key_N = 78,
            Virtual_Key_O = 79,
            Virtual_Key_P = 80,
            Virtual_Key_Q = 81,
            Virtual_Key_R = 82,
            Virtual_Key_S = 83,
            Virtual_Key_T = 84,
            Virtual_Key_U = 85,
            Virtual_Key_V = 86,
            Virtual_Key_W = 87,
            Virtual_Key_X = 88,
            Virtual_Key_Y = 89,
            Virtual_Key_Z = 90,
            Virtual_Key_Left_Windows = 91,
            Virtual_Key_Right_Windows = 92,
            Virtual_Key_Application = 93,
            Virtual_Key_Sleep = 95,
            Virtual_Key_Number_Pad_0 = 96,
            Virtual_Key_Number_Pad_1 = 97,
            Virtual_Key_Number_Pad_2 = 98,
            Virtual_Key_Number_Pad_3 = 99,
            Virtual_Key_Number_Pad_4 = 100,
            Virtual_Key_Number_Pad_5 = 101,
            Virtual_Key_Number_Pad_6 = 102,
            Virtual_Key_Number_Pad_7 = 103,
            Virtual_Key_Number_Pad_8 = 104,
            Virtual_Key_Number_Pad_9 = 105,
            Virtual_Key_Multiply = 106,
            Virtual_Key_Add = 107,
            Virtual_Key_Separator = 108,
            Virtual_Key_Subtract = 109,
            Virtual_Key_Decimal = 110,
            Virtual_Key_Divide = 111,
            Virtual_Key_F1 = 112,
            Virtual_Key_F2 = 113,
            Virtual_Key_F3 = 114,
            Virtual_Key_F4 = 115,
            Virtual_Key_F5 = 116,
            Virtual_Key_F6 = 117,
            Virtual_Key_F7 = 118,
            Virtual_Key_F8 = 119,
            Virtual_Key_F9 = 120,
            Virtual_Key_F10 = 121,
            Virtual_Key_F11 = 122,
            Virtual_Key_F12 = 123,
            Virtual_Key_F13 = 124,
            Virtual_Key_F14 = 125,
            Virtual_Key_F15 = 126,
            Virtual_Key_F16 = 127,
            Virtual_Key_F17 = 128,
            Virtual_Key_F18 = 129,
            Virtual_Key_F19 = 130,
            Virtual_Key_F20 = 131,
            Virtual_Key_F21 = 132,
            Virtual_Key_F22 = 133,
            Virtual_Key_F23 = 134,
            Virtual_Key_F24 = 135,
            Virtual_Key_Navigation_View = 136,
            Virtual_Key_Navigation_Menu = 137,
            Virtual_Key_Navigation_Up = 138,
            Virtual_Key_Navigation_Down = 139,
            Virtual_Key_Navigation_Left = 140,
            Virtual_Key_Navigation_Right = 141,
            Virtual_Key_Navigation_Accept = 142,
            Virtual_Key_Navigation_Cancel = 143,
            Virtual_Key_Number_Key_Lock = 144,
            Virtual_Key_Scroll = 145,
            Virtual_Key_Left_Shift = 160,
            Virtual_Key_Right_Shift = 161,
            Virtual_Key_Left_Control = 162,
            Virtual_Key_Right_Control = 163,
            Virtual_Key_Left_Menu = 164,
            Virtual_Key_Right_Menu = 165,
            Virtual_Key_Go_Back = 166,
            Virtual_Key_Go_Forward = 167,
            Virtual_Key_Refresh = 168,
            Virtual_Key_Stop = 169,
            Virtual_Key_Search = 170,
            Virtual_Key_Favorites = 171,
            Virtual_Key_GoHome = 172,
            Virtual_Key_GamePad_A = 195,
            Virtual_Key_GamePad_B = 196,
            Virtual_Key_GamePad_X = 197,
            Virtual_Key_GamePad_Y = 198,
            Virtual_Key_GamePad_Right_Shoulder = 199,
            Virtual_Key_GamePad_Left_Shoulder = 200,
            Virtual_Key_GamePad_Left_Trigger = 201,
            Virtual_Key_GamePad_Right_Trigger = 202,
            Virtual_Key_GamePad_Dpad_Up = 203,
            Virtual_Key_GamePad_Dpad_Down = 204,
            Virtual_Key_GamePad_Dpad_Left = 205,
            Virtual_Key_GamePad_Dpad_Right = 206,
            Virtual_Key_GamePad_Menu = 207,
            Virtual_Key_GamePad_View = 208,
            Virtual_Key_GamePad_Left_Thumbstick_Button = 209,
            Virtual_Key_GamePad_Right_Thumbstick_Button = 210,
            Virtual_Key_GamePad_Left_ThumbStick_Up = 211,
            Virtual_Key_GamePad_Left_ThumbStick_Down = 212,
            Virtual_Key_GamePad_Left_ThumbStick_Right = 213,
            Virtual_Key_GamePad_Left_ThumbStick_Left = 214,
            Virtual_Key_GamePad_Right_ThumbStick_Up = 215,
            Virtual_Key_GamePad_Right_ThumbStick_Down = 216,
            Virtual_Key_GamePad_Right_ThumbStick_Right = 217,
            Virtual_Key_GamePad_Right_ThumbStick_Left = 218
        };

        struct KeyboardButtonState
        {
            uint64_t                m_States[4];

            KeyboardButtonState()
            {
                m_States[0] = 0;
                m_States[1] = 1;
                m_States[2] = 2;
                m_States[3] = 3;
            }

            ~KeyboardButtonState() = default;

            KeyboardButtonState(const KeyboardButtonState& o) = default;
            KeyboardButtonState& operator=(const KeyboardButtonState& o) = default;

            KeyboardButtonState(KeyboardButtonState&& o) = default;
            KeyboardButtonState& operator=(KeyboardButtonState&& o) = default;
        };

        struct KeyboardState
        {
            KeyboardButtonState     m_State;
            std::vector<uint32_t>   m_CharacterCodes;

            KeyboardState()
            {
                
            }

            ~KeyboardState() = default;

            KeyboardState(const KeyboardState& o) = default;
            KeyboardState& operator=(const KeyboardState& o) = default;

            KeyboardState(KeyboardState&& o) = default;
            KeyboardState& operator=(KeyboardState&& o) = default;
        };

        class UC_ENGINE_API Keyboard : private NonCopyable
        {
            class Impl;

            public:

            Keyboard(const CreateKeyboardParams&);
            ~Keyboard();

            KeyboardState Sample();

            Keyboard(Keyboard&&) = default;
            Keyboard& operator=(Keyboard&&);

        private:
            details::pimpl<Impl> m_impl;
        };

        class KeyboardStateHelper
        {

            public:

            static inline uint32_t Difference(const KeyboardButtonState& prev, const KeyboardButtonState current, KeyboardButton button)
            {
                uint32_t byte = button / 64;
                //uint32_t bit  = button % 64;

                return static_cast<uint32_t>( prev.m_States[byte] ) ^ static_cast<uint32_t>(current.m_States[byte] );
            }

            static inline bool IsButtonUp(const KeyboardButtonState& state, KeyboardButton button)
            {
                uint32_t byte   = button / 64;
                uint32_t bit    = button % 64;

                return (static_cast<uint32_t>(state.m_States[byte] ) & static_cast<uint32_t>(1 << bit ) ) == 0;
            }

            static inline bool IsButtonDown(const KeyboardButtonState& state, KeyboardButton button)
            {
                uint32_t byte   = button / 64;
                uint32_t bit    = button % 64;

                return (static_cast<uint32_t>(state.m_States[byte] ) & static_cast<uint32_t>( 1 << bit)) != 0;
            }

            static inline bool HasDifferenceInState(const KeyboardButtonState& prev, const KeyboardButtonState& current, KeyboardButton button)
            {
                auto d = Difference(prev, current, button);
                uint32_t bit = button % 64;
                return (d & static_cast<uint32_t>( 1 << bit )) != 0;
            }

            static inline bool ButtonWasPressed(const KeyboardButtonState& prev, const KeyboardButtonState& current, KeyboardButton button)
            {
                return (HasDifferenceInState(prev, current, button) && IsButtonDown(current, button));
            }

            static inline bool ButtonWasReleased(const KeyboardButtonState& prev, const KeyboardButtonState& current, KeyboardButton button)
            {
                return (HasDifferenceInState(prev, current, button) && IsButtonUp(current, button));
            }
        };

    }
}

