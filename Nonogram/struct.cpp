
#include "struct.h"

std::ostream &operator<<(std::ostream &o, const nonogram_t &nonogram) {
    using namespace std;
    for (int y = 0; y < nonogram.height + nonogram.top_params_height; y++) {
        for (int x = 0; x < nonogram.width + nonogram.left_params_width; x++) {
            auto &val = nonogram.params_board[y * (nonogram.width + nonogram.left_params_width) + x];
            if (y < nonogram.top_params_height) {
                o << ((val > 0) ? "    " + to_string(val) : "") << "\t";
            } else if (x < nonogram.left_params_width) {
                o << ((val > 0) ? "    " + to_string(val) : "") << "\t";
            }
            if (y >= nonogram.top_params_height && x >= nonogram.left_params_width) {
                auto &center_val = nonogram.board[(y - nonogram.top_params_height) * (nonogram.width) +
                                                  (x - nonogram.left_params_width)];
                o << "|" << ((center_val == -1) ? "  ***" : (center_val == -2) ? "   x" : "")
                  << ((x == nonogram.width + nonogram.left_params_width - 1) ? "\t|" : "\t");
            }
        }
        o << endl;
    }
    return o;
}

bool operator==(nonogram_t l, nonogram_t r) {
    if (l.width != r.width) return false;
    if (l.height != r.height) return false;
    for (unsigned i = 0; i < r.board.size(); i++) {
        if (l.board.at(i) != r.board.at(i)) return false;
    }
    return true;
}
