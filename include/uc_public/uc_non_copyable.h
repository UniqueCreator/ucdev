#pragma once

#include <uc_public/uc_base.h>

namespace UniqueCreator
{
    //inherit this class, if you want the classes not to be copyable
    //suitable for class with virtual functions, which will be managed by pointer

    class UC_ENGINE_API NonCopyable
    {
        protected:

		NonCopyable() {}
        ~NonCopyable() {}

		NonCopyable(NonCopyable&&) = default;
		NonCopyable& operator=(NonCopyable&&) = default;

        private:  // emphasize the following members are private
		NonCopyable(const NonCopyable&) = delete;
        NonCopyable& operator=(const NonCopyable&) = delete;
    };
}
