#include "pch.h"

#include <uc_public/render/skinned_render_object_factory.h>
#include <uc_public/uc_pimpl_impl.h>

#include <uc_public/assets/assets.h>
#include <uc_public/graphics/upload_queue.h>
#include <uc_public/graphics/buffer.h>
#include <uc_public/graphics/geometry/geometry.h>
#include <uc_public/graphics/resource_create_context.h>


#include "skinned_render_object_factory_impl.h"
#include "skinned_render_object_impl.h"

#include <uc_dev/gx/geo/vertex_strides.h>

#include <gsl/gsl>

namespace UniqueCreator
{
    namespace Render
    {
        template details::pimpl<SkinnedRenderObjectFactory::Impl>;

        class SkinnedRenderObjectInternal : public SkinnedRenderObject
        {
            public:
                SkinnedRenderObjectInternal(
                    std::unique_ptr<Graphics::IndexedGeometryAllocation>&& indices,
                    std::unique_ptr<Graphics::SkinnedGeometryAllocation>&& geometry,
                    std::vector< Graphics::PrimitiveRange> && ranges,
                    std::vector< std::unique_ptr<Graphics::Texture2D> >&& textures
                ) 
                {
                    m_impl = { std::move(indices), std::move(geometry), std::move(ranges), std::move(textures) };
                }
        };

        SkinnedRenderObjectFactory::SkinnedRenderObjectFactory()
        {

        }

        SkinnedRenderObjectFactory::~SkinnedRenderObjectFactory() = default;
        SkinnedRenderObjectFactory& SkinnedRenderObjectFactory::operator=(SkinnedRenderObjectFactory&&) = default;

        SkinnedRenderObjectFactory::Impl*   SkinnedRenderObjectFactory::GetImpl()
        {
            return m_impl.get();
        }

        const SkinnedRenderObjectFactory::Impl*   SkinnedRenderObjectFactory::GetImpl() const
        {
            return m_impl.get();
        }

        std::unique_ptr<SkinnedRenderObject> SkinnedRenderObjectFactory::Create(const Assets::SkinnedModel* asset, Graphics::SkinnedGeometryFactory* geometry, Graphics::IndexedGeometryFactory* primitives, Graphics::UploadQueue* uploadQueue, Graphics::ResourceCreateContext* resourceCreateContext )
        {
            auto vertex_count   = asset->GetPositionSize();

            std::unique_ptr<Graphics::SkinnedGeometryAllocation> ge;
            std::unique_ptr<Graphics::IndexedGeometryAllocation> in;

            //geometry
            {
                using namespace uc::gx::geo;

                constexpr auto pos_stride = default_geometry_position::stride::value;
                constexpr auto uv_stride = default_geometry_uv::stride::value;
                constexpr auto blend_weight_stride = default_geometry_blend_weight::stride::value;
                constexpr auto blend_index_stride = default_geometry_blend_index::stride::value;

                auto r = geometry->CreateSkinnedGeometry(static_cast<uint32_t>(vertex_count));
                auto offset = r->GetVertexOffset();

                uploadQueue->UploadBuffer(geometry->GetPositions(), asset->GetPosition(), vertex_count * pos_stride, offset * pos_stride);
                uploadQueue->UploadBuffer(geometry->GetUV(), asset->GetUV(), vertex_count * uv_stride, offset * uv_stride);
                uploadQueue->UploadBuffer(geometry->GetBlendWeights(), asset->GetBlendWeight(), vertex_count * blend_weight_stride, offset * blend_weight_stride);
                uploadQueue->UploadBuffer(geometry->GetBlendIndices(), asset->GetBlendIndex(), vertex_count * blend_index_stride, offset * blend_index_stride);

                ge = std::move(r);
            }

            //indices
            {
                using namespace uc::gx::geo;

                constexpr auto index_stride = 4;
                auto index_count    = asset->GetIndexSize();
                auto r              = primitives->CreateIndexedGeometry(static_cast<uint32_t>(index_count));
                auto offset         = r->GetIndexOffset();
                uploadQueue->UploadBuffer(primitives->GetIndices(), asset->GetIndex(), index_count * index_stride, offset * index_stride);
                in = std::move(r);
            }

            //primitive ranges, which parts of the mesh are rendered with a texture
            std::vector< Graphics::PrimitiveRange > primitive_ranges;
            {
                auto asset_ranges = asset->GetPrimitiveRanges();
                auto s = static_cast<size_t>(asset_ranges.m_ranges_size);
                auto v = gsl::make_span<Graphics::PrimitiveRange>(asset_ranges.m_ranges, s);
                primitive_ranges.resize(s);
                std::copy(v.cbegin(), v.cend(), primitive_ranges.begin());
            }

            //textures
            std::vector< std::unique_ptr<Graphics::Texture2D> >  textures;
            {
                Assets::SkinnedModel::Texture2DDescription description = asset->GetTextureDescription();

                textures.resize(static_cast<size_t>(description.m_textures_size));

                for (auto i = 0U; i < description.m_textures_size; ++i)
                {
                    auto&& texture = description.m_textures[i];
                    auto desc = texture->GetDescription();
                    auto graphicsFormat = static_cast<Graphics::GraphicsFormat>(desc.m_view_format);
                    auto pitch = Graphics::TextureFormatUtils::GetRowSlicePitch(graphicsFormat, desc.m_width, desc.m_height);
                    
                    Graphics::SubresourceData s = { texture->GetData(), pitch.m_RowPitch, pitch.m_SlicePitch };
                    textures[i] = resourceCreateContext->CreateTexture2D({ static_cast<float>(desc.m_width), static_cast<float>(desc.m_height) }, graphicsFormat);
                    uploadQueue->UploadTexture(textures[i].get(), &s, 0, 1);
                }
            }

            return std::make_unique<SkinnedRenderObjectInternal>(std::move(in), std::move(ge), std::move(primitive_ranges), std::move(textures));
        }
    }
}


