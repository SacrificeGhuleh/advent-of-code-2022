#include <cstdint>
#include <fstream>
#include <iostream>
#include <tuple>

constexpr uint8_t baseScoreTable[3] = {1, 2, 3};

// A - Rock
// B - Paper
// C - Scissors

constexpr uint8_t scoreTable[3][3] = {
    /*     A  B  C*/
    /*A*/ {3, 6, 0},
    /*B*/ {0, 3, 6},
    /*C*/ {6, 0, 3},
};

// X - Lose
// Y - Draw
// Z - Win
constexpr char moveTable[3][3] = {
    /*     X     Y    Z*/
    /*A*/ {'C', 'A', 'B'},
    /*B*/ {'A', 'B', 'C'},
    /*C*/ {'B', 'C', 'A'},
};

constexpr std::pair<char, char> getWinningStrategy(std::pair<char, char> input) {
    const char opponent = input.first;
    const char result = input.second;
    const size_t resultIdx = static_cast<size_t>(result - 'X');
    const size_t opponentIdx = static_cast<size_t>(opponent - 'A');

    const char player = moveTable[opponentIdx][resultIdx];

    return {opponent, player};
}

constexpr uint8_t getScore(char opponent, char player) {
    const size_t playerIdx = static_cast<size_t>(player - 'A');
    const size_t opponentIdx = static_cast<size_t>(opponent - 'A');

    const uint8_t baseScore = baseScoreTable[playerIdx];
    const uint8_t gameScore = scoreTable[opponentIdx][playerIdx];

    return baseScore + gameScore;
}

constexpr uint8_t getScore(std::pair<char, char> input) {
    return getScore(input.first, input.second);
}

constexpr std::pair<char, char> parseInput(const char* line) {
    return {line[0], line[2]};
}

int main() {
    static_assert(parseInput("A Y").first == 'A');
    static_assert(parseInput("A Y").second == 'Y');

    static_assert(getWinningStrategy(parseInput("A Y")).second == 'A');
    static_assert(getWinningStrategy(parseInput("B Y")).second == 'B');
    static_assert(getWinningStrategy(parseInput("C Y")).second == 'C');

    static_assert(getWinningStrategy(parseInput("A X")).second == 'C');
    static_assert(getWinningStrategy(parseInput("B X")).second == 'A');
    static_assert(getWinningStrategy(parseInput("C X")).second == 'B');

    static_assert(getWinningStrategy(parseInput("A Z")).second == 'B');
    static_assert(getWinningStrategy(parseInput("B Z")).second == 'C');
    static_assert(getWinningStrategy(parseInput("C Z")).second == 'A');

    static_assert(getScore(getWinningStrategy(parseInput("A Y"))) == 4);
    static_assert(getScore(getWinningStrategy(parseInput("B X"))) == 1);
    static_assert(getScore(getWinningStrategy(parseInput("C Z"))) == 7);

    // Create a text string, which is used to output the text file
    std::string input;

    // Read from the text file
    std::ifstream inputFile("data/input.txt");

    uint32_t score = 0;

    while (std::getline(inputFile, input)) {
        score += getScore(getWinningStrategy(parseInput(input.c_str())));
    }

    std::cout << "Score: " << score << "\n";
    // Close the file
    inputFile.close();
}