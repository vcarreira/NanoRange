// Range v3 library
//
//  Copyright Eric Niebler 2014
//  Copyright Gonzalo Brito Gadeschi 2014
//
//  Use, modification and distribution is subject to the
//  Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
// Project home: https://github.com/ericniebler/range-v3
//
//  Copyright 2005 - 2007 Adobe Systems Incorporated
//  Distributed under the MIT License(see accompanying file LICENSE_1_0_0.txt
//  or a copy at http://stlab.adobe.com/licenses.html)

//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// Implementation based on the code in libc++
//   http://http://libcxx.llvm.org/

#include <nanorange/algorithm/is_sorted_until.hpp>
#include "../catch.hpp"
#include "../test_utils.hpp"
#include "../test_iterators.hpp"

namespace stl2 = nano;

namespace {

/// Calls the iterator interface of the algorithm
template <class Iter>
struct iter_call {
	using begin_t = Iter;
	using sentinel_t = typename sentinel_type<Iter>::type;

	template <class B, class E, class... Args>
	auto operator()(B&& It, E&& e, Args&& ... args)
	-> decltype(stl2::is_sorted_until(begin_t{It}, sentinel_t{e},
									  std::forward<Args>(args)...))
	{
		return stl2::is_sorted_until(begin_t{It}, sentinel_t{e},
									 std::forward<Args>(args)...);
	}
};

/// Calls the range interface of the algorithm
template <class Iter>
struct range_call {
	using begin_t = Iter;
	using sentinel_t = typename sentinel_type<Iter>::type;

