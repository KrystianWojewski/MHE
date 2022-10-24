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
        for (int x = 0; x < nonogram.width; x++) {
            auto &val = nonogram.board[y * nonogram.width + x];
            o << ((val > 0) ? "    " + to_string(val) : ((val < 0) ? "  ***" : "")) << "\t";
            if (y >= 2 && x >= 1) {
                o << "|";
            }
        }
        o << endl;
    }
    return o;
}

int main() {
    using namespace std;
    nonogram_t nonogram = {
            7,
            7,
            {
                    0, 0, 3, 3, 0, 1, 0,
                    0, 0, 1, 1, 2, 1, 1,
                    3, 1, 0, 0, 0,0, 0,
                    0, 4, 0, 0, 0,0, 0,
                    0,2,0,0, 0,0, 0,
                    0,1,0,0, 0, 0, 0,
                    0,2,0,0, 0, 0, 0}
    };
    nonogram_t nonogram_solution = {
            7,
            7,
            {
                    0, 0, 3, 3, 0, 1, 0,
                    0, 0, 1, 1, 2, 1, 1,
                    3, 1, -1, -1, -1,0, -1,
                    0, 4, -1, -1, -1,-1, 0,
                    0,2,-1,-1, 0,0, 0,
                    0,1,0,0, 0, -1, 0,
                    0,2,-1,-1, 0, 0, 0}
    };
    cout << nonogram << endl;
    cout << nonogram_solution;
    return 0;
}