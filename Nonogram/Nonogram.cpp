#include <iostream>
#include <vector>
#include <cmath>
#include <random>
#include <iomanip>
#include <map>
#include <list>
#include <functional>

std::random_device rd;
std::mt19937 mt(rd());

struct nonogram_t {
    int width;
    int height;
    std::vector<int> board;
    int left_parmas_width;
    int top_params_height;
    std::vector<int> params_board;


    int &get(const int x, const int y) {
        return board[y * width + x];
    }

    int get_from_param(const int x, const int y) const {
        if ((x < 0) || (x >= width + left_parmas_width) || (y < 0) || (y >= height + top_params_height)) {
            throw std::invalid_argument("out of board");
        } else if ((x >= left_parmas_width && y < top_params_height) ||
                   (x < left_parmas_width && y >= top_params_height)) {
            return params_board.at(y * (width + left_parmas_width) + x);
        }
        return 0;
    }

    int get_from_board(const int x, const int y) const {
        if ((x < 0) || (x >= width) || (y < 0) || (y >= height)) {
            throw std::invalid_argument("out of board");
            return 0;
        } else {
            return board.at(y * width + x);
        }
//        return 0;
    }
};

std::ostream &operator<<(std::ostream &o, const nonogram_t &nonogram) {
    using namespace std;
    for (int y = 0; y < nonogram.height + nonogram.top_params_height; y++) {
        for (int x = 0; x < nonogram.width + nonogram.left_parmas_width; x++) {
            auto &val = nonogram.params_board[y * (nonogram.width + nonogram.left_parmas_width) + x];
            if (y < nonogram.top_params_height) {
                o << ((val > 0) ? "    " + to_string(val) : "") << "\t";
            } else if (x < nonogram.left_parmas_width) {
                o << ((val > 0) ? "    " + to_string(val) : "") << "\t";
            }
            if (y >= nonogram.top_params_height && x >= nonogram.left_parmas_width) {
                auto &center_val = nonogram.board[(y - nonogram.top_params_height) * (nonogram.width) +
                                                  (x - nonogram.left_parmas_width)];
                o << "|" << ((center_val == -1) ? "  ***" : (center_val == -2) ? "   x" : "")
                  << ((x == nonogram.width + nonogram.left_parmas_width - 1) ? "\t|" : "\t");
            }
        }
        o << endl;
    }
    return o;
}

