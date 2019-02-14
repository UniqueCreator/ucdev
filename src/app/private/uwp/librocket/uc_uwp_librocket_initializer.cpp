#include "pch.h"

#include "uc_uwp_librocket_initializer.h"

namespace uc
{
    namespace uwp
    {
        namespace librocket
        {
            initializer::initializer( Rocket::Core::SystemInterface* core )
            {
                Rocket::Core::SetSystemInterface( core );
                Rocket::Core::Initialise();
            }

            initializer::~initializer()
            {
                Rocket::Core::Shutdown();
            }
        }
    }
}
