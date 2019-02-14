// UniqueCreator.Build.Tasks.h

#pragma once

#include <vector>
#include <string>
#include <boost/algorithm/string.hpp>

namespace uc
{
    namespace build
    {
        namespace tasks
        {
            inline std::vector<std::string> split(const std::string& s)
            {
                std::vector<std::string> strs;
                if (!s.empty() )
                {
                    boost::split(strs, s, boost::is_any_of(";"));
                }
                
                
                return strs;
            }

            inline std::vector<std::string> split(const std::string& s, char ch)
            {
                char ch2[2] = { ch, 0x0 };
                std::vector<std::string> strs;
                boost::split(strs, s, boost::is_any_of(std::string(&ch2[0])));
                return strs;
            }
        }
    }
}

