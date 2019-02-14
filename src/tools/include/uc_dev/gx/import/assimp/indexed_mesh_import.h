#pragma once


#include <uc_dev/gx/import/geo/indexed_mesh.h>

#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/vector3.h>

#include <gsl/gsl>



namespace uc
{
    namespace gx
    {
        namespace import
        {
            namespace assimp
            {
                namespace details
                {
                    inline std::vector<gx::import::geo::indexed_mesh::position> create_vertices(const aiMesh* m)
                    {
                        std::vector< gx::import::geo::indexed_mesh::position >   vertices;

                        auto vs = m->mVertices;
                        auto vc = m->mNumVertices;

                        vertices.resize(vc);

                        auto v0 = reinterpret_cast<gx::import::geo::indexed_mesh::position*>(vs);
                        auto v1 = reinterpret_cast<gx::import::geo::indexed_mesh::position*>(&vertices[0]);

                        auto span0 = gsl::make_span(v0, vc);
                        auto span1 = gsl::make_span(v1, vc);

                        using namespace std;

                        //note: does not use mem copy, check why?
                        //_Trivially_copyable_ptr_iterator_tag
                        copy(begin(span0), end(span0), begin(span1));

                        return vertices;
                    }

                    inline std::vector<gx::import::geo::indexed_mesh::face> create_faces(const aiMesh* m)
                    {
                        std::vector< gx::import::geo::indexed_mesh::face >     faces;

                        auto fc = m->mNumFaces;
                        faces.resize(fc);

                        for (auto i = 0U; i < fc; ++i)
                        {
                            auto f0 = m->mFaces[i];
                            auto f1 = &faces[i];

                            f1->v0 = f0.mIndices[0];
                            f1->v1 = f0.mIndices[1];
                            f1->v2 = f0.mIndices[2];
                        }

                        return faces;
                    }

                    inline std::vector<gx::import::geo::indexed_mesh::uv> create_uv(const aiMesh* m)
                    {
                        std::vector< gx::import::geo::indexed_mesh::uv >   uv;

                        if (m->HasTextureCoords(0))
                        {
                            auto vs = m->mTextureCoords[0];
                            auto vc = m->mNumVertices;

                            uv.reserve(vc);

                            auto v0 = reinterpret_cast<aiVector3D*>(vs);
                            auto span0 = gsl::make_span(v0, vc);

                            using namespace std;

                            for_each(begin(span0), end(span0), [&uv](const auto& p)
                            {
                                gx::import::geo::indexed_mesh::uv u;

                                u.x = p[0];
                                u.y = p[1];
                                uv.push_back(u);
                            });
                        }

                        return uv;
                    }

                }
            }
        }
    }
}







