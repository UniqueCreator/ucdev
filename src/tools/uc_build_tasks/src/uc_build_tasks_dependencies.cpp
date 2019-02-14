#include "pch.h"
#include "uc_build_tasks_dependencies.h"

namespace uc
{
    namespace build
    {
        namespace tasks
        {
            bool operator==(const dependencies& a, const dependencies& b)
            {
                if (a.m_files.size() == b.m_files.size())
                {
                    //check a < b and b < a

                    for (auto&& i : a.m_files)
                    {
                        if (b.m_files.find(i) == std::end(b.m_files))
                        {
                            return false;
                        }
                    }

                    for (auto&& i : b.m_files)
                    {
                        if (a.m_files.find(i) == std::end(a.m_files))
                        {
                            return false;
                        }
                    }

                    for (auto&& i : b.m_files)
                    {
                        if ( *a.m_files.find(i) != i )
                        {
                            return false;
                        }
                    }

                    return true;
                }
                else
                {
                    return false;
                }
            }
        }
    }
}