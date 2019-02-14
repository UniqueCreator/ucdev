// ucdev_include_parser.cpp : Defines the entry point for the console application.
//
#pragma once

#include <uc_dev/gx/lip/geo.h>
#include <uc_dev/gx/import/geo/multi_material_mesh.h>

namespace uc
{
    namespace model
    {
        template <typename mesh_view>
        inline std::vector < lip::primitive_range > ranges( mesh_view* v )
        {
            std::vector<lip::primitive_range> r;

            size_t begin = 0;
            size_t end = 0;

            size_t s = v->material_count();

            for ( auto i = 0U; i < s; ++i)
            {
                auto&& faces = v->face(i);
                //auto&& pos   = v->position(i);

                end += faces.size();

                lip::primitive_range range;

                range.m_begin   = static_cast<uint32_t>(begin);
                range.m_end     = static_cast<uint32_t>(end);

                r.push_back(std::move(range));

                begin = end;
            }

            return r;
        }
    }
}

