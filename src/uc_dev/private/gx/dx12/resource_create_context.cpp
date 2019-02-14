#include "pch.h"

#include <uc_dev/gx/dx12/gpu/resource_create_context.h>
#include <uc_dev/util/pimpl_impl.h>

#include <uc_dev/gx/dx12/api/error.h>
#include <uc_dev/gx/dx12/api/helpers.h>
#include <uc_dev/gx/dx12/gpu/descriptions.h>
#include <uc_dev/gx/dx12/gpu/descriptor_heap.h>
#include <uc_dev/gx/dx12/gpu/allocators/buddy_allocator.h>

#include <uc_dev/gx/dx12/gpu/allocators/placement_allocator.h>
#include <uc_dev/gx/dx12/gpu/allocators/buddy_allocator.h>
#include <uc_dev/gx/dx12/gpu/allocators/coalesceable_allocator.h>

#include <concurrent_vector.h>

#include <uc_dev/gx/dx12/gpu/back_buffer.h>
#include <uc_dev/gx/dx12/gpu/buffer.h>
#include <uc_dev/gx/dx12/gpu/color_buffer.h>
#include <uc_dev/gx/dx12/gpu/depth_buffer.h>
#include <uc_dev/gx/dx12/gpu/read_back_buffer.h>
#include <uc_dev/gx/dx12/gpu/texture_2d.h>
#include <uc_dev/gx/dx12/gpu/texture_2d_array.h>
#include <uc_dev/gx/dx12/gpu/upload_buffer.h>


namespace uc
{
    namespace gx
    {
        namespace dx12
        {
            namespace details
            {

                static inline Microsoft::WRL::ComPtr<ID3D12Heap> create_upload_heap(ID3D12Device* device, uint64_t size, D3D12_HEAP_FLAGS flags)
                {
                    D3D12_HEAP_DESC d = upload_heap(size, flags);
                    Microsoft::WRL::ComPtr<ID3D12Heap> result;

                    throw_if_failed(device->CreateHeap(&d, IID_PPV_ARGS(&result)));
                    return result;
                }

                static inline Microsoft::WRL::ComPtr<ID3D12Heap> create_upload_buffer_heap(ID3D12Device* device, uint64_t size)
                {
                    return create_upload_heap(device, size, D3D12_HEAP_FLAG_ALLOW_ONLY_BUFFERS);
                }

                static inline Microsoft::WRL::ComPtr<ID3D12Heap> create_upload_texture_heap(ID3D12Device* device, uint64_t size)
                {
                    return create_upload_heap(device, size, D3D12_HEAP_FLAG_ALLOW_ONLY_RT_DS_TEXTURES);
                }

                static inline Microsoft::WRL::ComPtr<ID3D12Heap> create_read_back_heap(ID3D12Device* device, uint64_t size, D3D12_HEAP_FLAGS flags)
                {
                    D3D12_HEAP_DESC d = read_back_heap(size, flags);
                    Microsoft::WRL::ComPtr<ID3D12Heap> result;
                    throw_if_failed(device->CreateHeap(&d, IID_PPV_ARGS(&result)));
                    return result;
                }

                static inline Microsoft::WRL::ComPtr<ID3D12Heap> create_default_heap(ID3D12Device* device, uint64_t size, D3D12_HEAP_FLAGS flags)
                {
                    D3D12_HEAP_DESC d = default_heap(size, flags);
                    Microsoft::WRL::ComPtr<ID3D12Heap> result;

                    throw_if_failed(device->CreateHeap(&d, IID_PPV_ARGS(&result)));
                    return result;
                }

                static inline Microsoft::WRL::ComPtr<ID3D12Heap> create_render_target_heap(ID3D12Device* device, uint64_t size, D3D12_HEAP_FLAGS flags)
                {
                    D3D12_HEAP_DESC d = render_target_heap(size, flags);
                    Microsoft::WRL::ComPtr<ID3D12Heap> result;

                    throw_if_failed(device->CreateHeap(&d, IID_PPV_ARGS(&result)));
                    return result;
                }


                static inline auto create_upload_buffers_allocator(ID3D12Device* d, uint64_t size)
                {
                    return new placement_heap_allocator<locking_policy_mutex>(d, create_upload_heap(d, size, D3D12_HEAP_FLAG_ALLOW_ONLY_BUFFERS), size);
                }

                static inline auto create_upload_textures_allocator(ID3D12Device* d, uint64_t size)
                {
                    return new placement_heap_allocator<locking_policy_mutex>(d, create_upload_heap(d, size, D3D12_HEAP_FLAG_ALLOW_ONLY_RT_DS_TEXTURES), size);
                }

                static inline auto create_read_back_textures_allocator(ID3D12Device* d, uint64_t size)
                {
                    return new placement_heap_allocator<locking_policy_mutex>(d, create_read_back_heap(d, size, D3D12_HEAP_FLAG_ALLOW_ONLY_RT_DS_TEXTURES), size);
                }

                static inline auto create_default_render_target_allocator(ID3D12Device* d, uint64_t size)
                {
                    return new placement_heap_allocator<locking_policy_mutex>(d, create_render_target_heap(d, size, D3D12_HEAP_FLAG_ALLOW_ONLY_RT_DS_TEXTURES), size);
                }

                static inline auto create_default_heap_textures_allocator(ID3D12Device* d, uint64_t size)
                {
                    return new buddy_heap_allocator(d, create_default_heap(d, size, D3D12_HEAP_FLAG_ALLOW_ONLY_NON_RT_DS_TEXTURES), size);
                }

