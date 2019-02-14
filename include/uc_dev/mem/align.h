#pragma once

#include <memory>

namespace uc
{
	namespace mem
	{
		namespace details
		{
			template <typename t> struct aligned_object
			{
				void operator () (t* d) const
				{
					d->~t();
					_aligned_free(d);
				}
			};
		}

		template < typename t> using aligned_unique_ptr = std::unique_ptr< t, details::aligned_object<t> >;

		template <typename t, typename... args> inline aligned_unique_ptr<t> make_aligned_unique_ptr( args&&... a)
		{
			void* memory = reinterpret_cast<t*> (_aligned_malloc(sizeof(t), 16) );

			if (memory)
			{

				//todo: check what to do with the exceptions here
				t*    o = new (memory) t(std::forward(a)...);
				return aligned_unique_ptr<t>(o, details::aligned_object<t>());
			}
			else
			{
				throw std::bad_alloc();
			}
		}

	}
}



