#include "pch.h"

#include "copy_command_context_impl.h"

namespace UniqueCreator
{
    namespace Graphics
    {
        CopyCommandContext::Impl::Impl( uc::gx::dx12::managed_copy_command_context&& context ) : m_context( std::move(context) )
        {

        }
    }
}


