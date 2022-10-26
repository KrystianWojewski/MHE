#include <iostream>
#include <vector>

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
//            throw std::invalid_argument("out of board");
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
                o << "|" << ((center_val < 0) ? "  ***" : "")
                  << ((x == nonogram.width + nonogram.left_parmas_width - 1) ? "\t|" : "\t");
            }
        }
        o << endl;
    }
    return o;
}

int count_inside_bag_from_point(int x, int y, const nonogram_t &nonogram) {
    using namespace std;

    auto func = [](int sum, int cx, int cy, int dx, int dy, const nonogram_t &nonogram) {
        int count = 0;
        int py = cy, px = cx;
        try {
            while ((py == 0 && px >= nonogram.left_parmas_width) || (px == 0 && py >= nonogram.top_params_height)) {

                if (nonogram.get_from_param(cx, cy) >= 0) {
                    count += nonogram.get_from_param(cx, cy);
                }
                if (nonogram.get_from_board(cx - nonogram.left_parmas_width, cy - nonogram.top_params_height) < 0) {
                    sum++;
                }
                cx += dx;
                cy += dy;
            }
        } catch (...) {
        }
        if (count == sum) {
            return 0;
        } else {
            return count - sum;
        }
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

//    for (auto [dx, dy]: directions) {
//        int cx = x + dx, cy = y + dy;
//        try {
//            while ((nonogram.get(cx, cy)) != 0) {
//                sum++;
//                cx += dx;
//                cy += dy;
//            }
//        } catch (...) {
//        }
//    }
    return sum;
}

int count_inconsistent(const nonogram_t &nonogram) {
    int error = 0;
    for (int y = 0; y < nonogram.height + nonogram.top_params_height; y++)
        for (int x = 0; x < nonogram.width + nonogram.left_parmas_width; x++) {
            if (nonogram.get_from_param(x, y) >= 0) {
                int count = count_inside_bag_from_point(x, y, nonogram);
                error += std::abs(count);
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

int main() {
    using namespace std;
    nonogram_t nonogram = {
            5,
            5,
            {
                    0, 0, 0, 0, 0,
                    0, 0, 0, 0, 0,
                    0, 0, 0, 0, 0,
                    0, 0, 0, 0, 0,
                    0, 0, 0, 0, 0,},
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
    nonogram_t nonogram0 = {
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
    nonogram_t nonogram_solution = {
            3,
            3,
            {
                    -1, -1, 0,
                    -1, 0, -1,
                    0, 0, -1},
            2,
            1,
            {
                    0, 0, 2, 1, 2,
                    0, 2, 0, 0, 0,
                    1, 1, 0, 0, 0,
                    0, 1, 0, 0, 0
            }
    };
   cout << nonogram << endl;
    int n = 0;
    while (next_solution(nonogram)) {
//        if ((n % 10000) == 0) {
//            cout << n << " : " << evaluate(nonogram) << endl << nonogram << endl;
//        }
        cout << evaluate(nonogram) << endl;
        cout << nonogram << endl;
        if (evaluate(nonogram) == 0) {
            cout << nonogram << endl;
            break;
        }
        n++;
    }
//    cout << evaluate(nonogram_solution) << endl;
//    cout << nonogram_solution << endl;
    return 0;
}