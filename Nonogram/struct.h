#ifndef MHE_STRUCT_H
#define MHE_STRUCT_H

#include <vector>
#include <iostream>

struct nonogram_t {
    int width;
    int height;
    std::vector<int> board;
    int left_params_width;
    int top_params_height;
    std::vector<int> params_board;


    int &get(const int x, const int y) {
        return board[y * width + x];
    }

    int get_from_param(const int x, const int y) const {
        if ((x < 0) || (x >= width + left_params_width) || (y < 0) || (y >= height + top_params_height)) {
            throw std::invalid_argument("out of board");
        } else if ((x >= left_params_width && y < top_params_height) ||
                   (x < left_params_width && y >= top_params_height)) {
            return params_board.at(y * (width + left_params_width) + x);
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
    }
};

std::ostream &operator<<(std::ostream &o, const nonogram_t &nonogram);
bool operator==(nonogram_t l, nonogram_t r);

#endif
