#include <iostream>
#include <vector>
struct puzzle_t {
    int width;
    int height;
    std::vector<int> board;
    int &get(const int x, const int y) {
        return board[y*width+x];
    }
    int get(const int x, const int y) const {
        return board.at(y*width+x);
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
int count_inconsistent(puzzle_t & puzzle) {
    bool first_seen = false;
//    for (int y = 0; y < puzzle.width)
// TODO: kolejny wyklad
}
double evaluate(const puzzle_t & puzzle) {
}
int main() {
    using namespace std;
    puzzle_t puzzle = {
            4,
            4,
            {
                    0, 0, 0, 3,
                    0, 4, 0, 0,
                    5, 0, 6, 0,
                    0, 0, 0, 2}
    };
    puzzle_t puzzle_solution = {
            4,
            4,
            {
                    0, -1, -1, 3,
                    0, 4, -1, -1,
                    5, -1, 6, -1,
                    -1, 0, 0, 2}
    };
    cout << puzzle << endl;
    cout << puzzle_solution;
    return 0;
}