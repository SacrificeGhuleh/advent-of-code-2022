#include <iostream>
#include <cstring>
#include <cassert>
#include <unordered_set>
#include <fstream>
#include <cstdint>
#include <array>
#include <algorithm>
#include <vector>
#include <regex>
#include <deque>

const std::regex moveRgx("^move\\s*(\\d+)\\s*from\\s*(\\d+)\\s*to\\s*(\\d+)\\s*$");
const std::regex crateRgx("^( *\\[\\w\\] *)+$");

std::ostream &operator<<(std::ostream &os, const std::smatch &v)
{
    os << "[";
    for (auto ii = v.begin(); ii != v.end(); ++ii)
    {
        os << " "
           << "\"" << *ii << "\"";
    }
    os << "]";
    return os;
}

struct Command
{
    constexpr Command(int moveCount = -1,
                      int srcCol = -1,
                      int dstCol = -1) : moveCount{moveCount}, srcCol{srcCol - 1}, dstCol{dstCol - 1} {}

    const int moveCount;
    const int srcCol;
    const int dstCol;
};

inline size_t getNumberOfColumns(const std::string &str)
{
    return (str.length() + 1) / 4;
}

Command parseCommand(const std::string &str)
{
    std::smatch matches;

    if (std::regex_search(str, matches, moveRgx))
    {
        return Command(std::stoi(matches.str(1)), std::stoi(matches.str(2)), std::stoi(matches.str(3)));
    }
    return Command();
}

struct CrateHandler
{
    void addRow(const std::string &str)
    {
        std::smatch match;
        if (std::regex_match(str, match, moveRgx))
        {
            commands.emplace_back(parseCommand(str));
        }
        else if (std::regex_match(str, match, crateRgx))
        {
            const size_t columns = getNumberOfColumns(str);
            numberOfCols = std::max(numberOfCols, columns);

            while (crateStacks.size() < numberOfCols)
            {
                crateStacks.emplace_back(std::deque<char>());
            }

            for (size_t i = 0; i < numberOfCols; i++)
            {
                const size_t idx = (4 * i) + 1;
                const char ch = str[idx];
                if (ch != ' ')
                {
                    crateStacks[i].push_back(ch);
                }
            }
        }
    }

    void processCommands()
    {
        for (const auto &command : commands)
        {
            for (int i = 0; i < command.moveCount; i++)
            {
                crateStacks.at(command.dstCol).push_front(crateStacks.at(command.srcCol).front());
                crateStacks.at(command.srcCol).pop_front();
            }
        }
    }

    void printTopCrates()
    {
        for (const auto &dq : crateStacks)
        {
            std::cout << dq.front();
        }
        std::cout << "\n";
    }

    size_t numberOfCols{0};
    std::vector<Command> commands;
    std::vector<std::deque<char>> crateStacks;
};

void test()
{
    std::string testInput[] = {
        "    [D]    ",
        "[N] [C]    ",
        "[Z] [M] [P]",
        " 1   2   3 ",
        "",
        "move 1 from 2 to 1",
        "move 3 from 1 to 3",
        "move 2 from 2 to 1",
        "move 1 from 1 to 2"};

    Command testCommand = parseCommand("move 1 from 2 to 3");

    assert(testCommand.moveCount == 1);
    assert(testCommand.srcCol == 1);
    assert(testCommand.dstCol == 2);

    assert(getNumberOfColumns("[A]") == 1);
    assert(getNumberOfColumns("[A] [B]") == 2);
    assert(getNumberOfColumns("[A] [B] [C]") == 3);
    assert(getNumberOfColumns("[A] [B] [C] [D]") == 4);

    CrateHandler parser;
    for (const auto &str : testInput)
    {
        parser.addRow(str);
    }

    parser.processCommands();

    parser.printTopCrates();
}

int main()
{
    test();

    // Create a text string, which is used to output the text file
    std::string input;

    // Read from the text file
    std::ifstream inputFile("data/input.txt");
    CrateHandler parser;

    while (std::getline(inputFile, input))
    {
        parser.addRow(input);
    }
    // Close the file
    inputFile.close();

    parser.processCommands();

    parser.printTopCrates();
}
