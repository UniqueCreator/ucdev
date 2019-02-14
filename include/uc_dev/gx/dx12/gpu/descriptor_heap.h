#pragma once

#include <assert.h>
#include <atomic>
#include <mutex>
#include <cstdint>
#include <vector>

#include <d3d12.h>
#include <wrl/client.h>

#include <uc_dev/gx/dx12/api/error.h>
#include <uc_dev/mem/ref_counter.h>
#include <uc_dev/util/noncopyable.h>

namespace uc
{
    namespace gx
    {
        namespace dx12
        {
            struct cpu_descriptor_handle : public D3D12_CPU_DESCRIPTOR_HANDLE
            {
                cpu_descriptor_handle()
                {
                    this->ptr = 0U;
                }
            };

            struct descriptor_handle
            {
                public:

                    descriptor_handle(  )
                    {
                        m_h0.ptr = 0U;
                        m_h1.ptr = 0U;
                    }

                    descriptor_handle(D3D12_CPU_DESCRIPTOR_HANDLE h0, D3D12_GPU_DESCRIPTOR_HANDLE h1) :
                        m_h0(h0)
                        , m_h1(h1)
                    {

                    }

                    operator D3D12_GPU_DESCRIPTOR_HANDLE() const
                    {
                        return m_h1;
                    }

                    operator D3D12_CPU_DESCRIPTOR_HANDLE() const
                    {
                        return m_h0;
                    }

                    descriptor_handle increment(uint64_t offset) const
                    {
                        return descriptor_handle({ m_h0.ptr + static_cast<size_t>(offset) }, { m_h1.ptr + offset });
                    }

                private:

                    D3D12_CPU_DESCRIPTOR_HANDLE m_h0;
                    D3D12_GPU_DESCRIPTOR_HANDLE m_h1;
            };

            struct incrementable_descriptor_handle
            {
                incrementable_descriptor_handle( descriptor_handle handle ,uint64_t increment_size ) : m_handle(handle), m_increment_size( increment_size )
                {

                }

                operator D3D12_GPU_DESCRIPTOR_HANDLE() const
                {
                    return static_cast<D3D12_GPU_DESCRIPTOR_HANDLE>(m_handle);
                }

                operator D3D12_CPU_DESCRIPTOR_HANDLE() const
                {
                    return static_cast<D3D12_CPU_DESCRIPTOR_HANDLE>(m_handle);
                }

                incrementable_descriptor_handle increment(uint64_t offset) const
                {
                    auto handle = m_handle.increment(offset * m_increment_size);
                    return incrementable_descriptor_handle(handle, m_increment_size);
                }

                incrementable_descriptor_handle operator+(uint64_t offset) const
                {
                    return increment(offset);
                }

                descriptor_handle m_handle;
                uint64_t          m_increment_size;
            };

            template <typename heap> struct heap_handle : public descriptor_handle
            {
                private:

                using base = descriptor_handle;

                public:

                using is_shader_visible = typename heap::is_shader_visible;

                using base::base;
            };

            struct default_increment_policy
            {
                protected:

                uint64_t increment_offset(uint32_t count)
                {
                    auto offset = m_offset;
                    m_offset += count;
                    return offset;
                }

                protected:

                uint64_t  m_offset = 0;
            };

            struct fenced_increment_policy
            {
                protected:

                    uint64_t increment_offset(uint32_t count)
                    {
                        return std::atomic_fetch_add(&m_offset, count);
                    }

                protected:

                    std::atomic<uint64_t>  m_offset = 0;
            };

            template <bool> struct shader_visible_type;
            template <> struct shader_visible_type<true>{};
            template <> struct shader_visible_type<false> {};

            //=================================================================================================================================
            //descriptor heap, which allocates up to a maximum. suitable for static data
            template < D3D12_DESCRIPTOR_HEAP_TYPE heap, D3D12_DESCRIPTOR_HEAP_FLAGS flags, class increment_policy >
            class gpu_descriptor_heap : protected increment_policy
            {
                using increment_policy::increment_offset;
                using this_type = gpu_descriptor_heap<heap, flags, increment_policy>;
                
                public:
                using is_shader_visible = typename shader_visible_type < (flags & D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE ) != 0 > ;

                using descriptor_handle = heap_handle<this_type>;

                gpu_descriptor_heap(ID3D12Device* device, UINT descriptorCount, UINT NodeMask = 0)
                {
                    D3D12_DESCRIPTOR_HEAP_DESC desc = {};

                    desc.Type = static_cast<D3D12_DESCRIPTOR_HEAP_TYPE> (heap);
                    desc.Flags = static_cast<D3D12_DESCRIPTOR_HEAP_FLAGS> (flags);
                    desc.NumDescriptors = descriptorCount;
                    desc.NodeMask = NodeMask;

                    throw_if_failed(device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&m_resource)));

