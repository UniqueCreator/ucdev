#pragma once

#include <uc_public/uc_implementation.h>

#include <uc_public/graphics/graphics_types.h>

namespace UniqueCreator
{
    namespace Graphics
    {
        class RootSignature : private NonCopyable
        {

            public:

            class Impl;

            UC_ENGINE_API ~RootSignature();

            UC_ENGINE_API RootSignature(RootSignature&&) = default;
            UC_ENGINE_API RootSignature& operator=(RootSignature&&);

            Impl*         GetImpl();
            const Impl*   GetImpl() const;

        protected:
            RootSignature();
            details::pimpl<Impl> m_impl;
        };

        //private data, todo: fix visual assist warning
        extern template details::pimpl<RootSignature::Impl>;
    }
}