                static inline auto create_default_heap_geometry_allocator(ID3D12Device* d, uint64_t size)
                {
                    return new coalesceable_heap_allocator(d, create_default_heap(d, size, D3D12_HEAP_FLAG_ALLOW_ONLY_BUFFERS), size);
                }

                static inline auto create_null_resource(ID3D12Device* d)
                {
                    auto desc = describe_buffer(65536);
                    auto heap = default_properties();

                    desc.Flags |= D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;
                    auto r = create_committed_resource(d, &heap, D3D12_HEAP_FLAG_NONE, &desc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr);
                    return new gpu_virtual_resource(r.Get());
                }

                static inline auto create_sampler(D3D12_TEXTURE_ADDRESS_MODE mode, D3D12_FILTER filter)
                {
                    D3D12_SAMPLER_DESC s = {};

                    s.Filter = filter;

                    s.AddressU = mode;
                    s.AddressV = mode;
                    s.AddressW = mode;
                    s.MipLODBias = 0.0f;
                    s.MinLOD = 0.0f;
                    s.MaxLOD = D3D12_FLOAT32_MAX;
                    s.MaxAnisotropy = 1;
                    s.ComparisonFunc = D3D12_COMPARISON_FUNC_ALWAYS;
                    return s;
                }
            }

            class gpu_resource_create_context::gpu_resource_create_context_impl
            {
                public:

                using placement_heap_allocator = placement_heap_allocator<locking_policy_mutex>;

                placement_heap_allocator* upload_allocator() const;
                placement_heap_allocator* read_back_allocator() const;
                buddy_heap_allocator*   textures_allocator() const;
                coalesceable_heap_allocator*   geometry_allocator() const;

                Microsoft::WRL::ComPtr<ID3D12Device>              m_device;

                //frame heaps for the command contexts
                gpu_dsv_descriptor_heap                           m_frame_dsv_heap[3];
                gpu_rtv_descriptor_heap                           m_frame_rtv_heap[3];

                gpu_srv_descriptor_heap                           m_frame_gpu_srv_heap[3];
                cpu_srv_descriptor_heap                           m_frame_cpu_srv_heap[3];

                gpu_sampler_descriptor_heap                       m_frame_gpu_sampler_heap[3];
                cpu_sampler_descriptor_heap                       m_frame_cpu_sampler_heap[3];
                std::unique_ptr< placement_heap_allocator>        m_frame_render_target_allocator;

                std::unique_ptr< placement_heap_allocator >       m_upload_allocator[3];       //holds only buffers
                std::unique_ptr< placement_heap_allocator>        m_read_back_allocator[3];

                //window dependent resources, frame buffer, view dependent depth buffer, or frame dependent resources
                //shadow maps, hdr buffers, light buffers, 

                //view dependent resources
                gpu_dsv_descriptor_heap                           m_view_dependent_dsv_heap;
                cpu_srv_descriptor_heap                           m_view_dependent_srv_heap;
                gpu_rtv_descriptor_heap                           m_view_dependent_rtv_heap;
                std::unique_ptr< placement_heap_allocator >       m_view_dependent_render_target_allocator;

                persistent_cpu_srv_descriptor_heap                m_textures_descriptor_heap;  //textures descriptor heap
                std::unique_ptr<buddy_heap_allocator>             m_textures_allocator;        //textures heap

                std::unique_ptr<coalesceable_heap_allocator>      m_geometry_heap;             //textures heap

                cpu_sampler_descriptor_heap                       m_sampler_descriptor_heap;   //samplers for the whole application,

                cpu_srv_descriptor_heap                           m_null_resource_srv_heap;
                cpu_sampler_descriptor_heap                       m_null_sampler_heap;
                std::unique_ptr< gpu_virtual_resource >           m_null_resource;

                descriptor_handle                                 m_null_srv;
                descriptor_handle                                 m_null_uav;
                descriptor_handle                                 m_null_cbv;
                descriptor_handle                                 m_null_sampler;

                descriptor_handle                                 m_point_clamp_sampler;
                descriptor_handle                                 m_point_wrap_sampler;
                descriptor_handle                                 m_linear_clamp_sampler;
                descriptor_handle                                 m_linear_wrap_sampler;

                uint32_t                                          m_frame_index;

                //textures
                //todo: add concurrent queues
                std::mutex                                                              m_delete_textures_mutex;
                concurrency::concurrent_vector< gpu_texture_2d* >                       m_frame_delete_texture_2d[3];
                concurrency::concurrent_vector< gpu_read_write_texture_2d* >            m_frame_delete_read_write_texture_2d[3];
                concurrency::concurrent_vector< gpu_texture_2d_array* >                 m_frame_delete_texture_2d_array[3];

                //buffers
                //todo: add concurrent queues
                std::mutex                                          m_delete_buffers_mutex;
                concurrency::concurrent_vector< gpu_buffer* >       m_frame_delete_buffers[3];

                gpu_resource_create_context_impl(ID3D12Device* d);

                void    free_texture_2d_internal(gpu_texture_2d* texture);
                void    flush_deleted_texture_2d(uint32_t frame_index);

                void    free_texture_2d_array_internal(gpu_texture_2d_array* texture);
                void    flush_deleted_texture_2d_array(uint32_t frame_index);

                void    free_read_write_texture_2d_internal(gpu_read_write_texture_2d* texture);
                void    flush_deleted_read_write_texture_2d(uint32_t frame_index);


                void    free_buffer_internal(gpu_buffer* buffer);
                void    flush_deleted_buffers(uint32_t frame_index);
            };
            
