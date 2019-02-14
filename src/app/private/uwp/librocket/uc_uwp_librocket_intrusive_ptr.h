#pragma once

#include <uc_dev/mem/intrusive_ptr.h>

#include <Rocket/Core.h>

namespace Rocket
{
    namespace Core
    {
        inline void intrusive_ptr_add_ref( ReferenceCountable  * p )
        {
            p->AddReference();
        }

        inline void intrusive_ptr_release( ReferenceCountable * p )
        {
            p->RemoveReference();
        }
    }
}
