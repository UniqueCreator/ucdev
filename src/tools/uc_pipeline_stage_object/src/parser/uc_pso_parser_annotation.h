#pragma once

#include <boost/variant/apply_visitor.hpp>
#include <boost/type_traits/is_base_of.hpp>
#include <boost/mpl/bool.hpp>

#include <uc_pso_ast.h>


namespace uc
{
    namespace pso
    {
        ///////////////////////////////////////////////////////////////////////////////
        //  The annotation handler links the AST to a map of iterator positions
        //  for the purpose of subsequent semantic error handling when the
        //  program is being compiled.
        ///////////////////////////////////////////////////////////////////////////////
        template <typename Iterator>
        struct annotation
        {
            template <typename, typename>
            struct result { typedef void type; };

            std::vector<uint32_t>& iters;
            Iterator m_first;

            annotation(std::vector<uint32_t>& iters, Iterator first)
                : iters(iters), m_first(first) {}

            struct set_id
            {
                typedef void result_type;

                int id;
                set_id(int id) : id(id) {}

                template <typename T>
                void operator()(T& x) const
                {
                    this->dispatch(x, boost::is_base_of<ast::tagged, T>());
                }

                // This will catch all nodes except those inheriting from ast::tagged
                template <typename T>
                void dispatch(T& x, boost::mpl::false_) const
                {
                    // (no-op) no need for tags
                }

                // This will catch all nodes inheriting from ast::tagged
                template <typename T>
                void dispatch(T& x, boost::mpl::true_) const
                {
                    x.id = id;
                }
            };

            template <typename t>
            void operator()(t& ast, Iterator pos) const
            {
                auto id = iters.size();
                iters.push_back( static_cast<uint32_t>(pos - m_first) );
                ast.id = id;
            }

            /*
            void operator()(ast::identifier& ast, Iterator pos) const
            {
                auto id = iters.size();
                iters.push_back(pos);
                ast.id = id;
            }
            */
            /*
            void operator()(ast::identifier& ast, Iterator pos) const
            {
                auto id = iters.size();
                iters.push_back(pos);
                boost::apply_visitor(set_id(id), ast);
            }
            */
        };
    }
}



