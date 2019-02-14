// ucdev_include_parser.cpp : Defines the entry point for the console application.
//
#pragma once

#include <vector>
#include <gsl/gsl>

#include <uc_dev/gx/lip/geo.h>

#include <uc_dev/gx/import/geo/indexed_mesh.h>
#include <uc_dev/gx/import/geo/skinned_mesh.h>

namespace uc
{
    namespace model
    {
        template <typename t>
        void copy_indices(const std::vector< gx::import::geo::indexed_mesh::face >& faces, t& out)
        {
            size_t index = 0;

            for (auto&& i : faces)
            {
                out[index++] = i.v0;
                out[index++] = i.v1;
                out[index++] = i.v2;
            }
        }

        template <typename t>
        void copy_positions( const std::vector< gx::import::geo::indexed_mesh::position >& positions, t& out )
        {
            size_t vertex = 0;

            for (auto&& i : positions)
            {
                out[vertex].m_x = i.x;
                out[vertex].m_y = i.y;
                out[vertex].m_z = i.z;
                vertex++;
            }
        }

        template <typename t>
        void copy_normals(const std::vector< gx::import::geo::indexed_mesh::normal >& normals, t& out)
        {
            size_t vertex = 0;

            for (auto&& i : normals)
            {
                out[vertex].m_x = i.x;
                out[vertex].m_y = i.y;
                out[vertex].m_z = i.z;
                vertex++;
            }
        }

        template <typename t>
        void copy_tangents(const std::vector< gx::import::geo::indexed_mesh::tangent >& tangents, t& out)
        {
            size_t vertex = 0;

            for (auto&& i : tangents)
            {
                out[vertex].m_x = i.x;
                out[vertex].m_y = i.y;
                out[vertex].m_z = i.z;
                vertex++;
            }
        }

        template <typename t>
        void copy_uv(const std::vector< gx::import::geo::indexed_mesh::uv >& uv, t& out)
        {
            size_t vertex = 0;

            for (auto&& i : uv)
            {
                out[vertex].m_x = i.x;
                out[vertex].m_y = i.y;
                vertex++;
            }
        }

        template <typename t>
        void copy_blend_weights(const std::vector< gx::import::geo::skinned_mesh::blend_weight_t >& weight, t& out)
        {
            size_t vertex = 0;

            for (auto&& i : weight)
            {
                out[vertex].m_x = i.x;
                out[vertex].m_y = i.y;
                out[vertex].m_z = i.z;
                out[vertex].m_w = i.w;
                vertex++;
            }
        }

        template <typename t>
        void copy_blend_indices(const std::vector< gx::import::geo::skinned_mesh::blend_index_t >& index, t& out)
        {
            size_t vertex = 0;

            for (auto&& i : index)
            {
                out[vertex].m_x = static_cast<uint8_t> (i.x);
                out[vertex].m_y = static_cast<uint8_t> (i.y);
                out[vertex].m_z = static_cast<uint8_t> (i.z);
                out[vertex].m_w = static_cast<uint8_t> (i.w);
                vertex++;
            }
        }

    }
}

