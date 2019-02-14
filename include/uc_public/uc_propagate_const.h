#include <functional>
#include <type_traits>

#include <uc_public/uc_base.h>

// <experimental/propagate_const> -*- C++ -*-

// Copyright (C) 2015-2016 Free Software Foundation, Inc.
//
// This file is part of the GNU ISO C++ Library.  This library is free
// software; you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the
// Free Software Foundation; either version 3, or (at your option)
// any later version.

// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// Under Section 7 of GPL version 3, you are granted additional
// permissions described in the GCC Runtime Library Exception, version
// 3.1, as published by the Free Software Foundation.

// You should have received a copy of the GNU General Public License and
// a copy of the GCC Runtime Library Exception along with this program;
// see the files COPYING3 and COPYING.RUNTIME respectively.  If not, see
// <http://www.gnu.org/licenses/>.

/** @file experimental/propagate_const
*  This is a TS C++ Library header.
*/

#ifndef _GLIBCXX_EXPERIMENTAL_PROPAGATE_CONST
#define _GLIBCXX_EXPERIMENTAL_PROPAGATE_CONST 1

#define _GLIBCXX_BEGIN_NAMESPACE_VERSION
#define _GLIBCXX_END_NAMESPACE_VERSION

#define _GLIBCXX_VISIBILITY(x)

#if 0
# include <bits/c++14_warning.h>
#else

#include <type_traits>
#include <functional>

namespace std
{
	template<typename...>
	struct __or_;

	template<>
	struct __or_<>
		: public false_type
	{ };

	template<typename _B1>
	struct __or_<_B1>
		: public _B1
	{ };

	template<typename _B1, typename _B2>
	struct __or_<_B1, _B2>
		: public conditional<_B1::value, _B1, _B2>::type
	{ };

	template<typename _B1, typename _B2, typename _B3, typename... _Bn>
	struct __or_<_B1, _B2, _B3, _Bn...>
		: public conditional<_B1::value, _B1, __or_<_B2, _B3, _Bn...>>::type
	{ };

	template<typename...>
	struct __and_;

	template<>
	struct __and_<>
		: public true_type
	{ };

	template<typename _B1>
	struct __and_<_B1>
		: public _B1
	{ };

	template<typename _B1, typename _B2>
	struct __and_<_B1, _B2>
		: public conditional<_B1::value, _B2, _B1>::type
	{ };

	template<typename _B1, typename _B2, typename _B3, typename... _Bn>
	struct __and_<_B1, _B2, _B3, _Bn...>
		: public conditional<_B1::value, __and_<_B2, _B3, _Bn...>, _B1>::type
	{ };

	template<typename _Pp>
	struct __not_
		: public integral_constant<bool, !_Pp::value>
	{ };

}

