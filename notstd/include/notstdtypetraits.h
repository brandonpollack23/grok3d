
/**@file*/

#ifndef __NOTSTD_TYPETRAITS__
#define __NOTSTD_TYPETRAITS__

#include <type_traits>
#include <utility>

namespace notstd {
/** Meta Function that is used to expose a function that will allow us to use Template Meta
 * Programming to have a constant integral_type returned that tells us if we do or do not have a
 * member in a class/type (std::true_type if they are all accessable, std::false_type if they
 * aren't)
 *
 * this stuff is not easy, if you want a full understanding read this:
 * https://jguegant.github.io/blogs/tech/sfinae-introduction.html
 *
 * @tparam TemplatedFunctor a templated lambda which attempts to run or access the required members*/
template<class TemplatedFunctor>
struct MemberChecker {
 private:
  //sfinae will cause this specialization to fail and cascade to the next function def
  //if there is not a valid type returning from ClassUnderTest being passed to the templated functor
  //this happens if there is no such method specified by the TemplatedFunctor inside of the ClassUnderTest
  template<class... MembersUnderTest>
  constexpr auto testValidity(int /*unused*/) -> decltype(std::declval<TemplatedFunctor>()(std::declval<MembersUnderTest>()...), std::true_type()) {
    return std::true_type();
  }

  // ... is always lowest priority for overloads
  template<class... MembersUnderTest>
  constexpr auto testValidity(...) -> std::false_type {
    return std::false_type();
  }
 public:
  template<class... MembersUnderTest>
  constexpr auto operator()(MembersUnderTest &&...) {
    return testValidity<MembersUnderTest...>(int());
  }
};

/**This function takes in an auto lambda (templated functor) that tries to access all the
 * members you need of a type, and returns an integral_type if they are all usable, this is all
 * for compile time introspection*/
template<class TemplatedFunctor>
constexpr auto is_valid(const TemplatedFunctor &&t) {
  //inject the TemplatedFunctor into the member checker
  //the MemberChecker (a meta Functor on it's own) () operator will then attempt to call
  //the TemplatedFunctor's application on the ClassUnderTest (template param for the application of MemberChecker)
  //if sfinae occurs, then there is no member!  It will fall back to the ... parameter taking version, returning false_type
  return MemberChecker<TemplatedFunctor>();
};
}

#endif
