#pragma once

#include <uc_public/math/math.h>

namespace UniqueCreator
{
    namespace Render
    {
        //strong typedef for the perspective transform. value class
        class alignas(16) ViewTransform3D final
        {
        public:
            ViewTransform3D()
            {
                Simd::Store(&m_value, Simd::Identity());
            }

            ViewTransform3D(Simd::Matrix4x4::Param m)
            {
                Simd::Store(&m_value, m);
            }

            Simd::Matrix4x4 Value() const
            {
                return Simd::Load(&m_value);
            }

        private:
            Simd::Float4x4A m_value;
        };
    }
}


