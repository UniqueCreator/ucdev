#pragma once

#include <cstdint>
#include <vector>
#include <windows.h>

#include <uc_dev/util/bits.h>


namespace uc
{
    namespace io
    {
        class keyboard_state
        {
            public:

            enum mask : uint32_t
            {
                button_0 = 0,
                button_1 = 1,
                button_2 = 2,
                button_3 = 3,
                button_4 = 4,
                button_5 = 5,
                button_6 = 6,
                button_7 = 7,
                button_8 = 8,
                button_9 = 9,

                button_10 = 10,
                button_11 = 11,
                button_12 = 12,
                button_13 = 13,
                button_14 = 14,
                button_15 = 15,
                button_16 = 16,
                button_17 = 17,
                button_18 = 18,
                button_19 = 19,

                button_20 = 20,
                button_21 = 21,
                button_22 = 22,
                button_23 = 23,
                button_24 = 24,
                button_25 = 25,
                button_26 = 26,
                button_27 = 27,
                button_28 = 28,
                button_29 = 29,

                button_30 = 30,
                button_31 = 31,
                button_32 = 32,
                button_33 = 33,
                button_34 = 34,
                button_35 = 35,
                button_36 = 36,
                button_37 = 37,
                button_38 = 38,
                button_39 = 39,

                button_40 = 40,
                button_41 = 41,
                button_42 = 42,
                button_43 = 43,
                button_44 = 44,
                button_45 = 45,
                button_46 = 46,
                button_47 = 47,
                button_48 = 48,
                button_49 = 49,

                button_50 = 50,
                button_51 = 51,
                button_52 = 52,
                button_53 = 53,
                button_54 = 54,
                button_55 = 55,
                button_56 = 56,
                button_57 = 57,
                button_58 = 58,
                button_59 = 59,

                button_60 = 60,
                button_61 = 61,
                button_62 = 62,
                button_63 = 63,
                button_64 = 64,
                button_65 = 65,
                button_66 = 66,
                button_67 = 67,
                button_68 = 68,
                button_69 = 69,

                button_70 = 70,
                button_71 = 71,
                button_72 = 72,
                button_73 = 73,
                button_74 = 74,
                button_75 = 75,
                button_76 = 76,
                button_77 = 77,
                button_78 = 78,
                button_79 = 79,

                button_80 = 80,
                button_81 = 81,
                button_82 = 82,
                button_83 = 83,
                button_84 = 84,
                button_85 = 85,
                button_86 = 86,
                button_87 = 87,
                button_88 = 88,
                button_89 = 89,

                button_90 = 90,
                button_91 = 91,
                button_92 = 92,
                button_93 = 93,
                button_94 = 94,
                button_95 = 95,
                button_96 = 96,
                button_97 = 97,
                button_98 = 98,
                button_99 = 99,

                button_100 = 100,
                button_101 = 101,
                button_102 = 102,
                button_103 = 103,
                button_104 = 104,
                button_105 = 105,
                button_106 = 106,
                button_107 = 107,
                button_108 = 108,
                button_109 = 109,

                button_110 = 110,
                button_111 = 111,
                button_112 = 112,
                button_113 = 113,
                button_114 = 114,
                button_115 = 115,
                button_116 = 116,
                button_117 = 117,
                button_118 = 118,
                button_119 = 119,

                button_120 = 120,
                button_121 = 121,
                button_122 = 122,
                button_123 = 123,
                button_124 = 124,
                button_125 = 125,
                button_126 = 126,
                button_127 = 127,
                button_128 = 128,
                button_129 = 129,

                button_130 = 130,
                button_131 = 131,
                button_132 = 132,
                button_133 = 133,
                button_134 = 134,
                button_135 = 135,
                button_136 = 136,
                button_137 = 137,
                button_138 = 138,
                button_139 = 139,

                button_140 = 140,
                button_141 = 141,
                button_142 = 142,
                button_143 = 143,
                button_144 = 144,
                button_145 = 145,
                button_146 = 146,
                button_147 = 147,
                button_148 = 148,
                button_149 = 149,

