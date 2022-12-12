#include <fstream>
#include <random>
#include <list>
#include <functional>
#include "funcs.h"

std::random_device rd;
std::mt19937 mt(rd());

std::ofstream c_out("stdout.txt");

int calls = 0;

int evaluate_puzzle(int x, int y, const nonogram_t &nonogram) {
    using namespace std;

    auto func = [](int sum, int cx, int cy, int dx, int dy, const nonogram_t &nonogram) {
        vector<int> actual_params;
        vector<int> result;
        int error = 0;
        int py = cy, px = cx;
        try {
            while ((py == 0 && px >= nonogram.left_params_width) || (px == 0 && py >= nonogram.top_params_height)) {

                if (nonogram.get_from_param(cx, cy) > 0) {
                    actual_params.push_back(nonogram.get_from_param(cx, cy));
                }
                if (cx >= nonogram.left_params_width && cy >= nonogram.top_params_height) {
                    if (nonogram.get_from_board(cx - nonogram.left_params_width, cy - nonogram.top_params_height) ==
                        -1) {
                        sum++;
                    }
                    if (dy == 1 && cy == nonogram.height + nonogram.top_params_height - 1 && sum > 0) {
                        result.push_back(sum);
                        sum = 0;
                    }
                    if (dx == 1 && cx == nonogram.width + nonogram.left_params_width - 1 && sum > 0) {
                        result.push_back(sum);
                        sum = 0;
                    }
                    if (nonogram.get_from_board(cx - nonogram.left_params_width, cy - nonogram.top_params_height) ==
                        0 && sum > 0) {
                        result.push_back(sum);
                        sum = 0;
                    }
                }
                cx += dx;
                cy += dy;
            }
        } catch (...) {
        }
        if (actual_params.size() == result.size()) {
            for (int i = 0; i < actual_params.size(); i++) {
                error += abs(actual_params[i] - result[i]);
            }
        }
        if (actual_params.size() > result.size()) {
            int i = 0;
            for (; i < result.size(); i++) {
                error += abs(actual_params[i] - result[i]);
            }
            for (; i < actual_params.size(); i++) {
                error += actual_params[i];
            }
        }
        if (actual_params.size() < result.size()) {
            int i = 0;
            for (; i < actual_params.size(); i++) {
                error += abs(result[i] - actual_params[i]);
            }
            for (; i < result.size(); i++) {
                error += result[i];
            }
        }
        return error;
    };

    int sum = 0;
    if (y == 0 && x >= nonogram.left_params_width) {
        int dx = 0, dy = 1;
        sum += func(sum, x, y, dx, dy, nonogram);
    }
    if (x == 0 && y >= nonogram.top_params_height) {
        int dx = 1, dy = 0;
        sum += func(sum, x, y, dx, dy, nonogram);
    }
    return sum;
}

int count_inconsistent(const nonogram_t &nonogram) {
    int error = 0;
    for (int y = 0; y < nonogram.height + nonogram.top_params_height; y++)
        for (int x = 0; x < nonogram.width + nonogram.left_params_width; x++) {
            if (nonogram.get_from_param(x, y) >= 0) {
                int count = evaluate_puzzle(x, y, nonogram);
                error += count;
            }
        }
    return error;
}

double evaluate(const nonogram_t &nonogram) {
    calls += 1;
    return count_inconsistent(nonogram);
}

bool next_solution(nonogram_t &nonogram) {
    int i = 0;
    for (; i < nonogram.board.size(); i++) {
        if (nonogram.board[i] == 0) {
            nonogram.board[i] = -1;
            break;
        } else if (nonogram.board[i] == -1) {
            nonogram.board[i] = 0;
        }
    }
    return (i != nonogram.board.size());
}

std::vector<nonogram_t> generate_neighbours(nonogram_t &p) {
    std::vector<nonogram_t> neighbours;
    using namespace std;
    for (int i = 0; i < p.board.size(); i++) {
        try {
            auto new_board = p;
            new_board.board[i] = -1 - new_board.board[i];
            neighbours.push_back(new_board);

        } catch (...) {
        }
    }
    return neighbours;
}

