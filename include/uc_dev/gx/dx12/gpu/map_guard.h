#pragma once

#include <uc_dev/util/noncopyable.h>

namespace uc
{
    namespace gx
    {
        namespace dx12
        {

            template <typename buffer>
            class gpu_map_guard : public util::noncopyable
            {
               public:

                gpu_map_guard(buffer* b, uint32_t offset = 0) :
                    m_buffer(b)
                    , m_offset(offset)
                {
                    m_address = m_buffer->map(offset);
                }

                ~gpu_map_guard()
                {
                    m_buffer->unmap(m_offset);
                }

                void* cpu_address() const
                {
                    return m_address;
                }

                private:
                buffer*    m_buffer;
                void*      m_address;
                uint32_t   m_offset;
            };
        }
    }
}