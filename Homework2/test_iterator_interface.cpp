#include "student_vector.h"
#include "student_vector.h"

#include <cstddef>
#include <iterator>
#include <type_traits>


template <typename T>
using student_vector = student_std::vector<T>;

template <typename, typename = std::void_t<>>
struct has_iterator_type : std::false_type {};

template <typename T>
struct has_iterator_type<T, std::void_t<typename T::iterator>> : std::true_type {};

template <typename T>
constexpr bool has_iterator_type_v = has_iterator_type<T>::value;

template <typename, typename = std::void_t<>>
struct iterator_difference_type_is_ptrdiff : std::false_type {};

template <typename T>
struct iterator_difference_type_is_ptrdiff<T, std::void_t<typename T::iterator::difference_type>>
  : std::is_same<typename T::iterator::difference_type, std::ptrdiff_t> {};

template <typename T>
constexpr bool iterator_difference_type_is_ptrdiff_v = iterator_difference_type_is_ptrdiff<T>::value;

template<typename T, typename U, class = void>
struct iterator_value_is_u : std::false_type {};

template<typename T, typename U>
struct iterator_value_is_u<T, U, std::void_t<typename T::iterator::value_type>>
  : std::is_same<typename T::iterator::value_type, U> {};

template<typename T, typename U>
inline constexpr bool iterator_value_is_u_v = iterator_value_is_u<T, U>::value;

template<typename T, typename U, class = void>
struct iterator_pointer_is_uptr : std::false_type {};

template<typename T, typename U>
struct iterator_pointer_is_uptr<T, U, std::void_t<typename T::iterator::pointer>>
  : std::is_same<typename T::iterator::pointer, U*> {};

template<typename T, typename U>
inline constexpr bool iterator_pointer_is_uptr_v = iterator_pointer_is_uptr<T, U>::value;

template<typename T, typename U, class = void>
struct iterator_reference_is_uref : std::false_type {};

template<typename T, typename U>
struct iterator_reference_is_uref<T, U, std::void_t<typename T::iterator::reference>>
  : std::is_same<typename T::iterator::reference, U&> {};

template<typename T, typename U>
inline constexpr bool iterator_reference_is_uref_v = iterator_reference_is_uref<T, U>::value;

template<typename T, class = void>
struct iterator_cat_is_random_access : std::false_type {};

template<typename T>
struct iterator_cat_is_random_access<T, std::void_t<typename T::iterator::iterator_category>>
  : std::is_same<typename T::iterator::iterator_category, std::random_access_iterator_tag> {};

template<typename T>
inline constexpr bool iterator_cat_is_random_access_v = iterator_cat_is_random_access<T>::value;

template<typename T, class = void>
struct iterator_is_default_constructible : std::false_type {};

template<typename T>
struct iterator_is_default_constructible<T, std::void_t<typename T::iterator>>
  : std::is_default_constructible<typename T::iterator> {};

template<typename T>
inline constexpr bool iterator_is_default_constructible_v = iterator_is_default_constructible<T>::value;

template<typename T, typename U, class = void>
struct iterator_is_constructible_from_uptr : std::false_type {};

template<typename T, typename U>
struct iterator_is_constructible_from_uptr<T, U, std::void_t<typename T::iterator>>
  : std::is_constructible<typename T::iterator, U*> {};

template<typename T, typename U>
inline constexpr bool iterator_is_constructible_from_uptr_v = iterator_is_constructible_from_uptr<T, U>::value;

template<typename T, typename = void>
struct iterator_is_copy_constructible : std::false_type {};

template<typename T>
struct iterator_is_copy_constructible<T, std::void_t<typename T::iterator>>
  : std::is_copy_constructible<typename T::iterator> {};

template<typename T>
inline constexpr bool iterator_is_copy_constructible_v = iterator_is_copy_constructible<T>::value;

template<typename T, class = void>
struct iterator_is_copy_assignable : std::false_type {};

template<typename T>
struct iterator_is_copy_assignable<T, std::void_t<typename T::iterator>>
  : std::is_copy_assignable<typename T::iterator> {};

template<typename T>
inline constexpr bool iterator_is_copy_assignable_v = iterator_is_copy_assignable<T>::value;

template<typename T, class = void>
struct iterator_preincrement_returns_ref : std::false_type {};

template<typename T>
struct iterator_preincrement_returns_ref<T, std::void_t<decltype(++std::declval<typename T::iterator&>())>>
  : std::is_same<decltype(++std::declval<typename T::iterator&>()), typename T::iterator&> {};

template<typename T>
inline constexpr bool iterator_preincrement_returns_ref_v = iterator_preincrement_returns_ref<T>::value;

template<typename T, class = void>
struct iterator_predecrement_returns_ref : std::false_type {};

