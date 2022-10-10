#include <fstream>
#include <functional>
#include <iostream>
#include "json.hpp"
#include <memory>
#include <string>
#include <vector>
#include <conio.h>

int main(int argc, char **argv) {
    using namespace std;
    std::ifstream file(argv[1]);
    nlohmann::json data = nlohmann::json::parse(file);
    int h = data["h"];
    int w = data["w"];
    int x =0;

    string text = data["text"];

    int textCenter = text.size()/2;

    for (int i = 0; i < h; i++) {
        cout << "*";
        for (int l = 1; l < w - 1; l++) {
            if (i == (h-2)/2 && l >=  (w-2)/2 - textCenter && x <= text.size()) {
                cout << text[x];
                x++;
            }
            else if (i > 0 && i < h - 1) {
                cout << " ";
            }
            else
                cout << "*";}
        cout << "*\n";
    }
    return 0;
}