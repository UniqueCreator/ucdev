#pragma once

#include <string>

#include <boost/unordered_map.hpp>
#include <boost/serialization/boost_unordered_map.hpp>
#include <boost/serialization/boost_unordered_set.hpp>

#include "uc_build_tasks_dependencies.h"

// custom specialization of std::hash can be injected in namespace std or boost
namespace boost
{
    namespace serialization
    {
        template<class Archive>
        void serialize(Archive & ar, uc::build::tasks::file_cache & g, const unsigned int version)
        {
            ar & g.m_cache_data;
        }

        template<class Archive>
        void serialize(Archive & ar, uc::build::tasks::dependency_cache_item & g, const unsigned int version)
        {
            ar & g.m_key;
            ar & g.m_contents;
            ar & g.m_type;
        }

        template<class Archive>
        void serialize(Archive & ar, uc::build::tasks::dependencies & g, const unsigned int version)
        {
            ar & g.m_files;
        }
    }

}

