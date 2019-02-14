#pragma once

#include <memory>
#include <string>

#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/vector3.h>

#include <uc_dev/gx/error.h>

namespace uc
{
    namespace gx
    {
        namespace import
        {
            namespace assimp
            {
                enum validation_option
                {
                    meshes = 1,
                    animation = 2
                };

                inline void validate_scene(const aiScene* s, uint32_t validation_mask)
                {
                    if ((validation_mask & validation_option::meshes) && !s->HasMeshes())
                    {
                        ::uc::gx::throw_exception<uc::gx::assimp_exception>("missing meshes");
                    }

                    if ((validation_mask & validation_option::animation) && !s->HasAnimations())
                    {
                        ::uc::gx::throw_exception<uc::gx::assimp_exception>("missing animations");
                    }
                }

                std::shared_ptr<const aiScene> load_assimp_scene(const std::string& file_name, uint32_t import_flags)
                {
                    auto importer = std::make_shared<Assimp::Importer>();
                    auto result = importer->ReadFile(file_name.c_str(), import_flags);

                    if (result == nullptr)
                    {
                        ::uc::gx::throw_exception<uc::gx::assimp_exception>(importer->GetErrorString());
                    }

                    return std::shared_ptr<const aiScene>(importer, result);
                }
            }
        }
    }

}