            gpu_resource_create_context::gpu_resource_create_context_impl::gpu_resource_create_context_impl(ID3D12Device* device) :
                m_device(device)
                , m_frame_dsv_heap{ { device, 1024 },{ device, 1024 },{ device, 1024 } }
                , m_frame_rtv_heap{ { device, 1024 },{ device, 1024 },{ device, 1024 } }
                , m_frame_gpu_srv_heap{ { device, 2048 },{ device, 2048 },{ device, 2048 } }
                , m_frame_cpu_srv_heap{ { device, 2048 },{ device, 2048 },{ device, 2048 } }
                , m_frame_gpu_sampler_heap{ { device, 1024 },{ device, 1024 },{ device, 1024 } }
                , m_frame_cpu_sampler_heap{ { device, 1024 },{ device, 1024 },{ device, 1024 } }

                , m_view_dependent_dsv_heap(device, 256)
                , m_view_dependent_srv_heap(device, 256)
                , m_view_dependent_rtv_heap(device, 256)

                , m_textures_descriptor_heap(device, 256)
                , m_sampler_descriptor_heap(device, 16)
                , m_null_resource_srv_heap(device, 16)
                , m_null_sampler_heap(device, 16)
                , m_frame_index(0)
            {

            }

            //template uc::util::details::pimpl<gpu_resource_create_context::gpu_resource_create_context_impl>;

            gpu_resource_create_context::gpu_resource_create_context(ID3D12Device* device) : m_impl(device)
            {
                using placement_heap_allocator = gpu_resource_create_context::gpu_resource_create_context_impl::placement_heap_allocator;

                m_impl->m_upload_allocator[0]       = std::unique_ptr< placement_heap_allocator >(details::create_upload_buffers_allocator(device, mb(32)));         //allocators for uploading resources
                m_impl->m_upload_allocator[1]       = std::unique_ptr< placement_heap_allocator >(details::create_upload_buffers_allocator(device, mb(32)));         //allocators for uploading resources
                m_impl->m_upload_allocator[2]       = std::unique_ptr< placement_heap_allocator >(details::create_upload_buffers_allocator(device, mb(32)));         //allocators for uploading resources

                m_impl->m_read_back_allocator[0]    = std::unique_ptr< placement_heap_allocator >(details::create_read_back_textures_allocator(device, mb(8)));        //allocators for downloading resources
                m_impl->m_read_back_allocator[1]    = std::unique_ptr< placement_heap_allocator >(details::create_read_back_textures_allocator(device, mb(8)));        //allocators for downloading resources
                m_impl->m_read_back_allocator[2]    = std::unique_ptr< placement_heap_allocator >(details::create_read_back_textures_allocator(device, mb(8)));        //allocators for downloading resources

                m_impl->m_view_dependent_render_target_allocator   = std::unique_ptr< placement_heap_allocator >(details::create_default_render_target_allocator(device, mb(32)));    //per view render targets and depth buffers, their lifetime depends on the view
                m_impl->m_frame_render_target_allocator            = std::unique_ptr< placement_heap_allocator >(details::create_default_render_target_allocator(device, mb(192)));   //per frame render targets and depth buffers, their lifetime depends on the frame
                m_impl->m_textures_allocator                       = std::unique_ptr< buddy_heap_allocator>     (details::create_default_heap_textures_allocator(device, mb(128)));   //static data, default textures go here for example, that live for the entire application

                m_impl->m_geometry_heap             = std::unique_ptr< coalesceable_heap_allocator>(details::create_default_heap_geometry_allocator(device, mb(128)));  //static data, geometry and index buffers go here
                m_impl->m_null_resource             = std::unique_ptr< gpu_virtual_resource>(details::create_null_resource(device));

                m_impl->m_null_srv                  = m_impl->m_null_resource_srv_heap.allocate(1);
                m_impl->m_null_uav                  = m_impl->m_null_resource_srv_heap.allocate(1);
                m_impl->m_null_cbv                  = m_impl->m_null_resource_srv_heap.allocate(1);
                m_impl->m_null_sampler              = m_impl->m_null_sampler_heap.allocate();

                D3D12_CPU_DESCRIPTOR_HANDLE h0 = m_impl->m_null_srv;
                D3D12_CPU_DESCRIPTOR_HANDLE h1 = m_impl->m_null_uav;
                D3D12_CPU_DESCRIPTOR_HANDLE h2 = m_impl->m_null_cbv;
                D3D12_CPU_DESCRIPTOR_HANDLE h3 = m_impl->m_null_sampler;

                
                {
                    D3D12_SHADER_RESOURCE_VIEW_DESC srv_desc = {};
                    srv_desc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
                    srv_desc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
                    srv_desc.Buffer.FirstElement = 0;
                    srv_desc.Format = DXGI_FORMAT_UNKNOWN;
                    srv_desc.Buffer.NumElements = 1;
                    srv_desc.Buffer.StructureByteStride = 1;
                    srv_desc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
                    device->CreateShaderResourceView(m_impl->m_null_resource->resource(), &srv_desc, h0);
                }
                
                {
                    D3D12_UNORDERED_ACCESS_VIEW_DESC uav_desc = {};
                    uav_desc.ViewDimension = D3D12_UAV_DIMENSION_BUFFER;
                    uav_desc.Buffer.CounterOffsetInBytes = 0;
                    uav_desc.Buffer.FirstElement = 0;
                    uav_desc.Format = DXGI_FORMAT_UNKNOWN;
                    uav_desc.Buffer.NumElements = 1;
                    uav_desc.Buffer.StructureByteStride = 1;
                    uav_desc.Buffer.Flags = D3D12_BUFFER_UAV_FLAG_NONE;
                    device->CreateUnorderedAccessView(m_impl->m_null_resource->resource(), nullptr, &uav_desc, h1);
                }

                {
                    D3D12_CONSTANT_BUFFER_VIEW_DESC cbv_desc = {};
                    cbv_desc.BufferLocation = m_impl->m_null_resource->virtual_address();
                    cbv_desc.SizeInBytes    = 256;
                    device->CreateConstantBufferView(&cbv_desc, h2);
                }

                {
                    D3D12_SAMPLER_DESC s = details::create_sampler(D3D12_TEXTURE_ADDRESS_MODE_CLAMP, D3D12_FILTER_MIN_MAG_MIP_POINT);
                    device->CreateSampler(&s, h3);
                }

                m_impl->m_point_clamp_sampler   = m_impl->m_sampler_descriptor_heap.allocate(1);
                m_impl->m_point_wrap_sampler    = m_impl->m_sampler_descriptor_heap.allocate(1);
                m_impl->m_linear_clamp_sampler  = m_impl->m_sampler_descriptor_heap.allocate(1);
                m_impl->m_linear_wrap_sampler   = m_impl->m_sampler_descriptor_heap.allocate(1);

                {
                    D3D12_SAMPLER_DESC s = details::create_sampler(D3D12_TEXTURE_ADDRESS_MODE_CLAMP, D3D12_FILTER_MIN_MAG_MIP_POINT);
                    device->CreateSampler(&s, m_impl->m_point_clamp_sampler);
                }

                {
                    D3D12_SAMPLER_DESC s = details::create_sampler(D3D12_TEXTURE_ADDRESS_MODE_WRAP, D3D12_FILTER_MIN_MAG_MIP_POINT);
                    device->CreateSampler(&s, m_impl->m_point_wrap_sampler);
                }

                {
                    D3D12_SAMPLER_DESC s = details::create_sampler(D3D12_TEXTURE_ADDRESS_MODE_CLAMP, D3D12_FILTER_MIN_MAG_MIP_LINEAR);
                    device->CreateSampler(&s, m_impl->m_linear_clamp_sampler);
                }

                {
                    D3D12_SAMPLER_DESC s = details::create_sampler(D3D12_TEXTURE_ADDRESS_MODE_WRAP, D3D12_FILTER_MIN_MAG_MIP_LINEAR);
                    device->CreateSampler(&s, m_impl->m_linear_wrap_sampler);
                }

                
            }