namespace std _GLIBCXX_VISIBILITY(default)
{
	namespace experimental
	{
		inline namespace fundamentals_v2
		{
			_GLIBCXX_BEGIN_NAMESPACE_VERSION

				/**
				* @defgroup propagate_const Const-propagating wrapper
				* @ingroup experimental
				*
				* A const-propagating wrapper that propagates const to pointer-like members,
				* as described in n4388 "A Proposal to Add a Const-Propagating Wrapper
				* to the Standard Library".
				*
				* @{
				*/

				/// Const-propagating wrapper.
				template <typename _Tp>
			class propagate_const
			{
			public:
				typedef remove_reference_t<decltype(*declval<_Tp&>())> element_type;

			private:
				template <typename _Up>
				struct __is_propagate_const : false_type
				{ };

				template <typename _Up>
				struct __is_propagate_const<propagate_const<_Up>> : true_type
				{ };

				template <typename _Up>
				friend constexpr const _Up&
					get_underlying(const propagate_const<_Up>& __pt) noexcept;
				template <typename _Up>
				friend constexpr _Up&
					get_underlying(propagate_const<_Up>& __pt) noexcept;

				template <typename _Up>
				static constexpr element_type*
					__to_raw_pointer(_Up* __u)
				{
					return __u;
				}

				template <typename _Up>
				static constexpr element_type*
					__to_raw_pointer(_Up& __u)
				{
					return __u.get();
				}

				template <typename _Up>
				static constexpr const element_type*
					__to_raw_pointer(const _Up* __u)
				{
					return __u;
				}

				template <typename _Up>
				static constexpr const element_type*
					__to_raw_pointer(const _Up& __u)
				{
					return __u.get();
				}

			public:
				static_assert(__and_<is_object<typename remove_pointer<_Tp>::type>,
					__not_<is_array<_Tp>>,
					__or_<is_class<_Tp>, is_pointer<_Tp>>>::value,
					"propagate_const requires a class or a pointer to an"
					" object type");

				// [propagate_const.ctor], constructors
				constexpr propagate_const() = default;
				propagate_const(const propagate_const& __p) = delete;
				constexpr propagate_const(propagate_const&& __p) = default;
				template <typename _Up, typename
					enable_if<__and_<is_constructible<_Tp, _Up&&>,
					is_convertible<_Up&&, _Tp>>::value, bool
					>::type = true>
					constexpr propagate_const(propagate_const<_Up>&& __pu)
					: _M_t(std::move(get_underlying(__pu)))
				{}
				template <typename _Up, typename
					enable_if<__and_<is_constructible<_Tp, _Up&&>,
					__not_<is_convertible<_Up&&, _Tp>>>::value,
					bool>::type = false>
					constexpr explicit propagate_const(propagate_const<_Up>&& __pu)
					: _M_t(std::move(get_underlying(__pu)))
				{}
				template <typename _Up, typename
					enable_if<__and_<is_constructible<_Tp, _Up&&>,
					is_convertible<_Up&&, _Tp>,
					__not_<__is_propagate_const<
					typename decay<_Up>::type>>
					>::value, bool>::type = true>
					constexpr propagate_const(_Up&& __u)
					: _M_t(std::forward<_Up>(__u))
				{}
				template <typename _Up, typename
					enable_if<__and_<is_constructible<_Tp, _Up&&>,
					__not_<is_convertible<_Up&&, _Tp>>,
					__not_<__is_propagate_const<
					typename decay<_Up>::type>>
					>::value, bool>::type = false>
					constexpr explicit propagate_const(_Up&& __u)
					: _M_t(std::forward<_Up>(__u))
				{}

				// [propagate_const.assignment], assignment
				propagate_const& operator=(const propagate_const& __p) = delete;
				propagate_const& operator=(propagate_const&& __p) = default;

				template <typename _Up, typename =
					typename enable_if<is_convertible<_Up&&, _Tp>::value>::type>
					propagate_const& operator=(propagate_const<_Up>&& __pu)
				{
					_M_t = std::move(get_underlying(__pu));
				}

				template <typename _Up, typename =
					typename enable_if<__and_<is_convertible<_Up&&, _Tp>,
					__not_<__is_propagate_const<
					typename decay<_Up>::type>>
					>::value>::type>
					propagate_const& operator=(_Up&& __u)
				{
					_M_t = std::forward<_Up>(__u);
				}

				// [propagate_const.const_observers], const observers
				explicit constexpr operator bool() const
				{
					return bool(_M_t);
				}

				constexpr const element_type* operator->() const
				{
					return get();
				}

				template <typename _Up = _Tp,
					typename enable_if<__or_<is_pointer<_Up>,
					is_convertible<_Up,
					const element_type*>
				>::value, bool>::type = true>
					constexpr operator const element_type*() const
				{
					return get();
				}

				constexpr const element_type& operator*() const
				{
					return *get();
				}

				constexpr const element_type* get() const
				{
					return __to_raw_pointer(_M_t);
				}

				// [propagate_const.non_const_observers], non-const observers
				const element_type* operator->()
				{
					return get();
				}

				template <typename _Up = _Tp,
					typename enable_if<__or_<is_pointer<_Up>,
					is_convertible<_Up,
					const element_type*>
				>::value, bool>::type = true>
					operator element_type*()
				{
					return get();
				}

				element_type& operator*()
				{
					return *get();
				}

				element_type* get()
				{
					return __to_raw_pointer(_M_t);
				}

				// [propagate_const.modifiers], modifiers
				void
					swap(propagate_const& __pt) noexcept(__is_nothrow_swappable<_Tp>::value)
				{
					using std::swap;
					swap(_M_t, get_underlying(__pt));
				}

			private:
				_Tp _M_t;
			};

			// [propagate_const.relational], relational operators
			template <typename _Tp>
			constexpr bool
				operator==(const propagate_const<_Tp>& __pt, nullptr_t)
			{
				return get_underlying(__pt) == nullptr;
			}

			template <typename _Tp>
			constexpr bool
				operator==(nullptr_t, const propagate_const<_Tp>& __pu)
			{
				return nullptr == get_underlying(__pu);
			}

			template <typename _Tp>
			constexpr bool
				operator!=(const propagate_const<_Tp>& __pt, nullptr_t)
			{
				return get_underlying(__pt) != nullptr;
			}

			template <typename _Tp>
			constexpr bool operator!=(nullptr_t, const propagate_const<_Tp>& __pu)
			{
				return nullptr != get_underlying(__pu);
			}

			template <typename _Tp, typename _Up>
			constexpr bool
				operator==(const propagate_const<_Tp>& __pt,
					const propagate_const<_Up>& __pu)
			{
				return get_underlying(__pt) == get_underlying(__pu);
			}

			template <typename _Tp, typename _Up>
			constexpr bool
				operator!=(const propagate_const<_Tp>& __pt,
					const propagate_const<_Up>& __pu)
			{
				return get_underlying(__pt) != get_underlying(__pu);
			}

			template <typename _Tp, typename _Up>
			constexpr bool
				operator<(const propagate_const<_Tp>& __pt,
					const propagate_const<_Up>& __pu)
			{
				return get_underlying(__pt) < get_underlying(__pu);
			}

			template <typename _Tp, typename _Up>
			constexpr bool
				operator>(const propagate_const<_Tp>& __pt,
					const propagate_const<_Up>& __pu)
			{
				return get_underlying(__pt) > get_underlying(__pu);
			}

			template <typename _Tp, typename _Up>
			constexpr bool
				operator<=(const propagate_const<_Tp>& __pt,
					const propagate_const<_Up>& __pu)
			{
				return get_underlying(__pt) <= get_underlying(__pu);
			}

			template <typename _Tp, typename _Up>
			constexpr bool
				operator>=(const propagate_const<_Tp>& __pt,
					const propagate_const<_Up>& __pu)
			{
				return get_underlying(__pt) >= get_underlying(__pu);
			}

			template <typename _Tp, typename _Up>
			constexpr bool
				operator==(const propagate_const<_Tp>& __pt, const _Up& __u)
			{
				return get_underlying(__pt) == __u;
			}

			template <typename _Tp, typename _Up>
			constexpr bool
				operator!=(const propagate_const<_Tp>& __pt, const _Up& __u)
			{
				return get_underlying(__pt) != __u;
			}

			template <typename _Tp, typename _Up>
			constexpr bool
				operator<(const propagate_const<_Tp>& __pt, const _Up& __u)
			{
				return get_underlying(__pt) < __u;
			}

			template <typename _Tp, typename _Up>
			constexpr bool
				operator>(const propagate_const<_Tp>& __pt, const _Up& __u)
			{
				return get_underlying(__pt) > __u;
			}

			template <typename _Tp, typename _Up>
			constexpr bool
				operator<=(const propagate_const<_Tp>& __pt, const _Up& __u)
			{
				return get_underlying(__pt) <= __u;
			}

			template <typename _Tp, typename _Up>
			constexpr bool
				operator>=(const propagate_const<_Tp>& __pt, const _Up& __u)
			{
				return get_underlying(__pt) >= __u;
			}

			template <typename _Tp, typename _Up>
			constexpr bool
				operator==(const _Tp& __t, const propagate_const<_Up>& __pu)
			{
				return __t == get_underlying(__pu);
			}

			template <typename _Tp, typename _Up>
			constexpr bool
				operator!=(const _Tp& __t, const propagate_const<_Up>& __pu)
			{
				return __t != get_underlying(__pu);
			}

			template <typename _Tp, typename _Up>
			constexpr bool
				operator<(const _Tp& __t, const propagate_const<_Up>& __pu)
			{
				return __t < get_underlying(__pu);
			}

			template <typename _Tp, typename _Up>
			constexpr bool
				operator>(const _Tp& __t, const propagate_const<_Up>& __pu)
			{
				return __t > get_underlying(__pu);
			}

			template <typename _Tp, typename _Up>
			constexpr bool
				operator<=(const _Tp& __t, const propagate_const<_Up>& __pu)
			{
				return __t <= get_underlying(__pu);
			}

			template <typename _Tp, typename _Up>
			constexpr bool
				operator>=(const _Tp& __t, const propagate_const<_Up>& __pu)
			{
				return __t >= get_underlying(__pu);
			}

			// [propagate_const.algorithms], specialized algorithms
			template <typename _Tp>
			constexpr void
				swap(propagate_const<_Tp>& __pt, propagate_const<_Tp>& __pt2)
				noexcept(__is_nothrow_swappable<_Tp>::value)
			{
				__pt.swap(__pt2);
			}

			// [propagate_const.underlying], underlying pointer access
			template <typename _Tp>
			constexpr const _Tp&
				get_underlying(const propagate_const<_Tp>& __pt) noexcept
			{
				return __pt._M_t;
			}

			template <typename _Tp>
			constexpr _Tp&
				get_underlying(propagate_const<_Tp>& __pt) noexcept
			{
				return __pt._M_t;
			}

			// @} group propagate_const
			_GLIBCXX_END_NAMESPACE_VERSION
		} // namespace fundamentals_v2
	} // namespace experimental

