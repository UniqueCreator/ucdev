#pragma once

#include <cstring>

#include <immintrin.h>
#include <emmintrin.h>
#include <assert.h>


namespace uc
{
    namespace sys
    {
        namespace
        {
            template <size_t alignment>
            inline uintptr_t align(uintptr_t size) throw()
            {
                static_assert ((alignment & alignment - 1) == 0, "alignment is a power of two");
                return size + (alignment - 1)  & ~(alignment - 1);
            }

            template <size_t alignment>
            inline bool is_aligned(uintptr_t size) throw()
            {
                static_assert ((alignment & alignment - 1) == 0, "alignment is a power of two");
                return (size & (alignment - 1)) == 0;
            }

            template <size_t alignment>
            inline bool is_aligned(const void* pointer) throw()
            {
                static_assert ((alignment & alignment - 1) == 0, "alignment is a power of two");
                return is_aligned<alignment>(reinterpret_cast<uintptr_t> (pointer));
            }

            template <size_t alignment>
            inline size_t divide_up(size_t value)
            {
                return align<alignment>(value) / alignment;
            }
        }

        // A faster version of memcopy that uses SSE instructions.  TODO:  Write an ARM variant if necessary.
        inline void simd_mem_copy(void* __restrict dest, const void* __restrict source, size_t quad_word_count)
        {
            assert(is_aligned<16>(dest));
            assert(is_aligned<16>(source));

            //todo: use this: _mm256_stream_si256 

            __m128i* __restrict Dest = (__m128i* __restrict)dest;
            const __m128i* __restrict Source = (const __m128i* __restrict)source;

            // Discover how many quadwords precede a cache line boundary.  Copy them separately.
            size_t InitialQuadwordCount = (4 - ((size_t)Source >> 4) & 3) & 3;

            if (InitialQuadwordCount > quad_word_count)
                InitialQuadwordCount = quad_word_count;

            switch (InitialQuadwordCount)
            {
            case 3: _mm_stream_si128(Dest + 2, _mm_load_si128(Source + 2));	 // Fall through
            case 2: _mm_stream_si128(Dest + 1, _mm_load_si128(Source + 1));	 // Fall through
            case 1: _mm_stream_si128(Dest + 0, _mm_load_si128(Source + 0));	 // Fall through
            default:
                break;
            }

            if (quad_word_count == InitialQuadwordCount)
                return;

            Dest += InitialQuadwordCount;
            Source += InitialQuadwordCount;
            quad_word_count -= InitialQuadwordCount;

            size_t CacheLines = quad_word_count >> 2;

            switch (CacheLines)
            {
            default:
            case 10: _mm_prefetch((char*)(Source + 36), _MM_HINT_NTA);	// Fall through
            case 9:  _mm_prefetch((char*)(Source + 32), _MM_HINT_NTA);	// Fall through
            case 8:  _mm_prefetch((char*)(Source + 28), _MM_HINT_NTA);	// Fall through
            case 7:  _mm_prefetch((char*)(Source + 24), _MM_HINT_NTA);	// Fall through
            case 6:  _mm_prefetch((char*)(Source + 20), _MM_HINT_NTA);	// Fall through
            case 5:  _mm_prefetch((char*)(Source + 16), _MM_HINT_NTA);	// Fall through
            case 4:  _mm_prefetch((char*)(Source + 12), _MM_HINT_NTA);	// Fall through
            case 3:  _mm_prefetch((char*)(Source + 8), _MM_HINT_NTA);	// Fall through
            case 2:  _mm_prefetch((char*)(Source + 4), _MM_HINT_NTA);	// Fall through
            case 1:  _mm_prefetch((char*)(Source + 0), _MM_HINT_NTA);	// Fall through

                                                                        // Do four quadwords per loop to minimize stalls.
                for (size_t i = CacheLines; i > 0; --i)
                {
                    // If this is a large copy, start prefetching future cache lines.  This also prefetches the
                    // trailing quadwords that are not part of a whole cache line.
                    if (i >= 10)
                        _mm_prefetch((char*)(Source + 40), _MM_HINT_NTA);

                    _mm_stream_si128(Dest + 0, _mm_load_si128(Source + 0));
                    _mm_stream_si128(Dest + 1, _mm_load_si128(Source + 1));
                    _mm_stream_si128(Dest + 2, _mm_load_si128(Source + 2));
                    _mm_stream_si128(Dest + 3, _mm_load_si128(Source + 3));

                    Dest += 4;
                    Source += 4;
                }

            case 0:	// No whole cache lines to read
                break;
            }

            // Copy the remaining quadwords
            switch (quad_word_count & 3)
            {
            case 3: _mm_stream_si128(Dest + 2, _mm_load_si128(Source + 2));	 // Fall through
            case 2: _mm_stream_si128(Dest + 1, _mm_load_si128(Source + 1));	 // Fall through
            case 1: _mm_stream_si128(Dest + 0, _mm_load_si128(Source + 0));	 // Fall through
            default:
                break;
            }

            _mm_sfence();
        }

