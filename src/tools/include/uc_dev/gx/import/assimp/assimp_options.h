#pragma once

#include <string>

#include <assimp/postprocess.h>

namespace uc
{
    namespace gx
    {
        namespace import
        {
            namespace assimp
            {

                inline std::string assimp_postprocess_option_to_string(uint32_t option)
                {
                    std::vector<std::string> r;

                    if (option & aiProcess_CalcTangentSpace)
                    {
                        r.push_back("calc_tangent_space");
                    }

                    if (option & aiProcess_JoinIdenticalVertices)
                    {
                        r.push_back("join_identical_vertices");
                    }
                    if (option & aiProcess_MakeLeftHanded)
                    {
                        r.push_back("make_left_handed");
                    }
                    if (option & aiProcess_Triangulate)
                    {
                        r.push_back("triangulate");
                    }
                    if (option & aiProcess_RemoveComponent)
                    {
                        r.push_back("remove_component");
                    }
                    if (option & aiProcess_GenNormals)
                    {
                        r.push_back("gen_normals");
                    }
                    if (option & aiProcess_GenSmoothNormals)
                    {
                        r.push_back("gen_smooth_normals");
                    }
                    if (option & aiProcess_SplitLargeMeshes)
                    {
                        r.push_back("split_large_meshes");
                    }
                    if (option & aiProcess_PreTransformVertices)
                    {
                        r.push_back("pre_transform_vertices");
                    }
                    if (option & aiProcess_LimitBoneWeights)
                    {
                        r.push_back("limit_bone_weights");
                    }
                    if (option & aiProcess_ValidateDataStructure)
                    {
                        r.push_back("validate_data_structure");
                    }
                    if (option & aiProcess_ImproveCacheLocality)
                    {
                        r.push_back("improve_cache_locality");
                    }
                    if (option & aiProcess_RemoveRedundantMaterials)
                    {
                        r.push_back("remove_redundant_materials");
                    }
                    if (option & aiProcess_FixInfacingNormals)
                    {
                        r.push_back("fix_infacing_normals");
                    }
                    if (option & aiProcess_SortByPType)
                    {
                        r.push_back("sort_by_ptype");
                    }
                    if (option & aiProcess_FindDegenerates)
                    {
                        r.push_back("find_degenerates");
                    }
                    if (option & aiProcess_FindInvalidData)
                    {
                        r.push_back("find_invalid_data");
                    }
                    if (option & aiProcess_GenUVCoords)
                    {
                        r.push_back("gen_uv_coords");
                    }

                    if (option & aiProcess_TransformUVCoords)
                    {
                        r.push_back("transofm_uv_coords");
                    }

                    if (option & aiProcess_FindInstances)
                    {
                        r.push_back("find_instances");
                    }
                    if (option & aiProcess_OptimizeMeshes)
                    {
                        r.push_back("optimize_meshes");
                    }
                    if (option & aiProcess_OptimizeGraph)
                    {
                        r.push_back("optimize_graphc");
                    }
                    if (option & aiProcess_FlipUVs)
                    {
                        r.push_back("flip_uvs");
                    }
                    if (option & aiProcess_FlipWindingOrder)
                    {
                        r.push_back("flip_winding_order");
                    }
                    if (option & aiProcess_SplitByBoneCount)
                    {
                        r.push_back("split_by_bone_count");
                    }

                    if (option & aiProcess_Debone)
                    {
                        r.push_back("debone");
                    }

                    std::string result;

                    for (auto&& i : r)
                    {
                        result += i + ",";
                    }

                    if (result.length() > 1)
                    {
                        return result.substr(0, result.length() - 1);
                    }
                    else
                    {
                        return result;
                    }
                }

            }
        }
    }
}
