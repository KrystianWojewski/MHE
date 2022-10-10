#include <iostream>
#include <vector>

struct nonogram_t{
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

std::ostream &operator<<(std::ostream &o, const nonogram_t &nonogram) {
    using namespace std;
    for (int y = 0; y < nonogram.height; y++) {
        o << "|";
        for (int x = 0; x < nonogram.width; x++) {
            auto &val = nonogram.board[y * nonogram.width + x];
            o << ((val > 0) ? to_string(val) : ((val < 0) ? "  ***" : "")) << "\t|";
        }
        o << endl;
    }
    return o;
}

int main() {
    using namespace std;
    nonogram_t nonogram = {
            4,
            4,
            {
                    0, 0, 0, 3,
                    0, 4, 0, 0,
                    5, 0, 6, 0,
                    0, 0, 0, 2}
    };
    nonogram_t nonogram_solution = {
            4,
            4,
            {
                    0, -1, -1, 3,
                    0, 4, -1, -1,
                    5, -1, 6, -1,
                    -1, 0, 0, 2}
    };
    cout << nonogram << endl;
    cout << nonogram_solution;
    return 0;
}