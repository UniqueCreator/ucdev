#include "pch.h"

#include <uc_public/assets/graphics/skinned_model_factory.h>
#include <uc_public/uc_pimpl_impl.h>
#include <uc_public/assets/graphics/skinned_model.h>

#include "skinned_model_factory_impl.h"
#include "skinned_model_impl.h"
#include "texture_2d_mip_chain_impl.h"
#include <uc_dev/gx/lip/file.h>

namespace UniqueCreator
{
    namespace Assets
    {
        class SkinnedModelFactory::Impl
        {
        public:

            Impl() {}
        };

        template details::pimpl<SkinnedModelFactory::Impl>;

        class SkinnedModelInternal : public SkinnedModel
        {
        public:

            SkinnedModelInternal(uc::lip::unique_lip_pointer<uc::lip::derivatives_skinned_model> && model)
            {
                m_impl = std::move(model);
            }
        };

        SkinnedModelFactory::SkinnedModelFactory()
        {

        }

        SkinnedModelFactory::~SkinnedModelFactory() = default;
        SkinnedModelFactory& SkinnedModelFactory::operator=(SkinnedModelFactory&&) = default;

        SkinnedModelFactory::Impl*   SkinnedModelFactory::GetImpl()
        {
            return m_impl.get();
        }

        const SkinnedModelFactory::Impl*   SkinnedModelFactory::GetImpl() const
        {
            return m_impl.get();
        }

        std::unique_ptr<SkinnedModel> SkinnedModelFactory::CreateFromFile(const wchar_t* fileName)
        {
            auto model = uc::lip::create_from_compressed_lip_file<uc::lip::derivatives_skinned_model>(fileName);
            return std::make_unique<SkinnedModelInternal>(std::move(model));
        }
    }
}


