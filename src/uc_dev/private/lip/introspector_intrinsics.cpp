#include "pch.h"
#include <uc_dev/lip/introspector_manager.h>

namespace uc
{
    namespace lip
    {
        template <typename t> struct registrar
        {
            registrar()
            {
                static introspector<t> g_introspector;
                introspector_database()->register_introspector(make_type_info<t>().type_id(), &g_introspector);
            }
        };

        //register types into the introspector
        registrar<char>                 register_char;
        registrar<unsigned char>        register_unsigned_char;
        registrar<short>                register_short;
        registrar<unsigned short>       register_unsigned_short;

        registrar<int>                  register_int_short;
        registrar<unsigned int>         register_unsigned_int;
        registrar<unsigned long>        register_unsigned_long;
        registrar<long long>            register_long_long;

        registrar<unsigned long long>   register_unsigned_long_long;
        registrar<float>                register_float;
        registrar<double>               register_double;
        registrar<bool>                 register_bool;

        registrar<signed char>          register_signed_char;
    }
}