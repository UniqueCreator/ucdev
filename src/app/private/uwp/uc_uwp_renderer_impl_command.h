#pragma once

#include <cstdint>

#include <winrt/windows.foundation.h>
#include <winrt/windows.ui.core.h>

#include <wrl/client.h>
#include <wrl/wrappers/corewrappers.h>
#include <wrl/event.h>

#include "uc_uwp_renderer_impl_window.h"

namespace uc
{
    namespace uwp
    {
        //add other commands here with variant
        struct resize_window_command
        {
            window_environment                                                           m_window_environment;
        };
    }
    
}