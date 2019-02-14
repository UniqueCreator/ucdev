#pragma once

#include <vector>
#include <uc_dev/math/math.h>

#include <uc_dev/gx/lip/animation.h>

namespace uc {
    namespace gx {
        namespace anm
        {
            class skeleton_instance
            {
                public:

                explicit skeleton_instance(const lip::skeleton* s);
                void reset();

                std::vector< math::float4x4 >& local_transforms();
                std::vector< math::float4x4 >  concatenate_transforms(math::afloat4x4 locomotion_transform);
                private:
                std::vector < math::float4x4 > m_joint_local_transforms2;
                const lip::skeleton*           m_skeleton;
            };
        }
    }
}
