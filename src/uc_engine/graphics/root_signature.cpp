#include "pch.h"

#include <uc_public/graphics/root_signature.h>
#include <uc_public/uc_pimpl_impl.h>

#include "root_signature_impl.h"

namespace UniqueCreator
{
    namespace Graphics
    {
        template details::pimpl<RootSignature::Impl>;

        RootSignature::RootSignature()
        {

        }

        RootSignature::~RootSignature() = default;
        RootSignature& RootSignature::operator=(RootSignature&&) = default;

        RootSignature::Impl*   RootSignature::GetImpl()
        {
            return m_impl.get();
        }

        const RootSignature::Impl*   RootSignature::GetImpl() const
        {
            return m_impl.get();
        }
    }
}


