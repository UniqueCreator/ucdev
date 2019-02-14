#include "pch.h"

#include <uc_dev/mem/streamflow.h>
#include <memory>

//comment this, if you do not want c++ allocations to go through streamflow
//#define MEM_STREAMFLOW_OVERRIDE_OPERATOR_NEW

#if defined(MEM_STREAMFLOW_OVERRIDE_OPERATOR_NEW)

namespace
{
    static inline std::size_t align_16(std::size_t s)
    {
        return s + (16 - 1) & ~(16);
    }
}

//---------------------------------------------------------------------------------------
void* operator new(std::size_t size)
{
    using namespace uc;
    void* result = mem::streamflow::get_heap(0)->allocate( align_16(size) );

    if (result == nullptr)
    {
        throw std::bad_alloc();
    }

    return result;
}

void operator delete(void* pointer) throw()
{
    using namespace uc;
    if (pointer != nullptr)
    {
        mem::streamflow::get_heap(0)->free(pointer);
    }
}
//---------------------------------------------------------------------------------------
void* operator new   (std::size_t size, const std::nothrow_t&) throw()
{
    using namespace uc;
    return mem::streamflow::get_heap(0)->allocate(align_16(size) );
}

void operator delete (void* pointer, const std::nothrow_t&) throw()
{
    using namespace uc;
    if (pointer != nullptr)
    {
        mem::streamflow::get_heap(0)->free(pointer);
    }
}
//---------------------------------------------------------------------------------------
void* operator new  [](std::size_t size)
{
    using namespace uc;
    void* result = mem::streamflow::get_heap(0)->allocate( align_16(size) );

    if (result == nullptr)
    {
        throw std::bad_alloc();
    }

    return result;
}

void operator delete[](void* pointer) throw()
{
    using namespace uc;
    if (pointer != nullptr)
    {
        mem::streamflow::get_heap(0)->free(pointer);
    }
}
//---------------------------------------------------------------------------------------
void* operator new  [](std::size_t size, const std::nothrow_t&) throw()
{
    using namespace uc;
    return mem::streamflow::get_heap(0)->allocate( align_16(size) );
}

void operator delete[](void* pointer, const std::nothrow_t&) throw()
{
    using namespace uc;
    if (pointer != nullptr)
    {
        mem::streamflow::get_heap(0)->free(pointer);
    }
}
//---------------------------------------------------------------------------------------
#endif