	  // [propagate_const.hash], hash support
	template <typename _Tp>
	struct hash<experimental::propagate_const<_Tp>>
	{
		using result_type = size_t;
		using argument_type = experimental::propagate_const<_Tp>;

		size_t
			operator()(const experimental::propagate_const<_Tp>& __t) const
			noexcept(noexcept(hash<_Tp>{}(get_underlying(__t))))
		{
			return hash<_Tp>{}(get_underlying(__t));
		}
	};

	// [propagate_const.comparison_function_objects], comparison function objects
	template <typename _Tp>
	struct equal_to<experimental::propagate_const<_Tp>>
	{
		constexpr bool
			operator()(const experimental::propagate_const<_Tp>& __x,
				const experimental::propagate_const<_Tp>& __y) const
		{
			return equal_to<_Tp>{}(get_underlying(__x), get_underlying(__y));
		}

		typedef experimental::propagate_const<_Tp> first_argument_type;
		typedef experimental::propagate_const<_Tp> second_argument_type;
		typedef bool result_type;
	};

	template <typename _Tp>
	struct not_equal_to<experimental::propagate_const<_Tp>>
	{
		constexpr bool
			operator()(const experimental::propagate_const<_Tp>& __x,
				const experimental::propagate_const<_Tp>& __y) const
		{
			return not_equal_to<_Tp>{}(get_underlying(__x), get_underlying(__y));
		}

