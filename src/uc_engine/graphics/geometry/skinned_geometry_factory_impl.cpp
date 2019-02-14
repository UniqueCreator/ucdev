#include "pch.h"

#include "skinned_geometry_factory_impl.h"
#include "../resource_create_context_impl.h"

namespace UniqueCreator
{
    namespace Graphics
    {
        SkinnedGeometryFactory::Impl::Impl(ResourceCreateContext* rc, uint32_t vertexCount)  : m_mesh_allocator( std::make_unique<uc::gx::geo::skinned_geometry_allocator>( rc->GetImpl()->rc(), vertexCount)),
            m_position(m_mesh_allocator->skinned_mesh_position())
            , m_uv(m_mesh_allocator->skinned_mesh_uv())
            , m_blend_weight(m_mesh_allocator->skinned_mesh_blend_weight())
            , m_blend_index(m_mesh_allocator->skinned_mesh_blend_index())
        {
            
        }

        uc::gx::geo::skinned_geometry_allocator*  SkinnedGeometryFactory::Impl::allocator() const
        {
            return m_mesh_allocator.get();
        }

        const Buffer* SkinnedGeometryFactory::Impl::position() const
        {
            return &m_position;
        
        }
        const Buffer* SkinnedGeometryFactory::Impl::uv() const
        {
            return &m_uv;
        }

        const Buffer* SkinnedGeometryFactory::Impl::blend_weight() const
        {
            return &m_blend_weight;
        }
        const Buffer* SkinnedGeometryFactory::Impl::blend_index() const
        {
            return &m_blend_index;
        }
    }
}