        // A faster version of memcopy that uses AVX instructions.  works slightly faster ( 5 - 7%) than sse version on haswell
        inline void simd_mem_copy2(void* __restrict dest, const void* __restrict source, size_t double_quad_word_count)
        {
            assert(is_aligned<32>(dest));
            assert(is_aligned<32>(source));

            __m256i* __restrict Dest = (__m256i* __restrict)dest;
            const __m256i* __restrict Source = (const __m256i* __restrict)source;

            // Discover how many double quadwords precede a cache line boundary.  Copy them separately.
            //assume cache line 64 bytes
            size_t InitialQuadwordCount = (2 - ((size_t)Source >> 5) & 1) & 1;

            if (InitialQuadwordCount > double_quad_word_count)
                InitialQuadwordCount = double_quad_word_count;

            switch (InitialQuadwordCount)
            {
            case 1: _mm256_stream_si256(Dest + 0, _mm256_load_si256(Source + 0));	 // Fall through
            default:
                break;
            }

            if (double_quad_word_count == InitialQuadwordCount)
                return;

            Dest += InitialQuadwordCount;
            Source += InitialQuadwordCount;
            double_quad_word_count -= InitialQuadwordCount;

            size_t CacheLines = double_quad_word_count >> 1;

            switch (CacheLines)
            {
            default:
            case 10: _mm_prefetch((char*)(Source + 36), _MM_HINT_NTA);	// Fall through
            case 9:  _mm_prefetch((char*)(Source + 32), _MM_HINT_NTA);	// Fall through
            case 8:  _mm_prefetch((char*)(Source + 28), _MM_HINT_NTA);	// Fall through
            case 7:  _mm_prefetch((char*)(Source + 24), _MM_HINT_NTA);	// Fall through
            case 6:  _mm_prefetch((char*)(Source + 20), _MM_HINT_NTA);	// Fall through
            case 5:  _mm_prefetch((char*)(Source + 16), _MM_HINT_NTA);	// Fall through
            case 4:  _mm_prefetch((char*)(Source + 12), _MM_HINT_NTA);	// Fall through
            case 3:  _mm_prefetch((char*)(Source + 8), _MM_HINT_NTA);	// Fall through
            case 2:  _mm_prefetch((char*)(Source + 4), _MM_HINT_NTA);	// Fall through
            case 1:  _mm_prefetch((char*)(Source + 0), _MM_HINT_NTA);	// Fall through

                                                                        // Do four quadwords per loop to minimize stalls.
                for (size_t i = CacheLines; i > 0; --i)
                {
                    // If this is a large copy, start prefetching future cache lines.  This also prefetches the
                    // trailing quadwords that are not part of a whole cache line.
                    if (i >= 10)
                        _mm_prefetch((char*)(Source + 40), _MM_HINT_NTA);

                    _mm256_stream_si256(Dest + 0, _mm256_load_si256(Source + 0));
                    _mm256_stream_si256(Dest + 1, _mm256_load_si256(Source + 1));

                    Dest += 2;
                    Source += 2;
                }

            case 0:	// No whole cache lines to read
                break;
            }

            // Copy the remaining quadwords
            switch (double_quad_word_count & 1)
            {
            case 1: _mm256_stream_si256(Dest + 0, _mm256_load_si256(Source + 0));	 // Fall through
            default:
                break;
            }

            _mm_sfence();
        }

