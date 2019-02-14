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
                inline auto merge_blend_weights(mesh_view* v)
                {
                    using blend_weight = typename mesh_view::blend_weight_t;
                    std::vector<blend_weight> r;

                    r.resize(v->vertex_count());

                    auto s = v->material_count();
                    auto vc = size_t(0);

                    for (auto i = 0U; i < s; ++i)
                    {
                        auto&& p = v->blend_weight(i);
                        auto source = &p[0];
                        auto destination = &r[vc];
                        std::copy(source, source + p.size(), destination);
                        vc += p.size();
                    }

                    return r;
                }

                template <typename mesh_view>
                inline auto merge_blend_indices(mesh_view* v)
                {
                    using blend_index = typename mesh_view::blend_index_t;
                    std::vector<blend_index> r;

                    r.resize(v->vertex_count());

                    auto s = v->material_count();
                    auto vc = size_t(0);
                    //auto bc = size_t(0);

                    for (auto i = 0U; i < s; ++i)
                    {
                        auto&& p = v->blend_index(i);

                        auto source = &p[0];
                        auto destination = &r[vc];

                        std::copy(source, source + p.size(), destination);
                        vc += p.size();
                    }

                    return r;
                }
            }
        }
    }
}