int evaluate_puzzle(int x, int y, const nonogram_t &nonogram) {
    using namespace std;

    auto func = [](int sum, int cx, int cy, int dx, int dy, const nonogram_t &nonogram) {
        vector<int> actual_params;
        vector<int> result;
        int error = 0;
        int py = cy, px = cx;
        try {
            while ((py == 0 && px >= nonogram.left_parmas_width) || (px == 0 && py >= nonogram.top_params_height)) {

                if (nonogram.get_from_param(cx, cy) > 0) {
                    actual_params.push_back(nonogram.get_from_param(cx, cy));
                }
                if (cx >= nonogram.left_parmas_width && cy >= nonogram.top_params_height) {
                    if (nonogram.get_from_board(cx - nonogram.left_parmas_width, cy - nonogram.top_params_height) ==
                        -1) {
                        sum++;
                    }
                    if (dy == 1 && cy == nonogram.height + nonogram.top_params_height - 1 && sum > 0) {
                        result.push_back(sum);
                        sum = 0;
                    }
                    if (dx == 1 && cx == nonogram.width + nonogram.left_parmas_width - 1 && sum > 0) {
                        result.push_back(sum);
                        sum = 0;
                    }
                    if (nonogram.get_from_board(cx - nonogram.left_parmas_width, cy - nonogram.top_params_height) ==
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
    if (y == 0 && x >= nonogram.left_parmas_width) {
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
        for (int x = 0; x < nonogram.width + nonogram.left_parmas_width; x++) {
            if (nonogram.get_from_param(x, y) >= 0) {
                int count = evaluate_puzzle(x, y, nonogram);
                error += count;
            }
        }
    return error;
}

double evaluate(const nonogram_t &nonogram) {
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

nonogram_t random_sampling(nonogram_t nonogram, int iterations){
    auto result =  generate_random_solution(nonogram);
    for (int iteration = 0; iteration < iterations; iteration++) {
        nonogram_t gen_value = generate_random_solution(nonogram);
        if (evaluate(result) > evaluate(gen_value)) {
            result = gen_value;
        }
    }
    return result;
}

nonogram_t hill_climb_det(nonogram_t start_nonogram, int iterations) {
    nonogram_t best_p = start_nonogram;
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
        if (evaluate(best_neighbour) < evaluate(best_p)) best_p = best_neighbour;
    }
    return best_p;
}

nonogram_t hill_climb_rand(nonogram_t start_nonogram, int iterations) {
    nonogram_t best_p = start_nonogram;
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
        if (evaluate(best_neighbour) < evaluate(best_p)) best_p = best_neighbour;
    }
    return best_p;
}

nonogram_t tabu_search(nonogram_t nonogram, int iterations) {
    using namespace std;
    const int tabu_size = 1000;
    list<nonogram_t> tabu_list;
    tabu_list.push_back(nonogram);
    auto best_so_far = tabu_list.back();
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
                                              [](auto l, auto r) { return evaluate(l) < evaluate(r); }));
        if (evaluate(tabu_list.back()) <= evaluate(best_so_far)) {
            best_so_far = tabu_list.back();
        }
        if (tabu_list.size() > tabu_size) tabu_list.pop_front();
    }
    return best_so_far;
}

int main(int argc, char **argv) {
    using namespace std;

    map<string, nonogram_t> formatery_puzzle;

    formatery_puzzle["nonogram1"] = {
            5,
            5,
            {
                    0, 0, 0, 0, 0,
                    0, 0, 0, 0, 0,
                    0, 0, 0, 0, 0,
                    0, 0, 0, 0, 0,
                    0, 0, 0, 0, 0},
            3,
            2,
            {
                    0, 0, 0, 0, 0, 1, 0, 0,
                    0, 0, 0, 3, 3, 3, 2, 1,
                    1, 1, 1, 0, 0, 0, 0, 0,
                    0, 0, 1, 0, 0, 0, 0, 0,
                    0, 0, 3, 0, 0, 0, 0, 0,
                    0, 0, 3, 0, 0, 0, 0, 0,
                    0, 0, 3, 0, 0, 0, 0, 0}

    };
    formatery_puzzle["nonogram0"] = {
            3,
            3,
            {
                    0, 0, 0,
                    0, 0, 0,
                    0, 0, 0},
            2,
            1,
            {
                    0, 0, 2, 1, 2,
                    0, 2, 0, 0, 0,
                    1, 1, 0, 0, 0,
                    0, 1, 0, 0, 0
            }

    };
    formatery_puzzle["nonogram_solution"] = {
            5,
            5,
            {
                    -1, 0, -1, 0, -1,
                    -1, 0, 0, 0, 0,
                    -1, -1, -1, 0, 0,
                    0, -1, -1, -1, 0,
                    0, -1, -1, -1, 0,},
            3,
            2,
            {
                    0, 0, 0, 0, 0, 1, 0, 0,
                    0, 0, 0, 3, 3, 3, 2, 1,
                    1, 1, 1, 0, 0, 0, 0, 0,
                    0, 0, 1, 0, 0, 0, 0, 0,
                    0, 0, 3, 0, 0, 0, 0, 0,
                    0, 0, 3, 0, 0, 0, 0, 0,
                    0, 0, 3, 0, 0, 0, 0, 0}

    };
    formatery_puzzle["nonogram_wrong"] = {
            5,
            5,
            {
                    0, 0, 0, 0, -1,
                    0, 0, -1, 0, 0,
                    -1, -1, 0, -1, 0,
                    -1, -1, -1, 0, 0,
                    -1, -1, -1, 0, 0,},
            3,
            2,
            {
                    0, 0, 0, 0, 0, 1, 0, 0,
                    0, 0, 0, 3, 3, 3, 2, 1,
                    1, 1, 1, 0, 0, 0, 0, 0,
                    0, 0, 1, 0, 0, 0, 0, 0,
                    0, 0, 3, 0, 0, 0, 0, 0,
                    0, 0, 3, 0, 0, 0, 0, 0,
                    0, 0, 3, 0, 0, 0, 0, 0}

    };
    map<string, function<void()>> formatery_calc;

    string calc_method = argv[1];
    int iterations = stoi(argv[2]);

    auto nonogram = formatery_puzzle.at(argv[3]);
//    auto nonogram = generate_random_solution(nonogram);

    formatery_calc["brute_force"] = [&]() {
        cout << nonogram << endl;
        int n = 0;
        while (next_solution(nonogram)) {
            if ((n % 10000) == 0) {
                cout << n << " : " << evaluate(nonogram) << endl << nonogram << endl;
            }
            if (evaluate(nonogram) == 0) {
                cout << nonogram << endl;
                break;
            }
            n++;
        }
    };

    formatery_calc["random_sampling"] = [&]() {
        cout << nonogram << endl;
        cout << "-----------------------------------------------" << endl;
        auto result = random_sampling(nonogram, iterations);
        cout << evaluate(result) << endl;
        cout << result << endl;
    };

    formatery_calc["hill_climb_det"] = [&]() {
        cout << nonogram << endl;
        cout << "-----------------------------------------------" << endl;
        auto result = hill_climb_det(generate_random_solution(nonogram), iterations);
        cout << evaluate(result) << endl;
        cout << result << endl;
    };

    formatery_calc["hill_climb_rand"] = [&]() {
        cout << nonogram << endl;
        cout << "-----------------------------------------------" << endl;
        auto result = hill_climb_rand(generate_random_solution(nonogram), iterations);
        cout << evaluate(result) << endl;
        cout << result << endl;
    };

    formatery_calc["tabu_search"] = [&]() {
        cout << nonogram << endl;
        cout << "-----------------------------------------------" << endl;
        auto result = tabu_search(generate_random_solution(nonogram), iterations);
        cout << evaluate(result) << endl;
        cout << result << endl;
    };

    formatery_calc.at(calc_method)();

//  -----------------------check_random_neighbors-----------------------
//    for (auto neighbour: generate_random_neighbours(nonogram)) {
//        cout << " ------------------------------" << endl;
//        cout << evaluate(neighbour) << endl;
//        cout << neighbour << endl;
//    }

//  -----------------------check_neighbors-----------------------
//    for (auto neighbour: generate_neighbours(nonogram)) {
//        cout << " ------------------------------" << endl;
//        cout << evaluate(neighbour) << endl;
//        cout << neighbour << endl;
//    }

//    cout << "----------------------------------------" << endl;
//    cout <<  generate_random_solution(nonogram) << endl;


//    cout << evaluate(nonogram) << endl;
//    cout << nonogram << endl;

    return 0;
}