                button_150 = 150,
                button_151 = 151,
                button_152 = 152,
                button_153 = 153,
                button_154 = 154,
                button_155 = 155,
                button_156 = 156,
                button_157 = 157,
                button_158 = 158,
                button_159 = 159,

                button_160 = 160,
                button_161 = 161,
                button_162 = 162,
                button_163 = 163,
                button_164 = 164,
                button_165 = 165,
                button_166 = 166,
                button_167 = 167,
                button_168 = 168,
                button_169 = 169,

                button_170 = 170,
                button_171 = 171,
                button_172 = 172,
                button_173 = 173,
                button_174 = 174,
                button_175 = 175,
                button_176 = 176,
                button_177 = 177,
                button_178 = 178,
                button_179 = 179,

                button_180 = 180,
                button_181 = 181,
                button_182 = 182,
                button_183 = 183,
                button_184 = 184,
                button_185 = 185,
                button_186 = 186,
                button_187 = 187,
                button_188 = 188,
                button_189 = 189,

                button_190 = 190,
                button_191 = 191,
                button_192 = 192,
                button_193 = 193,
                button_194 = 194,
                button_195 = 195,
                button_196 = 196,
                button_197 = 197,
                button_198 = 198,
                button_199 = 199,

                button_200 = 200,
                button_201 = 201,
                button_202 = 202,
                button_203 = 203,
                button_204 = 204,
                button_205 = 205,
                button_206 = 206,
                button_207 = 207,
                button_208 = 208,
                button_209 = 209,

                button_210 = 210,
                button_211 = 211,
                button_212 = 212,
                button_213 = 213,
                button_214 = 214,
                button_215 = 215,
                button_216 = 216,
                button_217 = 217,
                button_218 = 218,
                button_219 = 219,


                button_220 = 220,
                button_221 = 221,
                button_222 = 222,
                button_223 = 223,
                button_224 = 224,
                button_225 = 225,
                button_226 = 226,
                button_227 = 227,
                button_228 = 228,
                button_229 = 229,

                button_230 = 230,
                button_231 = 231,
                button_232 = 232,
                button_233 = 233,
                button_234 = 234,
                button_235 = 235,
                button_236 = 236,
                button_237 = 237,
                button_238 = 238,
                button_239 = 239,

                button_240 = 240,
                button_241 = 241,
                button_242 = 242,
                button_243 = 243,
                button_244 = 244,
                button_245 = 245,
                button_246 = 246,
                button_247 = 247,
                button_248 = 248,
                button_249 = 249,

                button_250 = 250,
                button_251 = 251,
                button_252 = 252,
                button_253 = 253,
                button_254 = 254,
                button_255 = 255,
                button_256 = 256,
                button_257 = 257,
                button_258 = 258,
                button_259 = 259
            };

