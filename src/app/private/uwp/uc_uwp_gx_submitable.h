#pragma once

#include <memory>
#include <vector>

#include <uc_dev/gx/dx12/dx12.h>
#include <uc_dev/util/noncopyable.h>

namespace uc
{
    namespace uwp
    {
        class submitable
        {
            public:
            submitable() = default;
            virtual ~submitable() = default;

            void submit()
            {
                on_submit();
            }

            private:
                    
            submitable(const submitable&) = delete;
            submitable& operator=(const submitable&) = delete;

            virtual void on_submit() = 0;
        };


        template <typename sub > class context_submitable : public submitable
        {

            public:
            context_submitable(sub&& ctx) : m_ctx(std::move(ctx))
            {

            }

            private:
            context_submitable(const context_submitable&) = delete;
            context_submitable& operator=(const submitable&) = delete;
            sub m_ctx;

            void on_submit()
            {
                m_ctx->submit();
            }
        };


        using graphics_submitable           = context_submitable<gx::dx12::managed_graphics_command_context>;
        using compute_submitable            = context_submitable<gx::dx12::managed_compute_command_context>;
        using graphics_compute_submitable   = context_submitable<gx::dx12::managed_graphics_compute_command_context>;

    }
}

