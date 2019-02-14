#pragma once

#include <cstdint>
#include <memory>

#include "uc_uwp_gx_render_object.h"


namespace uc
{
    namespace gx
    {
        namespace geo
        {
            class skinned_meshes_allocator;
        }
    }
    namespace uwp
    {
        class device_resources;
        namespace gxu
        {
            template <typename t> struct render_object_factory
            {
                template <typename... args>
                static std::unique_ptr<t> make_render_object(args&&... a);
            };

            template <typename t, typename... args> std::unique_ptr<t> make_render_object_from_file( const wchar_t* file_name, device_resources* r, args&&... a )
            {
                return render_object_factory<t>::make_render_object(file_name, r, std::forward<args>(a)...);
            }

            template<> struct render_object_factory< skinned_render_object>
            {
                static std::unique_ptr< skinned_render_object > make_render_object(const wchar_t* file_name, device_resources* resources, gx::geo::geometry_allocator* r );
            };

            template<> struct render_object_factory< static_render_object>
            {
                static std::unique_ptr< static_render_object > make_render_object(const wchar_t* file_name, device_resources* resources, gx::geo::geometry_allocator* r);
            };

            template<> struct render_object_factory< parametrized_render_object>
            {
                static std::unique_ptr< parametrized_render_object > make_render_object(const wchar_t* file_name, device_resources* resources, gx::geo::geometry_allocator* r);
            };
        }
    }
}
