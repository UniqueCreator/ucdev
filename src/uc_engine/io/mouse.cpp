#include "pch.h"

#include <uc_public/io/mouse.h>
#include <uc_public/uc_pimpl_impl.h>
#include <uc_dev/io/mouse.h>

namespace UniqueCreator
{
    namespace InputOutput
    {
        class Mouse::Impl
        {
        public:

            Mouse::Impl(const CreateMouseParams& p)
            {
                m_native_mouse.set_window(p.m_window, p.m_dpi);
            }

            MouseState Sample()
            {
                uc::io::mouse_state s = m_native_mouse.update(uc::io::mouse_state());
                MouseState r = { s.m_state.m_buttons, s.m_state.m_x, s.m_state.m_y };
                return r;
            }

            uc::io::mouse m_native_mouse;
        };

        Mouse::Mouse(const CreateMouseParams& p) : m_impl(p)
        {

        }

        Mouse::~Mouse() = default;
        Mouse& Mouse::operator=(Mouse&&) = default;

        MouseState Mouse::Sample()
        {
            return m_impl->Sample();
        }
    }
}
