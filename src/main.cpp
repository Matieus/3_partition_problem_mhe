#include <iostream>
#include <vector>
#include <string>
#include <array>
#include <random>


using problem_t = std::vector<int>;
using set_of_3_int = std::array<int, 3>;
using solution_t = std::vector<set_of_3_int>;

std::random_device rd;
std::mt19937 gen(rd());


int sum_of_problem(problem_t problem) {
    int sum_of_numbers = 0;
    for (int number: problem)
        sum_of_numbers += number;
}

bool size_of_problem_is_divided_by_3(problem_t problem) {
    return problem.size() % 3 == 0;
}

bool sum_of_problem_is_divided_by_3(int sum_of_problem) {
    return sum_of_problem % 3 == 0;
}


void show_solution_of_problem(solution_t solution) {
    for (auto i: solution) {
        // print solution by 1 set of 3
    }
}

int sum_of_set(set_of_3_int set_of_3ints) {
    int sum = 0;
    for (auto i: set_of_3ints)
        sum += set_of_3ints[i];
    return sum;
}


problem_t generate_random_problem(int number_of_sets_by_3, int min_rd, int max_rd) {
    std::vector<int> problem_set;
    std::uniform_int_distribution<int> random(min_rd, max_rd);

    for (int i = 0; i < number_of_sets_by_3 * 3; i++) {
        problem_set.push_back(random(gen));
    }

    int sum = sum_of_problem(problem_set);

    while (sum % number_of_sets_by_3 != 0) {
        problem_set[random(rd)] += 1;
        sum = sum_of_problem(problem_set);
    }

    return problem_set;
}


int main() {}