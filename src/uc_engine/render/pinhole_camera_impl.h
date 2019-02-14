#pragma once

#include <uc_public/render/pinhole_camera.h>

#include <uc_public/math/math.h>

namespace UniqueCreator
{
    namespace Render
    {
        class alignas(16) PinholeCamera::Impl
        {
        public:

            Impl()
            {
                using namespace Simd;

                //view parameters
                Store(&m_view_position_ws, Set(0.0f,0.0f,0.0f,1.0f));
                Store(&m_forward_ws, Set(0.0f, 0.0f, 1.0f, 0.0f));
                Store(&m_up_ws, Set(0.0f, 1.0f, 0.0f, 0.0f));

                //perspective parameters
                m_fov           = 3.1415f / 4.0f;
                m_aspect_ratio  = 16.0f / 9.0f;
                m_near = 0.1f;  //meters
                m_far = 100.0f; //meters;
            }

            //todo: cpp 17, when comes out

            static void* operator new(std::size_t count)
            {
                auto r = _aligned_malloc(count, 16);

                if (r == nullptr) { throw std::bad_alloc();}
                return r;
            }

            static void* operator new[](std::size_t count)
            {
                auto r = _aligned_malloc(count, 16);

                if (r == nullptr) { throw std::bad_alloc(); }
                return r;
            }

            static void* operator new  (std::size_t count, const std::nothrow_t& )
            {
                return _aligned_malloc(count, 16);
            }

            static void* operator new[](std::size_t count, const std::nothrow_t& )
            {
                return _aligned_malloc(count, 16);
            }

            static void operator delete  (void* ptr)
            {
                _aligned_free(ptr);
            }

            static void operator delete[](void* ptr)
            {
                _aligned_free(ptr);
            }

            Simd::Float4A       m_view_position_ws;
            Simd::Float4A       m_forward_ws;
            Simd::Float4A       m_up_ws;

            float               m_fov;
            float               m_aspect_ratio;
            float               m_near;
            float               m_far;
        };
    }
}


