#pragma once

#include <cstdint>
#include <memory>


#if defined( UC_ENGINE_API_IMPORT )
    #define UC_ENGINE_API __declspec(dllimport)
#elif defined( UC_ENGINE_API_EXPORT )
    #define UC_ENGINE_API __declspec(dllexport)
#else
    #define UC_ENGINE_API 
#endif



