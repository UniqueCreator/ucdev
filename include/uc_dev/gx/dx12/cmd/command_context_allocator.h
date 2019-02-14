#pragma once

#include <queue>

#include <uc_dev/util/noncopyable.h>
#include "base_command_context.h"

namespace uc
{
    namespace gx
    {
        namespace dx12
        {
            class gpu_command_context_allocator : public util::noncopyable
            {

                public:
                gpu_command_context_allocator( gpu_resource_create_context* rc, gpu_command_manager* m, gpu_command_queue* q ) :
                m_resource_context(rc)
                ,m_command_manager(m)
                , m_command_queue(q)
                
                
                {

                }

                gpu_base_command_context* create_base_command_context()
                {
                    std::lock_guard<std::mutex> g(this->m_pool_lock);

                    return new gpu_base_command_context(m_resource_context, m_command_manager, m_command_queue);

                    /*
                    auto&& free = m_free_contexts[m_context_index];

                    if (free.empty())
                    {
                        
                        auto r = d.get();
                        m_context_pool.emplace_back(std::move(d));
                        return r;
                    }
                    else
                    {
                        auto r = free.front();
                        free.pop();
                        r->reset();
                        return r;
                    }
                    */
                }

                void free_base_command_context( gpu_base_command_context* ctx )
                {
                    std::lock_guard<std::mutex> g(m_pool_lock);
                    auto&& free = m_free_contexts[m_context_index];
                    free.push( std::unique_ptr<gpu_base_command_context>(ctx) );
                }

                void sync()
                {
                    m_command_manager->sync();

                    m_context_index = (m_context_index + 1 ) % 3;

                    //todo: flush these less often
                    auto&& free = m_free_contexts[m_context_index];
                    std::queue<std::unique_ptr<gpu_base_command_context> > empty_queue;
                    free.swap( empty_queue );
                }

                private:

                gpu_resource_create_context*                                m_resource_context;
                gpu_command_manager*                                        m_command_manager;  //allocate command lists here
                gpu_command_queue*                                          m_command_queue;    //submit command lists here

                uint64_t                                                    m_context_index = 2;
                

                std::mutex                                                  m_pool_lock;
                std::queue< std::unique_ptr<gpu_base_command_context> >     m_free_contexts[3];
            };
        }
    }
}