#pragma once
// The MIT License(MIT)
// 
// Copyright(c) 2015 Stefan Reinalter
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
// 
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#include <cstdint>
#include <string>

namespace uc
{
    namespace compile_time
    {
        template <size_t I>
        struct hash
        {
            template <size_t n>
            constexpr static inline uint32_t generate(const char(&str)[n])
            {
                return static_cast<uint32_t>(static_cast<uint64_t>(hash<I - 1u>::generate(str) ^ uint32_t(str[I - 1u])) * 16777619ull);
            }
        };

        template <>
        struct hash<0u>
        {
            template <size_t n>
            constexpr static inline uint32_t generate(const char(&str)[n])
            {
                str;
                return 2166136261u;
            }
        };
    }

    namespace run_time
    {
        inline uint32_t fnv1ahash(uint32_t hash, const char* str, size_t length)
        {
            for (size_t i = 0; i < length; ++i)
            {
                const uint32_t value = static_cast<uint32_t>(*str++);
                hash ^= value;
                hash *= 16777619u;
            }

            return hash;
        }

        inline uint32_t fnv1ahash(const char* str, size_t length)
        {
            return fnv1ahash(2166136261u, str, length);
        }

        inline uint32_t fnv1ahash(const char* str)
        {
            return fnv1ahash(str, strlen(str));
        }
    }

    namespace detail
    {
        template <typename T>
        struct hash_helper {};

        template <>
        struct hash_helper<const char*>
        {
            static inline uint32_t generate(const char* str)
            {
                return run_time::fnv1ahash(str);
            }
        };

        template <size_t n>
        struct hash_helper<char[n]>
        {
            constexpr static inline uint32_t generate(const char(&str)[n])
            {
                using namespace uc::compile_time;
                return hash<n - 1u>::generate(str);
            }
        };
    }


    template <typename T>
    constexpr static inline uint32_t generate_hash(const T& str)
    {
        return detail::hash_helper<T>::generate(str);
    }

    class string_hash
    {

    public:

        template <typename T>
        string_hash(const T& str)
            : m_hash(generate_hash(str))
        {

        }

        inline uint32_t get_hash(void) const
        {
            return m_hash;
        }

    private:

        const uint32_t m_hash;
    };


	inline string_hash make_string_hash(const std::string& s)
	{
		return string_hash(s.c_str());
	}
}
