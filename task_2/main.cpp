#include <iostream>
#include <cstdint>
#include <fstream>
#include <tuple>

constexpr uint8_t baseScoreTable[3] = {1, 2, 3};

constexpr uint8_t scoreTable[3][3] = {
    {3, 6, 0},
    {0, 3, 6},
    {6, 0, 3},
};

constexpr uint8_t getScore(char opponent, char player)
{
    const size_t playerIdx = player - 'X';
    const size_t opponentIdx = opponent - 'A';

    const uint8_t baseScore = baseScoreTable[playerIdx];
    const uint8_t gameScore = scoreTable[opponentIdx][playerIdx];

    return baseScore + gameScore;
}

constexpr uint8_t getScore(std::pair<char, char> input)
{
    return getScore(input.first, input.second);
}

constexpr std::pair<char, char> parseInput(const char *line)
{
    return {line[0], line[2]};
}

int main()
{
    static_assert(getScore('A', 'Y') == 8);
    static_assert(getScore('B', 'X') == 1);
    static_assert(getScore('C', 'Z') == 6);

    static_assert(parseInput("A Y").first == 'A');
    static_assert(parseInput("A Y").second == 'Y');

    static_assert(getScore(parseInput("A Y")) == 8);
    static_assert(getScore(parseInput("B X")) == 1);
    static_assert(getScore(parseInput("C Z")) == 6);

    // Create a text string, which is used to output the text file
    std::string input;

    // Read from the text file
    std::ifstream inputFile("data/input.txt");

    uint32_t score = 0;

    while (std::getline(inputFile, input))
    {
        score += getScore(parseInput(input.c_str()));
    }

    std::cout << "Score: " << score << "\n";
    // Close the file
    inputFile.close();
}