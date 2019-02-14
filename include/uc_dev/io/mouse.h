#pragma once

#include <cstdint>
#include <windows.h>

#include <uc_dev/util/bits.h>
#include <uc_dev/util/noncopyable.h>

#if defined(_PC)
    #include <uc_dev/io/platforms/pc/native_mouse.h>
#endif

namespace uc
{
    namespace io
    {
        struct mouse : public native_mouse, private util::noncopyable
        {
            using base = native_mouse;

            mouse_state update(const mouse_state& o)
            {
                return base::update(o);
            }
        };
    };
}

