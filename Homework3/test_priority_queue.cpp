#if !(__has_include("student_priority_queue.h"))
#include <iostream>

int main() {
    std::cout << "student_priority_queue.h not found. Score: 0.0 / 3.5\n";
    return 0;
}

#else
#include "student_priority_queue.h"
#include "student_priority_queue.h"

#include <cstddef>
#include <iomanip>
#include <iostream>
#include <type_traits>
#include <vector>

template <typename T, typename Cont>
using student_priority_queue = student_std::priority_queue<T, Cont>;

template <typename, typename, typename = std::void_t<>>
struct container_type_is_u : std::false_type {};

template <typename T, typename U>
struct container_type_is_u<T, U, std::void_t<typename T::size_type>>
  : std::is_same<typename T::container_type, U> {};

template <typename T, typename U>
constexpr bool container_type_is_u_v = container_type_is_u<T, U>::value;

template <typename, typename = std::void_t<>>
struct size_type_is_size_t : std::false_type {};

template <typename T>
struct size_type_is_size_t<T, std::void_t<typename T::size_type>>
  : std::is_same<typename T::size_type, std::size_t> {};

template <typename T>
constexpr bool size_type_is_size_t_v = size_type_is_size_t<T>::value;

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
struct empty_returns_bool : std::false_type {};

template<class T>
struct empty_returns_bool<T,
    std::void_t<decltype(std::declval<const T&>().empty())>>
  : std::is_same<decltype(std::declval<const T&>().empty()), bool> {};

template<class T>
inline constexpr bool empty_returns_bool_v = empty_returns_bool<T>::value;

template<class T, class = void>
struct push_returns_void : std::false_type {};

template<class T>
struct push_returns_void<T,
    std::void_t<decltype(std::declval<T&>().push(std::declval<const typename T::value_type&>()))>>
  : std::is_same<decltype(std::declval<T&>().push(std::declval<const typename T::value_type&>())), void> {};

template<class T>
inline constexpr bool push_returns_void_v = push_returns_void<T>::value;

template<class T, class = void>
struct pop_returns_void : std::false_type {};

template<class T>
struct pop_returns_void<T,
    std::void_t<decltype(std::declval<T&>().pop())>>
  : std::is_same<decltype(std::declval<T&>().pop()), void> {};

template<class T>
inline constexpr bool pop_returns_void_v = pop_returns_void<T>::value;

template<class T, class = void>
struct swap_returns_void : std::false_type {};

template<class T>
struct swap_returns_void<T,
    std::void_t<decltype(std::declval<T&>().swap(std::declval<T&>()))>>
  : std::is_same<decltype(std::declval<T&>().swap(std::declval<T&>())),void> {};

template<class T>
inline constexpr bool swap_returns_void_v = swap_returns_void<T>::value;

template<class T, class = void>
struct top_returns_tref : std::false_type {};

template<class T>
struct top_returns_tref<T,
    std::void_t<decltype(std::declval<const T&>().top())>>
  : std::integral_constant<bool, 
        std::is_same<decltype(std::declval<const T&>().top()), const typename T::value_type&>::value> {};

template<class T>
inline constexpr bool top_returns_tref_v = top_returns_tref<T>::value;

namespace {

struct test_type {
    int v = 0;
    bool operator<(test_type const& o) const { return v < o.v; }
};

int count = 0;
struct copy_counter {
    copy_counter(copy_counter const& o) : m_val(o.m_val) { ++count; }
    copy_counter(int v) : m_val(v) {};
    copy_counter(copy_counter&& o) { m_val = o.m_val; };
    copy_counter& operator=(copy_counter const& o) { m_val = o.m_val; ++count; return *this; }
    ~copy_counter() = default;
    bool operator<(copy_counter const& o) const { return m_val < o.m_val; }
    int val() const { return m_val; }
private:
    int m_val = 0;
};

}

const char* check(bool b) {
    return b ? "SUCCESS" : "FAIL";
}

