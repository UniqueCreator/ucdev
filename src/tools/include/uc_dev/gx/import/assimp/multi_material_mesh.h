#pragma once


#include <cwctype>
#include <locale>
#include <algorithm>

#include <assimp/postprocess.h>

#include <uc_dev/gx/import/assimp/assimp_details.h>
#include <uc_dev/gx/import/geo/multi_material_mesh.h>
#include <uc_dev/gx/import/assimp/indexed_mesh_import.h>

#include <gsl/gsl>

#include <ppl.h>
#include <uc_dev/util/utf8_conv.h>
#include <experimental/filesystem>

namespace uc
{
    namespace gx
    {
        namespace import
        {
            namespace assimp
            {
                inline std::vector< geo::multi_material_mesh::position_t > vertex( const aiMesh* mesh )
                {
                    using position_t = geo::multi_material_mesh::position_t;
                    std::vector< position_t > vertices;
                    vertices.resize(mesh->mNumVertices );

                    auto v0 = reinterpret_cast<position_t*>(&mesh->mVertices[0]);
                    auto v1 = reinterpret_cast<position_t*>(&vertices[0]);

                    std::memcpy(v1, v0, vertices.size() * sizeof(position_t));

                    return vertices;
                }

                inline std::vector< geo::multi_material_mesh::positions_t > vertices( const gsl::span<aiMesh*> meshes)
                {
                    using positions_t = geo::multi_material_mesh::positions_t;
                    std::vector< positions_t > vertices;
                    vertices.resize( meshes.size() );

                    concurrency::parallel_for(0U, static_cast<uint32_t>(meshes.size()), [&vertices, &meshes](uint32_t i)
                    {
                        vertices[i] = vertex(meshes[i]);
                    });

                    return vertices;
                }

                inline std::vector< geo::multi_material_mesh::uv_t > uv(const aiMesh* mesh)
                {
                    using uv_t = geo::multi_material_mesh::uv_t;
                    std::vector< uv_t > uv;
                    uv.reserve(mesh->mNumVertices);

                    auto uv_span = gsl::make_span(mesh->mTextureCoords[0], mesh->mNumVertices);

                    for (auto&& i : uv_span)
                    {
                        uv_t p;
                        p.x = i.x;
                        p.y = i.y;
                        uv.push_back(p);
                    }

                    return uv;
                }

                inline std::vector< geo::multi_material_mesh::normal_t > normal(const aiMesh* mesh)
                {
                    using normal_t = geo::multi_material_mesh::normal_t;
                    std::vector< normal_t > normal;
                    normal.reserve(mesh->mNumVertices);

                    auto normal_span = gsl::make_span(mesh->mNormals, mesh->mNumVertices);

                    for (auto&& i : normal_span)
                    {
                        normal_t p;
                        p.x = i.x;
                        p.y = i.y;
                        p.z = i.z;
                        normal.push_back(p);
                    }

                    return normal;
                }

                inline std::vector< geo::multi_material_mesh::tangent_t > tangent(const aiMesh* mesh)
                {
                    using tangent_t = geo::multi_material_mesh::tangent_t;
                    std::vector< tangent_t > tangent;
                    tangent.reserve(mesh->mNumVertices);

                    auto normal_span = gsl::make_span(mesh->mTangents, mesh->mNumVertices);

                    for (auto&& i : normal_span)
                    {
                        tangent_t p;
                        p.x = i.x;
                        p.y = i.y;
                        p.z = i.z;
                        //p.w = 0;
                        tangent.push_back(p);
                    }

                    return tangent;
                }

                inline std::vector< geo::multi_material_mesh::uvs_t > uvs(const gsl::span<aiMesh*> meshes)
                {
                    using uvs_t = geo::multi_material_mesh::uvs_t;
                    std::vector< uvs_t > vertices;
                    vertices.resize(meshes.size());

                    concurrency::parallel_for(0U, static_cast<uint32_t>(meshes.size()), [&vertices, &meshes](uint32_t i)
                    {
                        vertices[i] = uv(meshes[i]);
                    });
                    return vertices;
                }

                inline std::vector< geo::multi_material_mesh::normals_t > normals(const gsl::span<aiMesh*> meshes)
                {
                    using normals_t = geo::multi_material_mesh::normals_t;
                    std::vector< normals_t > vertices;
                    vertices.resize(meshes.size());

                    concurrency::parallel_for(0U, static_cast<uint32_t>(meshes.size()), [&vertices, &meshes](uint32_t i)
                    {
                        vertices[i] = normal(meshes[i]);
                    });
                    return vertices;
                }

                inline std::vector< geo::multi_material_mesh::tangents_t > tangents(const gsl::span<aiMesh*> meshes)
                {
                    using tangents_t = geo::multi_material_mesh::tangents_t;
                    std::vector< tangents_t > vertices;
                    vertices.resize(meshes.size());

                    concurrency::parallel_for(0U, static_cast<uint32_t>(meshes.size()), [&vertices, &meshes](uint32_t i)
                    {
                        vertices[i] = tangent(meshes[i]);
                    });
                    return vertices;
                }

