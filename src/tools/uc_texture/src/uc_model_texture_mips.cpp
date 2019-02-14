#include "pch.h"

#include "uc_model_texture_mips.h"
namespace uc
{
    namespace model
    {
        gx::imaging::cpu_texture make_mip(const gx::imaging::cpu_texture& o )
        {
            gx::imaging::cpu_texture r(o.width() / 2, o.height() / 2, o.type());

            return r;
        }

    }
}

