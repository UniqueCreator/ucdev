#pragma once

#include <mutex>
#include <vector>
#include <queue>

#include <uc_dev/mem/alloc.h>

#include <uc_dev/util/noncopyable.h>

#include <uc_dev/gx/dx12/api/helpers.h>
#include <uc_dev/gx/dx12/gpu/resource_create_context.h>
#include <uc_dev/gx/dx12/gpu/upload_buffer.h>
#include <uc_dev/gx/dx12/cmd/command_queue.h>

namespace uc
{
    namespace gx
    {
        namespace dx12
        {
            class gpu_upload_page : util::noncopyable
            {

                public:

                gpu_upload_page( std::unique_ptr<gpu_upload_buffer>&& buffer) : m_buffer( std::move(buffer) )
                {
                    map();
                }

                ~gpu_upload_page()
                {
                    unmap();
                    assert(m_cpu_address == 0); //must be unmapped on deletion
                }

                void* cpu_address() const
                {
                    return m_cpu_address;
                }

                D3D12_GPU_VIRTUAL_ADDRESS gpu_address() const
                {
                    return m_buffer->virtual_address();
                }

                void map()
                {
                    throw_if_failed(m_buffer->resource()->Map(0, nullptr, &m_cpu_address));
                }

                void unmap()
                {
                    m_buffer->resource()->Unmap(0, nullptr);
                    m_cpu_address = 0;
                }

                gpu_upload_buffer* resource() const
                {
                    return m_buffer.get();
                }

            private:

                void*                                  m_cpu_address;
                std::unique_ptr< gpu_upload_buffer  >  m_buffer;

            };

            struct gpu_upload_allocation
            {

            public:

                gpu_upload_allocation(gpu_upload_page* r, uint64_t offset) :
                    m_page(r)
                    , m_offset(offset)
                {

                }


                ~gpu_upload_allocation()
                {

                }

                uint64_t offset() const
                {
                    return m_offset;
                }

                void* cpu_address() const
                {
                    return reinterpret_cast<void*> (reinterpret_cast<uintptr_t>(m_page->cpu_address()) + offset());
                }

                D3D12_GPU_VIRTUAL_ADDRESS gpu_address() const
                {
                    return m_page->gpu_address() + offset();
                }

                gpu_virtual_resource* resource() const
                {
                    return m_page->resource();
                }

            private:
                gpu_upload_page*                m_page;
                uint64_t                        m_offset;

            };

            class gpu_upload_allocator : public util::noncopyable
            {

             private:

                enum cpu_page_size
                {
                    size_2mb  = 2 * 1024 * 1024U,
                    size_64kb = 64 * 1024U
                };

                static inline uint64_t max_(uint64_t alignment)
                {
                    return alignment > 32U ? alignment : 32U;
                }

                gpu_upload_page* allocate_page()
                {
                    return new gpu_upload_page(std::unique_ptr<gpu_upload_buffer>(m_rc->create_upload_buffer(2 * 1024 * 1024)));
                }

                gpu_upload_page* allocate_page(size_t size)
                {
                    return new gpu_upload_page(std::unique_ptr<gpu_upload_buffer>(m_rc->create_upload_buffer(size)));
                }

            public:

                gpu_upload_allocator( gpu_resource_create_context* m, cpu_page_size page_size = cpu_page_size::size_2mb) :
                    m_rc(m)
                    , m_offset(0)
                    , m_page_size(page_size)
                {

                }

                gpu_upload_allocation allocate( uint64_t size, uint64_t alignment = 256 )
                {
                    auto aligned_size   = mem::align(size, max_(alignment));
                    auto aligned_offset = mem::align(m_offset, max_(alignment));

                    if ( aligned_size < m_page_size )
                    {
                        if ( aligned_offset + aligned_size > m_page_size)
                        {
                            m_full_pages.emplace_back(std::move(m_page));
                        }

                        m_offset = aligned_offset;

                        if (m_page == nullptr)
                        {
                            m_page = std::unique_ptr<gpu_upload_page>(allocate_page());
                            m_offset = 0;
                        }

                        auto allocation = gpu_upload_allocation(m_page.get(), m_offset);

                        m_offset += aligned_size;

                        return allocation;
                    }
                    else
                    {
                        std::unique_ptr<gpu_upload_page> page = std::unique_ptr<gpu_upload_page>(allocate_page());
                        auto allocation = gpu_upload_allocation(m_page.get(), m_offset);
                        m_buffers.emplace_back(std::move(page));
                        return allocation;
                    }
                }

                void reset()
                {
                    m_full_pages.resize(0);
                    m_buffers.resize(0);
                    m_page.reset();
                    m_offset = 0;
                }

            protected:

                gpu_resource_create_context*                            m_rc;
                std::unique_ptr<gpu_upload_page>                        m_page;
                uint64_t                                                m_offset;
                cpu_page_size                                           m_page_size;

                std::vector<  std::unique_ptr<gpu_upload_page> >        m_full_pages;
                std::vector<  std::unique_ptr<gpu_upload_page> >        m_buffers;   //buffers above 2MB limit
            };
        }
    }
}
