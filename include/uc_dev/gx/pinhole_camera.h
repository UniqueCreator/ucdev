#ifndef __GX_PINHOLE_CAMERA_H__
#define __GX_PINHOLE_CAMERA_H__

#include <cstdint>

#include <uc_dev/math/graphics.h>

namespace uc
{
    namespace gx
    {
        struct pinhole_camera
        {

            public:

            //view parameters
            math::float4 position() const
            {
                return m_view_position_ws;
            }

            math::float4 forward() const
            {
                return m_forward_ws;
            }

            math::float4 up() const
            {
                return m_up_ws;
            }

            //perspective parameters
            float				fov() const
            {
                return m_fov;
            }

            float aspect_ratio() const
            {
                return m_aspect_ratio;
            }

            float get_near() const
            {
                return m_near;
            }

            float get_far() const
            {
                return m_far;
            }

            //view parameters
            void set_view_position(math::float4 position_ws)
            {
                m_view_position_ws = position_ws;
            }

            void set_forward(math::float4 view_direction_ws)
            {
                m_forward_ws = view_direction_ws;
            }

            void set_up(math::float4 up_ws)
            {
                m_up_ws = up_ws;
            }

            //perspective parameters
            void set_fov(float	fov)
            {
                m_fov = fov;
            }

            void set_aspect_ratio(float	aspect_ratio)
            {
                m_aspect_ratio = aspect_ratio;
            }

            void set_near(float	value)
            {
                m_near = value;
            }

            void set_far(float	value)
            {
                m_far = value;
            }

        public:

            //view parameters
            math::float4 m_view_position_ws = math::set(0.0f, 0.0f, 0.0f, 1.0f);
            math::float4 m_forward_ws       = math::set(0.0f, 0.0f, 1.0f, 0.0f);;
            math::float4 m_up_ws            = math::set(0.0f, 1.0f, 0.0f, 0.0f);

            //perspective parameters
            float   m_fov          = 3.1415f / 4.0f;
            float   m_aspect_ratio = 16.0f / 9.0f;
            float   m_near         = 0.1f;      //meters
            float   m_far          = 100.0f;	//meters;
        };

        struct orthographic_camera
        {

        public:

            //view parameters
            math::float4 position() const
            {
                return m_view_position_ws;
            }

            math::float4 forward() const
            {
                return m_forward_ws;
            }

            math::float4 up() const
            {
                return m_up_ws;
            }

            //perspective parameters
            float x_min() const
            {
                return m_x_min;
            }

            float x_max() const
            {
                return m_x_max;
            }

            float y_min() const
            {
                return m_y_min;
            }

            float y_max() const
            {
                return m_y_max;
            }

            float z_min() const
            {
                return m_near;
            }

            float z_max() const
            {
                return m_far;
            }

            float get_near() const
            {
                return m_near;
            }

            float get_far() const
            {
                return m_far;
            }

            //view parameters
            void set_view_position(math::float4 position_ws)
            {
                m_view_position_ws = position_ws;
            }

            void set_forward(math::float4 view_direction_ws)
            {
                m_forward_ws = view_direction_ws;
            }

            void set_up(math::float4 up_ws)
            {
                m_up_ws = up_ws;
            }

            //perspective parameters
            void set_x_min(float value)
            {
                m_x_min = value;
            }

            void set_x_max(float value)
            {
                m_x_max = value;
            }

            void set_y_min(float	value)
            {
                m_y_min = value;
            }

            void set_y_max(float	value)
            {
                m_y_max = value;
            }

            void set_z_min(float	value)
            {
                m_near = value;
            }

            void set_z_max(float	value)
            {
                m_far = value;
            }

        public:

            //view parameters
            math::float4 m_view_position_ws = math::set(0.0f, 0.0f, 0.0f, 1.0f);
            math::float4 m_forward_ws = math::set(0.0f, 0.0f, 1.0f, 0.0f);;
            math::float4 m_up_ws = math::set(0.0f, 1.0f, 0.0f, 0.0f);

            //perspective parameters
            float   m_x_min         = -1.0f;    //meters
            float   m_x_max         =  1.0f;    //meters

            float   m_y_min         = -1.0f;    //meters
            float   m_y_max         = 1.0f;     //meters
            
            float   m_near          = 1.0f;     //meters
            float   m_far           = 100.0f;   //meters
        };

        namespace pinhole_camera_helper
        {
            inline void set_view_position(pinhole_camera* c, float x, float y, float z)
            {
                c->set_view_position(math::set(x, y, z, 1.0f));
            }

            inline void set_forward(pinhole_camera* c, float x, float y, float z)
            {
                c->set_forward(math::normalize3(math::set(x, y, z, 0.0f)));
            }

            inline void set_up(pinhole_camera* c, float x, float y, float z)
            {
                c->set_up(math::normalize3(math::set(x, y, z, 0.0f)));
            }

            inline void set_look_at(pinhole_camera* c, math::float4 position, math::float4 look_at, math::float4 up)
            {
                math::float4 forward_ = math::normalize3(math::sub(look_at, position));
                math::float4 up_ = math::normalize3(up);

                c->set_view_position(position);
                c->set_forward(forward_);
                c->set_up(up_);
            }
        }

        inline math::float4x4 view_matrix(const pinhole_camera * camera)
        {
            using namespace math;
            return view(camera->position(), normalize3(camera->forward()), normalize3(camera->up()));
        }

        inline math::float4x4 view_matrix(const pinhole_camera & camera)
        {
            using namespace math;
            return view(camera.position(), normalize3(camera.forward()), normalize3(camera.up()));
        }

        inline math::float4x4 inverse_view_matrix(const pinhole_camera * camera)
        {
            using namespace math;
            return inverse_view(camera->position(), normalize3(camera->forward()), normalize3(camera->up()));
        }

