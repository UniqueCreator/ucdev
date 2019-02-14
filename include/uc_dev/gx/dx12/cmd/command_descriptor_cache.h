#pragma once


#include <uc_dev/gx/dx12/gpu/resource_create_context.h>
#include <pix3.h>


namespace uc
{
    namespace gx
    {
        namespace dx12
        {
            struct gpu_command_descriptor_cache
            {

                public:

                gpu_command_descriptor_cache ( gpu_resource_create_context* rc ) :
                m_rc(rc)
                , m_size_srv(0)
                , m_size_dsv(0)
                , m_size_rtv(0)
                {
                    reset();
                }

                descriptor_handle allocate_srv()
                {
                    if (m_size_srv == 0)
                    {
                        request_srv_handles();
                    }

                    descriptor_handle r = m_handle_srv;
                    m_handle_srv = m_handle_srv.increment(m_increment_size_srv);
                    m_size_srv--;
                    return r;
                }

                descriptor_handle allocate_dsv()
                {
                    if (m_size_dsv == 0)
                    {
                        request_dsv_handles();
                    }

                    descriptor_handle r = m_handle_dsv;
                    m_handle_dsv = m_handle_dsv.increment(m_increment_size_dsv);
                    m_size_dsv--;
                    return r;
                }

                descriptor_handle allocate_rtv()
                {
                    if (m_size_rtv == 0)
                    {
                        request_rtv_handles();
                    }

                    descriptor_handle r = m_handle_rtv;
                    m_handle_rtv = m_handle_rtv.increment(m_increment_size_rtv);
                    m_size_rtv--;
                    return r;
                }

                void reset()
                {
                    request_handles();
                    m_increment_size_srv = m_rc->frame_cpu_srv_heap()->increment_size();
                    m_increment_size_dsv = m_rc->frame_dsv_heap()->increment_size();
                    m_increment_size_rtv = m_rc->frame_rtv_heap()->increment_size();
                }

                private:

                gpu_resource_create_context* m_rc;

                static const uint32_t srv_request_size = 16;
                static const uint32_t rtv_request_size = 16;
                static const uint32_t dsv_request_size = 16;

                descriptor_handle            m_handle_srv;
                descriptor_handle            m_handle_dsv;
                descriptor_handle            m_handle_rtv;

                uint32_t                     m_size_srv;
                uint32_t                     m_size_dsv;
                uint32_t                     m_size_rtv;

                uint64_t                     m_increment_size_srv;
                uint64_t                     m_increment_size_dsv;
                uint64_t                     m_increment_size_rtv;


                void request_handles()
                {
                    request_srv_handles();
                    request_dsv_handles();
                    request_rtv_handles();
                }

                void request_srv_handles()
                {
                    m_handle_srv = m_rc->frame_cpu_srv_heap()->allocate(srv_request_size);
                    m_size_srv = srv_request_size;
                }

                void request_dsv_handles()
                {
                    m_handle_dsv = m_rc->frame_dsv_heap()->allocate(dsv_request_size);
                    m_size_dsv = dsv_request_size;
                }

                void request_rtv_handles()
                {
                    m_handle_rtv = m_rc->frame_rtv_heap()->allocate(rtv_request_size);
                    m_size_rtv = rtv_request_size;
                }

            };
        }
    }
}