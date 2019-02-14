#pragma once

#include <string>

#include <boost/unordered_set.hpp>
#include <boost/unordered_map.hpp>

namespace uc
{
    namespace build
    {
        namespace tasks
        {
            enum dependency_type
            {
                unknown = 0x0,
                file    = 0x01
            };

            //cache for the hlsl files
            //keys are name value pairs, that must match
            //examples are file names and contents
            struct dependency_cache_item
            {
                std::string     m_key;
                std::string     m_contents;
                dependency_type m_type;

                dependency_cache_item( std::string&& key, std::string&& contents ) : m_key( std::move(key) ), m_contents(std::move( contents ) ), m_type(dependency_type::unknown)
                {

                }

                dependency_cache_item( std::string&& key, std::string&& contents, dependency_type type ) : m_key(std::move(key)), m_contents(std::move(contents)), m_type(type)
                {

                }

                dependency_cache_item( const std::string& key, std::string& contents, dependency_type type) : m_key(std::move(key)), m_contents(std::move(contents)), m_type(type)
                {

                }

                dependency_cache_item(const std::string& key, std::string& contents) : m_key(std::move(key)), m_contents(std::move(contents)), m_type(dependency_type::unknown)
                {

                }

                dependency_cache_item() : m_type(dependency_type::unknown)
                {

                }
            };

            inline bool operator == (const dependency_cache_item& a, const dependency_cache_item& b)
            {
                return (a.m_key == b.m_key && a.m_contents == b.m_contents && a.m_type == b.m_type);
            }

            inline bool operator != (const dependency_cache_item& a, const dependency_cache_item& b)
            {
                return !(a == b);
            }
        }
    }
}

template<> struct boost::hash< uc::build::tasks::dependency_cache_item >
{
    typedef uc::build::tasks::dependency_cache_item argument_type;
    typedef std::size_t result_type;
    inline result_type operator()(argument_type const& s) const
    {
        result_type const h1(std::hash<std::string>()(s.m_key));
        result_type const h2(std::hash<std::string>()(s.m_contents));   //todo: add m_type
        return h1 ^ (h2 << 1); // or use boost::hash_combine
    }
};


namespace uc
{
    namespace build
    {
        namespace tasks
        {
            struct dependencies
            {
                using dependency_container = boost::unordered_set < dependency_cache_item >;
                dependency_container         m_files;

                dependency_container::iterator begin()
                {
                    return m_files.begin();
                }

                dependency_container::iterator end()
                {
                    return m_files.begin();
                }

                dependency_container::const_iterator begin() const
                {
                    return m_files.begin();
                }

                dependency_container::const_iterator end() const
                {
                    return m_files.begin();
                }

                dependency_container::const_iterator cbegin() const
                {
                    return m_files.begin();
                }

                dependency_container::const_iterator cend() const
                {
                    return m_files.begin();
                }
            };

            bool operator==(const dependencies& a, const dependencies& b);

            inline bool operator!=(const dependencies& a, const dependencies& b)
            {
                return !(a == b);
            }

            struct file_cache
            {
                using cache_container      = boost::unordered_map< std::string, dependencies >;
                cache_container            m_cache_data;
            };
        }
    }
}
