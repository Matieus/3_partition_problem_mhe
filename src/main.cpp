#include <iostream>
#include <vector>
#include <string>
#include <array>

using problem_t = std::vector<int>;
using set_of_3_int = std::array<int, 3>;
using solution_t = std::vector<set_of_3_int>;


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


void show_solution_of_problem(solution_t solution){
    for (auto i: solution) {
        // print solution by 1 set of 3
    }
}

int sum_of_set(set_of_3_int set_of_3ints){
    int sum = 0;
    for (auto i: set_of_3ints)
        sum += set_of_3ints[i];
    return sum;
}


int main() {}