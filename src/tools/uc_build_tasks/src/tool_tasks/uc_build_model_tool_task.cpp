// This is the main DLL file.

#include "uc_build_model_tool_task.h"

namespace UniqueCreator
{
    namespace Build
    {
        namespace Tasks
        {
            Model::Model()
            {
                m_switchOrderList->Add("Source");
                m_switchOrderList->Add("OutputModel");
                m_switchOrderList->Add("OptionsSwapYZ");
                m_switchOrderList->Add("ModelType");

                m_boolean_options = gcnew System::Collections::ArrayList();
                
                m_boolean_options->Add(gcnew BooleanOption("OptionCalcTangentSpace", L"--calc_tangent_space", L"Calculates the tangents and bitangents for the imported meshes"));
                m_boolean_options->Add(gcnew BooleanOption("OptionJoinIdenticalVertices", L"--join_identical_vertices", L"Identifies and joins identical vertex data sets within all imported meshes."));
                m_boolean_options->Add(gcnew BooleanOption("OptionMakeLeftHanded", L"--make_left_handed", L"Converts all the imported data to a left-handed coordinate space."));
                m_boolean_options->Add(gcnew BooleanOption("OptionTriangulate", L"--triangulate", L"Triangulates all faces of all meshes."));

                m_boolean_options->Add(gcnew BooleanOption("OptionRemoveComponent", L"--remove_component", L"Removes some parts of the data structure (animations, materials,light sources, cameras, textures, vertex components)."));
                m_boolean_options->Add(gcnew BooleanOption("OptionGenNormals", L"--gen_normals", L"Generates normals for all faces of all meshes."));
                m_boolean_options->Add(gcnew BooleanOption("OptionGenSmoothNormals", L"--gen_smooth_normals", L"Generates smooth normals for all vertices in the mesh."));
                m_boolean_options->Add(gcnew BooleanOption("OptionSplitLargeMeshes", L"--split_large_meshes", L"Splits large meshes into smaller sub-meshes."));

                m_boolean_options->Add(gcnew BooleanOption("OptionPreTransformVertices", L"--pre_transform_vertices", L"Removes the node graph and pre-transforms all vertices with the local transformation matrices of their nodes."));
                m_boolean_options->Add(gcnew BooleanOption("OptionLimitBoneWeights", L"--limit_bone_weights", L"Limits the number of bones simultaneously affecting a single vertex to a maximum value ( 4 is the default in our case)"));
                m_boolean_options->Add(gcnew BooleanOption("OptionValidateDataStructure", L"--validate_data_structure", L"Validates the imported scene data structure. This makes sure that all indices are valid, all animations and bones are linked correctly, all material references are correct .. etc."));
                m_boolean_options->Add(gcnew BooleanOption("OptionImproveCacheLocality", L"--improve_cache_locality", L"Reorders triangles for better vertex cache locality."));

                m_boolean_options->Add(gcnew BooleanOption("OptionRemoveRedundantMaterials", L"--remove_redundant_materials", L"Searches for redundant/unreferenced materials and removes them."));
                m_boolean_options->Add(gcnew BooleanOption("OptionFixInfacingNormals", L"--fix_infacing_normals", L"This step tries to determine which meshes have normal vectors that are facing inwards and inverts them."));
                m_boolean_options->Add(gcnew BooleanOption("OptionSortByPType", L"--sort_by_ptype", L"This step splits meshes with more than one primitive type in homogeneous sub-meshes."));
                m_boolean_options->Add(gcnew BooleanOption("OptionFindDegenerates", L"--find_degenerates", L"This step searches all meshes for degenerate primitives and converts them to proper lines or points."));

                m_boolean_options->Add(gcnew BooleanOption("OptionFindInvalidData", L"--find_invalid_data", L"This step searches all meshes for invalid data, such as zeroed normal vectors or invalid UV coords and removes/fixes them. This is intended to get rid of some common exporter errors."));
                m_boolean_options->Add(gcnew BooleanOption("OptionGenUVCoords", L"--gen_uv_coords", L"This step converts non-UV mappings (such as spherical or cylindrical mapping) to proper texture coordinate channels."));
                m_boolean_options->Add(gcnew BooleanOption("OptionTransformUVCoords", L"--transform_uv_coords", L"This step applies per-texture UV transformations and bakes them into stand-alone vtexture coordinate channels."));
                m_boolean_options->Add(gcnew BooleanOption("OptionFindInstances", L"--find_instances", L"This step searches for duplicate meshes and replaces them with references to the first mesh."));

                m_boolean_options->Add(gcnew BooleanOption("OptionOptimizeMeshes", L"--optimize_meshes", L"A postprocessing step to reduce the number of meshes."));
                m_boolean_options->Add(gcnew BooleanOption("OptionOptimizeGraph", L"--optimize_graph", L"A postprocessing step to optimize the scene hierarchy."));
                m_boolean_options->Add(gcnew BooleanOption("OptionFlipUVs", L"--flip_uvs", L"This step flips all UV coordinates along the y-axis and adjusts material settings and bitangents accordingly."));
                m_boolean_options->Add(gcnew BooleanOption("OptionFlipWindingOrder", L"--flip_winding_order", L"This step adjusts the output face winding order to be CW. The default face winding order is counter clockwise (CCW)."));

                m_boolean_options->Add(gcnew BooleanOption("OptionSplitByBoneCount", L"--split_by_bone_count", L"This step splits meshes with many bones into sub-meshes so that each submesh has fewer or as many bones as a given limit."));
                m_boolean_options->Add(gcnew BooleanOption("OptionDebone", L"--debone", L"This step removes bones losslessly or according to some threshold."));


                for (auto i = 0; i < m_boolean_options->Count; ++i)
                {
                    auto o = (BooleanOption^)(m_boolean_options[i]);
                    m_switchOrderList->Add(o->m_name);
                }

                m_textures = gcnew System::Collections::Generic::List<ITaskItem^>();
            }

            System::String^ Model::GenerateCommandLineCommands()
            {
                CommandLineBuilder^ builder = gcnew CommandLineBuilder();

                if (Source != nullptr)
                {
                    builder->AppendTextUnquoted(gcnew System::String(" --input_model "));
                    builder->AppendFileNameIfNotNull(Source);
                }

                if ( m_textures->Count > 0 )
                {
                    for (auto i = 0; i < m_textures->Count; ++i)
                    {
                        builder->AppendTextUnquoted(gcnew System::String(" --textures "));
                        builder->AppendFileNameIfNotNull( m_textures[i] );
                    }

                    for (auto i = 0; i < m_textures->Count; ++i)
                    {
                        builder->AppendTextUnquoted(gcnew System::String(" --textures_formats "));

                        auto format = m_textures[i]->GetMetadata("format");
                        if ( format != nullptr )
                        {
                            builder->AppendTextUnquoted(format);
                        }
                        else
                        {
                            builder->AppendTextUnquoted("unknown");
                        }
                    }
                }

                auto r = base::GenerateResponseFileCommands() + " " + builder->ToString();
                return r;
            }
        }
    }
}