template<typename T>
struct iterator_predecrement_returns_ref<T, std::void_t<decltype(--std::declval<typename T::iterator&>())>>
  : std::is_same<decltype(--std::declval<typename T::iterator&>()), typename T::iterator&> {};

template<typename T>
inline constexpr bool iterator_predecrement_returns_ref_v = iterator_predecrement_returns_ref<T>::value;

template<typename T, class = void>
struct iterator_postincrement_returns_it : std::false_type {};

template<typename T>
struct iterator_postincrement_returns_it<T, std::void_t<decltype(std::declval<typename T::iterator&>()++)>>
  : std::is_same<decltype(std::declval<typename T::iterator&>()++), typename T::iterator> {};

template<typename T>
inline constexpr bool iterator_postincrement_returns_it_v = iterator_postincrement_returns_it<T>::value;

template<typename T, class = void>
struct iterator_postdecrement_returns_it : std::false_type {};

template<typename T>
struct iterator_postdecrement_returns_it<T, std::void_t<decltype(std::declval<typename T::iterator&>()--)>>
  : std::is_same<decltype(std::declval<typename T::iterator&>()--), typename T::iterator> {};

template<typename T>
inline constexpr bool iterator_postdecrement_returns_it_v = iterator_postdecrement_returns_it<T>::value;

template<typename T, typename U, class = void>
struct iterator_deref_returns_u : std::false_type {};

template<typename T, typename U>
struct iterator_deref_returns_u<T, U, std::void_t<decltype(*std::declval<const typename T::iterator&>())>>
  : std::is_same<decltype(*std::declval<const typename T::iterator&>()), U&> {};

template<typename T, typename U>
inline constexpr bool iterator_deref_returns_u_v = iterator_deref_returns_u<T, U>::value;

template<typename T, typename U, class = void>
struct iterator_memptr_returns_uptr : std::false_type {};

template<typename T, typename U>
struct iterator_memptr_returns_uptr<T, U, std::void_t<decltype(std::declval<const typename T::iterator&>().operator->())>>
  : std::is_same<decltype(std::declval<const typename T::iterator&>().operator->()), U*> {};

template<typename T, typename U>
inline constexpr bool iterator_memptr_returns_uptr_v = iterator_memptr_returns_uptr<T, U>::value;

template<typename T, class = void>
struct iterator_less_than_returns_bool : std::false_type {};

template<typename T>
struct iterator_less_than_returns_bool<T, std::void_t<decltype(std::declval<typename T::iterator>() < std::declval<typename T::iterator>())>>
  : std::is_same<decltype(std::declval<typename T::iterator>() < std::declval<typename T::iterator>()), bool> {};

template<typename T>
inline constexpr bool iterator_less_than_returns_bool_v = iterator_less_than_returns_bool<T>::value;

template<typename T, class = void>
struct iterator_lesseq_than_returns_bool : std::false_type {};

template<typename T>
struct iterator_lesseq_than_returns_bool<T, std::void_t<decltype(std::declval<typename T::iterator>() <= std::declval<typename T::iterator>())>>
  : std::is_same<decltype(std::declval<typename T::iterator>() <= std::declval<typename T::iterator>()), bool> {};

template<typename T>
inline constexpr bool iterator_lesseq_than_returns_bool_v = iterator_lesseq_than_returns_bool<T>::value;

template<typename T, class = void>
struct iterator_eq_returns_bool : std::false_type {};

template<typename T>
struct iterator_eq_returns_bool<T, std::void_t<decltype(std::declval<typename T::iterator>() == std::declval<typename T::iterator>())>>
  : std::is_same<decltype(std::declval<typename T::iterator>() == std::declval<typename T::iterator>()), bool> {};

template<typename T>
inline constexpr bool iterator_eq_returns_bool_v = iterator_eq_returns_bool<T>::value;

template<typename t, class = void>
struct iterator_ineq_returns_bool : std::false_type {};

template<typename t>
struct iterator_ineq_returns_bool<t, std::void_t<decltype(std::declval<typename t::iterator>() != std::declval<typename t::iterator>())>>
  : std::is_same<decltype(std::declval<typename t::iterator>() != std::declval<typename t::iterator>()), bool> {};

template<typename t>
inline constexpr bool iterator_ineq_returns_bool_v = iterator_ineq_returns_bool<t>::value;

template<typename T, class = void>
struct iterator_addassign_returns_itref : std::false_type {};

template<typename T>
struct iterator_addassign_returns_itref<T, std::void_t<decltype(std::declval<typename T::iterator>() += std::declval<typename T::iterator::difference_type>())>>
  : std::is_same<decltype(std::declval<typename T::iterator>() += std::declval<typename T::iterator::difference_type>()), typename T::iterator&> {};

