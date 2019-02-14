#pragma once

#include <cstdint>

#include <uc_dev/util/noncopyable.h>
#include <boost/pool/object_pool.hpp>

namespace uc
{
    namespace gx
    {
        namespace geo
        {
            class offset
            {
                public:

                offset() = default;

                offset(uint32_t value) : m_value(value)
                {

                }

                operator uint32_t() const
                {
                    return m_value;
                }

                uint32_t value() const
                {
                    return m_value;
                }

                offset& operator -=(const offset o) throw()
                {
                    m_value -= o.value();
                    return *this;
                }

                offset& operator +=(const offset o) throw()
                {
                    m_value += o.value();
                    return *this;
                }

                private:

                uint32_t m_value = 0;
            };

            class count
            {
                public:

                count() = default;

                count(uint32_t value)  throw() : m_value(value)
                {

                }

                operator uint32_t() const throw()
                {
                    return m_value;
                }

                uint32_t value() const throw()
                {
                    return m_value;
                }

                count& operator -=(const count o) throw()
                {
                    m_value -= o.value();
                    return *this;
                }

                count& operator +=(const count o) throw()
                {
                    m_value += o.value();
                    return *this;
                }

            private:

                uint32_t m_value = 0;
            };

            class object_allocation
            {
                public:
                object_allocation(offset o, count c, bool free = false) : m_offset(o), m_count(c), m_free(free) {}

                ~object_allocation() = default;
                    
                object_allocation* m_previous = nullptr;
                object_allocation* m_next     = nullptr;

                offset        m_offset;
                count         m_count;
                bool          m_free = false;

                offset        offset() const
                {
                    return    m_offset;
                }

                count         count() const
                {
                    return m_count;
                }

                void set_free(bool value)
                {
                    m_free = value;
                }

                bool is_free() const
                {
                    return m_free;
                }
            };

            class object_allocator : private util::noncopyable
            {
                public:

                using handle = object_allocation*;

                object_allocator(count max_object_count);
                ~object_allocator();

                handle allocate(count object_count);
                handle allocate(count object_count, std::nothrow_t ) noexcept;
                void   free( handle );

            private:
                template<class ElementType>          using object_pool = boost::object_pool<ElementType>;
                object_pool<object_allocation>       m_memory;
                object_allocation*                   m_allocations;
            };
            
        }
    }
}

