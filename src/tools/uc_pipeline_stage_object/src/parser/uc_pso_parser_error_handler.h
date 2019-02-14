#pragma once

#include <iostream>
#include <string>
#include <vector>

namespace uc
{
    namespace pso
    {
        namespace parser
        {
            ///////////////////////////////////////////////////////////////////////////////
            //  The error handler
            ///////////////////////////////////////////////////////////////////////////////
            template <typename iterator, typename context>
            struct error_handler
            {
                template <typename, typename, typename>
                struct result { typedef void type; };

                error_handler(iterator first, iterator last, const context& c)
                    : m_first(first), m_last(last), m_context(c) {}

                void operator()(iterator err_pos) const
                {
                    int32_t line;

                    iterator line_start = get_pos(err_pos, line);

                    if (err_pos != m_last)
                    {
                        auto column = static_cast<int32_t>(err_pos - line_start);
                        m_context(line, column);
                    }
                    else
                    {
                        m_context(0, 0);
                    }
                }

                iterator get_pos(iterator err_pos, int32_t& line) const
                {
                    line = 1;
                    auto i = m_first;
                    auto line_start = m_first;
                    while (i != err_pos)
                    {
                        bool eol = false;
                        if (i != err_pos && *i == '\r') // CR
                        {
                            eol = true;
                            line_start = ++i;
                        }
                        if (i != err_pos && *i == '\n') // LF
                        {
                            eol = true;
                            line_start = ++i;
                        }
                        if (eol)
                            ++line;
                        else
                            ++i;
                    }
                    return line_start;
                }

                std::string get_line(iterator err_pos) const
                {
                    auto i = err_pos;
                    // position i to the next EOL
                    while (i != last && (*i != '\r' && *i != '\n'))
                        ++i;
                    return std::string(err_pos, i);
                }

                iterator m_first;
                iterator m_last;
                context  m_context;

                std::vector<uint32_t> m_iters;
            };

            template <typename iterator, typename context>
            inline error_handler<iterator, context > make_error_handler(iterator begin, iterator end, const context& c)
            {
                return error_handler<iterator, context >(begin, end, c);
            }

            struct file_parser_handler
            {
                file_parser_handler(const std::string& file_name) : m_file_name(file_name)
                {

                }

                void operator() (std::uint32_t line, std::uint32_t column) const
                {
                    std::cerr << m_file_name << '(' << line << ',' << column << ')' << ":error 12345:error description" << "\r\n";
                }

                std::string m_file_name;
            };
        }
    }
}