            descriptor_handle gpu_resource_create_context::null_srv() const
            {
                return m_impl->m_null_srv;
            }

            descriptor_handle gpu_resource_create_context::null_uav() const
            {
                return m_impl->m_null_uav;
            }

            descriptor_handle gpu_resource_create_context::null_cbv() const
            {
                return m_impl->m_null_cbv;
            }

            descriptor_handle gpu_resource_create_context::null_sampler() const
            {
                return m_impl->m_null_sampler;
            }

            descriptor_handle gpu_resource_create_context::point_clamp_sampler()  const
            {
                return m_impl->m_point_clamp_sampler;
            }

            descriptor_handle gpu_resource_create_context::point_wrap_sampler()   const
            {
                return m_impl->m_point_wrap_sampler;
            }

            descriptor_handle gpu_resource_create_context::linear_clamp_sampler() const
            {
                return m_impl->m_linear_clamp_sampler;
            }

            descriptor_handle gpu_resource_create_context::linear_wrap_sampler()  const
            {
                return m_impl->m_linear_wrap_sampler;
            }

            gpu_texture_2d* gpu_resource_create_context::create_texture_2d(uint32_t width, uint32_t height, DXGI_FORMAT format, uint32_t mip_count)
            {
                auto desc = describe_texture_2d(width, height, format, 0);

                Microsoft::WRL::ComPtr<ID3D12Resource>  resource;
                auto allocator = m_impl->textures_allocator();

                persistent_cpu_srv_descriptor_heap_handle srv;

                {
                    std::lock_guard< std::mutex  > lock(m_impl->m_delete_textures_mutex);
                    resource = allocator->create_placed_resource(&desc, D3D12_RESOURCE_STATE_COMMON);

                    resource->SetName(L"Texture 2D");

                    srv = persistent_cpu_srv_descriptor_heap_handle::make(&m_impl->m_textures_descriptor_heap);
                }

                D3D12_SHADER_RESOURCE_VIEW_DESC  descSRV = {};
                D3D12_UNORDERED_ACCESS_VIEW_DESC descUAV = {};

                descSRV.Format = format;
                descSRV.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
                descSRV.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
                descSRV.Texture2D.MipLevels = mip_count;
                descSRV.Texture2D.MostDetailedMip = 0;
                descSRV.Texture2D.PlaneSlice = 0;

                m_impl->m_device->CreateShaderResourceView(resource.Get(), &descSRV, srv.handle());

                return new gpu_texture_2d(resource.Get(), std::move(srv));
            }

