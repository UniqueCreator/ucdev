#pragma once

#include <uc_public/uc_implementation.h>

#include <uc_public/graphics/graphics_types.h>

namespace UniqueCreator
{
    namespace Graphics
    {
        class Buffer;
        class ComputeQueue;
        class ComputePipelineState;
        class CopyQueue;
        class ViewColorBuffer;
        class ViewDepthBuffer;
        class DirectQueue;
        class GraphicsPipelineState;
        class RootSignature;
        class Texture2D;

        class ResourceCreateContext : private NonCopyable
        {
            public:

            class Impl;
            UC_ENGINE_API ResourceCreateContext();
            UC_ENGINE_API ~ResourceCreateContext();

            UC_ENGINE_API ResourceCreateContext(ResourceCreateContext&&) = default;
            UC_ENGINE_API ResourceCreateContext& operator=(ResourceCreateContext&&);

            UC_ENGINE_API std::unique_ptr<DirectQueue>              CreateDirectQueue();
            UC_ENGINE_API std::unique_ptr<CopyQueue>                CreateCopyQueue();
            UC_ENGINE_API std::unique_ptr<ComputeQueue>             CreateComputeQueue();

            UC_ENGINE_API std::unique_ptr<ViewColorBuffer>          CreateViewColorBuffer(Size2D s, ColorBufferFormat format);
            UC_ENGINE_API std::unique_ptr<ViewDepthBuffer>          CreateViewDepthBuffer(Size2D s, DepthBufferFormat format);

            UC_ENGINE_API std::unique_ptr<GraphicsPipelineState>    CreateGraphicsPipelineState(const GraphicsPipelineStateDescription* desc);
            UC_ENGINE_API std::unique_ptr<ComputePipelineState>     CreateComputePipelineState(const ComputePipelineStateDescription* description);

            UC_ENGINE_API std::unique_ptr<RootSignature>            CreateRootSignature(const RootSignatureByteCode* code);
            UC_ENGINE_API std::unique_ptr<Texture2D>                CreateTexture2D(Size2D s, GraphicsFormat format);
            UC_ENGINE_API std::unique_ptr<Texture2D>                CreateTexture2D(Size2D s, GraphicsFormat format, uint32_t mipCount);
            UC_ENGINE_API std::unique_ptr<Buffer>                   CreateBuffer(uint64_t size);

            UC_ENGINE_API void Sync();
            UC_ENGINE_API void ResetViewDependentResources();

            Impl*   GetImpl();
            const Impl*   GetImpl() const;

            protected:
            details::pimpl<Impl> m_impl;
        };

        //private data, todo: fix visual assist warning
        extern template details::pimpl<ResourceCreateContext::Impl>;
    }
}


