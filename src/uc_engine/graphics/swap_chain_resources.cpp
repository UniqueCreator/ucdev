#include "pch.h"

#include <uc_public/graphics/swap_chain_resources.h>
#include <uc_public/uc_pimpl_impl.h>

#include "swap_chain_resources_impl.h"


namespace UniqueCreator
{
    namespace Graphics
    {
        template details::pimpl<SwapChainResources::Impl>;

        SwapChainResources::SwapChainResources(SwapChainNative* s, DirectQueue* q, ResourceCreateContext* ctx) : m_impl(s, q, ctx)
        {

        }

        SwapChainResources::~SwapChainResources() = default;
        SwapChainResources& SwapChainResources::operator=(SwapChainResources&&) = default;

        void SwapChainResources::WaitForGpu()
        {
            m_impl->WaitForGpu();
        }

        void SwapChainResources::WaitForFence(const FenceHandle*v)
        {
            m_impl->WaitForFence(v);
        }

        void SwapChainResources::InsertWaitOn(const FenceHandle*v)
        {
            m_impl->InsertWaitOn(v);
        }

        void SwapChainResources::MoveToNextFrame()
        {
            m_impl->MoveToNextFrame();
        }

        void SwapChainResources::Sync()
        {
            m_impl->Sync();
        }

        void SwapChainResources::Present()
        {
            m_impl->Present();
        }

        void SwapChainResources::SetSourceSize(uint32_t width, uint32_t height)
        {
            m_impl->SetSourceSize(width, height);
        }

        void SwapChainResources::Resize(uint32_t width, uint32_t height)
        {
            m_impl->Resize(width, height);
        }

        SwapChainNative* SwapChainResources::GetSwapChain() const
        {
            return m_impl->GetSwapChain();
        }

        std::unique_ptr<DirectCommandContext> SwapChainResources::CreateCommandContext()
        {
            return m_impl->CreateCommandContext();
        }

        BackBuffer* SwapChainResources::GetBackBuffer() const
        {
            return m_impl->GetBackBuffer();
        }
    }
}