		typedef experimental::propagate_const<_Tp> first_argument_type;
		typedef experimental::propagate_const<_Tp> second_argument_type;
		typedef bool result_type;
	};

	template <typename _Tp>
	struct less<experimental::propagate_const<_Tp>>
	{
		constexpr bool
			operator()(const experimental::propagate_const<_Tp>& __x,
				const experimental::propagate_const<_Tp>& __y) const
		{
			return less<_Tp>{}(get_underlying(__x), get_underlying(__y));
		}

		typedef experimental::propagate_const<_Tp> first_argument_type;
		typedef experimental::propagate_const<_Tp> second_argument_type;
		typedef bool result_type;
	};

	template <typename _Tp>
	struct greater<experimental::propagate_const<_Tp>>
	{
		constexpr bool
			operator()(const experimental::propagate_const<_Tp>& __x,
				const experimental::propagate_const<_Tp>& __y) const
		{
			return greater<_Tp>{}(get_underlying(__x), get_underlying(__y));
		}

		typedef experimental::propagate_const<_Tp> first_argument_type;
		typedef experimental::propagate_const<_Tp> second_argument_type;
		typedef bool result_type;
	};

	template <typename _Tp>
	struct less_equal<experimental::propagate_const<_Tp>>
	{
		constexpr bool
			operator()(const experimental::propagate_const<_Tp>& __x,
				const experimental::propagate_const<_Tp>& __y) const
		{
			return less_equal<_Tp>{}(get_underlying(__x), get_underlying(__y));
		}

		typedef experimental::propagate_const<_Tp> first_argument_type;
		typedef experimental::propagate_const<_Tp> second_argument_type;
		typedef bool result_type;
	};

	template <typename _Tp>
	struct greater_equal<experimental::propagate_const<_Tp>>
	{
		constexpr bool
			operator()(const experimental::propagate_const<_Tp>& __x,
				const experimental::propagate_const<_Tp>& __y) const
		{
			return greater_equal<_Tp>{}(get_underlying(__x), get_underlying(__y));
		}

		typedef experimental::propagate_const<_Tp> first_argument_type;
		typedef experimental::propagate_const<_Tp> second_argument_type;
		typedef bool result_type;
	};
} // namespace std

#endif // C++14

#endif // _GLIBCXX_EXPERIMENTAL_PROPAGATE_CONST



