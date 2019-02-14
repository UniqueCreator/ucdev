#pragma once

#include <immintrin.h>
#include <emmintrin.h>
#include <assert.h>

#include <uc_dev/sys/memcpy.h>

namespace uc
{
    namespace gx
    {
        namespace dx12
        {
            // A faster version of memcopy that uses SSE instructions.  TODO:  Write an ARM variant if necessary.
            inline void simd_mem_copy(void* __restrict dest, const void* __restrict source, size_t quad_word_count)
            {
                return sys::simd_mem_copy(dest, source, quad_word_count);
            }

            // A faster version of memcopy that uses AVX instructions.  works slightly faster ( 5 - 7%) than sse version on haswell
            inline void simd_mem_copy2(void* __restrict dest, const void* __restrict source, size_t double_quad_word_count)
            {
                return sys::simd_mem_copy2(dest, source, double_quad_word_count);
            }

            inline void simd_mem_fill(void* __restrict dest, __m128 fill_vector, size_t num_quad_words)
            {
                return sys::simd_mem_fill(dest, fill_vector, num_quad_words);
            }

            inline void mem_copy(void* __restrict dest, const void* __restrict source, uint64_t byte_count)
            {
                return sys::memcpy(dest, source, byte_count);
            }

			inline void mem_copy(void* __restrict dest, const void* __restrict source, uint32_t byte_count)
			{
				return sys::memcpy(dest, source, byte_count);
			}

        }
    }
}
