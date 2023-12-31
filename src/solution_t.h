//
// Created by matie on 23.06.2023.
//

#include <iostream>
#include <algorithm>
#include <memory>
#include <numeric>
#include <random>
#include <string>
#include <vector>
#include <list>
#include <set>
#include <functional>

#include "problem_t.h"

#ifndef INC_3_PARTITION_PROBLEM_SOLUTION_T_H
#define INC_3_PARTITION_PROBLEM_SOLUTION_T_H

namespace mhe {
    class solution_t : public std::vector<int> {
    public:
        std::shared_ptr<problem_t> problem;
        int average; // average for each subset of 3 items

        int sum_of_problem();

        static solution_t for_problem(std::shared_ptr<problem_t> problem_);


        double goal();

        solution_t random_shuffle(std::mt19937 &rgen);

        static solution_t random_solution(problem_t problem, std::mt19937 &rgen);

        solution_t random_modify(std::mt19937 &rgen);

        };

    solution_t random_solution(problem_t problem, std::mt19937 &rgen);
    solution_t random_modify(std::mt19937 &rgen);
    void show_solution_of_problem(std::vector<int> solution);
    std::vector<solution_t> generate_neighbours(solution_t solution);
    solution_t best_neighbour(solution_t current_solution);
    std::ostream &operator<<(std::ostream &o, const solution_t solution);

} // mhe

#endif //INC_3_PARTITION_PROBLEM_SOLUTION_T_H