template<typename T>
inline constexpr bool iterator_addassign_returns_itref_v = iterator_addassign_returns_itref<T>::value;

template<typename T, class = void>
struct iterator_subassign_returns_itref : std::false_type {};

template<typename T>
struct iterator_subassign_returns_itref<T, std::void_t<decltype(std::declval<typename T::iterator>() -= std::declval<typename T::iterator::difference_type>())>>
  : std::is_same<decltype(std::declval<typename T::iterator>() -= std::declval<typename T::iterator::difference_type>()), typename T::iterator&> {};

template<typename T>
inline constexpr bool iterator_subassign_returns_itref_v = iterator_subassign_returns_itref<T>::value;

template<typename T, class = void>
struct iterator_add_returns_it : std::false_type {};

template<typename T>
struct iterator_add_returns_it<T, std::void_t<decltype(std::declval<typename T::iterator>() + std::declval<typename T::iterator::difference_type>())>>
  : std::is_same<decltype(std::declval<typename T::iterator>() + std::declval<typename T::iterator::difference_type>()), typename T::iterator> {};

template<typename T>
inline constexpr bool iterator_add_returns_it_v = iterator_add_returns_it<T>::value;

template<typename T, class = void>
struct iterator_sub_returns_it : std::false_type {};

template<typename T>
struct iterator_sub_returns_it<T, std::void_t<decltype(std::declval<typename T::iterator>() - std::declval<typename T::iterator::difference_type>())>>
  : std::is_same<decltype(std::declval<typename T::iterator>() - std::declval<typename T::iterator::difference_type>()), typename T::iterator> {};

template<typename T>
inline constexpr bool iterator_sub_returns_it_v = iterator_sub_returns_it<T>::value;

template<typename T, class = void>
struct iterator_subit_returns_diff : std::false_type {};

template<typename T>
struct iterator_subit_returns_diff<T, std::void_t<decltype(std::declval<typename T::iterator>() - std::declval<typename T::iterator>())>>
  : std::is_same<decltype(std::declval<typename T::iterator>() - std::declval<typename T::iterator>()), typename T::iterator::difference_type> {};

template<typename T>
inline constexpr bool iterator_subit_returns_diff_v = iterator_subit_returns_diff<T>::value;

template<typename T, class = void>
struct begin_returns_it : std::false_type {};

template<typename T>
struct begin_returns_it<T, std::void_t<decltype(std::declval<T&>().begin()), typename T::iterator>>
  : std::is_same<decltype(std::declval<T&>().begin()), typename T::iterator> {};

template<typename T>
inline constexpr bool begin_returns_it_v = begin_returns_it<T>::value;

template<typename T, class = void>
struct end_returns_it : std::false_type {};

template<typename T>
struct end_returns_it<T, std::void_t<decltype(std::declval<T&>().end()), typename T::iterator>>
  : std::is_same<decltype(std::declval<T&>().end()), typename T::iterator> {};

template<typename T>
inline constexpr bool end_returns_it_v = end_returns_it<T>::value;

template<typename T, class = void>
struct erase1_returns_it : std::false_type {};

template<typename T>
struct erase1_returns_it<T, std::void_t<decltype(std::declval<T&>().erase(std::declval<typename T::iterator>()))>>
  : std::is_same<decltype(std::declval<T&>().erase(std::declval<typename T::iterator>())), typename T::iterator> {};

template<typename T, class = void>
struct erase2_returns_it : std::false_type {};

template<typename T>
struct erase2_returns_it<T, std::void_t<decltype(std::declval<T&>().erase(std::declval<typename T::iterator>(), std::declval<typename T::iterator>()))>>
  : std::is_same<decltype(std::declval<T&>().erase(std::declval<typename T::iterator>(), std::declval<typename T::iterator>())), typename T::iterator> {};

template<typename T>
inline constexpr bool erase_returns_it_v = erase1_returns_it<T>::value && erase2_returns_it<T>::value;

template<typename T, class = void>
struct insert_returns_it : std::false_type {};

template<typename T>
struct insert_returns_it<T, std::void_t<decltype(std::declval<T&>().insert(std::declval<typename T::iterator>(), std::declval<const typename T::value_type&>()))>>
  : std::is_same<decltype(std::declval<T&>().insert(std::declval<typename T::iterator>(), std::declval<const typename T::value_type&>())), typename T::iterator> {};

template<typename T>
inline constexpr bool insert_returns_it_v = insert_returns_it<T>::value;

namespace {

struct test_type {
    int v = 0;
};

}

template<typename T>
struct test_iterator_interface {
    using test_vec = T;
};