        inline void simd_mem_fill(void* __restrict _Dest, __m128 FillVector, size_t NumQuadwords)
        {
            assert(is_aligned<32>(_Dest));

            const __m128i Source = _mm_castps_si128(FillVector);
            __m128i* __restrict Dest = (__m128i* __restrict)_Dest;

            switch (((size_t)Dest >> 4) & 3)
            {
                case 1: _mm_stream_si128(Dest++, Source); --NumQuadwords;	 // Fall through
                case 2: _mm_stream_si128(Dest++, Source); --NumQuadwords;	 // Fall through
                case 3: _mm_stream_si128(Dest++, Source); --NumQuadwords;	 // Fall through
                default:
                break;
            }

            size_t WholeCacheLines = NumQuadwords >> 2;

            // Do four quadwords per loop to minimize stalls.
            while (WholeCacheLines--)
            {
                _mm_stream_si128(Dest++, Source);
                _mm_stream_si128(Dest++, Source);
                _mm_stream_si128(Dest++, Source);
                _mm_stream_si128(Dest++, Source);
            }

            // Copy the remaining quadwords
            switch (NumQuadwords & 3)
            {
                case 3: _mm_stream_si128(Dest++, Source);	 // Fall through
                case 2: _mm_stream_si128(Dest++, Source);	 // Fall through
                case 1: _mm_stream_si128(Dest++, Source);	 // Fall through
                default:
                break;
            }

            _mm_sfence();
        }

        inline bool is_avx_supported()
        {
            return false;
        }

        inline void memcpy(void* __restrict destination, const void* __restrict source, uint32_t bytes_to_copy )
        {
            

            /*
            #define OSXSAVEFlag (1UL<<27)
            #define AVXFlag     ((1UL<<28)|OSXSAVEFlag)
            #define FMAFlag     ((1UL<<12)|AVXFlag|OSXSAVEFlag)
            #define CLMULFlag   ((1UL<< 1)|AVXFlag|OSXSAVEFlag)
            #define VAESFlag    ((1UL<<25)|AVXFlag|OSXSAVEFlag)
            inline bool SimdDetectFeature(U32 idFeature)
            {
                    int EAX, EBX, ECX, EDX;
                    cpuid(0, EAX, EBX, ECX, EDX);
                    if ((ECX & idFeature) != idFeature)
                        return false;
                    return true;
            }
            */

            //todo: x86
            if ( is_aligned<32>(source) && is_aligned<32>(destination) && is_aligned<32>( bytes_to_copy) && is_avx_supported() )
            {
                simd_mem_copy2(destination, source, divide_up<32>(bytes_to_copy) );
            }
            else
            {
                if (is_aligned<16>(source) && is_aligned<16>(destination) && is_aligned<16>( bytes_to_copy) )
                {
                    simd_mem_copy(destination, source, divide_up<16>(bytes_to_copy));
                }
                else
                {
                    std::memcpy(destination, source, bytes_to_copy);
                }
            }
        }

		inline void memcpy(void* __restrict destination, const void* __restrict source, uint64_t bytes_to_copy)
		{
			assert(bytes_to_copy < 0xFFFFFFFF);
			memcpy(destination, source, static_cast<uint32_t>(bytes_to_copy));
		}
    }
}
