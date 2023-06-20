#include <iostream>
#include <cstring>
#include <cassert>
#include <unordered_set>
#include <fstream>
#include <cstdint>

struct Rucksack
{
    Rucksack(const char *items) : items{items},
                                  compartments{std::string(items, strlen(items) / 2), std::string(items + strlen(items) / 2, strlen(items) / 2)},
                                  duplicate{getDuplicate(compartments)},
                                  priority(getDuplicateScore(duplicate))
    {
    }

    const std::string items;
    const std::string compartments[2];
    const char duplicate;
    const int priority;

    static int getDuplicateScore(char duplicate)
    {
        if (duplicate >= 'a' && duplicate <= 'z')
        {
            return duplicate - 'a' + 1;
        }
        if (duplicate >= 'A' && duplicate <= 'Z')
        {
            return duplicate - 'A' + 27;
        }
        return 0;
    }

    static char getDuplicate(const std::string *compartments)
    {
        const std::string cmp0 = compartments[0];
        const std::string cmp1 = compartments[1];

        std::unordered_set<char> items0;
        std::unordered_set<char> items1;

        for (int i = 0; i < cmp0.length(); ++i)
        {
            const char ch0 = cmp0[i];
            const char ch1 = cmp1[i];

            if (items1.find(ch0) != items1.end())
            {
                return ch0;
            }
            items0.insert(ch0);

            if (items0.find(ch1) != items0.end())
            {
                return ch1;
            }
            items1.insert(ch1);
        }

        return 0;
    }
};

void test()
{

    const Rucksack testRucksacks[6] = {
        Rucksack("vJrwpWtwJgWrhcsFMMfFFhFp"),
        Rucksack("jqHRNqRjqzjGDLGLrsFMfFZSrLrFZsSL"),
        Rucksack("PmmdzqPrVvPwwTWBwg"),
        Rucksack("wMqvLMZHhHMvwLHjbvcjnnSBnvTQFn"),
        Rucksack("ttgJtRGJQctTZtZT"),
        Rucksack("CrZsJsPPZsGzwwsLwLmpwMDw")};
    const char expectedDulicates[6] = {
        'p',
        'L',
        'P',
        'v',
        't',
        's',
    };
    const int expectedPriorities[6] = {16, 38, 42, 22, 20, 19};

    for (int i = 0; i < 6; i++)
    {
        assert(testRucksacks[i].duplicate == expectedDulicates[i]);
        assert(testRucksacks[i].priority == expectedPriorities[i]);
    }
}

int main()
{
    test();

    // Create a text string, which is used to output the text file
    std::string input;

    // Read from the text file
    std::ifstream inputFile("data/input.txt");

    uint32_t score = 0;

    while (std::getline(inputFile, input))
    {
        Rucksack rucksack(input.c_str());
        score += rucksack.priority;
    }

    std::cout << "Score: " << score << "\n";
    // Close the file
    inputFile.close();
}
