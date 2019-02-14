#pragma once

namespace uc
{
    namespace mem
    {
        //
        //  intrusive_ptr
        //
        //  A smart pointer that uses intrusive reference counting.
        //
        //  Relies on unqualified calls to
        //  
        //      void intrusive_ptr_add_ref(T * p);
        //      void intrusive_ptr_release(T * p);
        //
        //          (p != 0)
        //
        //  The object is responsible for destroying itself.
        //

        template<class T> class intrusive_ptr
        {
        private:

            typedef intrusive_ptr this_type;

        public:

            typedef T element_type;

            intrusive_ptr() throw() : px(0)
            {
            }

            intrusive_ptr(T * p, bool add_ref = true) : px(p)
            {
                if (px != 0 && add_ref) intrusive_ptr_add_ref(px);
            }

            template<class U>
            intrusive_ptr(intrusive_ptr<U> const & rhs)
                : px(rhs.get())
            {
                if (px != 0) intrusive_ptr_add_ref(px);
            }


            intrusive_ptr(intrusive_ptr const & rhs) : px(rhs.px)
            {
                if (px != 0) intrusive_ptr_add_ref(px);
            }

            ~intrusive_ptr()
            {
                if (px != 0) intrusive_ptr_release(px);
            }

            template<class U> intrusive_ptr & operator=(intrusive_ptr<U> const & rhs)
            {
                this_type(rhs).swap(*this);
                return *this;
            }

            intrusive_ptr(intrusive_ptr && rhs) throw() : px(rhs.px)
            {
                rhs.px = 0;
            }

            intrusive_ptr & operator=(intrusive_ptr && rhs) throw()
            {
                this_type(static_cast<intrusive_ptr &&>(rhs)).swap(*this);
                return *this;
            }

            intrusive_ptr & operator=(intrusive_ptr const & rhs)
            {
                this_type(rhs).swap(*this);
                return *this;
            }

            intrusive_ptr & operator=(T * rhs)
            {
                this_type(rhs).swap(*this);
                return *this;
            }

            void reset() throw()
            {
                this_type().swap(*this);
            }

            void reset(T * rhs)
            {
                this_type(rhs).swap(*this);
            }

            T * get() const throw()
            {
                return px;
            }

            T & operator*() const
            {
                return *px;
            }

            T * operator->() const
            {
                return px;
            }

            // implicit conversion to "bool"
            explicit operator bool() const throw()
            {
                return px != 0;
            }

            // operator! is redundant, but some compilers need it
            bool operator! () const throw()
            {
                return px == 0;
            }

            void swap(intrusive_ptr & rhs) throw()
            {
                T * tmp = px;
                px = rhs.px;
                rhs.px = tmp;
            }

        private:

            T * px;
        };

        template<class T, class U> inline bool operator==(intrusive_ptr<T> const & a, intrusive_ptr<U> const & b)
        {
            return a.get() == b.get();
        }

        template<class T, class U> inline bool operator!=(intrusive_ptr<T> const & a, intrusive_ptr<U> const & b)
        {
            return a.get() != b.get();
        }

        template<class T, class U> inline bool operator==(intrusive_ptr<T> const & a, U * b)
        {
            return a.get() == b;
        }

        template<class T, class U> inline bool operator!=(intrusive_ptr<T> const & a, U * b)
        {
            return a.get() != b;
        }

        template<class T, class U> inline bool operator==(T * a, intrusive_ptr<U> const & b)
        {
            return a == b.get();
        }

        template<class T, class U> inline bool operator!=(T * a, intrusive_ptr<U> const & b)
        {
            return a != b.get();
        }

        template<class T> inline bool operator==(intrusive_ptr<T> const & p, nullptr_t) throw()
        {
            return p.get() == 0;
        }

        template<class T> inline bool operator==(nullptr_t, intrusive_ptr<T> const & p)  throw()
        {
            return p.get() == 0;
        }

        template<class T> inline bool operator!=(intrusive_ptr<T> const & p, nullptr_t)  throw()
        {
            return p.get() != 0;
        }

        template<class T> inline bool operator!=(nullptr_t, intrusive_ptr<T> const & p)  throw()
        {
            return p.get() != 0;
        }

        template<class T> inline bool operator<(intrusive_ptr<T> const & a, intrusive_ptr<T> const & b)
        {
            return std::less<T *>()(a.get(), b.get());
        }

        template<class T> void swap(intrusive_ptr<T> & lhs, intrusive_ptr<T> & rhs)
        {
            lhs.swap(rhs);
        }

        // mem_fn support

        template<class T> T * get_pointer(intrusive_ptr<T> const & p)
        {
            return p.get();
        }

        template<class T, class U> intrusive_ptr<T> static_pointer_cast(intrusive_ptr<U> const & p)
        {
            return static_cast<T *>(p.get());
        }

        template<class T, class U> intrusive_ptr<T> const_pointer_cast(intrusive_ptr<U> const & p)
        {
            return const_cast<T *>(p.get());
        }

        template<class T, class U> intrusive_ptr<T> dynamic_pointer_cast(intrusive_ptr<U> const & p)
        {
            return dynamic_cast<T *>(p.get());
        }

        // hash_value

        template< class T > struct hash;

        template< class T > size_t hash_value(intrusive_ptr<T> const & p)
        {
            return std::hash< T* >()(p.get());
        }
    }
} 


