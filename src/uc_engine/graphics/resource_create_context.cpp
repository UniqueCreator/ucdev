#include "pch.h"

#include <uc_public/graphics/resource_create_context.h>
#include <uc_public/uc_pimpl_impl.h>

#include "resource_create_context_impl.h"


namespace UniqueCreator
{
    namespace Graphics
    {
        template details::pimpl<ResourceCreateContext::Impl>;

        ResourceCreateContext::ResourceCreateContext()
        {

        }

        ResourceCreateContext::~ResourceCreateContext() = default;
        ResourceCreateContext& ResourceCreateContext::operator=(ResourceCreateContext&&) = default;

        std::unique_ptr<DirectQueue> ResourceCreateContext::CreateDirectQueue()
        {
            return m_impl->CreateDirectQueue();
        }

        std::unique_ptr<CopyQueue> ResourceCreateContext::CreateCopyQueue()
        {
            return m_impl->CreateCopyQueue();
        }

        std::unique_ptr<ComputeQueue> ResourceCreateContext::CreateComputeQueue()
        {
            return m_impl->CreateComputeQueue();
        }

        std::unique_ptr<ViewColorBuffer> ResourceCreateContext::CreateViewColorBuffer(Size2D s, ColorBufferFormat format)
        {
            return m_impl->CreateViewColorBuffer(s, format);
        }

        std::unique_ptr<ViewDepthBuffer> ResourceCreateContext::CreateViewDepthBuffer(Size2D s, DepthBufferFormat format)
        {
            return m_impl->CreateViewDepthBuffer(s, format);
        }

        std::unique_ptr<GraphicsPipelineState> ResourceCreateContext::CreateGraphicsPipelineState(const GraphicsPipelineStateDescription* desc)
        {
            return m_impl->CreateGraphicsPipelineState(desc);
        }

        std::unique_ptr<ComputePipelineState> ResourceCreateContext::CreateComputePipelineState(const ComputePipelineStateDescription* description)
        {
            return m_impl->CreateComputePipelineState(description);
        }

        std::unique_ptr<Texture2D> ResourceCreateContext::CreateTexture2D(Size2D s, GraphicsFormat format)
        {
            return m_impl->CreateTexture2D(s, format);
        }

        std::unique_ptr<Texture2D> ResourceCreateContext::CreateTexture2D(Size2D s, GraphicsFormat format, uint32_t mipCount)
        {
            return m_impl->CreateTexture2D(s, format, mipCount);
        }

        std::unique_ptr<Buffer> ResourceCreateContext::CreateBuffer(uint64_t size)
        {
            return m_impl->CreateBuffer(size);
        }

        ResourceCreateContext::Impl*   ResourceCreateContext::GetImpl()
        {
            return m_impl.get();
        }

        const ResourceCreateContext::Impl*   ResourceCreateContext::GetImpl() const
        {
            return m_impl.get();
        }

        void ResourceCreateContext::Sync()
        {
            m_impl->Sync();
        }

        void ResourceCreateContext::ResetViewDependentResources()
        {
            m_impl->ResetViewDependentResources();
        }

        std::unique_ptr<RootSignature> ResourceCreateContext::CreateRootSignature(const RootSignatureByteCode* code)
        {
            return m_impl->CreateRootSignature(code);
        }
    }
}


