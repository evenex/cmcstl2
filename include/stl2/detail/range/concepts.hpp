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
#ifndef STL2_DETAIL_RANGE_CONCEPTS_HPP
#define STL2_DETAIL_RANGE_CONCEPTS_HPP

#include <initializer_list>

#include <stl2/type_traits.hpp>
#include <stl2/detail/fwd.hpp>
#include <stl2/detail/meta.hpp>
#include <stl2/detail/concepts/compare.hpp>
#include <stl2/detail/concepts/core.hpp>
#include <stl2/detail/concepts/object.hpp>
#include <stl2/detail/iterator/concepts.hpp>
#include <stl2/detail/range/access.hpp>

namespace std {
#ifndef __GLIBCXX__
#pragma message "These forward declarations will likely only work with libstdc++."
#endif
	template <class, class, class> class set;
	template <class, class, class> class multiset;
	template <class, class, class, class> class unordered_set;
	template <class, class, class, class> class unordered_multiset;
}

STL2_OPEN_NAMESPACE {
	///////////////////////////////////////////////////////////////////////////
	// Range [ranges.range]
	//
	template <class T>
	using iterator_t = decltype(__stl2::begin(declval<T&>()));

	template <class T>
	using sentinel_t = decltype(__stl2::end(declval<T&>()));

	template <class T>
	concept bool Range =
		requires { typename sentinel_t<T>; };

	namespace models {
		template <class>
		constexpr bool Range = false;
		__stl2::Range{R}
		constexpr bool Range<R> = true;
	}

	///////////////////////////////////////////////////////////////////////////
	// SizedRange [ranges.sized]
	//
	template <class R>
	constexpr bool disable_sized_range = false;

	template <class R>
	concept bool SizedRange =
		Range<R> &&
		!disable_sized_range<__uncvref<R>> &&
		requires(const remove_reference_t<R>& r) {
			{ __stl2::size(r) } -> Integral;
			{ __stl2::size(r) } -> difference_type_t<iterator_t<R>>;
		};

	namespace models {
		template <class>
		constexpr bool SizedRange = false;
		__stl2::SizedRange{R}
		constexpr bool SizedRange<R> = true;
	}

	///////////////////////////////////////////////////////////////////////////
	// View [ranges.view]
	//
	struct view_base {};

	template <class T>
	concept bool _ContainerLike =
		Range<T> && Range<const T> &&
		!Same<reference_t<iterator_t<T>>, reference_t<iterator_t<const T>>>;

	namespace models {
		template <class>
		constexpr bool _ContainerLike = false;
		__stl2::_ContainerLike{R}
		constexpr bool _ContainerLike<R> = true;
	}

	template <class T>
	struct enable_view {};

	template <class T>
	constexpr bool __view_predicate = true;

	template <class T>
		requires _Valid<meta::_t, enable_view<T>>
	constexpr bool __view_predicate<T> = meta::_v<enable_view<T>>;

	// TODO: Be very certain that "!" here works as intended.
	template <_ContainerLike T>
		requires !(DerivedFrom<T, view_base> ||
			_Valid<meta::_t, enable_view<T>>)
	constexpr bool __view_predicate<T> = false;

	template <class T>
	constexpr bool __view_predicate<std::initializer_list<T>> = false;
	template <class Key, class Compare, class Alloc>
	constexpr bool __view_predicate<std::set<Key, Compare, Alloc>> = false;
	template <class Key, class Compare, class Alloc>
	constexpr bool __view_predicate<std::multiset<Key, Compare, Alloc>> = false;
	template <class Key, class Hash, class Pred, class Alloc>
	constexpr bool __view_predicate<std::unordered_set<Key, Hash, Pred, Alloc>> = false;
	template <class Key, class Hash, class Pred, class Alloc>
	constexpr bool __view_predicate<std::unordered_multiset<Key, Hash, Pred, Alloc>> = false;

	template <class T>
	concept bool View =
		Range<T> &&
		__view_predicate<T> &&
		Semiregular<T>;

	namespace models {
		template <class>
		constexpr bool View = false;
		__stl2::View{V}
		constexpr bool View<V> = true;
	}

	///////////////////////////////////////////////////////////////////////////
	// BoundedRange [ranges.bounded]
	//
	template <class T>
	concept bool BoundedRange =
		Range<T> && Same<iterator_t<T>, sentinel_t<T>>;

	namespace models {
		template <class>
		constexpr bool BoundedRange = false;
		__stl2::BoundedRange{R}
		constexpr bool BoundedRange<R> = true;
	}

	///////////////////////////////////////////////////////////////////////////
	// OutputRange [ranges.output]
	// Not to spec: value category sensitive.
	//
	template <class R, class T>
	concept bool OutputRange =
		Range<R> && OutputIterator<iterator_t<R>, T>;

	namespace models {
		template <class, class>
		constexpr bool OutputRange = false;
		__stl2::OutputRange{R, T}
		constexpr bool OutputRange<R, T> = true;
	}

	///////////////////////////////////////////////////////////////////////////
	// InputRange [ranges.input]
	//
	template <class T>
	concept bool InputRange =
		Range<T> && InputIterator<iterator_t<T>>;

	namespace models {
		template <class>
		constexpr bool InputRange = false;
		__stl2::InputRange{R}
		constexpr bool InputRange<R> = true;
	}

	///////////////////////////////////////////////////////////////////////////
	// ForwardRange [ranges.forward]
	//
	template <class T>
	concept bool ForwardRange =
		Range<T> && ForwardIterator<iterator_t<T>>;

	namespace models {
		template <class>
		constexpr bool ForwardRange = false;
		__stl2::ForwardRange{R}
		constexpr bool ForwardRange<R> = true;
	}

	///////////////////////////////////////////////////////////////////////////
	// BidirectionalRange [ranges.bidirectional]
	//
	template <class T>
	concept bool BidirectionalRange =
		Range<T> && BidirectionalIterator<iterator_t<T>>;

	namespace models {
		template <class>
		constexpr bool BidirectionalRange = false;
		__stl2::BidirectionalRange{R}
		constexpr bool BidirectionalRange<R> = true;
	}

	///////////////////////////////////////////////////////////////////////////
	// RandomAccessRange [ranges.random.access]
	//
	template <class T>
	concept bool RandomAccessRange =
		Range<T> && RandomAccessIterator<iterator_t<T>>;

	namespace models {
		template <class>
		constexpr bool RandomAccessRange = false;
		__stl2::RandomAccessRange{R}
		constexpr bool RandomAccessRange<R> = true;
	}

	namespace ext {
		template <class R>
		concept bool ContiguousRange =
			SizedRange<R> &&
			ContiguousIterator<iterator_t<R>> &&
			requires(R& r) {
				{ __stl2::data(r) } -> Same<add_pointer_t<reference_t<iterator_t<R>>>>&&;
			};
	}

	namespace models {
		template <class>
		constexpr bool ContiguousRange = false;
		__stl2::ext::ContiguousRange{R}
		constexpr bool ContiguousRange<R> = true;
	}
} STL2_CLOSE_NAMESPACE

#endif