                    m_cpu_begin = m_resource->GetCPUDescriptorHandleForHeapStart();
                    m_gpu_begin = m_resource->GetGPUDescriptorHandleForHeapStart();

                    m_increment_size = device->GetDescriptorHandleIncrementSize(desc.Type);
                }

                descriptor_handle allocate(uint32_t count)
                {
                    D3D12_CPU_DESCRIPTOR_HANDLE cpu;
                    D3D12_GPU_DESCRIPTOR_HANDLE gpu;

                    std::lock_guard < std::mutex> g(m_lock);
                    auto offset = increment_offset(count);

                    assert(!full(count));

                    cpu.ptr = static_cast<size_t>(m_cpu_begin.ptr   + offset * m_increment_size);
                    gpu.ptr = static_cast<uint64_t>(m_gpu_begin.ptr + offset * m_increment_size);

                    return descriptor_handle(cpu, gpu);
                }

                incrementable_descriptor_handle allocate_incrementable(uint32_t count)
                {
                    return incrementable(allocate(count));
                }

                descriptor_handle allocate()
                {
                    return allocate(1);
                }

                descriptor_handle increment( descriptor_handle h) const
                {
                    return h.increment(m_increment_size);
                }

                incrementable_descriptor_handle incrementable(descriptor_handle h) const
                {
                    return incrementable_descriptor_handle(h, m_increment_size);
                }

                bool full(uint32_t count) const
                {
                    auto desc = m_resource->GetDesc();
                    return (m_offset + count >= desc.NumDescriptors);
                }

                void reset()
                {
                    this->m_offset = 0;
                }

                ID3D12DescriptorHeap* resource() const
                {
                    return m_resource.Get();
                }

                uint64_t increment_size() const
                {
                    return m_increment_size;
                }

                D3D12_GPU_DESCRIPTOR_HANDLE heap_start() const
                {
                    return m_gpu_begin;
                }

            private:

                Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_resource;
                D3D12_CPU_DESCRIPTOR_HANDLE                  m_cpu_begin;
                D3D12_GPU_DESCRIPTOR_HANDLE                  m_gpu_begin;
                uint64_t                                     m_increment_size;

                std::mutex                                   m_lock;

            };

            //=================================================================================================================================
            //use this for data that lives longer and copy to the frame descriptor heaps
            //useful for single objects, this texture, this depth buffer, this vertex buffer
            template < uint32_t descriptor_heap_type, uint32_t descriptor_heap_flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE >
            class managed_gpu_descriptor_heap 
            {

            private:

                using heap             = Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>;
                using handle_container = std::vector< descriptor_handle >;
                using this_type        = managed_gpu_descriptor_heap<descriptor_heap_type, descriptor_heap_flags>;

            public:
                using is_shader_visible = typename shader_visible_type < (descriptor_heap_flags & D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE) != 0 >;
                using descriptor_handle = heap_handle<this_type>;

                managed_gpu_descriptor_heap(ID3D12Device* device, uint32_t descriptor_count, UINT NodeMask = 0)
                {
                    D3D12_DESCRIPTOR_HEAP_DESC desc = {};

                    desc.Type = static_cast<D3D12_DESCRIPTOR_HEAP_TYPE> (descriptor_heap_type);
                    desc.Flags = static_cast<D3D12_DESCRIPTOR_HEAP_FLAGS> (descriptor_heap_flags);
                    desc.NumDescriptors = descriptor_count;
                    desc.NodeMask = NodeMask;

                    throw_if_failed(device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&m_resource)));

                    m_cpu_begin = m_resource->GetCPUDescriptorHandleForHeapStart();
                    m_gpu_begin = m_resource->GetGPUDescriptorHandleForHeapStart();
                    