            gpu_read_write_texture_2d* gpu_resource_create_context::create_read_write_texture_2d(uint32_t width, uint32_t height, DXGI_FORMAT format, uint32_t mip_count)
            {
                auto desc = describe_texture_2d(width, height, format, D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS);

                Microsoft::WRL::ComPtr<ID3D12Resource>  resource;
                auto allocator = m_impl->textures_allocator();

                persistent_cpu_srv_descriptor_heap_handle uav;
                persistent_cpu_srv_descriptor_heap_handle srv;

                {
                    std::lock_guard< std::mutex  > lock(m_impl->m_delete_textures_mutex);
                    resource = allocator->create_placed_resource(&desc, D3D12_RESOURCE_STATE_COMMON);

                    resource->SetName(L"Texture 2D");

                    uav = persistent_cpu_srv_descriptor_heap_handle::make(&m_impl->m_textures_descriptor_heap);
                    srv = persistent_cpu_srv_descriptor_heap_handle::make(&m_impl->m_textures_descriptor_heap);
                }

                D3D12_SHADER_RESOURCE_VIEW_DESC  descSRV = {};
                D3D12_UNORDERED_ACCESS_VIEW_DESC descUAV = {};

                descSRV.Format = format;
                descSRV.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
                descSRV.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
                descSRV.Texture2D.MipLevels = mip_count;
                descSRV.Texture2D.MostDetailedMip = 0;
                descSRV.Texture2D.PlaneSlice = 0;

                descUAV.Format = get_uav_format(format);
                descUAV.ViewDimension = D3D12_UAV_DIMENSION_TEXTURE2D;
                descUAV.Texture2D.MipSlice = 0;
                descUAV.Texture2D.PlaneSlice = 0;

                m_impl->m_device->CreateUnorderedAccessView(resource.Get(), nullptr, &descUAV, uav.handle());
                m_impl->m_device->CreateShaderResourceView(resource.Get(), &descSRV, srv.handle());

                return new gpu_read_write_texture_2d(resource.Get(), std::move(uav), std::move(srv));
            }

            //texture2d
            gpu_texture_2d* gpu_resource_create_context::create_texture_2d( uint32_t width, uint32_t height, DXGI_FORMAT format )
            {
                return create_texture_2d(width, height, format, 1);
            }

            gpu_texture_2d_array* gpu_resource_create_context::create_texture_2d_array(uint32_t width, uint32_t height, DXGI_FORMAT format, uint32_t elements)
            {
                auto desc = describe_texture(width, height, elements, 1, format, 0);

                Microsoft::WRL::ComPtr<ID3D12Resource>  resource;
                auto allocator = m_impl->textures_allocator();

                persistent_cpu_srv_descriptor_heap_handle srv;

                {
                    std::lock_guard< std::mutex  > lock(m_impl->m_delete_textures_mutex);
                    resource = allocator->create_placed_resource(&desc, D3D12_RESOURCE_STATE_COMMON);

                    resource->SetName(L"Texture 2D Array");

                    srv = persistent_cpu_srv_descriptor_heap_handle::make(&m_impl->m_textures_descriptor_heap);
                }

                D3D12_SHADER_RESOURCE_VIEW_DESC  descSRV = {};
                D3D12_UNORDERED_ACCESS_VIEW_DESC descUAV = {};

                descSRV.Format                          = format;
                descSRV.ViewDimension                   = D3D12_SRV_DIMENSION_TEXTURE2DARRAY;
                descSRV.Shader4ComponentMapping         = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
                descSRV.Texture2DArray.ArraySize        = elements;
                descSRV.Texture2DArray.FirstArraySlice  = 0;
                descSRV.Texture2DArray.MipLevels        = 1;
                descSRV.Texture2DArray.MostDetailedMip  = 0;
                descSRV.Texture2DArray.PlaneSlice       = 0;
                
                m_impl->m_device->CreateShaderResourceView(resource.Get(), &descSRV, srv.handle());
                return new gpu_texture_2d_array(resource.Get(), std::move(srv));
            }

            Microsoft::WRL::ComPtr<ID3D12Resource> gpu_resource_create_context::create_upload_buffer_resource(uint64_t size)
            {
                auto desc = describe_buffer(size);
                Microsoft::WRL::ComPtr<ID3D12Resource>  resource;
                auto allocator = m_impl->upload_allocator();

                return allocator->create_placed_resource(&desc, D3D12_RESOURCE_STATE_GENERIC_READ);
            }

            void gpu_resource_create_context::free_texture_2d(gpu_texture_2d* texture)
            {
                std::lock_guard< std::mutex  > lock(m_impl->m_delete_textures_mutex);
                auto& textures = m_impl->m_frame_delete_texture_2d[m_impl->m_frame_index];
                textures.push_back(texture);
            }

            void gpu_resource_create_context::free_read_write_texture_2d( gpu_read_write_texture_2d* texture )
            {
                std::lock_guard< std::mutex  > lock(m_impl->m_delete_textures_mutex);
                auto& textures = m_impl->m_frame_delete_read_write_texture_2d[m_impl->m_frame_index];
                textures.push_back(texture);
            }

            void gpu_resource_create_context::free_texture_2d_array(gpu_texture_2d_array* texture)
            {
                std::lock_guard< std::mutex  > lock(m_impl->m_delete_textures_mutex);
                auto& textures = m_impl->m_frame_delete_texture_2d_array[m_impl->m_frame_index];
                textures.push_back(texture);
            }

            void gpu_resource_create_context::gpu_resource_create_context_impl::free_texture_2d_internal( gpu_texture_2d* texture )
            {
                auto allocator = textures_allocator();
                allocator->free_placed_resource( texture->resource() );
                delete texture;
            }

            void gpu_resource_create_context::gpu_resource_create_context_impl::free_read_write_texture_2d_internal(gpu_read_write_texture_2d* texture)
            {
                auto allocator = textures_allocator();
                allocator->free_placed_resource(texture->resource());
                delete texture;
            }

            void gpu_resource_create_context::gpu_resource_create_context_impl::free_texture_2d_array_internal(gpu_texture_2d_array* texture)
            {
                auto allocator = textures_allocator();
                allocator->free_placed_resource(texture->resource());
                delete texture;
            }