std::vector<nonogram_t> generate_random_neighbours(const nonogram_t &p) {
    std::vector<nonogram_t> neighbours;
    using namespace std;
    uniform_int_distribution<int> distrY(0, p.height - 1);
    uniform_int_distribution<int> distrX(0, p.width - 1);

    int x = distrX(mt);
    int y = distrY(mt);
    std::vector<std::pair<int, int>> directions = {{-1, 0},
                                                   {1,  0},
                                                   {0,  -1},
                                                   {0,  1}};

    for (auto [dx, dy]: directions) {
        int cx = x + dx, cy = y + dy;
        try {
            auto new_board = p;
            new_board.board[cy * p.width + cx] = -1 - new_board.get_from_board(cx, cy);
            neighbours.push_back(new_board);

        } catch (...) {
        }
    }
    return neighbours;
}

nonogram_t generate_random_solution(nonogram_t &p) {
    using namespace std;
    uniform_int_distribution<int> distr(-1, 0);
    nonogram_t rand_sol = p;
    for (int i = 0; i < p.board.size(); i++) {
        if (p.board[i] <= 0) {
            auto new_board = p;
            rand_sol.board[i] = distr(mt);
        }
    }
    return rand_sol;
}

void print_f(std::string progress, std::string iter, bool show_progress = false, bool show_iterations = false) {
    if (show_iterations) {
        c_out << iter + "  ";
        std::cout << iter + "  ";
    }
    if (show_progress) {
        c_out << progress;
        std::cout << progress;
        c_out << std::endl;
        std::cout << std::endl;
    }
}

nonogram_t brute_force(nonogram_t nonogram_a, int iterations, bool show_progress = false, bool show_iterations = false) {
    auto nonogram = generate_random_solution(nonogram_a);
    auto best_so_far = nonogram;
    int best_value = evaluate(best_so_far);
    for (int i = 0; i < iterations; i++) {
        next_solution(nonogram);
        int nonogram_value = evaluate(nonogram);
        if (nonogram_value < best_value) {
            best_so_far = nonogram;
            best_value = evaluate(best_so_far);
        }

        std::string progress = std::to_string(nonogram_value) + "  " + std::to_string(best_value);
        std::string iter = std::to_string(i + 1);

        if (show_iterations || show_progress) print_f(progress, iter, show_progress, show_iterations);
        if (best_value == 0) break;
    }
    return best_so_far;
}

nonogram_t random_sampling(nonogram_t nonogram_a, int iterations, bool show_progress = false, bool show_iterations = false) {
    auto nonogram = generate_random_solution(nonogram_a);
    auto best_so_far = nonogram;
    int best_value = evaluate(best_so_far);
    for (int i = 0; i < iterations; i++) {
        nonogram = generate_random_solution(best_so_far);

        int nonogram_value = evaluate(nonogram);
        if (nonogram_value < best_value) {
            best_so_far = nonogram;
            best_value = evaluate(best_so_far);
        }

        std::string progress = std::to_string(nonogram_value) + "  " + std::to_string(best_value);
        std::string iter = std::to_string(i + 1);

        if (show_iterations || show_progress) print_f(progress, iter, show_progress, show_iterations);
        if (best_value == 0) break;
    }
    return best_so_far;
}

nonogram_t hill_climb_det(nonogram_t start_nonogram, int iterations, bool show_progress = false, bool show_iterations = false) {
    nonogram_t best_p = generate_random_solution(start_nonogram);
    int best_value = evaluate(best_p);
    for (int iteration = 0; iteration < iterations; iteration++) {
        auto close_points = generate_neighbours(best_p);

        auto best_neighbour_func = [=]() {
            auto result = close_points.at(0);
            for (int i = 1; i < close_points.size(); i++) {
                if (evaluate(result) > evaluate(close_points.at(i))) {
                    result = close_points.at(i);
                }
            }
            return result;
        };

        auto best_neighbour = best_neighbour_func();

        int nonogram_value = evaluate(best_neighbour);

        if (nonogram_value == best_value) {
            break;
        }

        if (nonogram_value < best_value) {
            best_p = best_neighbour;
            best_value = evaluate(best_p);
        }

        std::string progress = std::to_string(nonogram_value) + "  " + std::to_string(best_value);
        std::string iter = std::to_string(iteration + 1);

        if (show_iterations || show_progress) print_f(progress, iter, show_progress, show_iterations);
        if (best_value == 0) break;

    }
    return best_p;
}

