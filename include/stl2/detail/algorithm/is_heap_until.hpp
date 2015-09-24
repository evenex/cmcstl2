// cmcstl2 - A concept-enabled C++ standard library
//
//  Copyright Eric Niebler 2014
//  Copyright Casey Carter 2015
//
//  Use, modification and distribution is subject to the
//  Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
// Project home: https://github.com/caseycarter/cmcstl2
//
//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
#ifndef STL2_DETAIL_ALGORITHM_IS_HEAP_UNTIL_HPP
#define STL2_DETAIL_ALGORITHM_IS_HEAP_UNTIL_HPP

#include <stl2/functional.hpp>
#include <stl2/iterator.hpp>
#include <stl2/detail/fwd.hpp>
#include <stl2/detail/concepts/callable.hpp>

///////////////////////////////////////////////////////////////////////////
// is_heap_until [is.heap]
//
STL2_OPEN_NAMESPACE {
  template <RandomAccessIterator I, class Proj = identity,
            IndirectCallableStrictWeakOrder<Projected<I, Proj>> Comp = less<>>
  I __is_heap_until_n(I first, const DifferenceType<I> n,
                      Comp comp_ = Comp{}, Proj proj_ = Proj{}) {
    STL2_ASSERT(0 <= n);
    auto&& comp = __stl2::as_function(comp_);
    auto&& proj = __stl2::as_function(proj_);
    DifferenceType<I> p = 0, c = 1;
    I pp = first;
    while (c < n) {
      I cp = first + c;
      if (comp(proj(*pp), proj(*cp))) {
        return cp;
      }
      ++c;
      ++cp;
      if (c == n || comp(proj(*pp), proj(*cp))) {
        return cp;
      }
      ++p;
      ++pp;
      c = 2 * p + 1;
    }
    return first + n;
  }

  template <RandomAccessIterator I, Sentinel<I> S, class Proj = identity,
            IndirectCallableStrictWeakOrder<Projected<I, Proj>> Comp = less<>>
  I is_heap_until(I first, S last, Comp&& comp = Comp{}, Proj&& proj = Proj{}) {
    auto n = __stl2::distance(first, __stl2::move(last));
    return __stl2::__is_heap_until_n(__stl2::move(first), n,
                                     __stl2::forward<Comp>(comp),
                                     __stl2::forward<Proj>(proj));
  }

  template <RandomAccessRange Rng, class Proj = identity,
            IndirectCallableStrictWeakOrder<Projected<IteratorType<Rng>, Proj>> Comp = less<>>
  safe_iterator_t<Rng>
  is_heap_until(Rng&& rng, Comp&& comp = Comp{}, Proj&& proj = Proj{}) {
    return __stl2::__is_heap_until_n(__stl2::begin(rng), __stl2::distance(rng),
                                     __stl2::forward<Comp>(comp),
                                     __stl2::forward<Proj>(proj));
  }
} STL2_CLOSE_NAMESPACE

#endif