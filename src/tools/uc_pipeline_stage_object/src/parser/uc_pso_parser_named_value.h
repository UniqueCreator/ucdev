#pragma once

#include <parser/uc_pso_parser_includes.h>
#include "pipeline_state_types.h"

namespace uc
{
    namespace pso
    {
        namespace parser
        {
            template <typename iterator>
            struct named_value_string : public qi::grammar< iterator, std::string(), ascii::space_type >
            {
                using base = qi::grammar< iterator, std::string(), ascii::space_type  >;

                named_value_string(const std::string& named_value) : base(m_value, named_value)
                {
                    using qi::lexeme;
                    using qi::char_;
                    using qi::lit;

                    namespace px = ::boost::phoenix;

                    m_quoted_string = lexeme['"' > +(char_ - '"') > '"'];
                    m_value = lit(named_value) > '=' > m_quoted_string;
                    m_quoted_string.name("quoted_string");
                    m_value.name("value");
                }

                qi::rule<iterator, std::string(), ascii::space_type  > m_quoted_string;
                qi::rule<iterator, std::string(), ascii::space_type  > m_value;
            };

            template <typename iterator>
            struct named_value_int32 : public qi::grammar< iterator, int32_t, ascii::space_type  >
            {
                using base = qi::grammar< iterator, int32_t, ascii::space_type  >;

                named_value_int32(const std::string& named_value) : base(m_value, named_value)
                {
                    using qi::int_;
                    using qi::lit;

                    m_value = lit(named_value) > '=' > int_;
                    m_value.name(named_value);
                }

                qi::rule<iterator, int32_t, ascii::space_type  > m_value;
            };

            template <typename iterator>
            struct named_value_uint32 : public qi::grammar< iterator, uint32_t, ascii::space_type  >
            {
                using base = qi::grammar< iterator, uint32_t, ascii::space_type  >;

                named_value_uint32(const std::string& named_value) : base(m_value, named_value)
                {
                    using qi::uint_;
                    using qi::lit;

                    m_value = lit(named_value) > '=' > uint_;
                    m_value.name(named_value);
                }

                qi::rule<iterator, uint32_t, ascii::space_type  > m_value;
            };

            template <typename iterator>
            struct named_value_bool : public qi::grammar< iterator, bool, ascii::space_type  >
            {
                using base = qi::grammar< iterator, bool, ascii::space_type  >;

                named_value_bool(const std::string& named_value) : base(m_value, named_value)
                {
                    using qi::bool_;
                    using qi::lit;

                    m_value = lit(named_value) > '=' > bool_;
                    m_value.name(named_value);
                }

                qi::rule<iterator, bool, ascii::space_type  > m_value;
            };

            template <typename iterator>
            struct named_value_float : public qi::grammar< iterator, float, ascii::space_type  >
            {
                using base = qi::grammar< iterator, float, ascii::space_type  >;

                named_value_float(const std::string& named_value) : base(m_value, named_value)
                {
                    using qi::float_;
                    using qi::lit;

                    m_value = lit(named_value) > '=' > float_;
                    m_value.name(named_value);
                }

                qi::rule<iterator, float, ascii::space_type  > m_value;
            };
        }
    }
}
