#pragma once

#include <assert.h>
#include <cstdint>
#include <uc_dev/util/noncopyable.h>

namespace uc
{
    namespace gxu
    {
        struct roll_camera
        {
            float m_angle;
        };

        struct yaw_camera
        {
            float m_angle;
        };

        struct pitch_camera
        {
            float m_angle;
        };

        struct move_camera_forward
        {
            static const uint16_t	id = 0;

            uint16_t	m_command_id;
            float		m_distance;
            uint8_t		m_camera_number;
        };

        struct move_camera_backward
        {
            static const uint16_t	id = 1;

            uint16_t	m_command_id;
            float		m_distance;
            uint8_t		m_camera_number;
        };

        struct turn_camera_left
        {
            static const uint16_t	id = 2;

            uint16_t	m_command_id;
            float		m_angle_radians;
            uint8_t		m_camera_number;
        };

        struct turn_camera_right
        {
            static const uint16_t	id = 3;

            uint16_t	m_command_id;
            float		m_angle_radians;
            uint8_t		m_camera_number;
        };

        struct aim_camera_up
        {
            static const uint16_t	id = 4;

            uint16_t	m_command_id;
            float		m_angle_radians;
            uint8_t		m_camera_number;
        };

        struct aim_camera_down
        {
            static const uint16_t	id = 5;

            uint16_t	m_command_id;
            float		m_angle_radians;
            uint8_t		m_camera_number;
        };

        struct move_camera_up
        {
            static const uint16_t	id = 6;

            uint16_t	m_command_id;
            float		m_distance;
            uint8_t		m_camera_number;
        };

        struct move_camera_down
        {
            static const uint16_t	id = 7;

            uint16_t	m_command_id;
            float		m_distance;
            uint8_t		m_camera_number;
        };

        struct strafe_camera_left
        {
            static const uint16_t	id = 8;

            uint16_t	m_command_id;
            float		m_distance;
            uint8_t		m_camera_number;
        };

        struct strafe_camera_right
        {
            static const uint16_t	id = 9;

            uint16_t	m_command_id;
            float		m_distance;
            uint8_t		m_camera_number;
        };

        struct move_camera_xy
        {
            static const uint16_t	id = 10;

            uint16_t	    m_command_id;
            float		    m_direction_x;
            float           m_direction_y;
            uint8_t		    m_camera_number;
        };

        struct move_camera_xz
        {
            static const uint16_t	id = 11;

            uint16_t	m_command_id;
            float		m_direction_x;
            float       m_direction_z;
            uint8_t		m_camera_number;
        };

        struct move_camera_z
        {
            static const uint16_t	id = 12;

            uint16_t	m_command_id;
            float		m_distance; //can be negative
            uint8_t		m_camera_number;
        };

        struct camera_command_holder
        {
            uint16_t m_command_id;
            uint8_t  m_parameters[62];
        };

        struct camera_command
        {
            union
            {
                camera_command_holder	m_description;
                move_camera_forward		m_forward;
                move_camera_backward	m_backward;
                turn_camera_left		m_turn_left;
                turn_camera_right		m_turn_right;
                aim_camera_up			m_aim_up;
                aim_camera_down			m_aim_down;
                move_camera_xy          m_move_xy;
                move_camera_xz          m_move_xz;
                move_camera_z	        m_move_z;
            } m_command;
        };

        inline camera_command create_move_forward_command(float distance)
        {
            camera_command command;
            command.m_command.m_description.m_command_id = move_camera_forward::id;
            command.m_command.m_forward.m_distance = distance;
            command.m_command.m_forward.m_camera_number = 0;
            return command;
        }

        inline camera_command create_move_backward_command(float distance)
        {
            camera_command command;
            command.m_command.m_description.m_command_id = move_camera_backward::id;
            command.m_command.m_backward.m_distance = distance;
            command.m_command.m_backward.m_camera_number = 0;
            return command;
        }

        inline camera_command create_turn_camera_left_command(float m_angle_radians)
        {
            camera_command command;
            command.m_command.m_description.m_command_id = turn_camera_left::id;
            command.m_command.m_turn_left.m_angle_radians = m_angle_radians;
            command.m_command.m_turn_left.m_camera_number = 0;
            return command;
        }

        inline camera_command create_turn_camera_right_command(float m_angle_radians)
        {
            camera_command command;
            command.m_command.m_description.m_command_id = turn_camera_right::id;
            command.m_command.m_turn_right.m_angle_radians = m_angle_radians;
            command.m_command.m_turn_right.m_camera_number = 0;
            return command;
        }