                    m_unallocated_offset = 1;
                    m_increment_size = device->GetDescriptorHandleIncrementSize(desc.Type);
                }

                ~managed_gpu_descriptor_heap()
                {
                    //memory leak might occur?
                    //assert(empty());
                }

                descriptor_handle allocate()
                {
                    assert(!full());

                    //if we do not have free
                    if (!m_recycled.empty())
                    {
                        auto r = m_recycled.back();
                        m_recycled.pop_back();
                        return r;
                    }
                    else
                    {
                        D3D12_CPU_DESCRIPTOR_HANDLE cpu;
                        D3D12_GPU_DESCRIPTOR_HANDLE gpu;

                        cpu.ptr = static_cast<SIZE_T>(m_cpu_begin.ptr + m_unallocated_offset * m_increment_size);
                        gpu.ptr = static_cast<SIZE_T>(m_gpu_begin.ptr + m_unallocated_offset * m_increment_size);

                        m_unallocated_offset++;
                        return descriptor_handle(cpu, gpu);
                    }
                }

                void free( descriptor_handle h )
                {
                    m_recycled.push_back(h);
                }

                bool empty() const
                {
                    return m_recycled.empty();
                }

                bool full() const
                {
                    return (m_recycled.size() == m_resource->GetDesc().NumDescriptors);
                }

                ID3D12DescriptorHeap* resource() const
                {
                    return m_resource.Get();
                }

            private:

                Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_resource;

                D3D12_CPU_DESCRIPTOR_HANDLE                  m_cpu_begin;
                D3D12_GPU_DESCRIPTOR_HANDLE                  m_gpu_begin;
                
                uint32_t                                     m_unallocated_offset;  //unallocated offset
                std::vector<descriptor_handle>               m_recycled;

                uint32_t                                     m_increment_size;
            };


            //heap allocation info
            template < typename persistent_heap > 
            struct descriptor_heap_handle 
            {
                using this_type = descriptor_heap_handle < persistent_heap >;
                using descriptor_handle = typename persistent_heap::descriptor_handle;

                persistent_heap*  m_heap = nullptr;
                descriptor_handle m_handle;

                static descriptor_heap_handle make(persistent_heap* heap)
                {
                    this_type r;

                    r.m_heap = heap;
                    r.m_handle = heap->allocate();

                    return r;
                }

                descriptor_handle handle() const
                {
                    return m_handle;
                }

                persistent_heap* heap() const
                {
                    return m_heap;
                }
            };

            //=================================================================================================================================
            using persistent_cpu_srv_descriptor_heap = managed_gpu_descriptor_heap< D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, D3D12_DESCRIPTOR_HEAP_FLAG_NONE >;
            using persistent_cpu_rtv_descriptor_heap = managed_gpu_descriptor_heap< D3D12_DESCRIPTOR_HEAP_TYPE_RTV, D3D12_DESCRIPTOR_HEAP_FLAG_NONE>;
            using persistent_cpu_dsv_descriptor_heap = managed_gpu_descriptor_heap< D3D12_DESCRIPTOR_HEAP_TYPE_DSV, D3D12_DESCRIPTOR_HEAP_FLAG_NONE>;

            using persistent_cpu_srv_descriptor_heap_handle = descriptor_heap_handle< persistent_cpu_srv_descriptor_heap >;
            using persistent_cpu_rtv_descriptor_heap_handle = descriptor_heap_handle< persistent_cpu_rtv_descriptor_heap >;
            using persistent_cpu_dsv_descriptor_heap_handle = descriptor_heap_handle< persistent_cpu_dsv_descriptor_heap >;

            using gpu_srv_descriptor_heap                   = gpu_descriptor_heap< D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE, fenced_increment_policy >;
            using gpu_rtv_descriptor_heap                   = gpu_descriptor_heap< D3D12_DESCRIPTOR_HEAP_TYPE_RTV, D3D12_DESCRIPTOR_HEAP_FLAG_NONE, fenced_increment_policy>;
            using gpu_dsv_descriptor_heap                   = gpu_descriptor_heap< D3D12_DESCRIPTOR_HEAP_TYPE_DSV, D3D12_DESCRIPTOR_HEAP_FLAG_NONE, fenced_increment_policy>;
            using gpu_sampler_descriptor_heap               = gpu_descriptor_heap< D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER, D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE, fenced_increment_policy >;


            using cpu_srv_descriptor_heap                   = gpu_descriptor_heap< D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, D3D12_DESCRIPTOR_HEAP_FLAG_NONE, fenced_increment_policy >;
            using cpu_rtv_descriptor_heap                   = gpu_descriptor_heap< D3D12_DESCRIPTOR_HEAP_TYPE_RTV, D3D12_DESCRIPTOR_HEAP_FLAG_NONE, fenced_increment_policy>;
            using cpu_dsv_descriptor_heap                   = gpu_descriptor_heap< D3D12_DESCRIPTOR_HEAP_TYPE_DSV, D3D12_DESCRIPTOR_HEAP_FLAG_NONE, fenced_increment_policy>;
            using cpu_sampler_descriptor_heap               = gpu_descriptor_heap< D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER, D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE, fenced_increment_policy >;

        }
    }
}