/*

namespace UniqueCreator
{

	template <typename T>
	class propagate_const
	{
		typedef decltype(*std::declval<T>()) reference_type;

		public:

		using value_type = typename std::enable_if_t< std::is_lvalue_reference<reference_type>::value, typename std::remove_reference<reference_type>::type>::type;

		~propagate_const() = default;
		propagate_const() = default;

		template <typename U, typename V = std::enable_if_t<std::is_convertible<U, T&gt::value>>>
		propagate_const(U&& u) : t{ std::forward<U>(u) }
		{
		}
		template <typename U, typename V = std::enable_if_t<std::is_convertible<U, T&gt::value>>>
		propagate_const<T>& operator = (U&& u)
		{
			t = std::forward<U>(u);
			return *this;
		}

		template <typename U, typename V = std::enable_if_t<std::is_convertible<U, T&gt::value>>>
		propagate_const(const propagate_const<U>& pu) : t{ pu.t }
		{
		}
		
		template <typename U, typename V = std::enable_if_t<std::is_convertible<U, T&gt::value>>>
		propagate_const(propagate_const<U>&& pu) : t{ std::move(pu.t) }
		{
		}

		template <typename U, typename V = std::enable_if_t<std::is_convertible<U, T&gt::value>>>
		propagate_const<T>& operator = ( const propagate_const<U>& pt )
		{
			t = pt.t;
			return *this;
		}

		template <typename U, typename V = std::enable_if_t<std::is_convertible<U, T&gt::value>>>
		propagate_const<T>& operator = (propagate_const<U>&& pt)
		{
			t = std::move(pt.t);
			return *this;
		}

		value_type* operator->()
		{
			return underlying_pointer(t);
		}

		const value_type* operator->() const
		{
			return underlying_pointer(t);
		}

		value_type* get()
		{
			return underlying_pointer(t);
		}

		const value_type* get() const
		{
			return underlying_pointer(t);
		}

		template <typename T_ = T, typename V = std::enable_if_t<std::is_pointer<T_>::value>>
		operator value_type*()
		{
			return underlying_pointer(t);
		}

		template <typename T_ = T, typename V = std::enable_if_t<std::is_pointer<T_>::value>>
		operator const value_type*() const
		{
			return underlying_pointer(t);
		}

		value_type& operator*()
		{
			return *t;
		}
		
		const value_type& operator*() const
		{
			return *t;
		}

		explicit operator bool() const
		{
			return static_cast<bool>(t);
		}

	private:

		T t;
		template<typename U>
		static value_type* underlying_pointer(U* p)
		{
			return p;
		}

		template<typename U>
		static value_type* underlying_pointer(U& p)
		{
			return p.get();
		}

		template<typename U>
		static const value_type* underlying_pointer(const U* p)
		{
			return p;
		}

		template<typename U>
		static const value_type* underlying_pointer(const U& p)
		{
			return p.get();
		}
	};

	template <typename T, typename U>
	inline bool operator == (const propagate_const<T>& pt, const propagate_const<U>& pu)
	{
		return pt.t == pu.t;
	}

	template <typename T, typename U>
	inline bool operator != (const propagate_const<T>& pt, const propagate_const<U>& pu)
	{
		return pt.t != pu.t;
	}

	template <typename T, typename U>
	inline bool operator < (const propagate_const<T>& pt, const propagate_const<U>& pu)
	{
		return pt.t < pu.t;
	}

	template <typename T, typename U>
	inline bool operator > (const propagate_const<T>& pt, const propagate_const<U>& pu)
	{
		return pt.t > pu.t;
	}
	
	template <typename T, typename U>
	inline bool operator <= (const propagate_const<T>& pt, const propagate_const<U>& pu)
	{
		return pt.t <= pu.t;
	}

	template <typename T, typename U>
	inline bool operator >= (const propagate_const<T>& pt, const propagate_const<U>& pu)
	{
		return pt.t >= pu.t;
	}

	template <typename T, typename U>
	inline void swap(propagate_const<T>& pt1, propagate_const<U>& pt2)
	{
		swap(pt1.t, pt2.t);
	}

	template <typename T>
	const T& get_underlying(const propagate_const<T>& pt)
	{
		return pt.t;
	}

	template <typename T>
	T& get_underlying(propagate_const<T>& pt)
	{
		return pt.t;
	}
}

namespace std
{
	template <typename T>
	struct hash<UniqueCreator::propagate_const<T>> : std::hash<T>
	{
		size_t operator()(const UniqueCreator::propagate_const<T>& p) const
		{
			return operator()(get_underlying(p));
		}
	};
}

*/