        inline camera_command create_aim_camera_up_command(float m_angle_radians)
        {
            camera_command command;
            command.m_command.m_description.m_command_id = aim_camera_up::id;
            command.m_command.m_aim_up.m_angle_radians = m_angle_radians;
            command.m_command.m_aim_up.m_camera_number = 0;
            return command;
        }

        inline camera_command create_aim_camera_down_command(float m_angle_radians)
        {
            camera_command command;
            command.m_command.m_description.m_command_id = aim_camera_down::id;
            command.m_command.m_aim_down.m_angle_radians = m_angle_radians;
            command.m_command.m_aim_down.m_camera_number = 0;
            return command;
        }

        inline camera_command create_move_camera_xy_command(float direction_x, float direction_y)
        {
            camera_command command;
            command.m_command.m_description.m_command_id = move_camera_xy::id;
            command.m_command.m_move_xy.m_direction_x = direction_x;
            command.m_command.m_move_xy.m_direction_y = direction_y;
            command.m_command.m_move_xy.m_camera_number = 0;
            return command;
        }

        inline camera_command create_move_camera_xz_command(float direction_x, float direction_z)
        {
            camera_command command;
            command.m_command.m_description.m_command_id = move_camera_xz::id;
            command.m_command.m_move_xz.m_direction_x = direction_x;
            command.m_command.m_move_xz.m_direction_z = direction_z;
            command.m_command.m_move_xz.m_camera_number = 0;
            return command;
        }

        inline camera_command create_move_camera_z_command(float distance)
        {
            camera_command command;
            command.m_command.m_description.m_command_id = move_camera_z::id;
            command.m_command.m_move_z.m_distance = distance;
            command.m_command.m_move_z.m_camera_number = 0;
            return command;
        }

        //processes stream of camera commands and dispatches them
        //suitable for recording and playback
        class camera_command_dispatcher : private util::noncopyable
        {
        protected:

            virtual ~camera_command_dispatcher()
            {

            }

        public:

            template <typename iterator>
            void process(iterator begin, iterator end)
            {
                std::for_each(begin, end, [=](const camera_command& command) { process(&command); });
            }

            void process(const camera_command* command)
            {
                switch (command->m_command.m_description.m_command_id)
                {
                case move_camera_forward::id:
                {
                    on_move_forward(reinterpret_cast<const move_camera_forward*>(command));
                    break;
                }

                case move_camera_backward::id:
                {
                    on_move_backward(reinterpret_cast<const move_camera_backward*>(command));
                    break;
                }

                case turn_camera_left::id:
                {
                    on_turn_left(reinterpret_cast<const turn_camera_left*>(command));
                    break;
                }

                case turn_camera_right::id:
                {
                    on_turn_right(reinterpret_cast<const turn_camera_right*>(command));
                    break;
                }

                case aim_camera_up::id:
                {
                    on_aim_up(reinterpret_cast<const aim_camera_up*>(command));
                    break;
                }

                case aim_camera_down::id:
                {
                    on_aim_down(reinterpret_cast<const aim_camera_down*>(command));
                    break;
                }

                case move_camera_xy::id:
                {
                    on_move_xy(reinterpret_cast<const move_camera_xy*>(command));
                    break;
                }

                case move_camera_xz::id:
                {
                    on_move_xz(reinterpret_cast<const move_camera_xz*>(command));
                    break;
                }

                case move_camera_z::id:
                {
                    on_move_z(reinterpret_cast<const move_camera_z*>(command));
                    break;
                }

                default:
                    assert(false);
                    break;
                }
            }

        private:
            virtual void on_move_forward(const move_camera_forward*)
            {

            }

            virtual void on_move_backward(const move_camera_backward*)
            {

            }

            virtual void on_turn_left(const turn_camera_left*)
            {
            }

            virtual void on_turn_right(const turn_camera_right*)
            {
            }

            virtual void on_aim_up(const aim_camera_up*)
            {
            }

            virtual void on_aim_down(const aim_camera_down*)
            {

            }

            virtual void on_move_xy(const move_camera_xy*)
            {

            }

            virtual void on_move_xz(const move_camera_xz*)
            {

            }

            virtual void on_move_z(const move_camera_z*)
            {

            }
        };
    }
}

