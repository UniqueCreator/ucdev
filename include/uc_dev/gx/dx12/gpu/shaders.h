#pragma once

#include <uc_dev/gx/dx12/api/error.h>

#include "virtual_resource.h"
#include "descriptor_heap.h"

namespace uc
{
    namespace gx
    {
        namespace dx12
        {
            //stub classes to be able to make a difference between shader types in the pipeline stage objects
            //these types we will allow us to make a difference between a geometry and vertex shader

            struct gpu_shader_code : public D3D12_SHADER_BYTECODE
            {
                using base = D3D12_SHADER_BYTECODE;

                template <uint32_t s > gpu_shader_code(const BYTE(&arg)[s])
                {
                    pShaderBytecode = &arg[0];
                    BytecodeLength = s;
                }

                gpu_shader_code()
                {
                    pShaderBytecode = nullptr;
                    BytecodeLength = 0;
                }

                const void* code() const
                {
                    return this->pShaderBytecode;
                }

                size_t      code_size() const
                {
                    return this->BytecodeLength;
                }
            };

            struct gpu_vertex_shader_code : public gpu_shader_code
            {
                using base = gpu_shader_code;

                template <uint32_t s > gpu_vertex_shader_code( const BYTE(&arg)[s] ) : base( arg )
                {

                }

                gpu_vertex_shader_code() {}
            };

            struct gpu_pixel_shader_code : public gpu_shader_code
            {
                using base = gpu_shader_code;

                template <uint32_t s > gpu_pixel_shader_code(const BYTE(&arg)[s]) : base<s>(arg)
                {

                }

                gpu_pixel_shader_code() {}
            };

            struct gpu_geometry_shader_code : public gpu_shader_code
            {
                using base = gpu_shader_code;

                template <uint32_t s > gpu_geometry_shader_code(const BYTE(&arg)[s]) : base(arg)
                {
                }

                gpu_geometry_shader_code() {}
            };

            struct gpu_hull_shader_code : public gpu_shader_code
            {
                using base = gpu_shader_code;

                template <uint32_t s > gpu_hull_shader_code(const BYTE(&arg)[s]) : base(arg)
                {
                }

                gpu_hull_shader_code() {}
            };

            struct gpu_domain_shader_code : public gpu_shader_code
            {
                using base = gpu_shader_code;

                template <uint32_t s > gpu_domain_shader_code(const BYTE(&arg)[s]) : base(arg)
                {
                }

                gpu_domain_shader_code() {}
            };

            struct gpu_compute_shader_code : public gpu_shader_code
            {
                using base = gpu_shader_code;

                template <uint32_t s > gpu_compute_shader_code(const BYTE(&arg)[s]) : base(arg)
                {

                }

                gpu_compute_shader_code() {}
            };
        }
    }
}