	template <class B, class E, class... Args>
	auto operator()(B&& It, E&& e, Args&& ... args)
	-> decltype(stl2::is_sorted_until(
			::as_lvalue(stl2::ext::make_range(begin_t{It}, sentinel_t{e})),
			std::forward<Args>(args)...))
	{
		return stl2::is_sorted_until(
				::as_lvalue(stl2::ext::make_range(begin_t{It}, sentinel_t{e})),
				std::forward<Args>(args)...);
	}
};

template <class It, template <class> class FunT>
void test()
{
	using Fun = FunT<It>;

	{
		int a[] = {0};
		unsigned sa = sizeof(a) / sizeof(a[0]);
		CHECK(Fun{}(a, a) == It(a));
		CHECK(Fun{}(a, a + sa) == It(a + sa));
	}

	{
		int a[] = {0, 0};
		unsigned sa = sizeof(a) / sizeof(a[0]);
		CHECK(Fun{}(a, a + sa) == It(a + sa));
	}
	{
		int a[] = {0, 1};
		unsigned sa = sizeof(a) / sizeof(a[0]);
		CHECK(Fun{}(a, a + sa) == It(a + sa));
	}
	{
		int a[] = {1, 0};
		unsigned sa = sizeof(a) / sizeof(a[0]);
		CHECK(Fun{}(a, a + sa) == It(a + 1));
	}
	{
		int a[] = {1, 1};
		unsigned sa = sizeof(a) / sizeof(a[0]);
		CHECK(Fun{}(a, a + sa) == It(a + sa));
	}

	{
		int a[] = {0, 0, 0};
		unsigned sa = sizeof(a) / sizeof(a[0]);
		CHECK(Fun{}(a, a + sa) == It(a + sa));
	}
	{
		int a[] = {0, 0, 1};
		unsigned sa = sizeof(a) / sizeof(a[0]);
		CHECK(Fun{}(a, a + sa) == It(a + sa));
	}
	{
		int a[] = {0, 1, 0};
		unsigned sa = sizeof(a) / sizeof(a[0]);
		CHECK(Fun{}(a, a + sa) == It(a + 2));
	}
	{
		int a[] = {0, 1, 1};
		unsigned sa = sizeof(a) / sizeof(a[0]);
		CHECK(Fun{}(a, a + sa) == It(a + sa));
	}
	{
		int a[] = {1, 0, 0};
		unsigned sa = sizeof(a) / sizeof(a[0]);
		CHECK(Fun{}(a, a + sa) == It(a + 1));
	}
	{
		int a[] = {1, 0, 1};
		unsigned sa = sizeof(a) / sizeof(a[0]);
		CHECK(Fun{}(a, a + sa) == It(a + 1));
	}
	{
		int a[] = {1, 1, 0};
		unsigned sa = sizeof(a) / sizeof(a[0]);
		CHECK(Fun{}(a, a + sa) == It(a + 2));
	}
	{
		int a[] = {1, 1, 1};
		unsigned sa = sizeof(a) / sizeof(a[0]);
		CHECK(Fun{}(a, a + sa) == It(a + sa));
	}

	{
		int a[] = {0, 0, 0, 0};
		unsigned sa = sizeof(a) / sizeof(a[0]);
		CHECK(Fun{}(a, a + sa) == It(a + sa));
	}
	{
		int a[] = {0, 0, 0, 1};
		unsigned sa = sizeof(a) / sizeof(a[0]);
		CHECK(Fun{}(a, a + sa) == It(a + sa));
	}
	{
		int a[] = {0, 0, 1, 0};
		unsigned sa = sizeof(a) / sizeof(a[0]);
		CHECK(Fun{}(a, a + sa) == It(a + 3));
	}
	{
		int a[] = {0, 0, 1, 1};
		unsigned sa = sizeof(a) / sizeof(a[0]);
		CHECK(Fun{}(a, a + sa) == It(a + sa));
	}
	{
		int a[] = {0, 1, 0, 0};
		unsigned sa = sizeof(a) / sizeof(a[0]);
		CHECK(Fun{}(a, a + sa) == It(a + 2));
	}
	{
		int a[] = {0, 1, 0, 1};
		unsigned sa = sizeof(a) / sizeof(a[0]);
		CHECK(Fun{}(a, a + sa) == It(a + 2));
	}
	{
		int a[] = {0, 1, 1, 0};
		unsigned sa = sizeof(a) / sizeof(a[0]);
		CHECK(Fun{}(a, a + sa) == It(a + 3));
	}
	{
		int a[] = {0, 1, 1, 1};
		unsigned sa = sizeof(a) / sizeof(a[0]);
		CHECK(Fun{}(a, a + sa) == It(a + sa));
	}
	{
		int a[] = {1, 0, 0, 0};
		unsigned sa = sizeof(a) / sizeof(a[0]);
		CHECK(Fun{}(a, a + sa) == It(a + 1));
	}
	{
		int a[] = {1, 0, 0, 1};
		unsigned sa = sizeof(a) / sizeof(a[0]);
		CHECK(Fun{}(a, a + sa) == It(a + 1));
	}
	{
		int a[] = {1, 0, 1, 0};
		unsigned sa = sizeof(a) / sizeof(a[0]);
		CHECK(Fun{}(a, a + sa) == It(a + 1));
	}
	{
		int a[] = {1, 0, 1, 1};
		unsigned sa = sizeof(a) / sizeof(a[0]);
		CHECK(Fun{}(a, a + sa) == It(a + 1));
	}
	{
		int a[] = {1, 1, 0, 0};
		unsigned sa = sizeof(a) / sizeof(a[0]);
		CHECK(Fun{}(a, a + sa) == It(a + 2));
	}
	{
		int a[] = {1, 1, 0, 1};
		unsigned sa = sizeof(a) / sizeof(a[0]);
		CHECK(Fun{}(a, a + sa) == It(a + 2));
	}
	{
		int a[] = {1, 1, 1, 0};
		unsigned sa = sizeof(a) / sizeof(a[0]);
		CHECK(Fun{}(a, a + sa) == It(a + 3));
	}
	{
		int a[] = {1, 1, 1, 1};
		unsigned sa = sizeof(a) / sizeof(a[0]);
		CHECK(Fun{}(a, a + sa) == It(a + sa));
	}

	{
		int a[] = {0};
		unsigned sa = sizeof(a) / sizeof(a[0]);
		CHECK(Fun{}(a, a, std::greater<int>()) == It(a));
		CHECK(Fun{}(a, a + sa, std::greater<int>()) == It(a + sa));
	}
	{
		int a[] = {0, 0};
		unsigned sa = sizeof(a) / sizeof(a[0]);
		CHECK(Fun{}(a, a + sa, std::greater<int>()) == It(a + sa));
	}
	{
		int a[] = {0, 1};
		unsigned sa = sizeof(a) / sizeof(a[0]);
		CHECK(Fun{}(a, a + sa, std::greater<int>()) == It(a + 1));
	}
	{
		int a[] = {1, 0};
		unsigned sa = sizeof(a) / sizeof(a[0]);
		CHECK(Fun{}(a, a + sa, std::greater<int>()) == It(a + sa));
	}
	{
		int a[] = {1, 1};
		unsigned sa = sizeof(a) / sizeof(a[0]);
		CHECK(Fun{}(a, a + sa, std::greater<int>()) == It(a + sa));
	}

	{
		int a[] = {0, 0, 0};
		unsigned sa = sizeof(a) / sizeof(a[0]);
		CHECK(Fun{}(a, a + sa, std::greater<int>()) == It(a + sa));
	}
	{
		int a[] = {0, 0, 1};
		unsigned sa = sizeof(a) / sizeof(a[0]);
		CHECK(Fun{}(a, a + sa, std::greater<int>()) == It(a + 2));
	}
	{
		int a[] = {0, 1, 0};
		unsigned sa = sizeof(a) / sizeof(a[0]);
		CHECK(Fun{}(a, a + sa, std::greater<int>()) == It(a + 1));
	}
	{
		int a[] = {0, 1, 1};
		unsigned sa = sizeof(a) / sizeof(a[0]);
		CHECK(Fun{}(a, a + sa, std::greater<int>()) == It(a + 1));
	}
	{
		int a[] = {1, 0, 0};
		unsigned sa = sizeof(a) / sizeof(a[0]);
		CHECK(Fun{}(a, a + sa, std::greater<int>()) == It(a + sa));
	}
	{
		int a[] = {1, 0, 1};
		unsigned sa = sizeof(a) / sizeof(a[0]);
		CHECK(Fun{}(a, a + sa, std::greater<int>()) == It(a + 2));
	}
	{
		int a[] = {1, 1, 0};
		unsigned sa = sizeof(a) / sizeof(a[0]);
		CHECK(Fun{}(a, a + sa, std::greater<int>()) == It(a + sa));
	}
	{
		int a[] = {1, 1, 1};
		unsigned sa = sizeof(a) / sizeof(a[0]);
		CHECK(Fun{}(a, a + sa, std::greater<int>()) == It(a + sa));
	}

	{
		int a[] = {0, 0, 0, 0};
		unsigned sa = sizeof(a) / sizeof(a[0]);
		CHECK(Fun{}(a, a + sa, std::greater<int>()) == It(a + sa));
	}
	{
		int a[] = {0, 0, 0, 1};
		unsigned sa = sizeof(a) / sizeof(a[0]);
		CHECK(Fun{}(a, a + sa, std::greater<int>()) == It(a + 3));
	}
	{
		int a[] = {0, 0, 1, 0};
		unsigned sa = sizeof(a) / sizeof(a[0]);
		CHECK(Fun{}(a, a + sa, std::greater<int>()) == It(a + 2));
	}
	{
		int a[] = {0, 0, 1, 1};
		unsigned sa = sizeof(a) / sizeof(a[0]);
		CHECK(Fun{}(a, a + sa, std::greater<int>()) == It(a + 2));
	}
	{
		int a[] = {0, 1, 0, 0};
		unsigned sa = sizeof(a) / sizeof(a[0]);
		CHECK(Fun{}(a, a + sa, std::greater<int>()) == It(a + 1));
	}
	{
		int a[] = {0, 1, 0, 1};
		unsigned sa = sizeof(a) / sizeof(a[0]);
		CHECK(Fun{}(a, a + sa, std::greater<int>()) == It(a + 1));
	}
	{
		int a[] = {0, 1, 1, 0};
		unsigned sa = sizeof(a) / sizeof(a[0]);
		CHECK(Fun{}(a, a + sa, std::greater<int>()) == It(a + 1));
	}
	{
		int a[] = {0, 1, 1, 1};
		unsigned sa = sizeof(a) / sizeof(a[0]);
		CHECK(Fun{}(a, a + sa, std::greater<int>()) == It(a + 1));
	}
	{
		int a[] = {1, 0, 0, 0};
		unsigned sa = sizeof(a) / sizeof(a[0]);
		CHECK(Fun{}(a, a + sa, std::greater<int>()) == It(a + sa));
	}
	{
		int a[] = {1, 0, 0, 1};
		unsigned sa = sizeof(a) / sizeof(a[0]);
		CHECK(Fun{}(a, a + sa, std::greater<int>()) == It(a + 3));
	}
	{
		int a[] = {1, 0, 1, 0};
		unsigned sa = sizeof(a) / sizeof(a[0]);
		CHECK(Fun{}(a, a + sa, std::greater<int>()) == It(a + 2));
	}
	{
		int a[] = {1, 0, 1, 1};
		unsigned sa = sizeof(a) / sizeof(a[0]);
		CHECK(Fun{}(a, a + sa, std::greater<int>()) == It(a + 2));
	}
	{
		int a[] = {1, 1, 0, 0};
		unsigned sa = sizeof(a) / sizeof(a[0]);
		CHECK(Fun{}(a, a + sa, std::greater<int>()) == It(a + sa));
	}
	{
		int a[] = {1, 1, 0, 1};
		unsigned sa = sizeof(a) / sizeof(a[0]);
		CHECK(Fun{}(a, a + sa, std::greater<int>()) == It(a + 3));
	}
	{
		int a[] = {1, 1, 1, 0};
		unsigned sa = sizeof(a) / sizeof(a[0]);
		CHECK(Fun{}(a, a + sa, std::greater<int>()) == It(a + sa));
	}
	{
		int a[] = {1, 1, 1, 1};
		unsigned sa = sizeof(a) / sizeof(a[0]);
		CHECK(Fun{}(a, a + sa, std::greater<int>()) == It(a + sa));
	}
}

struct A { int a; };

}