        inline math::float4x4 inverse_view_matrix(const pinhole_camera & camera)
        {
            using namespace math;
            return inverse_view(camera.position(), normalize3(camera.forward()), normalize3(camera.up()));
        }

        inline math::float4x4 view_matrix(const orthographic_camera * camera)
        {
            using namespace math;
            return view(camera->position(), normalize3(camera->forward()), normalize3(camera->up()));
        }

        inline math::float4x4 view_matrix(const orthographic_camera & camera)
        {
            using namespace math;
            return view(camera.position(), normalize3(camera.forward()), normalize3(camera.up()));
        }

        inline math::float4x4 inverse_view_matrix(const orthographic_camera * camera)
        {
            using namespace math;
            return inverse_view(camera->position(), normalize3(camera->forward()), normalize3(camera->up()));
        }

        inline math::float4x4 inverse_view_matrix(const orthographic_camera & camera)
        {
            using namespace math;
            return inverse_view(camera.position(), normalize3(camera.forward()), normalize3(camera.up()));
        }

        inline math::float4x4 perspective_matrix(const pinhole_camera * camera)
        {
            return math::perspective_fov_lh(camera->fov(), camera->aspect_ratio(), camera->get_near(), camera->get_far());
        }

        inline math::float4x4 perspective_matrix(const pinhole_camera & camera)
        {
            return math::perspective_fov_lh(camera.fov(), camera.aspect_ratio(), camera.get_near(), camera.get_far());
        }

        inline math::float4x4 inverse_perspective_matrix(const pinhole_camera * camera)
        {
            return math::inverse_perspective_fov_lh(camera->fov(), camera->aspect_ratio(), camera->get_near(), camera->get_far());
        }

        inline math::float4x4 inverse_perspective_matrix(const pinhole_camera& camera)
        {
            return math::inverse_perspective_fov_lh(camera.fov(), camera.aspect_ratio(), camera.get_near(), camera.get_far());
        }

        inline math::float4x4 perspective_matrix(const orthographic_camera * camera)
        {
            return math::orthographic_offset_center_lh(camera->x_min(), camera->x_max(), camera->y_min(), camera->y_max(), camera->get_near(), camera->get_far());
        }

        inline math::float4x4 perspective_matrix(const orthographic_camera & camera)
        {
            return math::orthographic_offset_center_lh(camera.x_min(), camera.x_max(), camera.y_min(), camera.y_max(), camera.get_near(), camera.get_far());
        }

        inline math::float4x4 inverse_perspective_matrix(const orthographic_camera * camera)
        {
            assert(false); //not implemented
            return math::orthographic_offset_center_lh(camera->x_min(), camera->x_max(), camera->y_min(), camera->y_max(), camera->get_near(), camera->get_far());
        }

        inline math::float4x4 inverse_perspective_matrix(const orthographic_camera& camera)
        {
            assert(false); //not implemented
            return math::orthographic_offset_center_lh(camera.x_min(), camera.x_max(), camera.y_min(), camera.y_max(), camera.get_near(), camera.get_far());
        }

        inline math::float4x4 vp_matrix(const pinhole_camera * camera)
        {
            return math::mul(view_matrix(camera), perspective_matrix(camera));
        }

        inline math::float4x4 vp_matrix(const pinhole_camera & camera)
        {
            return math::mul(view_matrix(camera), perspective_matrix(camera));
        }

        inline math::float4x4 inverse_vp_matrix(const pinhole_camera * camera)
        {
            return math::mul(inverse_perspective_matrix(camera), inverse_view_matrix(camera));
        }

        inline math::float4x4 inverse_vp_matrix(const pinhole_camera & camera)
        {
            return math::mul(inverse_perspective_matrix(camera), inverse_view_matrix(camera));
        }

        inline math::float4x4 vp_matrix(const orthographic_camera * camera)
        {
            return math::mul(view_matrix(camera), perspective_matrix(camera));
        }

        inline math::float4x4 vp_matrix(const orthographic_camera & camera)
        {
            return math::mul(view_matrix(camera), perspective_matrix(camera));
        }

        inline math::float4x4 inverse_vp_matrix(const orthographic_camera * camera)
        {
            return math::mul(inverse_perspective_matrix(camera), inverse_view_matrix(camera));
        }

        inline math::float4x4 inverse_vp_matrix(const orthographic_camera & camera)
        {
            return math::mul(inverse_perspective_matrix(camera), inverse_view_matrix(camera));
        }

        inline void rotate(pinhole_camera* camera, math::float4 quaternion)
        {
            auto up = camera->up();
            auto direction = camera->forward();

            camera->set_up(math::rotate_vector3(up, quaternion));
            camera->set_forward(math::rotate_vector3(direction, quaternion));
        }

        inline void rotate(pinhole_camera & camera, math::float4 quaternion)
        {
            auto up = camera.up();
            auto direction = camera.forward();

            camera.set_up(math::rotate_vector3(up, quaternion));
            camera.set_forward(math::rotate_vector3(direction, quaternion));
        }

        inline void rotate(orthographic_camera* camera, math::float4 quaternion)
        {
            auto up = camera->up();
            auto direction = camera->forward();

            camera->set_up(math::rotate_vector3(up, quaternion));
            camera->set_forward(math::rotate_vector3(direction, quaternion));
        }

        inline void rotate(orthographic_camera & camera, math::float4 quaternion)
        {
            auto up = camera.up();
            auto direction = camera.forward();

            camera.set_up(math::rotate_vector3(up, quaternion));
            camera.set_forward(math::rotate_vector3(direction, quaternion));
        }
    }
}

#endif