#include <iostream>
#include <vector>
#include <cmath>
#include <random>
struct puzzle_t {
    int width;
    int height;
    std::vector<int> board;
    int &get(const int x, const int y) {
        return board[y * width + x];
    }
    int get(const int x, const int y) const {
        if ((x < 0) || (x >= width) || (y < 0) || (y >= height)) throw std::invalid_argument("out of board");
        return board.at(y * width + x);
    }
};
/**
 * -1 values are in "bag"
 */
std::ostream &operator<<(std::ostream &o, const puzzle_t &puzzle) {
    using namespace std;
    for (int y = 0; y < puzzle.height; y++) {
        o << "|";
        for (int x = 0; x < puzzle.width; x++) {
            auto &val = puzzle.board[y * puzzle.width + x];
            o <<
              ((val > 0) ? to_string(val) :
               ((val < 0) ? "  ***" : "")
              )
              << "\t|";
        }
        o << endl;
    }
    return o;
}
int count_inside_bag_from_point(int x, int y, const puzzle_t &puzzle) {
    using namespace std;
    vector<pair<int, int>> directions = {{-1, 0},
                                         {1,  0},
                                         {0,  -1},
                                         {0,  1}};
    int sum = 1;
    for (auto [dx, dy]: directions) {
        int cx = x + dx, cy = y + dy;
        try {
            while ((puzzle.get(cx, cy)) != 0) {
                sum++;
                cx += dx;
                cy += dy;
            }
        } catch (...) {
        }
    }
    return sum;
}
int count_inconsistent(const puzzle_t &puzzle) {
    int error = 0;
    for (int x = 0; x < puzzle.width; x++)
        for (int y = 0; y < puzzle.height; y++) {
            if (puzzle.get(x, y) > 0) {
                int count = count_inside_bag_from_point(x, y, puzzle);
                error += std::abs(count - puzzle.get(x, y));
            }
        }
    return error;
}
double evaluate(const puzzle_t &puzzle) {
    return count_inconsistent(puzzle);
}
bool next_solution(puzzle_t &puzzle) {
    int i = 0;
    for (; i < puzzle.board.size(); i++) {
        if (puzzle.board[i] == 0) {
            puzzle.board[i] = -1;
            break;
        } else if (puzzle.board[i] == -1) {
            puzzle.board[i] = 0;
        }
    }
    return (i != puzzle.board.size());
}
std::vector<puzzle_t> generate_neighbours(const puzzle_t &p) {
    std::vector<puzzle_t> neighbours;
    for (int i = 0; i < p.board.size(); i++) {
        if (p.board[i] <= 0) {
            auto new_board = p;
            new_board.board[i] = -1 - new_board.board[i];
            neighbours.push_back(new_board);
        }
    }
    return neighbours;
}
puzzle_t generate_random_solution(const puzzle_t &p) {
    using namespace  std;
    static random_device rd;
    static mt19937 mt(rd());
    uniform_int_distribution<int> distr(-1,0);
    puzzle_t rand_sol = p;
    for (int i = 0; i < p.board.size(); i++) {
        if (p.board[i] <= 0) {
            auto new_board = p;
            rand_sol.board[i] = distr(mt);
        }
    }
    return rand_sol;
}
puzzle_t hill_climb_det(puzzle_t puzzle, int iterations) {
}
int main() {
    using namespace std;
    puzzle_t puzzle0 = {
            4,
            4,
            {
                    0, 0, 0, 3,
                    0, 4, -1, 0,
                    5, 0, 6, 0,
                    0, 0, 0, 2}
    };
    puzzle_t puzzle1 = {
            7,
            7,
            {
                    0, 0, 0,0,0,0, 3,
                    2,0,3,0,0,0,0,
                    0,0,0,0,7,13,0,
                    0,0,0,0,0,0,0,
                    0,6,2,0,0,0,0,
                    0,0,0,0,3,0,6,
                    4,0,0,0,0,0,0}
    };
    auto puzzle = puzzle0;
    cout << puzzle << endl;
    for (auto neighbour : generate_neighbours(puzzle)) {
        cout << " ------------------------------" << endl;
        cout << neighbour << endl;
    }
    cout << " ------------------------------" << endl;
    cout << generate_random_solution(puzzle) << endl;
/*
    int n = 0;
    while (next_solution(puzzle)) {
        if ((n % 10000) == 0) {
            cout << n << " : " << evaluate(puzzle) << endl << puzzle << endl;
        }
        if (evaluate(puzzle) == 0) {
            cout << puzzle << endl;
            break;
        }
        n++;
    }
*/
    return 0;
}