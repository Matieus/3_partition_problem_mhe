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

#include "solution_t.h"
#include "problem_t.h"

std::random_device rd;
std::mt19937 rgen(rd());

using namespace mhe;

void print_results(solution_t solution, int i = NULL, double goal = NULL) {

    if (goal == NULL)
        goal = solution.goal();

    std::cout << "goal: " << goal << std::endl;
    if (i != NULL)
        std::cout << "iteration: " << i << std::endl;

    int sum = 0;
    for (int j = 0; j < solution.size(); j += 3) {
        for (int k = 0; k < 3; k++) {
            std::cout << solution[j + k] << ", ";
            sum += solution[j + k];
        }
        std::cout << "sum: " << sum << std::endl;
        sum = 0;
    }
    std::cout << "\n\n";
}

solution_t deterministic_hill_climb(solution_t solution) {
    solution_t new_solution;

    for (int i = 0; i < 5040; i++) {
        new_solution = best_neighbour(solution); //best neighbour

        double new_goal = new_solution.goal();
        double goal = solution.goal();

        if (new_goal >= goal) {
            solution = new_solution;

        }
    }
    return solution;
}

solution_t random_hill_climb(solution_t solution, std::mt19937 &rgen) {
    for (int i = 0; i < 5040; i++) {
        auto new_solution = solution.random_modify(rgen);
        if (new_solution.goal() >= solution.goal()) {
            solution = new_solution;
            std::cout << "[random_hill_climb]" << std::endl;
            print_results(solution, i);
        }
    }
    return solution;
}


solution_t tabu_search(solution_t solution) {
    std::list<solution_t> tabu_list;
    tabu_list.push_back(solution);

    std::set<solution_t> tabu_set;
    tabu_set.insert(solution);

    solution_t best_global = solution;

    for (int i = 0; i < 5040; i++) {
        auto neighbours = generate_neighbours(tabu_list.back());

        neighbours.erase(
                std::remove_if(
                        neighbours.begin(), neighbours.end(),
                        [&](solution_t x) { return tabu_set.find(x) != tabu_set.end(); }
                ),
                neighbours.end()
        );


        if (neighbours.size() == 0) {
            std::cout << "TABU: tail" << std::endl;
            print_results(best_global);
            return best_global;
        }


        auto next_solution = *std::max_element(
                neighbours.begin(),
                neighbours.end(),
                [](auto l, auto r) {
                    return l.goal() < r.goal();
                }
        );

        if (next_solution.goal() >= best_global.goal()) {
            best_global = next_solution;
        }
        tabu_list.push_back(next_solution);
    }

    return best_global;
}

solution_t sim_annealing(const solution_t solution, std::function<double(int)> T, std::mt19937 &rgen) {
    auto best_solution = solution;
    auto new_solution = solution;

    auto s_current_solution = solution;
    for (int i = 1; i < 5040; i++) {
        new_solution = best_solution.random_modify(rgen);
        if (new_solution.goal() >= s_current_solution.goal()) {
            s_current_solution = new_solution;
            if (new_solution.goal() >= best_solution.goal()) {
                best_solution = new_solution;
            }

        } else {
            std::uniform_real_distribution<double> u(0.0, 1.0);
            if (
                    u(rgen) < std::exp(-std::abs(new_solution.goal() - s_current_solution.goal()) / T(i))
                    ) {
                s_current_solution = new_solution;
            }

        }
    }
    return best_solution;
}

template<class T> //klasa szablonowa typem T
class generic_algoritm_config_t {
public:
    int population_size;

    virtual bool terminal_condition(std::vector<T>) = 0;

    virtual std::vector<T> get_initial_population() = 0;

    virtual double fitness(T) = 0;

    virtual std::vector<T> selection(std::vector<double>, std::vector<T>) = 0;

    virtual std::vector<T> crossover(std::vector<T>) = 0;

    virtual std::vector<T> mutation(std::vector<T>) = 0;

};


class tsp_config_t : public generic_algoritm_config_t<solution_t> {
public:
    problem_t problem;
    int iteration;
    int max_iterations;

    tsp_config_t(problem_t p, int i, int population_s, std::mt19937 &rgen) {
        problem = p;
        iteration = 0;
        max_iterations = i;
        population_size = population_s;
    }

    virtual bool terminal_condition(std::vector<solution_t>) {
        iteration++;
        return max_iterations >= iteration;
    };

    virtual std::vector<solution_t> get_initial_population() {
        std::vector<solution_t> ret;
        for (int i = 0; i < population_size; i++) {
            ret.push_back(solution_t::random_solution(problem, rgen));
        }

        return ret;
    };

    virtual double fitness(solution_t solution) {
        return 1.0 / (1 + solution.goal()); // 0.5 min, 1.0 max
    };

