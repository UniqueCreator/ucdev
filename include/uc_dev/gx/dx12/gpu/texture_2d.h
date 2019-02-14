#pragma once

#include "virtual_resource.h"
#include "descriptor_heap.h"

namespace uc
{
    namespace gx
    {
        namespace dx12
        {
            class gpu_texture_2d : public gpu_virtual_resource
            {
                private:

                    using base = gpu_virtual_resource;

                public:

                    gpu_texture_2d( ) : base( nullptr )
                    {

                    }

                    gpu_texture_2d(ID3D12Resource* resource, persistent_cpu_srv_descriptor_heap_handle srv ) : 
                        base(resource)
                        , m_srv(srv)
                    {

                    }

                    ~gpu_texture_2d()
                    {
                        if (m_srv.m_heap)
                        {
                            m_srv.m_heap->free(m_srv.m_handle);
                        }
                    }

                    descriptor_handle srv() const
                    {
                        return m_srv.m_handle;
                    }

                private:
                    persistent_cpu_srv_descriptor_heap_handle    m_srv;

            };

            class gpu_read_write_texture_2d : public gpu_texture_2d
            {
                private:

                    using base = gpu_texture_2d;

                public:

                    gpu_read_write_texture_2d() 
                    {

                    }

                    gpu_read_write_texture_2d(ID3D12Resource* resource, persistent_cpu_srv_descriptor_heap_handle uav, persistent_cpu_srv_descriptor_heap_handle srv) :
                        base(resource, srv)
                        , m_uav(uav)
                    {

                    }

                    ~gpu_read_write_texture_2d()
                    {
                        if (m_uav.m_heap)
                        {
                            m_uav.m_heap->free(m_uav.m_handle);
                        }
                    }

                    descriptor_handle uav() const
                    {
                        return m_uav.m_handle;
                    }

                private:
                    persistent_cpu_srv_descriptor_heap_handle    m_uav;

            };
        }
    }
}
