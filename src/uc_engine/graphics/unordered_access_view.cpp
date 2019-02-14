#include "pch.h"

#include <uc_public/graphics/unordered_access_view.h>
#include <uc_public/uc_pimpl_impl.h>

#include "unordered_access_view_impl.h"

namespace UniqueCreator
{
    namespace Graphics
    {
        UnorderedAccessView::UnorderedAccessView()
        {

        }

        UnorderedAccessView::~UnorderedAccessView() = default;
        UnorderedAccessView& UnorderedAccessView::operator=(UnorderedAccessView&&) = default;
    }
}