            void gpu_resource_create_context::gpu_resource_create_context_impl::flush_deleted_texture_2d(uint32_t frame_index)
            {
                std::lock_guard< std::mutex  > lock(m_delete_textures_mutex);
                auto& textures = m_frame_delete_texture_2d[frame_index];

                for (auto&& t : textures)
                {
                    free_texture_2d_internal(t);
                }

                textures.clear();
            }

            void gpu_resource_create_context::gpu_resource_create_context_impl::flush_deleted_read_write_texture_2d(uint32_t frame_index)
            {
                std::lock_guard< std::mutex  > lock(m_delete_textures_mutex);
                auto& textures = m_frame_delete_read_write_texture_2d[frame_index];

                for (auto&& t : textures)
                {
                    free_read_write_texture_2d_internal(t);
                }

                textures.clear();
            }

            void gpu_resource_create_context::gpu_resource_create_context_impl::flush_deleted_texture_2d_array(uint32_t frame_index)
            {
                std::lock_guard< std::mutex  > lock(m_delete_textures_mutex);
                auto& textures = m_frame_delete_texture_2d_array[frame_index];

                for (auto&& t : textures)
                {
                    free_texture_2d_array_internal(t);
                }

                textures.clear();
            }

            gpu_upload_buffer* gpu_resource_create_context::create_upload_buffer(uint64_t size)
            {
                return new gpu_upload_buffer(create_upload_buffer_resource(size).Get());
            }

            //read back buffer, for downloading from the gpu
            gpu_read_back_buffer* gpu_resource_create_context::create_read_back_buffer( uint64_t size )
            {
                auto desc = describe_buffer(static_cast<uint32_t>(size));
                Microsoft::WRL::ComPtr<ID3D12Resource>  resource;
                auto allocator = m_impl->read_back_allocator();

                resource = allocator->create_placed_resource(&desc, D3D12_RESOURCE_STATE_COPY_DEST);
                resource->SetName(L"Readback Buffer");
                return new gpu_read_back_buffer(resource.Get());
            }

            namespace details
            {
                template <typename t, typename allocator, typename srv_heap_t> t* create_color_buffer(ID3D12Device* d, uint32_t width, uint32_t height, DXGI_FORMAT format, D3D12_RESOURCE_STATES initial_state, allocator* allocator, gpu_rtv_descriptor_heap* rtv_heap, srv_heap_t* srv_heap)
                {
                    D3D12_CLEAR_VALUE v = {};
                    v.Format = format;

                    auto desc = describe_color_buffer(width, height, format);

                    Microsoft::WRL::ComPtr<ID3D12Resource>  resource;

                    D3D12_RENDER_TARGET_VIEW_DESC rtv = {};
                    rtv.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
                    rtv.Texture2D.MipSlice = 0;

                    resource = allocator->create_placed_resource(&desc, initial_state, &v);
                    resource->SetName(L"Color Buffer");

                    auto handle = rtv_heap->allocate();
                    d->CreateRenderTargetView(resource.Get(), &rtv, handle);

                    auto srv = srv_heap->allocate(1);
                    auto uav = srv_heap->allocate(1);

                    D3D12_SHADER_RESOURCE_VIEW_DESC  descSRV = {};
                    D3D12_UNORDERED_ACCESS_VIEW_DESC descUAV = {};

                    descSRV.Format = format;
                    descSRV.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
                    descSRV.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
                    descSRV.Texture2D.MipLevels = 1;
                    descSRV.Texture2D.MostDetailedMip = 0;
                    descSRV.Texture2D.PlaneSlice = 0;

                    descUAV.Format = get_uav_format(format);
                    descUAV.ViewDimension = D3D12_UAV_DIMENSION_TEXTURE2D;
                    descUAV.Texture2D.MipSlice = 0;
                    descUAV.Texture2D.PlaneSlice = 0;

                    d->CreateUnorderedAccessView(resource.Get(), nullptr, &descUAV, uav);
                    d->CreateShaderResourceView(resource.Get(), &descSRV, srv);

                    return new t(resource.Get(), handle, srv, uav);
                }
                template <typename t, typename allocator, typename srv_heap_t> t* create_depth_buffer(ID3D12Device* d, uint32_t width, uint32_t height, DXGI_FORMAT format, float clear_value, uint8_t stencil, allocator* allocator, gpu_dsv_descriptor_heap* dsv_heap, srv_heap_t* srv_heap)
                {
                    D3D12_CLEAR_VALUE v = {};
                    v.Format = format;
                    v.DepthStencil.Depth = clear_value;
                    v.DepthStencil.Stencil = stencil;

                    auto desc = describe_depth_buffer(width, height, format);

                    Microsoft::WRL::ComPtr<ID3D12Resource>  resource;

                    resource = allocator->create_placed_resource(&desc, D3D12_RESOURCE_STATE_DEPTH_WRITE, &v);
                    resource->SetName(L"Depth Buffer");

                    // Create the shader resource view
                    D3D12_SHADER_RESOURCE_VIEW_DESC viewDesc = {};
                    viewDesc.Format = get_depth_format(format);
                    viewDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
                    viewDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
                    viewDesc.Texture2D.MipLevels = 1;

                    auto srvDepth = srv_heap->allocate();

                    d->CreateShaderResourceView(resource.Get(), &viewDesc, srvDepth);

                    auto srv = srvDepth;

                    //Create depth stencil view
                    auto dsvReadWrite = dsv_heap->allocate(1);
                    auto dsvReadDepth = dsv_heap->allocate(1);

                    D3D12_DEPTH_STENCIL_VIEW_DESC desc2 = {};
                    desc2.Format = format;
                    desc2.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
                    desc2.Texture2D.MipSlice = 0;
                    desc2.Flags = D3D12_DSV_FLAG_NONE;

                    d->CreateDepthStencilView(resource.Get(), &desc2, dsvReadWrite);

                    desc2.Flags = D3D12_DSV_FLAG_READ_ONLY_DEPTH;
                    d->CreateDepthStencilView(resource.Get(), &desc2, dsvReadDepth);

                    typename gpu_dsv_descriptor_heap::descriptor_handle dsv[] = { dsvReadWrite, dsvReadDepth };

                    return new t(resource.Get(), srv, dsv );
                }
                template <typename t, typename allocator, typename srv_heap_t> t* create_msaa_depth_buffer(ID3D12Device* d, uint32_t width, uint32_t height, DXGI_FORMAT format, float clear_value, uint8_t stencil, allocator* allocator, gpu_dsv_descriptor_heap* dsv_heap, srv_heap_t* srv_heap)
                {
                    D3D12_CLEAR_VALUE v = {};
                    v.Format = format;
                    v.DepthStencil.Depth = clear_value;
                    v.DepthStencil.Stencil = stencil;

                    auto desc = describe_msaa_depth_buffer(width, height, format, 4);

                    Microsoft::WRL::ComPtr<ID3D12Resource>  resource;

                    resource = allocator->create_placed_resource(&desc, D3D12_RESOURCE_STATE_DEPTH_WRITE, &v);
                    resource->SetName(L"MSAA Depth Buffer");

                    // Create the shader resource view
                    D3D12_SHADER_RESOURCE_VIEW_DESC viewDesc = {};
                    viewDesc.Format = get_depth_format(format);
                    viewDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2DMS;
                    viewDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;

                    descriptor_handle srvDepth = srv_heap->allocate();
                    d->CreateShaderResourceView(resource.Get(), &viewDesc, srvDepth);
                    descriptor_handle srv = srvDepth;

                    //Create depth stencil view
                    descriptor_handle dsvReadWrite = dsv_heap->allocate(1);
                    descriptor_handle dsvReadDepth = dsv_heap->allocate(1);

                    D3D12_DEPTH_STENCIL_VIEW_DESC desc2 = {};
                    desc2.Format = format;
                    desc2.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2DMS;
                    desc2.Flags = D3D12_DSV_FLAG_NONE;

                    d->CreateDepthStencilView(resource.Get(), &desc2, dsvReadWrite);

                    desc2.Flags = D3D12_DSV_FLAG_READ_ONLY_DEPTH;
                    d->CreateDepthStencilView(resource.Get(), &desc2, dsvReadDepth);

                    descriptor_handle dsv[] = { dsvReadWrite, dsvReadDepth };
                    return new t(resource.Get(), srv, dsv);
                }
            }
            

