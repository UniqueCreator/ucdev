#pragma once

#include <unordered_map>
#include <memory>

#include <unordered_set>

namespace uc
{
    namespace build
    {
        namespace tasks
        {
            std::unordered_set< std::string > get_includes(const std::string& file_name, const std::vector<std::string>& defines = std::vector<std::string>(), const std::vector<std::string>& include_paths = std::vector<std::string>());
        }
    }
}