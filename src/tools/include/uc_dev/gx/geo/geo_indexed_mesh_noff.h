#pragma once

#include <uc_dev/gx/geo/geo_indexed_mesh.h>

namespace uc
{
    namespace gx
    {
        namespace geo
        {
            inline std::shared_ptr<indexed_mesh> create_from_noff_file(const std::wstring& filename)
            {
                std::vector< indexed_mesh::vertex >   vertices;
                std::vector< indexed_mesh::face >     faces;

                std::ifstream file(filename, std::ifstream::in);

                if (file.good())
                {
                    std::string type;
                    file >> type;

                    uint32_t vertex_count = 0;
                    uint32_t face_count = 0;
                    uint32_t edge_count = 0;

                    file >> vertex_count;
                    file >> face_count;
                    file >> edge_count;

                    vertices.reserve(vertex_count);
                    faces.reserve(face_count);

                    for (uint32_t i = 0; i < vertex_count && file.good(); ++i)
                    {
                        {
                            indexed_mesh::vertex v;
                            file >> v.x >> v.y >> v.z;
                            vertices.push_back(v);
                        }

                        {
                            indexed_mesh::normal n = {};
                            file >> n.x >> n.y >> n.z; //skip normals, we calculate them
                        }
                    }

                    for (uint32_t i = 0; i < face_count && file.good(); ++i)
                    {
                        indexed_mesh::face  face;
                        uint32_t    face_size;

                        file >> face_size;
                        file >> face.v0 >> face.v1 >> face.v2;
                        faces.push_back(face);
                    }
                }

                return std::shared_ptr<indexed_mesh>(new indexed_mesh(std::move(vertices), std::move(faces)));
            };
        }
    }
}