nonogram_t hill_climb_rand(nonogram_t start_nonogram, int iterations, bool show_progress = false, bool show_iterations = false) {
    nonogram_t best_p = generate_random_solution(start_nonogram);
    int best_value = evaluate(best_p);
    for (int iteration = 0; iteration < iterations; iteration++) {
        auto close_points = generate_random_neighbours(best_p);
        auto best_neighbour_func = [=]() {
            auto result = close_points.at(0);
            for (int i = 1; i < close_points.size(); i++) {
                if (evaluate(result) > evaluate(close_points.at(i))) {
                    result = close_points.at(i);
                }
            }
            return result;
        };
        auto best_neighbour = best_neighbour_func();

        int nonogram_value = evaluate(best_neighbour);
        if (nonogram_value < best_value) {
            best_p = best_neighbour;
            best_value = evaluate(best_p);
        }

        std::string progress = std::to_string(nonogram_value) + "  " + std::to_string(best_value);
        std::string iter = std::to_string(iteration + 1);

        if (show_iterations || show_progress) print_f(progress, iter, show_progress, show_iterations);
        if (best_value == 0) break;
    }
    return best_p;
}

nonogram_t tabu_search(nonogram_t nonogram, int iterations, bool show_progress = false, bool show_iterations = false, int tabu_size = 1000) {
    using namespace std;
    list<nonogram_t> tabu_list;
    tabu_list.push_back(generate_random_solution(nonogram));
    auto best_so_far = tabu_list.back();
    int best_value = evaluate(best_so_far);
    for (int n = 0; n < iterations; n++) {
        vector<nonogram_t> neighbours;
        for (auto e: generate_neighbours(tabu_list.back())) {
            bool found = (std::find(tabu_list.begin(), tabu_list.end(), e) != tabu_list.end());
            if (!found) neighbours.push_back(e);
        }

        if (neighbours.size() == 0) {
            cerr << "we ate our tail :/" << endl;
            break;
        }
        tabu_list.push_back(*std::min_element(neighbours.begin(), neighbours.end(),
                                              [&](auto l, auto r) { return evaluate(l) < evaluate(r); }));

        int nonogram_value = evaluate(tabu_list.back());
        if (nonogram_value <= best_value) {
            best_so_far = tabu_list.back();
            best_value = evaluate(best_so_far);
        }

        std::string progress = std::to_string(nonogram_value) + "  " + std::to_string(best_value);
        std::string iter = std::to_string(n + 1);

        if (show_iterations || show_progress) print_f(progress, iter, show_progress, show_iterations);

        if (tabu_list.size() > tabu_size) tabu_list.pop_front();
        if (best_value == 0) break;
    }
    return best_so_far;
}

nonogram_t annealing(nonogram_t &nonogram, int iterations, bool show_progress = false, bool show_iterations = false,
                     std::function<double(int)> T) {
    using namespace std;
    auto s = generate_random_solution(nonogram);
    auto best_so_far = s;
    for (int k = 0; k < iterations; k++) {

        auto generated_neighbours = generate_neighbours(s);

        uniform_int_distribution<int> dist(0, (generated_neighbours.size()-1));
        auto rand = dist(mt);
        auto t = generated_neighbours[rand];

        if (evaluate(t) < evaluate(s)) {
            s = t;
            if (evaluate(s) < evaluate(best_so_far)) best_so_far = s;
        } else {
            uniform_real_distribution<double> u(0.0,1.0);
            double v = exp(-abs(evaluate(t) - evaluate(s))/T(k));
            if (u(mt) < v) {
                s = t;
            }
        }

        int nonogram_value = evaluate(s);
        int best_value = evaluate(best_so_far);

        std::string progress = std::to_string(nonogram_value) + "  " + std::to_string(best_value);
        std::string iter = std::to_string(k + 1);

        if (show_iterations || show_progress) print_f(progress, iter, show_progress, show_iterations);
        if (best_value == 0) break;
    }
    return best_so_far;
}