            gpu_view_color_buffer*  gpu_resource_create_context::create_view_color_buffer(uint32_t width, uint32_t height, DXGI_FORMAT format, D3D12_RESOURCE_STATES initial_state)
            {
                return details::create_color_buffer<gpu_view_color_buffer>(m_impl->m_device.Get(), width, height, format, initial_state,
                    m_impl->m_view_dependent_render_target_allocator.get(),
                    &m_impl->m_view_dependent_rtv_heap,
                    &m_impl->m_view_dependent_srv_heap
                    );
            }

            gpu_frame_color_buffer*  gpu_resource_create_context::create_frame_color_buffer(uint32_t width, uint32_t height, DXGI_FORMAT format, D3D12_RESOURCE_STATES initial_state)
            {
                return details::create_color_buffer<gpu_frame_color_buffer>(m_impl->m_device.Get(), width, height, format, initial_state,
                    m_impl->m_frame_render_target_allocator.get(),
                    frame_rtv_heap(),
                    frame_cpu_srv_heap()
                    );
            }

            gpu_view_depth_buffer*  gpu_resource_create_context::create_view_depth_buffer(uint32_t width, uint32_t height, DXGI_FORMAT format, float clear_value, uint8_t stencil )
            {
                return details::create_depth_buffer<gpu_view_depth_buffer>(m_impl->m_device.Get(), width, height, format, clear_value, stencil,
                    m_impl->m_view_dependent_render_target_allocator.get(),
                    &m_impl->m_view_dependent_dsv_heap,
                    &m_impl->m_view_dependent_srv_heap
                    );
            }

            gpu_frame_depth_buffer*  gpu_resource_create_context::create_frame_depth_buffer(uint32_t width, uint32_t height, DXGI_FORMAT format, float clear_value, uint8_t stencil)
            {
                return details::create_depth_buffer<gpu_frame_depth_buffer>(m_impl->m_device.Get(), width, height, format, clear_value, stencil,
                    m_impl->m_frame_render_target_allocator.get(),
                    frame_dsv_heap(),
                    frame_cpu_srv_heap()
                    );
            }

            gpu_view_msaa_depth_buffer*  gpu_resource_create_context::create_view_msaa_depth_buffer(uint32_t width, uint32_t height, DXGI_FORMAT format, float clear_value, uint8_t stencil)
            {
                return details::create_msaa_depth_buffer<gpu_view_msaa_depth_buffer>(m_impl->m_device.Get(), width, height, format, clear_value, stencil,
                    m_impl->m_view_dependent_render_target_allocator.get(),
                    &m_impl->m_view_dependent_dsv_heap,
                    &m_impl->m_view_dependent_srv_heap
                    );
            }

            gpu_frame_msaa_depth_buffer*  gpu_resource_create_context::create_frame_msaa_depth_buffer(uint32_t width, uint32_t height, DXGI_FORMAT format, float clear_value, uint8_t stencil)
            {
                return details::create_msaa_depth_buffer<gpu_frame_msaa_depth_buffer>(m_impl->m_device.Get(), width, height, format, clear_value, stencil,
                    m_impl->m_frame_render_target_allocator.get(),
                    frame_dsv_heap(),
                    frame_cpu_srv_heap()
                    );
            }

