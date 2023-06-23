//
// Created by matie on 23.06.2023.
//

#include <iostream>
#include <vector>
#include <random>


#ifndef INC_3_PARTITION_PROBLEM_PROBLEM_T_H
#define INC_3_PARTITION_PROBLEM_PROBLEM_T_H

namespace mhe {
    using problem_t = std::vector<int>;

    bool size_of_problem_is_divided_by_3();
    int sum_of_problem(problem_t problem);

    problem_t generate_random_problem(int number_of_subsets, int min_rd, int max_rd, std::mt19937 &rgen);

    problem_t random_value_modify(problem_t problem_set, int min_rd, int max_rd);

    problem_t random_shuffle_problem(problem_t problem, std::mt19937 &rgen);

    std::ostream &operator<<(std::ostream &o, const problem_t v);

    }

 // mhe

#endif //INC_3_PARTITION_PROBLEM_PROBLEM_T_H
