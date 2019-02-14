#include "pch.h"

#include <uc_public/graphics/copy_command_context.h>
#include <uc_public/uc_pimpl_impl.h>

#include "copy_command_context_impl.h"
#include "gpu_virtual_resource_impl.h"

namespace UniqueCreator
{
    namespace Graphics
    {
        template details::pimpl<CopyCommandContext::Impl>;

        CopyCommandContext::CopyCommandContext() : m_impl()
        {

        }
        
        CopyCommandContext::~CopyCommandContext() = default;
        CopyCommandContext& CopyCommandContext::operator=(CopyCommandContext&&) = default;
   
    }
}


