#include "QuakeAssistController.h"
#include <iostream>
#include <fstream>
#include <string>

using namespace std;
// This main file is provided for you.
// It reads commands from the given input file and forwards them
// to the QuakeAssistController. You do NOT need to modify main.cpp.

int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    QuakeAssistController controller;
    string line;
    while (std::getline(cin, line)) {
        controller.parseAndExecute(line);
    }
    return 0;
}
