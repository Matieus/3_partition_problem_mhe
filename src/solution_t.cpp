//
// Created by matie on 23.06.2023.
//

#include "solution_t.h"

namespace mhe {

    int solution_t::sum_of_problem() {
        int sum_of_numbers = 0;
        for (auto number: *this) {
            sum_of_numbers += number;
        }

        return sum_of_numbers;
    }

    solution_t solution_t::for_problem(std::shared_ptr<problem_t> problem_) {
        solution_t solution;

        for (auto number: *problem_)
            solution.push_back(number);
        solution.problem = problem_;
        solution.average = solution.sum_of_problem() * 3 / problem_->size();
        return solution;
    }

    double solution_t::goal() {
        // returns values from 0 to 1
        // result 1 is the best

        double result = 0;
        auto &s = *this;
        int sum;

        for (int i = 0; i < size(); i += 3) {
            sum = s[i] + s[i + 1] + s[i + 2];

            if (sum == s.average) {
                result += 1;
            }
        }
        return result * 3 / size();
    }

    solution_t solution_t::random_shuffle() {
        auto &s = *this;
        std::random_device rd;
        std::mt19937 gen(rd());
        std::shuffle(s.begin(), s.end(), gen);
        return s;
    }

    solution_t solution_t::random_solution(problem_t problem) {

        auto solution = solution_t::for_problem(std::make_shared<problem_t>(problem));
        std::random_device rd;
        std::mt19937 gen(rd());
        std::shuffle(solution.begin(), solution.end(), gen);
        return solution;
    }

    solution_t solution_t::random_modify() {

        auto current_solution = *this;
        std::uniform_int_distribution<int> random_idx(0, current_solution.size() - 1);
        std::random_device rd;
        std::mt19937 gen(rd());

        int a = random_idx(gen);
        int b = random_idx(gen);
        if (a == b) {
            b = (a + 1) & (current_solution.size() - 1);

        }
        std::swap(current_solution[a], current_solution[b]);
        return current_solution;
    }


    std::ostream &operator<<(std::ostream &o, const solution_t solution) {
        o << "{ ";
        for (int i = 0; i < solution.size(); i += 3) {
            o << "{" << solution[i] << " ";
            o << solution[i + 1] << " ";
            o << solution[i + 2] << "} ";
        }
        o << "}";
        return o;
    }

} // mhe