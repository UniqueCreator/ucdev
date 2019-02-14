#pragma once

#include <uc_public/uc_implementation.h>
#include <uc_public/graphics/graphics_types.h>
#include <uc_public/render/primitive_range.h>

namespace UniqueCreator
{
    namespace Assets
    {
        class Texture2DMipChain;

        class SkinnedModel : private NonCopyable
        {
        public:

            struct Texture2DDescription
            {
                Texture2DMipChain** m_textures;
                size_t              m_textures_size;
            };

            class Impl;

            UC_ENGINE_API ~SkinnedModel();

            UC_ENGINE_API SkinnedModel(SkinnedModel&&) = default;
            UC_ENGINE_API SkinnedModel& operator=(SkinnedModel&&);

            Impl*         GetImpl();
            const Impl*   GetImpl() const;

            UC_ENGINE_API const void* GetPosition() const;
            UC_ENGINE_API uint64_t    GetPositionSize() const;

            UC_ENGINE_API const void* GetUV() const;
            UC_ENGINE_API uint64_t    GetUVSize() const;

            UC_ENGINE_API const void* GetBlendWeight() const;
            UC_ENGINE_API uint64_t    GetBlendWeightSize() const;

            UC_ENGINE_API const void* GetBlendIndex() const;
            UC_ENGINE_API uint64_t    GetBlendIndexSize() const;

            UC_ENGINE_API const void* GetIndex() const;
            UC_ENGINE_API uint64_t    GetIndexSize() const;

            UC_ENGINE_API const Graphics::PrimitiveRangeDescription GetPrimitiveRanges() const;
            UC_ENGINE_API const Texture2DDescription GetTextureDescription() const;


        protected:
            SkinnedModel();
            details::pimpl<Impl> m_impl;
        };

        //private data, todo: fix visual assist warning
        extern template details::pimpl<SkinnedModel::Impl>;
    }
}


