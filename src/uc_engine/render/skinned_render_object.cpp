#include "pch.h"

#include <uc_public/render/skinned_render_object.h>
#include <uc_public/uc_pimpl_impl.h>

#include "skinned_render_object_impl.h"

namespace UniqueCreator
{
    namespace Render
    {
        template details::pimpl<SkinnedRenderObject::Impl>;

        SkinnedRenderObject::SkinnedRenderObject()
        {

        }

        SkinnedRenderObject::~SkinnedRenderObject() = default;
        SkinnedRenderObject& SkinnedRenderObject::operator=(SkinnedRenderObject&&) = default;

        SkinnedRenderObject::Impl*   SkinnedRenderObject::GetImpl()
        {
            return m_impl.get();
        }

        const SkinnedRenderObject::Impl*   SkinnedRenderObject::GetImpl() const
        {
            return m_impl.get();
        }

        const Graphics::IndexedGeometryAllocation* SkinnedRenderObject::GetPrimitives() const
        {
            return m_impl->m_indices.get();
        }

        const Graphics::SkinnedGeometryAllocation* SkinnedRenderObject::GetGeometry() const
        {
            return m_impl->m_geometry.get();
        }

        const Graphics::PrimitiveRangeDescription SkinnedRenderObject::GetPrimitiveRanges() const
        {
            Graphics::PrimitiveRangeDescription r;
            
            r.m_ranges = const_cast<UniqueCreator::Graphics::PrimitiveRange *>(&m_impl->m_primitive_ranges[0]);
            r.m_ranges_size = m_impl->m_primitive_ranges.size();

            return r;
        }

        const SkinnedRenderObject::TexturesDescription SkinnedRenderObject::GetAlbedoTextures() const
        {
            Impl* impl = const_cast<Impl*>(GetImpl());
            TexturesDescription r =
            {
                &impl->m_texture_wrappers[0],
                impl->m_texture_wrappers.size()
            };

            return r;
        }
    }
}
