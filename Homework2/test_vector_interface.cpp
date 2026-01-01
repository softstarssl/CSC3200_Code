#include "student_vector.h"
#include "student_vector.h"

#include <type_traits>

template <typename T>
using student_vector = student_std::vector<T>;

template <typename, typename = std::void_t<>>
struct size_type_is_size_t : std::false_type {};

template <typename T>
struct size_type_is_size_t<T, std::void_t<typename T::size_type>>
  : std::is_same<typename T::size_type, std::size_t> {};

template <typename T>
constexpr bool size_type_is_size_t_v = size_type_is_size_t<T>::value;

template <typename, typename = std::void_t<>>
struct difference_type_is_ptrdiff : std::false_type {};

template <typename T>
struct difference_type_is_ptrdiff<T, std::void_t<typename T::difference_type>>
  : std::is_same<typename T::difference_type, std::ptrdiff_t> {};

template <typename T>
constexpr bool difference_type_is_ptrdiff_v = difference_type_is_ptrdiff<T>::value;

template<typename T, typename U, class = void>
struct value_is_u : std::false_type {};

template<typename T, typename U>
struct value_is_u<T, U, std::void_t<typename T::value_type>>
  : std::is_same<typename T::value_type, U> {};

template<typename T, typename U>
inline constexpr bool value_is_u_v = value_is_u<T, U>::value;

template<class T, class = void>
struct size_returns_size_t : std::false_type {};

template<class T>
struct size_returns_size_t<T,
    std::void_t<decltype(std::declval<const T&>().size())>>
  : std::is_same<decltype(std::declval<const T&>().size()), std::size_t> {};

template<class T>
inline constexpr bool size_returns_size_t_v = size_returns_size_t<T>::value;

template<class T, class = void>
struct capacity_returns_size_t : std::false_type {};

template<class T>
struct capacity_returns_size_t<T,
    std::void_t<decltype(std::declval<const T&>().capacity())>>
  : std::is_same<decltype(std::declval<const T&>().capacity()), std::size_t> {};

template<class T>
inline constexpr bool capacity_returns_size_t_v = capacity_returns_size_t<T>::value;

template<class T, class = void>
struct empty_returns_bool : std::false_type {};

template<class T>
struct empty_returns_bool<T,
    std::void_t<decltype(std::declval<const T&>().empty())>>
  : std::is_same<decltype(std::declval<const T&>().empty()), bool> {};

template<class T>
inline constexpr bool empty_returns_bool_v = empty_returns_bool<T>::value;

template<class T, class = void>
struct push_back_returns_void : std::false_type {};

template<class T>
struct push_back_returns_void<T,
    std::void_t<decltype(std::declval<T&>().push_back(std::declval<const typename T::value_type&>()))>>
  : std::is_same<decltype(std::declval<T&>().push_back(std::declval<const typename T::value_type&>())), void> {};

template<class T>
inline constexpr bool push_back_returns_void_v = push_back_returns_void<T>::value;

template<class T, class = void>
struct pop_back_returns_void : std::false_type {};

template<class T>
struct pop_back_returns_void<T,
    std::void_t<decltype(std::declval<T&>().pop_back())>>
  : std::is_same<decltype(std::declval<T&>().pop_back()), void> {};

template<class T>
inline constexpr bool pop_back_returns_void_v = pop_back_returns_void<T>::value;

template<class T, class = void>
struct resize1_returns_void : std::false_type {};

template<class T>
struct resize1_returns_void<T,
    std::void_t<decltype(std::declval<T&>().resize(std::declval<std::size_t>()))>>
  : std::is_same<decltype(std::declval<T&>().resize(std::declval<std::size_t>())), void> {};

template<class T, class = void>
struct resize2_returns_void : std::false_type {};

template<class T>
struct resize2_returns_void<T,
    std::void_t<decltype(std::declval<T&>().resize(std::declval<std::size_t>(), std::declval<const typename T::value_type&>()))>>
  : std::is_same<decltype(std::declval<T&>().resize(std::declval<std::size_t>(), std::declval<const typename T::value_type&>())), void> {};

template<class T>
inline constexpr bool resize_returns_void_v = resize1_returns_void<T>::value && resize2_returns_void<T>::value;

template<class T, class = void>
struct reserve_returns_void : std::false_type {};

template<class T>
struct reserve_returns_void<T,
    std::void_t<decltype(std::declval<T&>().reserve(std::declval<std::size_t>()))>>
  : std::is_same<decltype(std::declval<T&>().reserve(std::declval<std::size_t>())), void> {};

template<class T>
inline constexpr bool reserve_returns_void_v = reserve_returns_void<T>::value;

template<class T, class = void>
struct swap_returns_void : std::false_type {};

template<class T>
struct swap_returns_void<T,
    std::void_t<decltype(std::declval<T&>().swap(std::declval<T&>()))>>
  : std::is_same<decltype(std::declval<T&>().swap(std::declval<T&>())),void> {};

template<class T>
inline constexpr bool swap_returns_void_v = swap_returns_void<T>::value;

template<class T, class = void>
struct clear_returns_void : std::false_type {};

template<class T>
struct clear_returns_void<T,
    std::void_t<decltype(std::declval<T&>().clear())>>
  : std::is_same<decltype(std::declval<T&>().clear()), void> {};

template<class T>
inline constexpr bool clear_returns_void_v = clear_returns_void<T>::value;

template<class T, class = void>
struct data_returns_tptr : std::false_type {};