TEST_CASE("alg.is_sorted_until")
{
	test<forward_iterator<const int*>, iter_call>();
	test<bidirectional_iterator<const int*>, iter_call>();
	test<random_access_iterator<const int*>, iter_call>();
	test<const int*, iter_call>();

	test<forward_iterator<const int*>, range_call>();
	test<bidirectional_iterator<const int*>, range_call>();
	test<random_access_iterator<const int*>, range_call>();
	test<const int*, range_call>();

	/// Initializer list test:
	{
		std::initializer_list<int> r = {0,1,2,3,4,5,6,7,8,9,10};
		CHECK(stl2::is_sorted_until(r) == stl2::end(r));
	}

	/// Projection test:
	{
		A as[] = {{0}, {1}, {2}, {3}, {4}};
		CHECK(stl2::is_sorted_until(as, std::less<int>{}, &A::a) == stl2::end(as));
		CHECK(stl2::is_sorted_until(as, std::greater<int>{}, &A::a) == stl2::next(stl2::begin(as),1));
	}

	/// Rvalue range test:
	{
		A as[] = {{0}, {1}, {2}, {3}, {4}};
		// FIXME: Usual MSVC silliness
#ifndef _MSC_VER
		CHECK(stl2::is_sorted_until(std::move(as), std::less<int>{}, &A::a).get_unsafe() == stl2::end(as));
		CHECK(stl2::is_sorted_until(std::move(as), std::greater<int>{}, &A::a).get_unsafe() == stl2::next(stl2::begin(as),1));
#else
		CHECK(stl2::is_sorted_until(std::move(as), std::less<int>{}, &A::a) == stl2::end(as));
		CHECK(stl2::is_sorted_until(std::move(as), std::greater<int>{}, &A::a) == stl2::next(stl2::begin(as),1));
#endif
	}
}
