#pragma once

#if defined(UWP)

#if defined( UCDEV_RENDER_IMPORT )
    #define UC_RENDER_DLL __declspec(dllimport)
#elif defined( UCDEV_RENDER_EXPORT )
    #define UC_RENDER_DLL __declspec(dllexport)
#else
    #define UC_RENDER_DLL 
#endif

#endif
