#include "pch.h"

#include <uc_public/assets/graphics/texture_2d_factory.h>
#include <uc_public/uc_pimpl_impl.h>
#include <uc_public/assets/graphics/texture_2d.h>

#include "texture_2d_factory_impl.h"
#include "texture_2d_impl.h"
#include <uc_dev/gx/lip/file.h>

namespace UniqueCreator
{
    namespace Assets
    {
        template details::pimpl<Texture2DFactory::Impl>;

        class Texture2DInternal : public Texture2D
        {
            public:

            Texture2DInternal(uc::lip::unique_lip_pointer<uc::lip::texture2d> && texture) : m_texture(std::move(texture))
            {
                m_impl = std::move(m_texture.get());
            }

            uc::lip::unique_lip_pointer<uc::lip::texture2d> m_texture;
        };

        Texture2DFactory::Texture2DFactory()
        {

        }

        Texture2DFactory::~Texture2DFactory() = default;
        Texture2DFactory& Texture2DFactory::operator=(Texture2DFactory&&) = default;

        Texture2DFactory::Impl*   Texture2DFactory::GetImpl()
        {
            return m_impl.get();
        }

        const Texture2DFactory::Impl*   Texture2DFactory::GetImpl() const
        {
            return m_impl.get();
        }

        std::unique_ptr<Texture2D> Texture2DFactory::CreateFromFile(const wchar_t* fileName)
        {
            auto texture = uc::lip::create_from_compressed_lip_file<uc::lip::texture2d>(fileName);
            return std::make_unique<Texture2DInternal>(std::move(texture));
        }
    }
}