    virtual std::vector<solution_t> selection(std::vector<double> fitnesses, std::vector<solution_t> population) {
        std::vector<solution_t> ret;

        std::random_device rd;
        std::mt19937 gen(rd());

        while (ret.size() < population.size()) {
            std::uniform_int_distribution<int> dist(0, population.size() - 1);

            int a_idx = dist(gen);
            int b_idx = dist(gen);
            if (fitnesses[a_idx] >= fitnesses[b_idx])
                ret.push_back(population[b_idx]);

        }
        return ret;
    };


    std::pair<solution_t, solution_t> crossover_p(solution_t p_a, solution_t p_b) {
        return {p_a, p_b};
    }

    virtual std::vector<solution_t> crossover(std::vector<solution_t> population) {
        std::vector<solution_t> offspring;

        for (int i = 0; i < population.size(); i += 2) {

            auto [a, b] = crossover_p(population.at(i), population.at(i + 1));
            offspring.push_back(a);
            offspring.push_back(b);
        }

        return offspring;

    };

    virtual std::vector<solution_t> mutation(std::vector<solution_t> population) {
        std::uniform_real_distribution<double> distr(0.0, 1.0);
        std::vector<solution_t> ret(population.size());
        std::mt19937 rd;

        std::transform(
                population.begin(),
                population.end(),
                ret.begin(),
                [&](auto e) {
                    if (distr(rd) > 0.9)
                        return e.random_modify(rgen);
                    else
                        return e;
                });
        return ret;
    }
};


template<class T>
solution_t generic_algorithm(generic_algoritm_config_t<T> &cfg) {
    auto population = cfg.get_initial_population();

    while (cfg.terminal_condition(population)) {

        std::vector<double> fitnesses;
        for (int i = 0; i < population.size(); i++) {
            fitnesses.push_back(cfg.fitness(population[i]));
        }


        auto parents = cfg.selection(fitnesses, population);
        auto offspring = cfg.crossover(parents);
        offspring = cfg.mutation(offspring);
        population = offspring;

    }

    solution_t s = *std::min_element(
            population.begin(),
            population.end(),
            [&](T l, T r) {
                return cfg.fitness(l) < cfg.fitness(r);
            }
    );

    return s;
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


solution_t test_solution(solution_t solution) {
    double current_goal = solution.goal();

    print_results(solution, -1, current_goal);

    solution_t better_solution = solution;
    double better_goal = 0;

    for (int i = 0; i < 5040; i++) {
        better_solution = better_solution.random_modify(rgen);
        better_goal = better_solution.goal();

        if (better_goal > current_goal) {
            current_goal = better_goal;
            solution = better_solution;

            print_results(solution, i, current_goal);

            if (better_goal == 1)
                break;


//    auto current_solution = solution_t::for_problem(make_shared<problem_t>(problem));


//    std::cout << "_______________" << std::endl;
//    current_solution = random_shuffle(current_solution);
//    print_results(current_solution);
//
//    current_solution = tabu_search(current_solution);
//    print_results(current_solution);


//    current_solution = random_shuffle(current_solution);
//    print_results(current_solution);

//    std::cout << "_______________" << std::endl;
//
//    problem_t problem_bad = {2, 3, 3, 5, 5, 4, 5, 5, 12, 3, 9, 13, 8, 1, 6, 1, 11, 4, 7, 5, 4, 1, 2, 1};
//
//    auto current_solution = solution_t::for_problem(make_shared<problem_t>(problem_bad));
//
//    std::cout << current_solution << std::endl;
//    current_solution = sim_annealing(current_solution, [](int k) { return 1000 / k; });
//    print_results(current_solution);

        }

    }
    return solution;

}

int main() {
    using namespace std;

    // 5,5,5  4,5,6  4,4,7  9,3,3  2,8,5  1,3,11
    problem_t problem = {1, 2, 3, 3, 3, 4, 4, 4, 5, 5, 5, 5, 5, 6, 7, 8, 9, 11};

    // 30,30,30  20,25,45  23,27,40  49,22,19
    problem_t problem_2 = {20, 23, 25, 30, 49, 45, 27, 30, 30, 40, 22, 19};

    // 5,5,5  4,5,6  4,4,7  9,3,3  2,8,5  1,3,11  1,1,13, 1,2,12
    problem_t problem_3 = {1, 2, 3, 3, 3, 4, 4, 4, 5, 5, 5, 5, 5, 6, 7, 8, 9, 11, 1, 1, 13, 1, 2, 12};

    //auto current_solution = solution_t::for_problem(make_shared<problem_t>(problem));

    std::cout << problem_3 << std::endl;
    std::cout << "_______________" << std::endl;

    tsp_config_t config(problem_3, 120, 50, rgen);

    solution_t current_solution = generic_algorithm<solution_t>(config);
    print_results(current_solution);

}
