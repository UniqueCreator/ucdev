#pragma once

#include <uc_dev/gx/pinhole_camera.h>
#include <uc_dev/gxu/camera_command.h>

#include <uc_dev/math/math.h>


namespace uc
{
    namespace gxu
    {
        namespace details
        {
            inline void turn_pinhole_camera(gx::pinhole_camera* camera, float angle_in_radians)
            {
                auto    view_direction_ws = camera->forward();
                auto    up_direction_ws = camera->up();

                auto    quaternion = math::quaternion_axis_angle(up_direction_ws, angle_in_radians);
                auto    view_direction_ws_2 = math::rotate_vector3(view_direction_ws, quaternion);

                camera->set_forward(view_direction_ws_2);
            }

            inline void aim_pinhole_camera(gx::pinhole_camera* camera, float angle_in_radians)
            {
                auto    view_direction_ws_1 = camera->forward();
                auto    up_direction_ws_1 = camera->up();
                auto    cross = math::cross3(view_direction_ws_1, up_direction_ws_1);

                auto    quaternion = math::quaternion_axis_angle(cross, angle_in_radians);
                auto    view_direction_ws_2 = math::rotate_vector3(view_direction_ws_1, quaternion);
                auto    up_direction_ws_2 = math::rotate_vector3(up_direction_ws_1, quaternion);

                camera->set_forward(view_direction_ws_2);
                camera->set_up(up_direction_ws_2);
            }
        }

        //processes stream of camera commands and dispatches them
        //suitable for recording and playback
        class pinhole_camera_command_dispatcher : public camera_command_dispatcher
        {
        public:

            explicit pinhole_camera_command_dispatcher(gx::pinhole_camera*	pinhole_camera) :
                m_pinhole_camera(pinhole_camera)
            {

            }

        private:

            void on_move_forward(const move_camera_forward* command) override
            {
                auto	view_direction_ws = m_pinhole_camera->forward();
                auto	normalized_view_direction_ws = math::normalize3(view_direction_ws);

                auto	distance = fabsf(command->m_distance);
                auto	distance_ws = math::splat(distance);
                auto	displacement_ws = math::mul(distance_ws, normalized_view_direction_ws);

                m_pinhole_camera->m_view_position_ws = math::add(m_pinhole_camera->m_view_position_ws, displacement_ws);
            }

            void on_move_backward(const move_camera_backward* command) override
            {
                auto	view_direction_ws = m_pinhole_camera->forward();
                auto	normalized_view_direction_ws = math::normalize3(view_direction_ws);

                auto	distance = -1.0f * fabsf(command->m_distance);
                auto	distance_ws = math::splat(distance);
                auto	displacement_ws = math::mul(distance_ws, normalized_view_direction_ws);

                m_pinhole_camera->m_view_position_ws = math::add(m_pinhole_camera->m_view_position_ws, displacement_ws);
            }

            void on_turn_left(const turn_camera_left* command) override
            {
                details::turn_pinhole_camera(m_pinhole_camera, -1.0f * fabsf(command->m_angle_radians));
            }

            void on_turn_right(const turn_camera_right* command) override
            {
                details::turn_pinhole_camera(m_pinhole_camera, 1.0f * fabsf(command->m_angle_radians));
            }

            void on_aim_up(const aim_camera_up* command) override
            {
                details::aim_pinhole_camera(m_pinhole_camera, 1.0f * fabsf(command->m_angle_radians));
            }

            void on_aim_down(const aim_camera_down* command) override
            {
                details::aim_pinhole_camera(m_pinhole_camera, -1.0f * fabsf(command->m_angle_radians));
            }

            void on_move_xy(const move_camera_xy* command) override
            {
                auto	displacement_vs = math::set(command->m_direction_x, command->m_direction_y, 0.0f, 0.0f);
                auto    iv = math::transpose(math::inverse(gx::view_matrix(m_pinhole_camera)));
                auto    displacement_ws = math::mul(displacement_vs, iv);
                m_pinhole_camera->m_view_position_ws = math::add(m_pinhole_camera->position(), displacement_ws);
            }

            void on_move_xz(const move_camera_xz* command) override
            {
                auto	displacement_vs = math::set(command->m_direction_x, 0.0f, command->m_direction_z, 0.0f);
                auto    iv = math::transpose(math::inverse(gx::view_matrix(m_pinhole_camera)));
                auto    displacement_ws = math::mul(displacement_vs, iv);
                m_pinhole_camera->m_view_position_ws = math::add(m_pinhole_camera->position(), displacement_ws);
            }

            void on_move_z(const move_camera_z* command) override
            {
                auto	view_direction_ws = m_pinhole_camera->forward();
                auto	normalized_view_direction_ws = math::normalize3(view_direction_ws);

                auto	distance = command->m_distance;
                auto	distance_ws = math::splat(distance);
                auto	displacement_ws = math::mul(distance_ws, normalized_view_direction_ws);

                m_pinhole_camera->m_view_position_ws = math::add(m_pinhole_camera->m_view_position_ws, displacement_ws);
            }

            gx::pinhole_camera*	m_pinhole_camera;
        };
    }
}

