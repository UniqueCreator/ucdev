#pragma once

#include <uc_public/uc_non_copyable.h>

#include <cstdint>
#include <atomic>

namespace uc
{
    class referenced_object : private non_copyable
    {
        protected:

		referenced_object::referenced_object() : m_ref_count(1)
		{

		}

		virtual ~referenced_object()
        {}

    public:

		void add_ref()
		{
			m_ref_count++;
		}

		void remove_ref()
		{
			m_ref_count--;
			if (m_ref_count < 1)
			{
				delete this;
			}
		}

    private:

        std::atomic<int32_t> m_ref_count;
    };

    inline void intrusive_ptr_add_ref(referenced_object * p)
    {
        p->add_ref();
    }
    
    inline void intrusive_ptr_release(referenced_object * p)
    {
        p->remove_ref();
    }
}