int main() {
    using test_vec = student_vector<test_type>;
    
    static_assert(has_iterator_type_v<test_vec>, "student_std::vector must have public iterator type. All following tests will fail.");
    
    static_assert(iterator_difference_type_is_ptrdiff_v<test_vec>,
                  "student_std::vector::iterator must have public alias difference_type = std::ptrdiff_v.");
    static_assert(iterator_value_is_u_v<test_vec, test_type>,
                  "student_std::vector<T>::iterator must have public alias value_type = T.");
    static_assert(iterator_pointer_is_uptr_v<test_vec, test_type>,
                  "student_std::vector<T>::iterator must have public alias pointer = T*.");
    static_assert(iterator_reference_is_uref_v<test_vec, test_type>,
                  "student_std::vector<T>::iterator must have public alias reference = T&.");
    static_assert(iterator_cat_is_random_access_v<test_vec>,
                  "student_std::vector<T>::iterator must have public alias iterator_category = std::random_access_iterator_tag.");
    
    static_assert(iterator_is_default_constructible_v<test_vec>,
                  "student_std::vector<T>::iterator must be default constructible.");
    static_assert(iterator_is_constructible_from_uptr_v<test_vec, test_type>,
                  "student_std::vector<T>::iterator must be constructible from T*.");
    static_assert(iterator_is_copy_constructible_v<test_vec>,
                  "student_std::vector<T>::iterator must be copy constructible.");
    static_assert(iterator_is_copy_assignable_v<test_vec>,
                  "student_std::vector<T>::iterator must be copy assignable.");
    static_assert(iterator_preincrement_returns_ref_v<test_vec>,
                  "++student_std::vector<T>::iterator must return student_std::vector<T>::iterator&.");
    static_assert(iterator_predecrement_returns_ref_v<test_vec>,
                  "--student_std::vector<T>::iterator must return student_std::vector<T>::iterator&.");
    static_assert(iterator_postincrement_returns_it_v<test_vec>,
                  "student_std::vector<T>::iterator++ must return student_std::vector<T>::iterator.");
    static_assert(iterator_postdecrement_returns_it_v<test_vec>,
                  "student_std::vector<T>::iterator-- must return student_std::vector<T>::iterator.");
    static_assert(iterator_deref_returns_u_v<test_vec, test_type>,
                  "deref of const student_std::vector<T>::iterator must return T&.");
    static_assert(iterator_memptr_returns_uptr_v<test_vec, test_type>,
                  "const student_std::vector<T>::iterator-> must return T*.");
    static_assert(iterator_less_than_returns_bool_v<test_vec>,
                  "student_std::vector<T>::iterator < student_std::vector<T>::iterator must return bool.");
    static_assert(iterator_lesseq_than_returns_bool_v<test_vec>,
                  "student_std::vector<T>::iterator <= student_std::vector<T>::iterator must return bool.");
    static_assert(iterator_eq_returns_bool_v<test_vec>,
                  "student_std::vector<T>::iterator == student_std::vector<T>::iterator must return bool.");
    static_assert(iterator_ineq_returns_bool_v<test_vec>,
                  "student_std::vector<T>::iterator != student_std::vector<T>::iterator must return bool.");
    static_assert(iterator_addassign_returns_itref_v<test_vec>,
                  "student_std::vector<T>::iterator += student_std::vector<T>::iterator::difference_type must return student_std::vector<T>::iterator&.");
    static_assert(iterator_subassign_returns_itref_v<test_vec>,
                  "student_std::vector<T>::iterator -= student_std::vector<T>::iterator::difference_type must return student_std::vector<T>::iterator&.");
    static_assert(iterator_add_returns_it_v<test_vec>,
                  "student_std::vector<T>::iterator + student_std::vector<T>::iterator::difference_type must return student_std::vector<T>::iterator.");
    static_assert(iterator_sub_returns_it_v<test_vec>,
                  "student_std::vector<T>::iterator - student_std::vector<T>::iterator::difference_type must return student_std::vector<T>::iterator.");
    static_assert(iterator_subit_returns_diff_v<test_vec>,
                  "student_std::vector<T>::iterator - student_std::vector<T>::iterator must return student_std::vector<T>::iterator::difference_type.");


    static_assert(begin_returns_it_v<test_vec>,
                  "student_std::vector<T>::begin() must return student_std::vector<T>::iterator.");
    static_assert(end_returns_it_v<test_vec>,
                  "student_std::vector<T>::end() must return student_std::vector<T>::iterator.");
    static_assert(erase_returns_it_v<test_vec>,
                  "student_std::vector<T>::erase(iterator) and ...::erase(iterator, iterator) must return student_std::vector<T>::iterator.");
    static_assert(insert_returns_it_v<test_vec>,
                  "student_std::vector<T>::insert(iterator, const T&) must return student_std::vector<T>::iterator.");
    return 0;
}
