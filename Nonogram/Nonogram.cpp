#include <iostream>
#include <iomanip>
#include <map>
#include <list>
#include <functional>
#include <ctime>
#include <fstream>

#include "tp_args.hpp"
#include "funcs.h"
#include "struct.h"
#include "json.hpp"

int main(int argc, char **argv) {
    using namespace std;
    using namespace tp::args;

//    nonogram_t nonogram0 = {
//            3,
//            3,
//            {
//                    0, 0, 0,
//                    0, 0, 0,
//                    0, 0, 0},
//            2,
//            1,
//            {
//                    0, 0, 2, 1, 2,
//                    0, 2, 0, 0, 0,
//                    1, 1, 0, 0, 0,
//                    0, 1, 0, 0, 0}};
//
//    nonogram_t nonogram1 = {
//            5,
//            5,
//            {
//                    0, 0, 0, 0, 0,
//                    0, 0, 0, 0, 0,
//                    0, 0, 0, 0, 0,
//                    0, 0, 0, 0, 0,
//                    0, 0, 0, 0, 0},
//            3,
//            2,
//            {
//                    0, 0, 0, 0, 0, 1, 0, 0,
//                    0, 0, 0, 3, 3, 3, 2, 1,
//                    1, 1, 1, 0, 0, 0, 0, 0,
//                    0, 0, 1, 0, 0, 0, 0, 0,
//                    0, 0, 3, 0, 0, 0, 0, 0,
//                    0, 0, 3, 0, 0, 0, 0, 0,
//                    0, 0, 3, 0, 0, 0, 0, 0}};

    auto help = arg(argc, argv, "help", false);
    auto method = arg(argc, argv, "method", std::string("tabu_search"),
                      "Opt. method. Available are: brute_force tabu_search "
                      "random_probe hill_climb_det.");
    auto iterations =
            arg(argc, argv, "iterations", 100, "Maximal number of iterations.");
    auto nonogram = arg(argc, argv, "nonogram", string("data/condition.json"), "File with informations about board");
    auto do_chart = arg(argc, argv, "do_chart", false, "Show chart.");
    auto show_iter = arg(argc, argv, "show_iter", false, "Show iterations.");
    auto show_calls = arg(argc, argv, "show_calls", false, "Show function calls.");
    auto tabu_size = arg(argc, argv, "tabu_size", 1000, "Maximal number tabu list.");
    auto print_result =
            arg(argc, argv, "print_result", false, "Show the result.");
    auto print_result_eval = arg(argc, argv, "print_result_eval", false,
                                 "Show the evaluation result.");
    auto print_time = arg(argc, argv, "print_time", false,
                          "Show algorithm time.");
    if (help) {
        std::cout << "help screen.." << std::endl;
        args_info(std::cout);
        return 0;
    }

    std::ifstream file(nonogram);
    nlohmann::json conditions = nlohmann::json::parse(file);

    int width = conditions["width"];
    int height = conditions["height"];
    vector<int> board(conditions["board"].begin(), conditions["board"].end());
    int left_params_width = conditions["left_params_width"];
    int top_params_height = conditions["top_params_height"];
    vector<int> params_board(conditions["params_board"].begin(), conditions["params_board"].end());

    nonogram_t nonogram_board = {
            width,
            height,
            board,
            left_params_width,
            top_params_height,
            params_board
    };

    map<string, function<nonogram_t(nonogram_t, int, bool, bool) >> methods = {
            {"brute_force",    brute_force},
            {"random_probe",   random_sampling},
            {"hill_climb_det", hill_climb_det},
            {"tabu_search",    [&](nonogram_t p, int n, bool d, bool e) { return tabu_search(p, n, d, e,  tabu_size); }},
            {"annealing",      [](nonogram_t p, int n, bool d, bool e) { return annealing(p, n, d, e); }}
    };

    clock_t start = clock();
    nonogram_t result = methods.at(method)(nonogram_board, iterations, do_chart, show_iter);
    long algorithm_time = clock() - start;

    if (print_result)
        cout << result << endl;
    if (print_result_eval)
        cout << "erros: " << evaluate(result) << endl;
    if (show_calls)
        cout << "calls: " << calls << endl;
    if (print_time)
        cout << "algorithm_time: " << algorithm_time << " miliseconds" << endl;
}