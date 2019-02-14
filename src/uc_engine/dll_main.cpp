// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"

#pragma warning( disable:4447 )

BOOL APIENTRY DllMain( HMODULE,
                       DWORD  ul_reason_for_call,
                       LPVOID 
                     )
{

    switch (ul_reason_for_call)
    {
        case DLL_PROCESS_ATTACH:
        {
            break;
        }

        case DLL_THREAD_ATTACH:
        {
            break;
        }

        case DLL_THREAD_DETACH:
        {
            break;
        }
        case DLL_PROCESS_DETACH:
        {
            break;
        }
        break;
    }
    return TRUE;
}