            gpu_back_buffer* gpu_resource_create_context::create_back_buffer(ID3D12Resource* resource)
            {
                D3D12_RENDER_TARGET_VIEW_DESC rtv = {};
                rtv.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
                rtv.Texture2D.MipSlice = 0;

                auto handle = m_impl->m_view_dependent_rtv_heap.allocate();
                m_impl->m_device->CreateRenderTargetView(resource, &rtv, handle);

                return new gpu_back_buffer(resource, handle);
            }

            gpu_buffer* gpu_resource_create_context::create_buffer(uint32_t elements, uint32_t element_size)
            {
                auto desc = describe_buffer(elements, element_size);

                Microsoft::WRL::ComPtr<ID3D12Resource>  resource;
                auto allocator = m_impl->geometry_allocator();

                persistent_cpu_srv_descriptor_heap_handle uav;
                persistent_cpu_srv_descriptor_heap_handle srv;

                {
                    std::lock_guard< std::mutex  > lock(m_impl->m_delete_buffers_mutex);
                    resource = allocator->create_placed_resource(&desc, D3D12_RESOURCE_STATE_COMMON);
                }

                return new gpu_buffer(resource.Get());
            }

            void gpu_resource_create_context::free_buffer(gpu_buffer* buffer)
            {
                std::lock_guard< std::mutex  > lock(m_impl->m_delete_buffers_mutex);
                auto& buffers = m_impl->m_frame_delete_buffers[m_impl->m_frame_index];
                buffers.push_back(buffer);
            }

            void gpu_resource_create_context::gpu_resource_create_context_impl::free_buffer_internal(gpu_buffer* buffer)
            {
                auto allocator = geometry_allocator();
                allocator->free_placed_resource(buffer->resource());
                delete buffer;
            }

            void gpu_resource_create_context::gpu_resource_create_context_impl::flush_deleted_buffers(uint32_t frame_index)
            {
                std::lock_guard< std::mutex  > lock(m_delete_buffers_mutex);
                auto& buffers = m_frame_delete_buffers[frame_index];

                for (auto&& t : buffers)
                {
                    free_buffer_internal(t);
                }

                buffers.clear();
            }

            void gpu_resource_create_context::sync()
            {
                m_impl->m_frame_index++;
                m_impl->m_frame_index %= 3;

                m_impl->flush_deleted_texture_2d(m_impl->m_frame_index);
                m_impl->flush_deleted_texture_2d_array(m_impl->m_frame_index);
                m_impl->flush_deleted_read_write_texture_2d(m_impl->m_frame_index);
                m_impl->flush_deleted_buffers(m_impl->m_frame_index);

                frame_gpu_srv_heap()->reset();
                frame_cpu_srv_heap()->reset();
                frame_dsv_heap()->reset();
                frame_rtv_heap()->reset();

                m_impl->m_frame_render_target_allocator->reset();
                m_impl->upload_allocator()->reset();
                m_impl->read_back_allocator()->reset();
            }

            void gpu_resource_create_context::reset_view_dependent_resources()
            {
                //reset all allocators that can hold view dependent surfaces.
                m_impl->m_view_dependent_render_target_allocator->reset();

                m_impl->m_view_dependent_rtv_heap.reset();
                m_impl->m_view_dependent_srv_heap.reset();
                m_impl->m_view_dependent_dsv_heap.reset();
            }

            gpu_resource_create_context::gpu_resource_create_context_impl::placement_heap_allocator* gpu_resource_create_context::gpu_resource_create_context_impl::upload_allocator() const
            {
                return m_upload_allocator[m_frame_index].get();
            }

            gpu_resource_create_context::gpu_resource_create_context_impl::placement_heap_allocator* gpu_resource_create_context::gpu_resource_create_context_impl::read_back_allocator() const
            {
                return m_read_back_allocator[m_frame_index].get();
            }

            buddy_heap_allocator*   gpu_resource_create_context::gpu_resource_create_context_impl::textures_allocator() const
            {
                return m_textures_allocator.get();
            }

            coalesceable_heap_allocator*   gpu_resource_create_context::gpu_resource_create_context_impl::geometry_allocator() const
            {
                return m_geometry_heap.get();
            }

            gpu_srv_descriptor_heap* gpu_resource_create_context::frame_gpu_srv_heap()
            {
                return &m_impl->m_frame_gpu_srv_heap[m_impl->m_frame_index];
            }

            cpu_srv_descriptor_heap* gpu_resource_create_context::frame_cpu_srv_heap()
            {
                return &m_impl->m_frame_cpu_srv_heap[m_impl->m_frame_index];
            }

            gpu_rtv_descriptor_heap* gpu_resource_create_context::frame_rtv_heap()
            {
                return &m_impl->m_frame_rtv_heap[m_impl->m_frame_index];
            }

            gpu_dsv_descriptor_heap* gpu_resource_create_context::frame_dsv_heap()
            {
                return &m_impl->m_frame_dsv_heap[m_impl->m_frame_index];
            }

            gpu_sampler_descriptor_heap* gpu_resource_create_context::frame_gpu_sampler_heap()
            {
                return &m_impl->m_frame_gpu_sampler_heap[m_impl->m_frame_index];
            }

            cpu_sampler_descriptor_heap* gpu_resource_create_context::frame_cpu_sampler_heap()
            {
                return &m_impl->m_frame_cpu_sampler_heap[m_impl->m_frame_index];
            }

        }
    }
}
