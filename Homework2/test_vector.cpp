#include "student_vector.h"
#include "student_vector.h"

#include <iostream>


int main() {
    using int_vec = student_std::vector<int>;
    
    int_vec vec_default;
    int_vec vec_default_copy(vec_default);
    int_vec vec_default_assigned = vec_default;

    double score = 0.;

    bool defaults_empty = true;
    if(vec_default.size() != 0 || !vec_default.empty()) {
        std::cerr << "default constructed vector is not empty.\n";
        defaults_empty = false;
    }
    if(vec_default_copy.size() != 0 || !vec_default_copy.empty()) {
        std::cerr << "copy of default constructed vector is not empty.\n";
        defaults_empty = false;
    }
    if(vec_default_assigned.size() != 0 || !vec_default_assigned.empty()) {
        std::cerr << "copy assigned vector of default constructed vector is not empty.\n";
        defaults_empty = false;
    }
    if(defaults_empty)
        score += 0.5;
    std::cout << "default constructor, copy of empty, assigned from empty:\t\t" << score << " / 0.5\n";

    int_vec five_entries;
    five_entries.push_back(0);
    five_entries.push_back(1);
    five_entries.push_back(2);
    five_entries.push_back(3);
    five_entries.push_back(4);
    int_vec five_entries_copy(five_entries);
    int_vec five_entries_assigned = five_entries;

    bool five_size_cap = true;
    if(five_entries.size() != 5 || five_entries.capacity() < 5)
    {
        std::cerr << "After 5 push_backs size() != 5 or capacity() < 5.\n";
        five_size_cap = false;
    }
    if(five_entries_assigned.size() != 5 || five_entries_assigned.capacity() < 5)
    {
        std::cerr << "After 5 push_backs and assigning, size() != 5 or capacity() < 5.\n";
        five_size_cap = false;
    }
    if(five_entries.capacity() > 100) {
        std::cerr << "Capacity after five push_backs > 100 (unreasonable, do not try to get around dynamic realloc).\n";
        five_size_cap = false;
    }
    double five_entries_score = five_size_cap ? 0.5 : 0.;
    score += five_entries_score;
    std::cout << "Size and capacity after five push_backs, copying, and assigning:\t" << five_entries_score << " / 0.5\n";

    bool true_copy_reserve = true;
    if(five_entries.data() == five_entries_copy.data() || five_entries.data() == five_entries_assigned.data()) {
        std::cerr << "Memory of vector and copy or vector and copy assigned equal, no true copy.\n";
        true_copy_reserve = false;
    }
    int_vec vec_reserve;
    int_vec vec_no_reserve;
    vec_reserve.reserve(101);
    if(vec_reserve.capacity() < 101) {
        std::cerr << "After reserve(n) on empty vector, capacity < n.\n";
        true_copy_reserve = false;
    }
    vec_reserve.reserve(50);
    if(vec_reserve.capacity() < 101) {
        std::cerr << "After reserve(n), reserve(m) with m < n on empty vector, capacity < n (smaller reserves should not reduce capacity).\n";
        true_copy_reserve = false;
    }
    if(vec_reserve.size() != 0) {
        std::cerr << "After reserve(n) on empty vector, size changed.\n";
        true_copy_reserve = false;
    }
    int* old_loc = vec_reserve.data();
    for(int i = 0; i < 101; ++i) {
        vec_reserve.push_back(i);
        vec_no_reserve.push_back(i);
    }
    int* new_loc = vec_reserve.data();
    if(old_loc != new_loc) {
        std::cerr << "After reserve(n) and n times push_back, the location changed, even though enough should have been reserved.\n";
        true_copy_reserve = false;
    }
    if(vec_reserve.size() != 101 || vec_no_reserve.size() != 101) {
        std::cerr << "Incorrect size after reserve/no reserve and 101 push_backs.\n";
        true_copy_reserve = false;
    }
    double true_copy_score = true_copy_reserve ? 0.5 : 0;
    score += true_copy_score;
    std::cout << "True copy, correct reserve handling:\t\t\t\t\t" << true_copy_score << " / 0.5\n";

    bool pop_back_milestones = true;
    five_entries.pop_back();
    five_entries.pop_back();
    five_entries.pop_back();
    if(five_entries.size() != 2 || five_entries.empty()) {
        std::cerr << "size() != 2 or empty() == true after 5 push_backs and 3 pop_backs.\n";
        pop_back_milestones = false;
    }
    five_entries_copy.clear();
    if(five_entries_copy.size() != 0 || !five_entries_copy.empty())
    {
        std::cerr << "size() != 0 or empty() != true after clear().\n";
        pop_back_milestones = false;
    }
    double pop_back_score = pop_back_milestones ? 0.5 : 0.0;
    score += pop_back_score;
    std::cout << "Expected sizes after pop_back()/clear:\t\t\t\t\t" << pop_back_score << " / 0.5\n";

    bool expected_values = true;
    if(five_entries.front() != 0 || five_entries.back() != 1) {
        std::cerr << "front or back returned the wrong value.\n";
        expected_values = false;
    }
    try{
        five_entries.at(2);
        std::cerr << "No exception thrown for out of range at.\n";
        expected_values = false;
    }
    catch(std::out_of_range& e) {}
    catch(...) {
        std::cerr << "Wrong exception type thrown for out of range at.\n";
        expected_values = false;
    }
    if(five_entries_assigned[0] != 0 || five_entries_assigned[4] != 4
       || vec_reserve[0] != 0 || vec_reserve[100] != 100
       || vec_reserve.data()[0] != 0 || vec_reserve.data()[100] != 100) {
        std::cerr << "at() or operator[] or data()[] returned unexpected value.\n";
        expected_values = false;
    }
    five_entries[0] = 100;
    five_entries.at(1) = 101;
    if(five_entries.front() != 100 || five_entries.back() != 101) {
        std::cerr << "front() or back() produces unexpected value after assign.\n";
        expected_values = false;
    }
    double expected_values_score = expected_values ? 0.5 : 0.0;
    std::cout << "Expected values after push_backs, pop_backs and assignments:\t\t" << expected_values_score << " / 0.5\n";
    score += expected_values_score;

    std::cout << "\ntest_vector.cpp subtotal:\t\t\t\t\t\t" << score << " / 2.5\n";

    return 0;
}
