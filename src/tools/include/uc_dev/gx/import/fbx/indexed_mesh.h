#pragma once

#include <uc_dev/gx/import/geo/indexed_mesh.h>
#include <uc_dev/gx/import/fbx/fbx_common.h>
#include <uc_dev/gx/import/fbx/fbx_common_transform.h>
#include <uc_dev/gx/import/fbx/fbx_common_transform_dcc.h>



namespace uc
{
    namespace gx
    {
        namespace import
        {
            namespace fbx
            {
                //////////////////////
                inline  geo::indexed_mesh::positions_t  transform_dcc_positions(const geo::indexed_mesh::positions_t& positions, const fbx_context* ctx)
                {
                    ctx;
                    return positions;
                }

                inline  geo::indexed_mesh::normals_t  transform_dcc_normals(const geo::indexed_mesh::normals_t& positions, const fbx_context* ctx)
                {
                    ctx;
                    return positions;
                }

                inline std::shared_ptr<geo::indexed_mesh> create_mesh(const std::string& file_name)
                {
                    auto context = load_fbx_file(file_name);
                    auto scene = context->m_scene.get();

                    fbxsdk::FbxMesh* mesh;
                    fbxsdk::FbxNode* mesh_node;

                    std::tie( mesh, mesh_node ) = get_mesh(scene->GetRootNode());;

                    if (mesh == nullptr || !mesh->IsTriangleMesh())
                    {
                        ::uc::gx::throw_exception<uc::gx::fbx_exception>("file does not contain a triangle mesh");
                    }

                    assert(mesh_node);
          
                    mesh->RemoveBadPolygons();
                    mesh->ComputeBBox();


                    if (!has_normals(mesh))
                    {
                        mesh->GenerateNormals();
                    }

                    if (!has_tangents(mesh))
                    {
                        mesh->GenerateTangentsDataForAllUVSets(true);
                    }

                    assert(mesh->GetPolygonSize(0));

                    auto p = triangle_permutaion(context.get());

                    return std::make_shared<geo::indexed_mesh>(transform_dcc_positions(get_positions(mesh), context.get()), get_uvs(mesh), get_faces(mesh, p));
                }
            }
         }
     }
}
