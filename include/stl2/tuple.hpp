#ifndef STL2_TUPLE_HPP
#define STL2_TUPLE_HPP

#include <tuple>
#include <meta/meta.hpp>
#include <stl2/detail/fwd.hpp>
#include <stl2/detail/tagged.hpp>

namespace stl2 { inline namespace v1 {
using std::tuple;
using std::make_tuple;
using std::forward_as_tuple;
using std::tie;
using std::ignore;
using std::tuple_cat;
using std::tuple_size;
using std::tuple_element;
using std::get;
using std::uses_allocator;

// tagged_tuple
template <TaggedType...Types>
using tagged_tuple =
  tagged<tuple<__tag_elem<Types>...>, __tag_spec<Types>...>;

// make_tagged_tuple
template<TagSpecifier...Tags, class...Types,
  class Tuple = tagged_tuple<Tags(__unwrap_t<Types>)...>>
constexpr Tuple
make_tagged_tuple(Types&&...ts) {
  return Tuple{stl2::forward<Types>(ts)...};
}
}} // namespace stl2::v1

#endif