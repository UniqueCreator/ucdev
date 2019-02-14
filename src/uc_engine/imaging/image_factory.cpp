#include "pch.h"

#include <uc_public/imaging/image_factory.h>
#include <uc_public/uc_pimpl_impl.h>

#include <uc_public/imaging/image_2d.h>
#include <uc_dev/gx/img/img.h>

#include "image_factory_impl.h"

#include "image_2d_internal.h"



namespace UniqueCreator
{
    namespace Imaging
    {
        template details::pimpl<ImageFactory::Impl>;

        ImageFactory::ImageFactory()
        {

        }

        ImageFactory::~ImageFactory() = default;
        ImageFactory& ImageFactory::operator=(ImageFactory&&) = default;

        ImageFactory::Impl*   ImageFactory::GetImpl()
        {
            return m_impl.get();
        }

        const ImageFactory::Impl*   ImageFactory::GetImpl() const
        {
            return m_impl.get();
        }

        std::unique_ptr<Image2D> ImageFactory::CreateFromFile( const wchar_t* fileName )
        {
            using namespace uc::gx::imaging;
            return std::make_unique<Image2DInternal>(std::move(read_image(fileName)));
        }

        std::unique_ptr<Image2D> ImageFactory::CreateCheckerBoardImage(uint32_t width, uint32_t height)
        {
            using namespace uc::gx::imaging;
            cpu_texture r( width, height, image_type::r8_g8_b8_a8_unorm, checker_board_texture_vector(32, 32) );
            return std::make_unique<Image2DInternal>(std::move(r));
        }
    }
}


