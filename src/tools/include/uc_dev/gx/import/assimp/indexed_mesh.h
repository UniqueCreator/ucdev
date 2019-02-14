#pragma once


#include <uc_dev/gx/import/geo/indexed_mesh.h>
#include <uc_dev/gx/import/assimp/indexed_mesh_import.h>
#include <uc_dev/gx/error.h>

#include <assimp/cimport.h>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/vector3.h>
#include <assimp/Importer.hpp>

#include <gsl/gsl>

#include <uc_dev/util/utf8_conv.h>


namespace uc
{
    namespace gx
    {
        namespace import
        {
            namespace assimp
            {
                inline std::shared_ptr<geo::indexed_mesh> create_mesh(const std::string& file_name, uint32_t import_flags = aiProcess_ImproveCacheLocality | aiProcess_ValidateDataStructure | aiProcess_OptimizeMeshes | aiProcess_OptimizeGraph | aiProcess_FindInvalidData)
                {
                    Assimp::Importer i;

                    auto result = i.ReadFile(file_name.c_str(), import_flags);

                    if (result == nullptr)
                    {
                        ::uc::gx::throw_exception<uc::gx::assimp_exception>(i.GetErrorString());
                    }

                    if (result->mNumMeshes == 0)
                    {
                        ::uc::gx::throw_exception<uc::gx::assimp_exception>("Empty meshes");
                    }

                    auto m = result->mMeshes[0];
                    return std::shared_ptr<gx::import::geo::indexed_mesh>(new gx::import::geo::indexed_mesh(details::create_vertices(m), details::create_uv(m), details::create_faces(m)));
                }

                inline std::shared_ptr<gx::import::geo::indexed_mesh> create_mesh(const std::wstring& file_name, uint32_t import_flags = aiProcess_ImproveCacheLocality | aiProcess_ValidateDataStructure | aiProcess_OptimizeMeshes | aiProcess_OptimizeGraph | aiProcess_FindInvalidData)
                {
                    auto u = util::utf8_from_utf16(file_name);
                    return create_mesh(u, import_flags);
                }
            }
        }
    }
}


