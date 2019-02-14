#pragma once

#include <uc_dev/gx/import/fbx/fbx_common_multi_material_mesh.h>

namespace uc
{
    namespace gx
    {
        namespace import
        {
            namespace fbx
            {
                inline std::string material_name(const std::string& s)
                {
                    using namespace  std::experimental::filesystem;
                    auto wname = path(s).stem();
                    std::string data = util::utf8_from_utf16(wname);
                    std::transform(data.begin(), data.end(), data.begin(), [](auto x) -> char { return static_cast<char>(::tolower(x)); });
                    return data;
                }

                //returns translated materials up to material count
                inline std::vector<geo::multi_material_mesh::material> get_materials(const fbxsdk::FbxNode* mesh_node, uint32_t material_count)
                {
                    std::vector<geo::multi_material_mesh::material> r;

                    r.reserve(material_count);

                    assert(static_cast<int32_t>(material_count) <= mesh_node->GetMaterialCount());

                    for (auto i = 0U; i < material_count; ++i)
                    {
                        auto&& material = mesh_node->GetMaterial(i);

                        fbxsdk::FbxProperty diffuseTextures = material->FindProperty(fbxsdk::FbxSurfaceMaterial::sDiffuse);

                        if (diffuseTextures.IsValid())
                        {
                            auto&& texture = diffuseTextures.GetSrcObject<FbxFileTexture>(0);

                            geo::multi_material_mesh::material m;
                            m.m_type = geo::multi_material_mesh::diffuse;

                            //remove everything from the texture file name to simplify it
                            if (texture)
                            {
                                auto&& file_name = texture->GetFileName();
                                m.m_name = material_name(std::string(file_name));
                            }
                            else
                            {
                                m.m_name = "null";
                            }
                            r.push_back(m);
                        }
                    }

                    return r;
                }
            }
        }
    }
}

