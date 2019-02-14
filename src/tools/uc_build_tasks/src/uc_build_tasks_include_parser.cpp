#include "pch.h"

#include "uc_build_tasks_include_parser.h"

#include <unordered_set>

#include <boost/wave.hpp>
#include <boost/wave/cpplexer/cpp_lex_iterator.hpp>

namespace uc
{
    namespace build
    {
        namespace tasks
        {
            template <typename TokenT>
            class custom_hook : public boost::wave::context_policies::eat_whitespace<TokenT>
            {
            public:
                custom_hook()
                {

                }

                template <typename ContextT>
                void opened_include_file(ContextT const& ctx, std::string const& relname, std::string const& absname, bool is_system_include)
                {
                    m_files.insert(absname);
                }

                auto files_begin() const
                {
                    return m_files.begin();
                }

                auto files_end() const
                {
                    return m_files.end();
                }

                auto files_begin()
                {
                    return m_files.begin();
                }

                auto files_end()
                {
                    return m_files.end();
                }

            private:

                std::unordered_set<std::string> m_files;
            };

            std::unordered_set< std::string > get_includes(const std::string& file_name, const std::vector<std::string>& defines, const std::vector<std::string>& include_paths)
            {
                using namespace std;
                using namespace boost;
                using namespace boost::wave;
                using namespace boost::wave::cpplexer;

                using lex_iterator_type = lex_iterator< lex_token<> >;
                using hook = custom_hook<typename lex_iterator_type::token_type>;
                using context_type = context<  string::iterator, lex_iterator_type, iteration_context_policies::load_file_to_string, hook >;

                ifstream instream(file_name);

                if (!instream.good())
                {
                    std::string message = "cannot open " + file_name;
                    throw std::exception(message.c_str());
                }

                string input(istreambuf_iterator<char>(instream.rdbuf()), istreambuf_iterator<char>());
                context_type ctx(input.begin(), input.end(), file_name.c_str());

                for (auto&& i : defines)
                {
                    ctx.add_macro_definition(i);
                }

                for (auto&& i : include_paths)
                {
                    ctx.add_include_path(i.c_str());
                    ctx.add_sysinclude_path(i.c_str());
                }

                {
                    auto&& first = ctx.begin();
                    auto&& last = ctx.end();

                    // The input stream is preprocessed for you during iteration
                    // over [first, last)
                    while (first != last)
                    {
                        ++first;
                    }
                }

                {
                    auto&& hooks = ctx.get_hooks();
                    auto&& first = hooks.files_begin();
                    auto&& last = hooks.files_end();

                    std::unordered_set<std::string> result;

                    // The input stream is preprocessed for you during iteration
                    // over [first, last)
                    while (first != last)
                    {
                        result.insert(*first);
                        ++first;
                    }
                    return result;

                }
            }
        }
    }
}