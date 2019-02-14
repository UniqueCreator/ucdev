#include "pch.h"

#include <uc_public/assets/graphics/skinned_model.h>

#include <uc_public/uc_pimpl_impl.h>

#include "skinned_model_impl.h"
#include "texture_2d_mip_chain_impl.h"

namespace UniqueCreator
{
    namespace Assets
    {
        template details::pimpl<SkinnedModel::Impl>;

        SkinnedModel::SkinnedModel()
        {

        }

        SkinnedModel::~SkinnedModel() = default;
        SkinnedModel& SkinnedModel::operator=(SkinnedModel&&) = default;

        SkinnedModel::Impl*   SkinnedModel::GetImpl()
        {
            return m_impl.get();
        }

        const SkinnedModel::Impl*   SkinnedModel::GetImpl() const
        {
            return m_impl.get();
        }

        const void* SkinnedModel::GetPosition() const
        {
            return &m_impl->m_model->m_positions.m_data[0];
        }

        uint64_t    SkinnedModel::GetPositionSize() const
        {
            return m_impl->m_model->m_positions.m_data.size();
        }

        const void* SkinnedModel::GetUV() const
        {
            return &m_impl->m_model->m_uv.m_data[0];
        }

        uint64_t    SkinnedModel::GetUVSize() const
        {
            return m_impl->m_model->m_uv.m_data.size();
        }

        const void* SkinnedModel::GetBlendWeight() const
        {
            return &m_impl->m_model->m_blend_weights[0];
        }

        uint64_t    SkinnedModel::GetBlendWeightSize() const
        {
            return m_impl->m_model->m_blend_weights.size();
        }

        const void* SkinnedModel::GetBlendIndex() const
        {
            return &m_impl->m_model->m_blend_indices[0];
        }

        uint64_t    SkinnedModel::GetBlendIndexSize() const
        {
            return m_impl->m_model->m_blend_indices.size();
        }

        const void* SkinnedModel::GetIndex() const
        {
            return m_impl->m_model->m_indices.data();
        }

        uint64_t    SkinnedModel::GetIndexSize() const
        {
            return m_impl->m_model->m_indices.size();
        }

        const Graphics::PrimitiveRangeDescription SkinnedModel::GetPrimitiveRanges() const
        {
            auto size     = m_impl->m_model->m_primitive_ranges.size();
            void* begin   = const_cast<void*>(reinterpret_cast<const void*>(&m_impl->m_model->m_primitive_ranges[0]));

            Graphics::PrimitiveRangeDescription r =
            {
                reinterpret_cast<Graphics::PrimitiveRange*>(begin),
                size
            };

            return r;
        }

        const SkinnedModel::Texture2DDescription SkinnedModel::GetTextureDescription() const
        {
            Impl* impl = const_cast<Impl*>(GetImpl());
            Texture2DDescription r =
            {
                &impl->m_texture_wrappers[0],
                impl->m_texture_wrappers.size()
            };

            return r;
        }
    }
}

