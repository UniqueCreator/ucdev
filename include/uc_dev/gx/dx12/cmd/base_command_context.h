#pragma once

#include <queue>

#include <uc_dev/util/noncopyable.h>
#include <uc_dev/gx/dx12/api/helpers.h>
#include <uc_dev/gx/dx12/gpu/allocators/upload_allocator.h>
#include <uc_dev/gx/dx12/gpu/resource_create_context.h>

#include "command_queue.h"
#include "command_manager.h"
#include "command_descriptor_cache.h"
#include "descriptor_handle_cache.h"



namespace uc
{
    namespace gx
    {
        namespace dx12
        {
            struct graphics_pipeline_state;
            struct compute_pipeline_state;

            struct gpu_base_command_context : public util::noncopyable
            {
                gpu_command_manager*                    m_command_manager;          //allocate command lists here
                gpu_command_queue*                      m_command_queue;            //submit command lists here
                gpu_resource_create_context*            m_rc;
                Microsoft::WRL::ComPtr<ID3D12Device>    m_device;

                gpu_upload_allocator                    m_upload_allocator;             //allocate upload data
                gpu_command_manager::command_list       m_command_list;                 //current command list
                gpu_command_descriptor_cache            m_descriptor_cache;             //allocate handles per frame
                descriptor_handle_cache                 m_descriptor_handle_cache0;     //descriptor tables for the dynamic tables, if you have only graphics or compute goes here
                descriptor_handle_cache                 m_descriptor_handle_cache1;     //descriptor tables for the dynamic tables  if you have graphics and compute, compute goes here
                const graphics_pipeline_state*          m_graphics_pipeline_state = nullptr;
                const compute_pipeline_state*           m_compute_pipeline_state  = nullptr;

                static const uint32_t                   barrier_count = 16;
                D3D12_RESOURCE_BARRIER                  m_resource_bariers[barrier_count];
                uint32_t                                m_resource_bariers_count = 0;

                gpu_base_command_context( gpu_resource_create_context* rc, gpu_command_manager* m, gpu_command_queue* q ) : 
                m_command_manager(m)
                , m_command_queue(q)
                , m_rc(rc)
                , m_upload_allocator(rc)
                , m_descriptor_cache(rc)
                , m_descriptor_handle_cache0( m_command_manager->device(), root_signature_meta_data())
                , m_descriptor_handle_cache1( m_command_manager->device(), root_signature_meta_data())
                {
                    m_command_list = std::move(m_command_manager->create_command_list(gpu_command_manager::list_type(m_command_queue->type())));
                    m_command_list.m_list->GetDevice( __uuidof(ID3D12Device), reinterpret_cast<void**>(m_device.GetAddressOf()));
                }

                void reset()
                {
                    m_command_list = std::move(m_command_manager->create_command_list( gpu_command_manager::list_type( m_command_queue->type() )));
                    m_descriptor_cache.reset();
                    m_upload_allocator.reset();
                    m_descriptor_handle_cache0.reset();
                    m_descriptor_handle_cache1.reset();
                    m_resource_bariers_count = 0;
                    m_compute_pipeline_state = nullptr;
                    m_graphics_pipeline_state = nullptr;
                }

                ~gpu_base_command_context()
                {
                    if (m_command_list.m_allocator)
                    {
                        m_command_manager->free_command_list(0, m_command_list);
                    }
                }
            };


        }
    }
}