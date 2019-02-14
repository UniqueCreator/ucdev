#pragma once

#if defined( MEM_STREAMFLOW_DLL_IMPORT )
#define MEM_STREAMFLOW_DLL __declspec(dllimport)
#elif defined( MEM_STREAMFLOW_DLL_EXPORT )
#define MEM_STREAMFLOW_DLL __declspec(dllexport)
#else
#define MEM_STREAMFLOW_DLL 
#endif
