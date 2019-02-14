#include "pch.h"

#include "uc_build_md5.h"

#include "md5.h"

namespace uc
{
    namespace build
    {
        namespace tasks
        {
            std::string md5(const std::string& s)
            {
                MD5 m(s);
                return m.toStr();
            }

            std::string md5(const char* s)
            {

                MD5 m(s);
                return m.toStr();
            }
        }
    }
}




