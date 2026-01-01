#include "student_vector.h"
#include "student_vector.h"

#include <iostream>

namespace {
    struct foo {
        bool test() { return v == 1; }
        int v = 0;
    };
}

int main() {
    using int_vec = student_std::vector<int>;
    
    int_vec vec_10;
    for(int i = 0; i < 10; ++i)
        vec_10.push_back(i);
    double score = 0.;

    bool begin_end = true;
    if(*vec_10.begin() != 0 || *(vec_10.end() - 1) != 9) {
        std::cerr << "*begin() and *(end-1) return unexpected values.\n";
        begin_end = false;
    }
    score = begin_end ? 0.5 : 0.0;
    std::cout << "begin(), end():\t\t\t\t\t" << score << " / 0.5\n";

    bool ranged_for = true;
    int i = 0;
    for (const auto& entry : vec_10) {
        if (i++ != entry) {
            std::cerr << "unexpected value pointed at by iterator in ranged for loop (uses begin(), end(), iterator != end(), *iterator, and iterator++).\n";
            ranged_for = false;
        }
    }
    if(i != 10){
        std::cerr << "Ranged for loop (uses begin(), end(), iterator++) terminated early or late.\n";
        ranged_for = false;
    }
    i = 0;
    for (auto& entry : vec_10)
        entry *= 2;
    for (const auto& entry : vec_10) {
        if ((i++) * 2 != entry) {
            std::cerr << "unexpected value pointed at by iterator in ranged for loop (uses begin(), end(), iterator != end(), *iterator, and iterator++) after manipulation.\n";
            ranged_for = false;
        }
    }
    double score_ranged_for = ranged_for ? 0.5 : 0.0;
    std::cout << "ranged-for:\t\t\t\t\t" << score_ranged_for << " / 0.5\n";
    score += score_ranged_for;


    student_std::vector<foo> vec_foo;
    vec_foo.push_back(foo{1});
    vec_foo.push_back(foo{});
    vec_foo.push_back(foo{});

    bool deref = true;
    if( !vec_foo.begin()->test() ) {
        std::cerr << "iterator-> returned unexpected result.\n";
        deref = false;
    }
    double score_deref = deref ? 0.5 : 0.0;
    std::cout << "iterator->:\t\t\t\t\t" << score_deref << " / 0.5\n";
    score += score_deref;


    bool comparison = true;
    if(    !(vec_10.begin() < vec_10.begin() + 1) || !(vec_10.begin() < vec_10.end())
        || !( vec_10.begin() <= vec_10.begin()) || !(vec_10.begin() <= vec_10.begin() + 1)
        || !( vec_10.begin() <= vec_10.end())) {
        std::cerr << "operator< or operator<= produced unexpected results.\n";
        comparison = false;
    }
    if(    !(vec_10.begin() + 10 == vec_10.end()) || !(vec_10.begin() != vec_10.end())
        || !(vec_10.begin() == vec_10.begin()) || (vec_10.end() != vec_10.end()) ) {
        std::cerr << "operator== or operator!= produced unexpected results.\n";
        comparison = false;
    }
    auto it1 = vec_10.begin() + 2;
    auto it2 = vec_10.begin() + 4;
    auto it3 = it2;
    auto it4 = it1;
    it3 -= 2;
    it4 += 2;
    if( it2 - it1 != 2 || it1 - it2 != -2 || it2 - 2 != it1 || it3 != it1 || it4 != it2) {
        std::cerr << "operator-, operator+, operator-=, or operator+= produced unexpected results.\n";
        comparison = false;
    }
    double comparison_score = comparison ? 0.5 : 0.0;
    score += comparison_score;
    std::cout << "Comparisons:\t\t\t\t\t" << comparison_score << " / 0.5\n";

    bool insert_erase = true;
    auto it10 = vec_10.insert(vec_10.end(), 10);
    if(*it10 != 10) {
        std::cerr << "Incorrect iterator returned by insert.\n";
        insert_erase = false;
    }
    auto itm1 = vec_10.insert(vec_10.begin(), -1);
    if(*itm1 != -1) {
        std::cerr << "Incorrect iterator returned by insert.\n";
        insert_erase = false;
    }
    auto it1_ = vec_10.erase(vec_10.begin() + 1);
    if(*it1_ != 2) {
        std::cerr << "Incorrect iterator returned by erase, should point to 2.\n";
        insert_erase = false;
    }
    auto it10_ = vec_10.erase(vec_10.begin() + 1, vec_10.end() - 1);
    if(*it10_ != 10) {
        std::cerr << "Incorrect iterator returned by erase, should point to 10.\n";
        insert_erase = false;
    }
    if(vec_10.front() != -1 || vec_10.back() != 10 || vec_10.size() != 2) {
        std::cerr << "State incorrect after insert and erase.\n";
        insert_erase = false;
    }
    auto end_ = vec_10.erase(vec_10.begin(), vec_10.end());
    if( !vec_10.empty() || end_ != vec_10.end() ) {
        std::cerr << "State incorrect after erase(begin(), end()), or incorrect iterator returned (should return end()).\n";
        insert_erase = false;
    }
    auto it17 = vec_10.insert(vec_10.begin(), 17);
    if(vec_10.size() != 1 || vec_10.front() != 17 || vec_10.back() != 17 || *it17 != 17) {
        std::cerr << "State incorrect after insert(begin()) in empty vector.\n";
        insert_erase = false;
    }
    double insert_erase_score = insert_erase ? 0.5 : 0.0;
    std::cout << "Insert/Erase:\t\t\t\t\t" << insert_erase_score << " / 0.5\n\n";
    score += insert_erase_score;
    std::cout << "test_iterator.cpp subtotal:\t\t\t" << score << " / 2.5\n";



    return 0;
}
