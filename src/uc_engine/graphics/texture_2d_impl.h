#pragma once

#include <uc_public/graphics/texture_2d.h>

#include <uc_dev/gx/dx12/dx12.h>
#include <uc_dev/gx/dx12/gpu/texture_2d.h>

#include "shader_resource_view_internal.h"
#include "unordered_access_view_internal.h"

namespace UniqueCreator
{
    namespace Graphics
    {
        class Texture2D::Impl
        {

            public:

            Impl() {}

            Impl(uc::gx::dx12::managed_gpu_texture_2d && b) :
                m_texture(std::move(b))
                , m_shaderResourceView(m_texture->srv())
            {

            }

            uc::gx::dx12::gpu_texture_2d* texture() const
            {
                return m_texture.get();
            }

            const ShaderResourceView* GetShaderResourceView() const
            {
                return &m_shaderResourceView;
            }

            const UnorderedAccessView* GetUnorderedAccessView() const
            {
                return &m_unorderedResourceView;
            }

            uc::gx::dx12::managed_gpu_texture_2d                    m_texture;
            ShaderResourceViewInternal                              m_shaderResourceView;
            UnorderedAccessViewInternal                             m_unorderedResourceView;
        };
    }
}