template <template<typename, typename> typename T, typename Cont>
double priority_queue_test() {
    double score = 0.0;
    using int_pq = T<int, Cont>;

    int_pq pq_default;
    int_pq pq_default_copy(pq_default);
    int_pq pq_default_assigned = pq_default;

    bool defaults_empty = true;

    std::cout << "Priority Queue functionality test:\n";
    if(pq_default.size() != 0 || !pq_default.empty()) {
        std::cerr << "default constructed PQ is not empty.\n";
        defaults_empty = false;
    }
    if(pq_default_copy.size() != 0 || !pq_default_copy.empty()) {
        std::cerr << "copy of default constructed pq is not empty.\n";
        defaults_empty = false;
    }
    if(pq_default_assigned.size() != 0 || !pq_default_assigned.empty()) {
        std::cerr << "copy assigned pq of default constructed pq is not empty.\n";
        defaults_empty = false;
    }
    if(defaults_empty)
        score += 0.5;
    std::cout << "default constructor, copy of empty, assigned from empty:\t" << score << " / 0.5\n";

    int_pq five_entries;
    five_entries.push(4);
    five_entries.push(1);
    five_entries.push(3);
    five_entries.push(2);
    five_entries.push(0);
    int_pq five_entries_copy(five_entries);
    int_pq five_entries_assigned = five_entries;

    bool five_size_cap = true;
    if(five_entries.size() != 5)
    {
        std::cerr << "After 5 push_backs size() != 5.\n";
        five_size_cap = false;
    }
    if(five_entries_assigned.size() != 5)
    {
        std::cerr << "After 5 push_backs and assigning, size() != 5.\n";
        five_size_cap = false;
    }
    double five_entries_score = five_size_cap ? 0.5 : 0.;
    score += five_entries_score;
    std::cout << "Size after five push_backs, copying, and assigning:\t\t" << five_entries_score << " / 0.5\n";

    bool pop_back_milestones = true;
    bool expected_values = five_entries.top() == 4;
    five_entries.pop();
    expected_values = expected_values && five_entries.top() == 3;
    five_entries.pop();
    expected_values = expected_values && five_entries.top() == 2;
    five_entries.pop();
    expected_values = expected_values && five_entries.top() == 1;
    if(five_entries.size() != 2 || five_entries.empty()) {
        std::cerr << "size() != 2 or empty() == true after 5 pushes and 3 pops.\n";
        pop_back_milestones = false;
    }
    double pop_back_score = pop_back_milestones ? 0.5 : 0.0;
    score += pop_back_score;
    std::cout << "Expected sizes after pop():\t\t\t\t\t" << pop_back_score << " / 0.5\n";

    if(!expected_values)
        std::cerr << "top returned the wrong value.\n";
    double expected_values_score = expected_values ? 0.5 : 0.0;
    std::cout << "Expected values after push, pop and assignments:\t\t" << expected_values_score << " / 0.5\n\n";
    score += expected_values_score;

    T<copy_counter, std::list<copy_counter>> swap1, swap2;
    swap1.push(copy_counter(2));
    swap1.push(copy_counter(3));
    swap1.push(copy_counter(1));
    swap2.push(copy_counter(5));
    swap2.push(copy_counter(6));
    swap2.push(copy_counter(4));
    int count_before = count;
    swap1.swap(swap2);
    int count_after = count;
    bool swap_correct = swap1.top().val() == 6 && swap2.top().val() == 3 && count_before == count_after;
    if(!swap_correct)
        std::cerr << "swap() did not correctly swap contents or created copies.\n";
    double swap_score = swap_correct ? 0.5 : 0.0;
    std::cout << "Swap swapped contents without copying:\t\t\t\t" << swap_score << " / 0.5\n\n";
    score += swap_score;
    return score;
}

int main() {
    using test_pq = student_priority_queue
        <
            test_type,
            std::vector<test_type>
        >;
    double score = 0.0;

    constexpr bool interface_container_type = container_type_is_u_v<test_pq, std::vector<test_type>>;
    constexpr bool interface_size_type = size_type_is_size_t_v<test_pq>;
    constexpr bool interface_value_type = value_is_u_v<test_pq, test_type>;
    constexpr bool interface_default_ctor = std::is_default_constructible_v<test_pq>;
    constexpr bool interface_copy_ctor = std::is_copy_constructible_v<test_pq>;
    constexpr bool interface_assignable = std::is_copy_assignable_v<test_pq>;
    
    constexpr bool interface_size = size_returns_size_t_v<test_pq>;
    constexpr bool interface_empty = empty_returns_bool_v<test_pq>;
    
    constexpr bool interface_part1 =
        interface_container_type && interface_size_type && interface_value_type &&
        interface_default_ctor && interface_copy_ctor && 
        interface_assignable && interface_size && interface_empty;

    double score_interface_part1 = interface_part1 ? 0.5 : 0.0;
    score += score_interface_part1;
    std::cout << std::fixed << std::setprecision(1) << "Priority Queue Interface Part 1:\n"
        << "::container_type is Container: " << check(interface_container_type) << "\n"
        << "::size_type is size_t: " << check(interface_size_type) << "\n"
        << "::value_type is T: " << check(interface_value_type) << "\n"
        << "default constructible: " << check(interface_default_ctor) << "\n"
        << "copy constructible: " << check(interface_copy_ctor) << "\n"
        << "assignable: " << check(interface_assignable) << "\n"
        << "size() returns size_t: " << check(interface_size) << "\n"
        << "empty() returns bool: " << check(interface_empty) << "\n"
        << "Interface Part 1 Score:\t\t\t\t\t\t" << score_interface_part1 << " / 0.5\n\n";
    
    constexpr bool interface_push = push_returns_void_v<test_pq>;
    constexpr bool interface_pop = pop_returns_void_v<test_pq>;
    constexpr bool interface_swap = swap_returns_void_v<test_pq>;
    constexpr bool interface_top = top_returns_tref_v<test_pq>;

    constexpr bool interface_part2 =
        interface_push && interface_pop && interface_swap &&
        interface_top;

    double score_interface_part2 = interface_part2 ? 0.5 : 0.0;
    score += score_interface_part2;
    std::cout << "Priority Queue Interface Part 2:\n"
        << "push() returns void: " << check(interface_push) << "\n"
        << "pop() returns void: " << check(interface_pop) << "\n"
        << "top() returns const T&: " << check(interface_top) << "\n"
        << "swap() returns void: " << check(interface_swap) << "\n"
        << "Interface Part 2 Score:\t\t\t\t\t\t" << score_interface_part2 << " / 0.5\n\n";
    
    if constexpr(interface_part1 && interface_part2) {
        score += priority_queue_test<student_priority_queue, std::list<int>>();
    }
    else {
        std::cout << "Priority Queue functionality test not run due to interface failure: 0 / 2.5\n\n";
    }
    std::cout << "-------------------------------------------------------------------------\n";
    std::cout << "Problem 2 Subtotal:\t\t\t\t\t\t" << score << " / 3.5\n";
    std::cout << "=========================================================================\n\n";

    return 0;
}



#endif
