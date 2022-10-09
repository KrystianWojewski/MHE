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
    std::ifstream file("data/zadanie1.json");
    nlohmann::json data = nlohmann::json::parse(file);
    int h = data["h"];
    int w = data["w"];

    string text = data["text"];

    for (int i = 0; i < h; i++) {
        cout << "*";
        for (int l = 1; l < w - 1; l++) {
            if (i == 1 && l <= text.size()) {
                cout << text;
                break;
            }
            else if (i > 0 && i < h - 1) {
                cout << " ";
            }
            else
                cout << "*";
        }
        cout << "*\n";
    }
    return 0;
}