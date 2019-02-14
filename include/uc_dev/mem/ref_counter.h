#pragma once

#include <cstdint>

namespace uc
{
    namespace mem
    {
        template <class derived>
        class ref_counter
        {
        private:
            typedef ref_counter<derived> this_type;

        public:

            friend void intrusive_ptr_add_ref(const derived* pointer)
            {
                ++((const this_type*)pointer)->counter_;
            }

            friend void intrusive_ptr_release(const derived* pointer)
            {
                if (--((const this_type*)pointer)->counter_ == 0)
                {
                    typedef char type_must_be_complete[sizeof(derived) ? 1 : -1];
                    (void) sizeof(type_must_be_complete);

                    delete pointer;
                }
            }

        protected:
            ref_counter() : counter_(0) {}
            ref_counter(const ref_counter&) : counter_(0) {}
            ref_counter(const ref_counter&&) : counter_(0) {}
            ref_counter& operator=(const ref_counter&) { return *this; }
            ref_counter&& operator=(const ref_counter&&) { return *this; }
            ~ref_counter() {};
            void swap(ref_counter&) {};
        private:
            mutable uint32_t counter_;
        };
    }
}
