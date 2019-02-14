#pragma once

#include <cstdint>

#include <DXGI.h>
#include <DXGI1_2.h>
#include <wrl/client.h>

#include <d2d1.h>
#include <d2d1_2.h>
#include <d2d1_3.h>

#include <uc_dev/gx/d2d/api/error.h>

#include <d3d11.h>


namespace uc
{
    namespace gx
    {
        namespace d2d
        {

            inline Microsoft::WRL::ComPtr<ID2D1Factory3> create_d2d_factory_single_threaded()
            {
                Microsoft::WRL::ComPtr<ID2D1Factory3> result;

                D2D1_FACTORY_OPTIONS options = {};

                options.debugLevel = D2D1_DEBUG_LEVEL_INFORMATION;

                throw_if_failed(D2D1CreateFactory<ID2D1Factory3>(D2D1_FACTORY_TYPE_SINGLE_THREADED, options, result.GetAddressOf()));

                return result;
            }

            inline Microsoft::WRL::ComPtr<ID2D1Factory3> create_d2d_factory_multi_threaded()
            {
                Microsoft::WRL::ComPtr<ID2D1Factory3> result;

                D2D1_FACTORY_OPTIONS options = {};

                options.debugLevel = D2D1_DEBUG_LEVEL_INFORMATION;

                throw_if_failed(D2D1CreateFactory<ID2D1Factory3>(D2D1_FACTORY_TYPE_MULTI_THREADED, options, result.GetAddressOf()));

                return result;
            }

            inline Microsoft::WRL::ComPtr<ID2D1Device> create_device(ID2D1Factory3* factory, IDXGIDevice* dxgi)
            {
                Microsoft::WRL::ComPtr<ID2D1Device> result;

                throw_if_failed(factory->CreateDevice(dxgi, result.GetAddressOf() ));

                return result;
            }

            inline Microsoft::WRL::ComPtr<ID2D1DeviceContext> create_device_context(ID2D1Device* device, D2D1_DEVICE_CONTEXT_OPTIONS options = D2D1_DEVICE_CONTEXT_OPTIONS_NONE)
            {
                Microsoft::WRL::ComPtr<ID2D1DeviceContext> result;

                throw_if_failed(device->CreateDeviceContext(options, result.GetAddressOf()));
                return result;
            }

            inline D2D1_RENDER_TARGET_PROPERTIES create_default_render_target_properties(float dpi_x, float dpi_y)
            {
                D2D1_PIXEL_FORMAT pixel_format =
                {
                        DXGI_FORMAT_UNKNOWN,
                        D2D1_ALPHA_MODE_PREMULTIPLIED
                };

                D2D1_RENDER_TARGET_PROPERTIES properties =
                {
                    D2D1_RENDER_TARGET_TYPE_DEFAULT,
                    pixel_format,
                    dpi_x,
                    dpi_y,
                    D2D1_RENDER_TARGET_USAGE_NONE,
                    D2D1_FEATURE_LEVEL_10
                };

                return properties;
            }

            inline D2D1_BITMAP_PROPERTIES1 create_default_bitmap_properties(float dpi_x, float dpi_y)
            {
                D2D1_PIXEL_FORMAT pixel_format =
                {
                    DXGI_FORMAT_UNKNOWN,
                    D2D1_ALPHA_MODE_PREMULTIPLIED
                };

                D2D1_BITMAP_PROPERTIES1 properties =
                {
                    pixel_format,
                    dpi_x,
                    dpi_y,
                    D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW
                };

                return properties;
            }

            inline Microsoft::WRL::ComPtr<ID2D1RenderTarget> create_render_target(ID2D1Factory* factory, IDXGISurface* surface, float dpi_x, float dpi_y)
            {
                Microsoft::WRL::ComPtr<ID2D1RenderTarget> result;

                D2D1_RENDER_TARGET_PROPERTIES properties = create_default_render_target_properties(dpi_x, dpi_y);
                throw_if_failed(factory->CreateDxgiSurfaceRenderTarget(surface, &properties, result.GetAddressOf() ));
                return result;
            }

            inline Microsoft::WRL::ComPtr<ID2D1RenderTarget> create_render_target(ID2D1Factory* factory, ID3D11Texture2D* const texture, float dpi_x, float dpi_y)
            {
                Microsoft::WRL::ComPtr<IDXGISurface> surface;
                
                throw_if_failed(texture->QueryInterface( IID_IDXGISurface, reinterpret_cast<void**> (surface.GetAddressOf() )));
                return create_render_target(factory, surface.Get(), dpi_x, dpi_y);
            }

            inline Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> create_solid_color_brush(ID2D1RenderTarget* render_target)
            {
                Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> result;
                throw_if_failed(render_target->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White, 1.0f), &result));
                return result;
            }

            inline Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> create_solid_color_brush2(ID2D1RenderTarget* render_target)
            {
                Microsoft::WRL::ComPtr<ID2D1SolidColorBrush>  result;

                const float fraction = 1.0f / 32.0f;
                throw_if_failed(render_target->CreateSolidColorBrush(D2D1::ColorF(fraction, fraction, fraction, fraction), &result));
                return result;
            }

            inline Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> create_solid_color_brush(ID2D1DeviceContext* render_target)
            {
                Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> result;
                throw_if_failed(render_target->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White, 1.0f), &result));
                return result;
            }


            inline Microsoft::WRL::ComPtr<ID2D1Bitmap1> create_bitmap_from_dxgi_surface(ID2D1DeviceContext* context, IDXGISurface* surface, float dpi_x, float dpi_y)
            {
                Microsoft::WRL::ComPtr<ID2D1Bitmap1> r;

                throw_if_failed( context->CreateBitmapFromDxgiSurface(surface, create_default_bitmap_properties( dpi_x, dpi_y ), &r) );
                return r;
            }

            


        }
    }
}




