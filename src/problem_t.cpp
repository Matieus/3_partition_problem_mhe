//
// Created by matie on 23.06.2023.
//

#include <iostream>
#include <vector>
#include <random>
#include <algorithm>

#include "problem_t.h"

namespace mhe {
    bool size_of_problem_is_divided_by_3(problem_t problem) {
        return problem.size() % 3 == 0;
    }

    int sum_of_problem(problem_t problem) {
        int sum_of_numbers = 0;
        for (int number: problem)
            sum_of_numbers += number;

        return sum_of_numbers;
    }


    problem_t generate_random_problem(int number_of_subsets, int min_rd, int max_rd, std::mt19937 &rgen) {
        std::vector<int> problem_set;
        std::uniform_int_distribution<int> random(min_rd, max_rd);

        for (int i = 0; i < number_of_subsets * 3; i++) {
            problem_set.push_back(random(rgen));
        }

        int sum = sum_of_problem(problem_set);

        while (sum % number_of_subsets != 0) {
            problem_set[random(rgen)] += 1;
            sum = sum_of_problem(problem_set);
        }

        return problem_set;
    }

    problem_t random_value_modify(problem_t problem_set, int min_rd, int max_rd, std::mt19937 &rgen) {
        std::uniform_int_distribution<int> random_idx(0, problem_set.size());
        std::uniform_int_distribution<int> random_value(min_rd, max_rd);

        problem_set[random_idx(rgen)] += random_value(rgen);
        return problem_set;
    }

    problem_t random_shuffle_problem(problem_t problem, std::mt19937 &rgen) {
        std::shuffle(problem.begin(), problem.end(), rgen);
        return problem;
    }

    std::ostream &operator<<(std::ostream &o, const problem_t v) {
        o << "{";
        for (auto e: v)
            o << e << ", ";
        o << "}";
        return o;
    }
} // mhe