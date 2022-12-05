#ifndef MHE_FUNCS_H
#define MHE_FUNCS_H

#include "struct.h"

extern int calls;

int evaluate_puzzle(int x, int y, const nonogram_t &nonogram);
int count_inconsistent(const nonogram_t &nonogram);
double evaluate(const nonogram_t &nonogram);
bool next_solution(nonogram_t &nonogram);
std::vector<nonogram_t> generate_neighbours(nonogram_t &p);
std::vector<nonogram_t> generate_random_neighbours(const nonogram_t &p);
nonogram_t generate_random_solution(nonogram_t &p);
void print_f(std::string progress, std::string iter, bool show_progress, bool show_iterations);
nonogram_t brute_force(nonogram_t nonogram_a, int iterations, bool show_progress, bool show_iterations);
nonogram_t random_sampling(nonogram_t nonogram_a, int iterations, bool show_progress, bool show_iterations);
nonogram_t hill_climb_det(nonogram_t start_nonogram, int iterations, bool show_progress, bool show_iterations);
nonogram_t hill_climb_rand(nonogram_t start_nonogram, int iterations, bool show_progress, bool show_iterations);
nonogram_t tabu_search(nonogram_t nonogram, int iterations, bool show_progress, bool show_iterations, int tabu_size);
nonogram_t annealing(nonogram_t &nonogram, int iterations, bool show_progress, bool show_iterations,
                     std::function<double(int)> T = [](int k){return 1000.0/(k+1);});

#endif