template<class T>
struct data_returns_tptr<T,
    std::void_t<decltype(std::declval<const T&>().data())>>
  : std::integral_constant<bool, 
           std::is_same<decltype(std::declval<T&>().data()), typename T::value_type*>::value
        && std::is_same<decltype(std::declval<const T&>().data()), const typename T::value_type*>::value> {};

template<class T>
inline constexpr bool data_returns_tptr_v = data_returns_tptr<T>::value;

template<class T, class = void>
struct at_returns_tref : std::false_type {};

template<class T>
struct at_returns_tref<T,
    std::void_t<decltype(std::declval<const T&>().at(std::declval<std::size_t>()))>>
  : std::integral_constant<bool, 
           std::is_same<decltype(std::declval<T&>().at(std::declval<std::size_t>())), typename T::value_type&>::value
        && std::is_same<decltype(std::declval<const T&>().at(std::declval<std::size_t>())), const typename T::value_type&>::value> {};

template<class T>
inline constexpr bool at_returns_tref_v = at_returns_tref<T>::value;

template<class T, class = void>
struct subscript_returns_tref : std::false_type {};

template<class T>
struct subscript_returns_tref<T,
    std::void_t<decltype(std::declval<const T&>()[std::declval<std::size_t>()])>>
  : std::integral_constant<bool, 
           std::is_same<decltype(std::declval<T&>()[std::declval<std::size_t>()]), typename T::value_type&>::value
        && std::is_same<decltype(std::declval<const T&>()[std::declval<std::size_t>()]), const typename T::value_type&>::value> {};

template<class T>
inline constexpr bool subscript_returns_tref_v = subscript_returns_tref<T>::value;

template<class T, class = void>
struct front_returns_tref : std::false_type {};

template<class T>
struct front_returns_tref<T,
    std::void_t<decltype(std::declval<const T&>().front())>>
  : std::integral_constant<bool, 
           std::is_same<decltype(std::declval<T&>().front()), typename T::value_type&>::value
        && std::is_same<decltype(std::declval<const T&>().front()), const typename T::value_type&>::value> {};

template<class T>
inline constexpr bool front_returns_tref_v = front_returns_tref<T>::value;

template<class T, class = void>
struct back_returns_tref : std::false_type {};

template<class T>
struct back_returns_tref<T,
    std::void_t<decltype(std::declval<const T&>().back())>>
  : std::integral_constant<bool, 
           std::is_same<decltype(std::declval<T&>().back()), typename T::value_type&>::value
        && std::is_same<decltype(std::declval<const T&>().back()), const typename T::value_type&>::value> {};

template<class T>
inline constexpr bool back_returns_tref_v = back_returns_tref<T>::value;

namespace {

struct test_type {
    int v = 0;
};

}


int main() {
    using test_vec = student_vector<test_type>;
    static_assert(size_type_is_size_t_v<test_vec>,
                  "student_std::vector must have public alias size_type = std::size_t.");
    static_assert(difference_type_is_ptrdiff_v<test_vec>,
                  "student_std::vector must have public alias difference_type = std::ptrdiff_t.");
    static_assert(value_is_u_v<test_vec, test_type>,
                  "student_std::vector<T> must have public alias value_type = T.");
    static_assert(std::is_default_constructible_v<test_vec>,
                  "student_std::vector must have a default constructor.");
    static_assert(std::is_copy_constructible_v<test_vec>,
                  "student_std::vector must have a copy constructor.");
    static_assert(std::is_copy_assignable_v<test_vec>,
                  "student_std::vector must be copy assignable, i.e. operator=(const student_std::vector&).");

    static_assert(size_returns_size_t_v<test_vec>,
                  "const student_std::vector must have the public member function size_type size().");
    static_assert(capacity_returns_size_t_v<test_vec>,
                  "const student_std::vector must have the public member function size_type capacity().");
    static_assert(empty_returns_bool_v<test_vec>,
                  "const student_std::vector must have the public member function bool empty().");

    static_assert(push_back_returns_void_v<test_vec>,
                  "student_std::vector<T> must have the public member function void push_back(T const&).");
    static_assert(pop_back_returns_void_v<test_vec>,
                  "student_std::vector<T> must have the public member function void pop_back().");
    static_assert(resize_returns_void_v<test_vec>,
                  "student_std::vector<T> must have the public member functions void resize(std::size_t), void resize(std::size_t, const T&).");
    static_assert(reserve_returns_void_v<test_vec>,
                  "student_std::vector<T> must have the public member function void reserve(std::size_t).");
    
    static_assert(swap_returns_void_v<test_vec>,
                  "student_std::vector<T> must have the public member function void swap(student_std::vector&).");
    static_assert(clear_returns_void_v<test_vec>,
                  "student_std::vector<T> must have the public member function void clear().");
    static_assert(data_returns_tptr_v<test_vec>,
                  "(const) student_std::vector<T> must have the public member function (const) T* data().");
    static_assert(at_returns_tref_v<test_vec>,
                  "(const) student_std::vector<T> must have the public member function (const) T& at(size_t).");
    static_assert(subscript_returns_tref_v<test_vec>,
                  "(const) student_std::vector<T> must have the public operator (const) T& operator[](size_t).");
    static_assert(front_returns_tref_v<test_vec>,
                  "(const) student_std::vector<T> must have the public member function (const) T& front().");
    static_assert(back_returns_tref_v<test_vec>,
                  "(const) student_std::vector<T> must have the public member function (const) T& back().");
    return 0;
}
