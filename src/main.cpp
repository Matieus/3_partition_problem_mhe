#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <memory>

int sum_of_set(std::vector<int> set) {
    int sum_of_numbers = 0;
    for (int number: set)
        sum_of_numbers += number;

    return sum_of_numbers;
}

using problem_t = std::vector<int>;
using set_of_3_int = std::vector<int>;

class solution_t : public std::vector<int> {
    std::shared_ptr<problem_t> problem;

    solution_t(std::shared_ptr<problem_t> problem_) : problem(problem_) {

    }
    /* void  goal(){
        double result = 0;
        for (int i = 0; i < this->size(); i++){
            result

        }
        return result;
    }*/
};

std::random_device rd;
std::mt19937 gen(rd());


bool size_of_problem_is_divided_by_3(problem_t problem) {
    return problem.size() % 3 == 0;
}

bool sum_of_problem_is_divided_by_3(int sum_of_problem) {
    return sum_of_problem % 3 == 0;
}


void show_solution_of_problem(solution_t solution) {
    for (auto set_of_3: solution) {
        // std::cout << set_of_3 << sum_of_set(set_of_3) << std::endl;
    }
}


problem_t generate_random_problem(int number_of_sets_by_3, int min_rd, int max_rd) {
    std::vector<int> problem_set;
    std::uniform_int_distribution<int> random(min_rd, max_rd);

    for (int i = 0; i < number_of_sets_by_3 * 3; i++) {
        problem_set.push_back(random(gen));
    }

    int sum = sum_of_set(problem_set);

    while (sum % number_of_sets_by_3 != 0) {
        problem_set[random(rd)] += 1;
        sum = sum_of_set(problem_set);
    }

    return problem_set;
}

problem_t random_modify(problem_t problem_set, int min_rd, int max_rd) {
    std::uniform_int_distribution<int> random_idx(0, problem_set.size());
    std::uniform_int_distribution<int> random_value(min_rd, max_rd);

    problem_set[random_idx(rd)] += random_value(rd);
    return problem_set;
}


double goal_function(std::vector<int> solution, int average) {
    double result;

    std::vector<int> set_of_3;
    for (int i = 0; i < solution.size(); i += 3) {
        set_of_3 = {
                solution[i],
                solution[i + 1],
                solution[i + 2]
        };

        std::cout << "sum of set: ";
        std::cout << sum_of_set(set_of_3) << std::endl;

        if (sum_of_set(set_of_3) == average) {
            result += 1;
        }
    }
    return result / (solution.size() / 3);

}

/*
solution_t random_hillclimb(solution_t solution) {
    for (int i = 0; i < 5040; i++) {
        auto new_solution = random_modify(solution);
        if (new_solution.goal() <= goal_function(solution)) {
            solution = new_solution;
            //std::cout << i << " " << solution << "  " << solution << std::endl;
        }
    }
    return solution;
}
*/

solution_t brute_force(problem_t problem);


int main() {
    problem_t problem = {4, 5, 6, 5, 5, 5};
    double result_of_goal;
    result_of_goal = goal_function(problem, 15);
    std::cout << "result: ";
    std::cout << result_of_goal;
}
