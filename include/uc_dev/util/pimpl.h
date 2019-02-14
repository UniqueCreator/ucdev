#pragma once

#include <uc_dev/util/propagate_const.h>
#include <memory>

namespace uc
{
    namespace util
    {
        namespace details
        {
            template <typename t>
            class pimpl
            {
                public:
                pimpl();
                template <typename ...args> pimpl(args&&...);

                ~pimpl();
                const t* operator->() const;
                t* operator->();
                t& operator*();
                const t& operator*() const;

                const t* get() const;
                t* get();

                pimpl(const pimpl&) = delete;
                pimpl& operator=(const pimpl&) = delete;

                pimpl(pimpl&&);
                pimpl& operator=(pimpl&&);

                private:
                std::experimental::propagate_const< std::unique_ptr<t> > m;
            };
        }
    }
}