            enum virtual_key_mask : uint32_t
            {
                virtual_key_none = 0,
                virtual_key_left_button = 1,
                virtual_key_right_button = 2,
                virtual_key_cancel = 3,
                virtual_key_middle_button = 4,
                virtual_key_x_button_1 = 5,
                virtual_key_x_button_2 = 6,
                virtual_key_back = 8,
                virtual_key_tab = 9,
                virtual_key_clear = 12,
                virtual_key_enter = 13,
                virtual_key_shift = 16,
                virtual_key_control = 17,
                virtual_key_menu = 18,
                virtual_key_pause = 19,
                virtual_key_capital_lock = 20,
                virtual_key_kana = 21,
                virtual_key_hangul = 21,
                virtual_key_junja = 23,
                virtual_key_final = 24,
                virtual_key_hanja = 25,
                virtual_key_kanji = 25,
                virtual_key_escape = 27,
                virtual_key_convert = 28,
                virtual_key_non_convert = 29,
                virtual_key_accept = 30,
                virtual_key_mode_change = 31,
                virtual_key_space = 32,
                virtual_key_page_up = 33,
                virtual_key_page_down = 34,
                virtual_key_end = 35,
                virtual_key_home = 36,
                virtual_key_left = 37,
                virtual_key_up = 38,
                virtual_key_right = 39,
                virtual_key_down = 40,
                virtual_key_select = 41,
                virtual_key_print = 42,
                virtual_key_execute = 43,
                virtual_key_snapshot = 44,
                virtual_key_insert = 45,
                virtual_key_delete = 46,
                virtual_key_help = 47,
                virtual_key_number_0 = 48,
                virtual_key_number_1 = 49,
                virtual_key_number_2 = 50,
                virtual_key_number_3 = 51,
                virtual_key_number_4 = 52,
                virtual_key_number_5 = 53,
                virtual_key_number_6 = 54,
                virtual_key_number_7 = 55,
                virtual_key_number_8 = 56,
                virtual_key_number_9 = 57,
                virtual_key_a = 65,
                virtual_key_b = 66,
                virtual_key_c = 67,
                virtual_key_d = 68,
                virtual_key_e = 69,
                virtual_key_f = 70,
                virtual_key_g = 71,
                virtual_key_h = 72,
                virtual_key_i = 73,
                virtual_key_j = 74,
                virtual_key_k = 75,
                virtual_key_l = 76,
                virtual_key_m = 77,
                virtual_key_n = 78,
                virtual_key_o = 79,
                virtual_key_p = 80,
                virtual_key_q = 81,
                virtual_key_r = 82,
                virtual_key_s = 83,
                virtual_key_t = 84,
                virtual_key_u = 85,
                virtual_key_v = 86,
                virtual_key_w = 87,
                virtual_key_x = 88,
                virtual_key_y = 89,
                virtual_key_z = 90,
                virtual_key_left_windows = 91,
                virtual_key_right_windows = 92,
                virtual_key_application = 93,
                virtual_key_sleep = 95,
                virtual_key_number_pad_0 = 96,
                virtual_key_number_pad_1 = 97,
                virtual_key_number_pad_2 = 98,
                virtual_key_number_pad_3 = 99,
                virtual_key_number_pad_4 = 100,
                virtual_key_number_pad_5 = 101,
                virtual_key_number_pad_6 = 102,
                virtual_key_number_pad_7 = 103,
                virtual_key_number_pad_8 = 104,
                virtual_key_number_pad_9 = 105,
                virtual_key_multiply = 106,
                virtual_key_add = 107,
                virtual_key_separator = 108,
                virtual_key_subtract = 109,
                virtual_key_decimal = 110,
                virtual_key_divide = 111,
                virtual_key_f1 = 112,
                virtual_key_f2 = 113,
                virtual_key_f3 = 114,
                virtual_key_f4 = 115,
                virtual_key_f5 = 116,
                virtual_key_f6 = 117,
                virtual_key_f7 = 118,
                virtual_key_f8 = 119,
                virtual_key_f9 = 120,
                virtual_key_f10 = 121,
                virtual_key_f11 = 122,
                virtual_key_f12 = 123,
                virtual_key_f13 = 124,
                virtual_key_f14 = 125,
                virtual_key_f15 = 126,
                virtual_key_f16 = 127,
                virtual_key_f17 = 128,
                virtual_key_f18 = 129,
                virtual_key_f19 = 130,
                virtual_key_f20 = 131,
                virtual_key_f21 = 132,
                virtual_key_f22 = 133,
                virtual_key_f23 = 134,
                virtual_key_f24 = 135,
                virtual_key_navigation_view = 136,
                virtual_key_navigation_menu = 137,
                virtual_key_navigation_up = 138,
                virtual_key_navigation_down = 139,
                virtual_key_navigation_left = 140,
                virtual_key_navigation_right = 141,
                virtual_key_navigation_accept = 142,
                virtual_key_navigation_cancel = 143,
                virtual_key_number_key_lock = 144,
                virtual_key_scroll = 145,
                virtual_key_left_shift = 160,
                virtual_key_right_shift = 161,
                virtual_key_left_control = 162,
                virtual_key_right_control = 163,
                virtual_key_left_menu = 164,
                virtual_key_right_menu = 165,
                virtual_key_go_back = 166,
                virtual_key_go_forward = 167,
                virtual_key_refresh = 168,
                virtual_key_stop = 169,
                virtual_key_search = 170,
                virtual_key_favorites = 171,
                virtual_key_goHome = 172,
                virtual_key_gamepad_a = 195,
                virtual_key_gamepad_b = 196,
                virtual_key_gamepad_x = 197,
                virtual_key_gamepad_y = 198,
                virtual_key_gamepad_right_shoulder = 199,
                virtual_key_gamepad_left_shoulder = 200,
                virtual_key_gamepad_left_trigger = 201,
                virtual_key_gamepad_right_trigger = 202,
                virtual_key_gamepad_dpad_up = 203,
                virtual_key_gamepad_dpad_down = 204,
                virtual_key_gamepad_dpad_left = 205,
                virtual_key_gamepad_dpad_right = 206,
                virtual_key_gamepad_menu = 207,
                virtual_key_gamepad_view = 208,
                virtual_key_gamepad_left_thumbstick_button = 209,
                virtual_key_gamepad_right_thumbstick_button = 210,
                virtual_key_gamepad_left_thumbstick_up = 211,
                virtual_key_gamepad_left_thumbstick_down = 212,
                virtual_key_gamepad_left_thumbstick_right = 213,
                virtual_key_gamepad_left_thumbstick_left = 214,
                virtual_key_gamepad_right_thumbstick_up = 215,
                virtual_key_gamepad_right_thumbstick_down = 216,
                virtual_key_gamepad_right_thumbstick_right = 217,
                virtual_key_gamepad_right_thumbstick_left = 218
            };

