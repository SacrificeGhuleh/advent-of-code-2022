#include <array>
#include <cassert>
#include <cstdint>
#include <cstring>
#include <fstream>
#include <iostream>
#include <unordered_set>
#include <vector>

void test() {
    assert(0 == 1);
}

int main() {
    std::cout << "Tests:\n";
    test();
    std::cout << "\nSolution:\n";

    // Create a text string, which is used to output the text file
    std::string input;

    // Read from the text file
    std::ifstream inputFile("data/input.txt");

    while (std::getline(inputFile, input)) {
        // imput
    }
    // Close the file
    inputFile.close();
}