                inline std::vector< geo::multi_material_mesh::face_t > face(const aiMesh* mesh)
                {
                    using face_t = geo::multi_material_mesh::face_t;

                    std::vector< face_t > f;
                    f.resize(mesh->mNumFaces);

                    auto face_span = gsl::make_span( mesh->mFaces, mesh->mNumFaces );
                    
                    auto fc        = mesh->mNumFaces;

                    for (auto i = 0U; i < fc; ++i)
                    {
                        auto f0 = mesh->mFaces[i];
                        auto f1 = &f[i];

                        f1->v0  = f0.mIndices[0];
                        f1->v1  = f0.mIndices[1];
                        f1->v2  = f0.mIndices[2];
                    }

                    return f;
                }

                inline std::vector< geo::multi_material_mesh::faces_t > faces(const gsl::span<aiMesh*> meshes)
                {
                    using faces_t = geo::multi_material_mesh::faces_t;
                    std::vector< faces_t > vertices;
                    vertices.resize(meshes.size());

                    concurrency::parallel_for( 0U, static_cast<uint32_t>(meshes.size() ), [&vertices,&meshes](uint32_t i)
                    {
                        vertices[i] = face(meshes[i]);
                    });
                        
                    return vertices;
                }

                inline std::string material_name( const aiMaterial* m )
                {
                    aiString path0;
                    aiString path1;

                    if (AI_SUCCESS == aiGetMaterialString(m, AI_MATKEY_TEXTURE_OPACITY(0), &path0))
                    {

                    }
                    else if (AI_SUCCESS == aiGetMaterialString(m, AI_MATKEY_TEXTURE_DIFFUSE(0), &path0))
                    {

                    }
                    else
                    {
                        return "null";
                    }

                    using namespace  std::experimental::filesystem;
                    auto wname       = path(path0.C_Str() ) .stem();
                    std::string data = util::utf8_from_utf16(wname);
                    std::transform(data.begin(), data.end(), data.begin(), [](auto x) -> char { return static_cast<char>(::tolower(x)); });
                    return data;
                }

                inline std::string material_name(const std::string& s)
                {
                    using namespace  std::experimental::filesystem;
                    auto wname = path(s).stem();

                    std::string data = util::utf8_from_utf16(wname);
                    std::transform(data.begin(), data.end(), data.begin(), [](auto x) -> char { return static_cast<char>(::tolower(x)); } );
                    return data;
                }

                inline geo::multi_material_mesh::material material( const aiMaterial* m )
                {
                    geo::multi_material_mesh::material mat = { material_name(m), geo::multi_material_mesh::diffuse };
                    return mat;
                }

                inline std::vector< geo::multi_material_mesh::material > materials( const aiScene* scene, const gsl::span<aiMesh*> meshes  )
                {
                    using namespace std;
                    
                    std::vector< geo::multi_material_mesh::material > vertices;
                    vertices.reserve(meshes.size());

                    for (auto&& i : meshes)
                    {
                        vertices.push_back(material(scene->mMaterials[i->mMaterialIndex]));
                    }

                    return vertices;
                }

                inline std::unique_ptr< geo::multi_material_mesh > create_multi_material_mesh(const std::string& file_name, uint32_t import_flags = aiProcess_ImproveCacheLocality | aiProcess_ValidateDataStructure | aiProcess_OptimizeMeshes | aiProcess_OptimizeGraph | aiProcess_FindInvalidData)
                {
                    using namespace gx::import::assimp;
                    auto scene = load_assimp_scene(file_name, import_flags);
                    validate_scene(scene.get(), validation_option::meshes);

                    auto meshes     = gsl::make_span(scene->mMeshes, scene->mNumMeshes);
                    auto v          = vertices(meshes);
                    auto uv         = uvs(meshes);
                    auto face       = faces(meshes);
                    auto mat        = materials(scene.get(), meshes);
                    auto normal     = normals(meshes);
                    auto tangent    = tangents(meshes);

                    return std::make_unique<geo::multi_material_mesh>(std::move(v), std::move(normal), std::move(tangent), std::move(uv), std::move(face), std::move(mat));
                }

                inline std::vector< geo::multi_material_mesh::faces_t > create_multi_material_mesh_faces(const std::string& file_name, uint32_t import_flags = aiProcess_ImproveCacheLocality | aiProcess_ValidateDataStructure | aiProcess_OptimizeMeshes | aiProcess_OptimizeGraph | aiProcess_FindInvalidData)
                {
                    using namespace gx::import::assimp;
                    auto scene = load_assimp_scene(file_name, import_flags);
                    validate_scene(scene.get(), validation_option::meshes );

                    auto meshes = gsl::make_span(scene->mMeshes, scene->mNumMeshes);
                    return faces(meshes);
                }

            }
        }
    }
}






