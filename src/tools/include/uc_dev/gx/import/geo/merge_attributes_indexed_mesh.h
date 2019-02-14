#pragma once


namespace uc
{
    namespace gx
    {
        namespace import
        {
            namespace geo
            {
                template <typename mesh_view>
                inline auto merge_positions(mesh_view* v)
                {
                    using local_position = typename mesh_view::position_t;
                    std::vector<local_position> r;

                    r.resize(v->vertex_count());

                    auto s = v->material_count();
                    auto vc = size_t(0);

                    for (auto i = 0U; i < s; ++i)
                    {
                        auto&& p = v->position(i);
                        auto source = &p[0];
                        auto destination = &r[vc];
                        std::copy(source, source + p.size(), destination);
                        vc += p.size();
                    }

                    return r;
                }

                template <typename mesh_view>
                inline auto merge_uvs(mesh_view* v)
                {
                    using uv = typename mesh_view::uv_t;
                    std::vector<uv > r;

                    r.resize(v->vertex_count());

                    auto s = v->material_count();
                    auto vc = size_t(0);

                    for (auto i = 0U; i < s; ++i)
                    {
                        auto&& p = v->uv(i);

                        auto source = &p[0];
                        auto destination = &r[vc];

                        std::copy(source, source + p.size(), destination);
                        vc += p.size();
                    }

                    return r;
                }

                template <typename mesh_view>
                inline auto merge_normals(mesh_view* v)
                {
                    using normal = typename mesh_view::normal_t;
                    std::vector< normal > r;

                    r.resize(v->vertex_count());

                    auto s = v->material_count();
                    auto vc = size_t(0);

                    for (auto i = 0U; i < s; ++i)
                    {
                        auto&& p = v->normal(i);

                        auto source = &p[0];
                        auto destination = &r[vc];

                        std::copy(source, source + p.size(), destination);
                        vc += p.size();
                    }

                    return r;
                }

                template <typename mesh_view>
                inline auto merge_tangents(mesh_view* v)
                {
                    using tangent = typename mesh_view::tangent_t;
                    std::vector< tangent > r;

                    r.resize(v->vertex_count());

                    auto s = v->material_count();
                    auto vc = size_t(0);

                    for (auto i = 0U; i < s; ++i)
                    {
                        auto&& p = v->tangent(i);

                        auto source = &p[0];
                        auto destination = &r[vc];

                        std::copy(source, source + p.size(), destination);
                        vc += p.size();
                    }

                    return r;
                }

                template <typename mesh_view>
                inline auto merge_faces(mesh_view* v)
                {
                    using face_t = typename mesh_view::face_t;
                    std::vector< face_t > r;

                    r.resize(v->face_count());

                    auto s = v->material_count();
                    auto fc = size_t(0);
                    auto vc = uint32_t(0U);

                    for (auto i = 0U; i < s; ++i)
                    {
                        auto&& p = v->face(i);
                        auto&& pos = v->position(i);

                        auto source = &p[0];
                        auto destination = &r[fc];
                        //auto size           = p.size() * sizeof(face_t);

                        std::transform(source, source + p.size(), destination, [vc](const auto& face)
                        {
                            face_t f;

                            f.v0 = face.v0 + vc;
                            f.v1 = face.v1 + vc;
                            f.v2 = face.v2 + vc;

                            return f;
                        });

                        fc += p.size();
                        vc += static_cast<uint32_t>(pos.size());
                    }

                    return r;
                }
            }
        }
    }
}







