// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"

#if defined(_X64)

#include <uc_dev/mem/streamflow_algorithm.h>

#pragma warning( disable:4447 )

BOOL APIENTRY DllMain( HMODULE,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    using namespace uc::mem::streamflow;

    switch (ul_reason_for_call)
    {
        case DLL_PROCESS_ATTACH:
        {
            if ( initialize() == initialization_code::success)
            {
                return (thread_initialize() == initialization_code::success) ? TRUE : FALSE;
            }
            else
            {
                return FALSE;
            }
        }

        case DLL_THREAD_ATTACH:
        {
            return ( thread_initialize() == initialization_code::success) ? TRUE : FALSE;
        }

        case DLL_THREAD_DETACH:
        {
            if ( lpReserved == nullptr)
            {
                thread_finalize();
            }
            break;
        }
        case DLL_PROCESS_DETACH:
        {
            if ( lpReserved == nullptr)
            {
                finalize();
            }
            break;
        }
        break;
    }
    return TRUE;
}
#else

#pragma warning( disable:4447 )

#include <uc_dev/mem/defines.h>

MEM_STREAMFLOW_DLL void dummy_function()
{

}

BOOL APIENTRY DllMain(HMODULE,
	DWORD,
	LPVOID
)
{
	return true;
}

#endif