            keyboard_state()
            {
                m_states[0] = 0;
                m_states[1] = 0;
                m_states[2] = 0;
                m_states[3] = 0;

                m_shadow_states[0] = 0;
                m_shadow_states[1] = 0;
                m_shadow_states[2] = 0;
                m_shadow_states[3] = 0;
            }

			~keyboard_state() = default;

			keyboard_state(const keyboard_state& o) = default;
			keyboard_state& operator=(const keyboard_state& o) = default;

			keyboard_state(keyboard_state&& o) = default;
			keyboard_state& operator=(keyboard_state&& o) = default;


            void swap()
            {
                m_shadow_states[0] = m_states[0];
                m_shadow_states[1] = m_states[1];
                m_shadow_states[2] = m_states[2];
                m_shadow_states[3] = m_states[3];

                m_character_codes.clear();
            }

            void set_state(uint64_t s[])
            {
                m_states[0] = s[0];
                m_states[1] = s[1];
                m_states[2] = s[2];
                m_states[3] = s[3];
            }

            void button_down(uint32_t button)
            {
                uint32_t byte = button / 64;
                uint32_t bit = button % 64;

                m_states[byte] = util::bit_set(m_states[byte],bit);
            }

            void button_up(uint32_t button)
            {
                uint32_t byte = button / 64;
                uint32_t bit = button % 64;

                m_states[byte] = util::bit_reset(m_states[byte], bit);
            }

            bool is_button_down(uint32_t button) const
            {
                uint32_t byte = button / 64;
                uint32_t bit = button % 64;

                return util::bit_is_set(m_states[byte], bit);
            }

            bool is_button_up(uint32_t button) const
            {
                uint32_t byte   = button / 64;
                uint32_t bit    = button % 64;
                return !util::bit_is_set(m_states[byte], bit);
            }

            bool is_button_toggled(uint32_t button) const
            {
                uint32_t byte = button / 64;
                uint32_t bit = button % 64;

                uint64_t difference = m_states[byte] ^ m_shadow_states[byte];
                return util::bit_is_set(difference, bit);
            }

			public:

            uint64_t m_states[4];
            uint64_t m_shadow_states[4];
            std::vector<uint32_t> m_character_codes;
        };

        inline bool button_is_down(const keyboard_state & state, uint32_t button)
        {
            return (state.is_button_down(button));
        }

        inline bool button_is_up(const keyboard_state & state, uint32_t button)
        {
            return (state.is_button_up(button));
        }

        inline bool button_was_pressed(const keyboard_state & state, uint32_t button)
        {
            return (state.is_button_down(button) && state.is_button_toggled(button));
        }

        inline bool button_was_released(const keyboard_state & state, uint32_t button)
        {
            return (state.is_button_up(button) && state.is_button_toggled(button));
        }
    }
}
