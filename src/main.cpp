#include <iostream>
#include <algorithm>
#include <memory>
#include <numeric>
#include <random>
#include <string>
#include <vector>

using problem_t = std::vector<int>;

class solution_t : public std::vector<int> {
public:
    std::shared_ptr<problem_t> problem;
    int average;

    int sum_of_problem() {
        int sum_of_numbers = 0;
        for (auto number: *this) {
            sum_of_numbers += number;
        }

        return sum_of_numbers;
    }

    static solution_t for_problem(std::shared_ptr<problem_t> problem_) {
        solution_t solution;

        for (auto number: *problem_)
            solution.push_back(number);
        solution.problem = problem_;
        solution.average = solution.sum_of_problem() * 3 / problem_->size();
        return solution;
    }

    double goal() {
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
};


std::ostream &operator<<(std::ostream &o, const problem_t v) {
    o << "{";
    for (auto e: v)
        o << e << ", ";
    o << "}";
    return o;
}

std::ostream &operator<<(std::ostream &o, const solution_t solution) {
    int sum = 0;
    for (int i = 0; i < solution.size(); i += 3) {
        for (int j = 0; j < 3; j++) {
            o << solution[i + j] << ", ";
            sum += solution[i + j];
        }
        o << "sum: " << sum << std::endl;
        sum = 0;
    }
    return o;

}

void print_results(solution_t solution, int i = NULL, double goal = NULL) {

    if (goal == NULL)
        goal = solution.goal();

    std::cout << "goal: " << goal << std::endl;
    std::cout << "iteration: " << i << std::endl;
    std::cout << solution << std::endl;
}

std::random_device rd;
std::mt19937 gen(rd());

bool size_of_problem_is_divided_by_3(problem_t problem) {
    return problem.size() % 3 == 0;
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

int sum_of_problem(problem_t problem) {
    int sum_of_numbers = 0;
    for (int number: problem)
        sum_of_numbers += number;

    return sum_of_numbers;
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

problem_t random_value_modify(problem_t problem_set, int min_rd, int max_rd) {
    std::uniform_int_distribution<int> random_idx(0, problem_set.size());
    std::uniform_int_distribution<int> random_value(min_rd, max_rd);

    problem_set[random_idx(rd)] += random_value(rd);
    return problem_set;
}

solution_t random_modify(solution_t current_solution) {
    std::uniform_int_distribution<int> random_idx(0, current_solution.size() - 1);
    int a = random_idx(rd);
    int b = random_idx(rd);
    if (a == b) {
        b = (a + 1) & (current_solution.size() - 1);

    }
    std::swap(current_solution[a], current_solution[b]);
    return current_solution;
}

solution_t random_shuffle(solution_t solution) {
    std::shuffle(solution.begin(), solution.end(), rd);
    return solution;
}

problem_t random_shuffle_problem(problem_t problem) {
    std::shuffle(problem.begin(), problem.end(), rd);
    return problem;
}

solution_t random_hillclimb(solution_t solution) {
    print_results(solution);

    for (int i = 0; i < 5040; i++) {
        auto new_solution = random_modify(solution);
        if (new_solution.goal() > solution.goal()) {
            solution = new_solution;
            std::cout << "[random_hillclimb]" << std::endl;
            print_results(solution, i);

        }
    }
    return solution;
}

solution_t brute_force(solution_t solution) {
    print_results(solution);
    auto best_solution = solution;

    int i = 0;
    double goal = 0;
    double best_goal = 0;

    do {
        goal = solution.goal();

        if (goal > best_goal) {
            best_solution = solution;
            best_goal = goal;

            std::cout << "[brute_force]" << std::endl;
            print_results(best_solution, i, goal);
        }
        i++;

    } while (std::next_permutation(solution.begin(), solution.end()));
    return best_solution;
}

int main() {
    using namespace std;

    // 5,5,5  4,5,6  4,4,7  9,3,3  2,8,5  1,3,11
    problem_t problem = {1, 2, 3, 3, 3, 4, 4, 4, 5, 5, 5, 5, 5, 6, 7, 8, 9, 11};

    // 30,30,30  20,25,45  23,27,40  49,22,19
    problem_t problem_2 = {20, 23, 25, 30, 49, 45, 27, 30, 30, 40, 22, 19};

    problem = random_shuffle_problem(problem_2);

    auto current_solution = solution_t::for_problem(make_shared<problem_t>(problem));
    double current_goal = current_solution.goal();

    print_results(current_solution, -1, current_goal);

    solution_t better_solution = current_solution;
    double better_goal = 0;

    for (int i = 0; i < 1000; i++) {
        better_solution = random_modify(better_solution);
        better_goal = better_solution.goal();

        if (better_goal > current_goal) {
            current_goal = better_goal;
            current_solution = better_solution;

            print_results(current_solution, i, current_goal);

            if (better_goal == 1)
                break;
        }

    }
    std::cout << "_______________" << std::endl;
    current_solution = random_shuffle(current_solution);
    random_hillclimb(current_solution);

    std::cout << "_______________" << std::endl;
    current_solution = random_shuffle(current_solution);
    brute_force(current_solution);

}
