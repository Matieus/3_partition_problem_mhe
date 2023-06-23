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

    solution_t solution_t::random_shuffle(std::mt19937 &rgen) {
        auto &s = *this;
        std::shuffle(s.begin(), s.end(), rgen);
        return s;
    }

    solution_t solution_t::random_solution(problem_t problem, std::mt19937 &rgen) {

        auto solution = solution_t::for_problem(std::make_shared<problem_t>(problem));

        std::shuffle(solution.begin(), solution.end(), rgen);
        return solution;
    }

    solution_t solution_t::random_modify(std::mt19937 &rgen) {

        auto current_solution = *this;
        std::uniform_int_distribution<int> random_idx(0, current_solution.size() - 1);


        int a = random_idx(rgen);
        int b = random_idx(rgen);
        if (a == b) {
            b = (a + 1) & (current_solution.size() - 1);

        }
        std::swap(current_solution[a], current_solution[b]);
        return current_solution;
    }



    void show_solution_of_problem(std::vector<int> solution) {
        int sum;
        for (int i = 0; i < solution.size(); i++)
            std::cout << solution[i];

        std::cout << std::endl;

        for (int i = 0; i < solution.size(); i += 3) {
            for (int j = 0; j < 3; j++) {
                sum += solution[i + j];
                std::cout << std::to_string(solution[i + j]);
                std::cout << ", ";
            }
            std::cout << "sum: ";
            std::cout << sum;
            std::cout << std::endl;
            sum = 0;
        }
    }


    std::vector<solution_t> generate_neighbours(solution_t solution) {
        std::vector<solution_t> neighbours;
        solution_t neighbour;

        for (int k = 1; k < solution.size(); k++) {
            for (int i = 0; i < solution.size(); i++) {
                neighbour = solution;
                std::swap(neighbour[i], neighbour[(i + k) % solution.size()]);
                neighbours.push_back(neighbour);
            }
        }
        return neighbours;

    }

    solution_t best_neighbour(solution_t current_solution) {
        std::vector<solution_t> neighbours = generate_neighbours(current_solution);

        solution_t best_solution = current_solution;

        for (auto neighbour: neighbours) {
            if (best_solution.goal() <= neighbour.goal()) {
                best_solution = neighbour;
            }
        }
        return best_solution;
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