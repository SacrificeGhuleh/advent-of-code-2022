#include <iostream>
#include <cstring>
#include <cassert>
#include <unordered_set>
#include <fstream>
#include <cstdint>
#include <array>
#include <vector>

void test()
{
    assert(0 == 1);
}

int main()
{
    test();

    // Create a text string, which is used to output the text file
    std::string input;

    // Read from the text file
    std::ifstream inputFile("data/input.txt");

    while (std::getline(inputFile, input))
    {
        // imput
    }
    // Close the file
    inputFile.close();
}
