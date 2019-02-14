#include "pch.h"

#include "compute_command_context_impl.h"

namespace UniqueCreator
{
    namespace Graphics
    {
        ComputeCommandContext::Impl::Impl( uc::gx::dx12::managed_compute_command_context&& context ) : m_context( std::move(context) )
        {

        }
    }
}


