#include "pch.h"

#include "direct_command_context_impl.h"

namespace UniqueCreator
{
    namespace Graphics
    {
        DirectCommandContext::Impl::Impl( uc::gx::dx12::managed_graphics_command_context&& context ) : m_context( std::move(context) )
        {

        }
    }
}


