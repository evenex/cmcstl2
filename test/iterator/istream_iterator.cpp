// cmcstl2 - A concept-enabled C++ standard library
//
//  Copyright Casey Carter 2015
//
//  Use, modification and distribution is subject to the
//  Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
// Project home: https://github.com/caseycarter/cmcstl2
//
#include <stl2/detail/iterator/istream_iterator.hpp>
#include <stl2/iterator.hpp>
#include <stl2/type_traits.hpp>
#include <sstream>
#include "../simple_test.hpp"

using namespace __stl2;

struct Int {
  int value_;

  friend std::istream& operator>>(std::istream& is, Int& i) {
    return is >> i.value_;
  }
};

int main() {
  {
    using I = istream_iterator<int>;
    static_assert(is_same<DifferenceType<I>, std::ptrdiff_t>());
    static_assert(is_same<ValueType<I>, int>());
    static_assert(is_same<ReferenceType<I>, const int&>());
    static_assert(is_same<RvalueReferenceType<I>, const int&&>());
    static_assert(models::WeakInputIterator<I>);
    static_assert(models::InputIterator<I>);
    static_assert(!models::ForwardIterator<I>);

    static_assert(models::Sentinel<default_sentinel, I>);
    using C = CommonType<I, default_sentinel>;
    static_assert(is_same<C, I>());
  }
  {
    std::istringstream is("5 4 3 2 1 0");
    ::check_equal(
      ext::make_range(istream_iterator<int>{is}, default_sentinel{}),
        {5, 4, 3, 2, 1, 0});
  }
  {
    std::istringstream is("0.9 1.8 2.4 3.3");
    ::check_equal(
      ext::make_range(istream_iterator<double>{is}, default_sentinel{}),
        {0.9, 1.8, 2.4, 3.3});
  }

  {
    std::istringstream is("5 4 3 2 1 0");
    auto i = istream_iterator<Int>{is};
    for (auto n = 5; i != default_sentinel{} && n >= 0; --n, ++i) {
      CHECK(i->value_ == n);
    }
  }

  return ::test_result();
}
