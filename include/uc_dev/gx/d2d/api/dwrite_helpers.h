#pragma once

#include <cstdint>

#include <dwrite.h>
#include <cstdint>
#include <wrl/client.h>

#include <uc_dev/gx/d2d/api/dwrite_error.h>

namespace uc
{
    namespace gx
    {
        namespace dwrite
        {
            inline Microsoft::WRL::ComPtr<IDWriteFactory> create_dwrite_factory()
            {
                Microsoft::WRL::ComPtr<IDWriteFactory> result;
                
                throw_if_failed(DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(result.GetAddressOf())));
                return result;
            }
            
            inline Microsoft::WRL::ComPtr<IDWriteTextFormat>  create_text_format(IDWriteFactory* factory)
            {
                Microsoft::WRL::ComPtr<IDWriteTextFormat> result;

                throw_if_failed(factory->CreateTextFormat(
                    L"Courier",
                    NULL,
                    DWRITE_FONT_WEIGHT_REGULAR,
                    DWRITE_FONT_STYLE_NORMAL,
                    DWRITE_FONT_STRETCH_NORMAL,
                    14.0f,
                    L"en-us",
                    &result
                ));

                return result;
            }
        }
    }
}

