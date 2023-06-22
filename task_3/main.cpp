#include <array>
#include <cassert>
#include <cstdint>
#include <cstring>
#include <fstream>
#include <iostream>
#include <unordered_set>
#include <vector>

uint32_t getItemScore(char item) {
    if (item >= 'a' && item <= 'z') {
        return static_cast<uint32_t>(item - 'a' + 1);
    }
    if (item >= 'A' && item <= 'Z') {
        return static_cast<uint32_t>(item - 'A' + 27);
    }
    return 0;
}
struct Rucksack {
    Rucksack(const char* itms)
        : items{itms},
          compartments{std::string(itms, strlen(itms) / 2), std::string(itms + strlen(itms) / 2, strlen(itms) / 2)},
          duplicate{getDuplicate()},
          priority(getItemScore(duplicate)) {}

    std::unordered_set<char> heldItems;
    const std::string items;
    const std::string compartments[2];
    const char duplicate;
    const uint32_t priority;

private:
    char getDuplicate() {
        const std::string cmp0 = compartments[0];
        const std::string cmp1 = compartments[1];

        std::unordered_set<char> items0;
        std::unordered_set<char> items1;

        char ret = '\0';

        for (size_t i = 0; i < cmp0.length(); ++i) {
            const char ch0 = cmp0[i];
            const char ch1 = cmp1[i];

            if (items1.find(ch0) != items1.end()) {
                ret = ch0;
            }
            items0.insert(ch0);

            if (items0.find(ch1) != items0.end()) {
                ret = ch1;
            }
            items1.insert(ch1);

            heldItems.insert(ch0);
            heldItems.insert(ch1);
        }

        return ret;
    }
};

struct ElfGroup {
    ElfGroup(const Rucksack& r1, const Rucksack& r2, const Rucksack& r3)
        : elfs{r1, r2, r3}, duplicate{findDuplicate(elfs)}, priority(getItemScore(duplicate)) {}

    static char findDuplicate(const std::array<Rucksack, 3>& elfs) {
        const std::unordered_set<char>* mostItems = &elfs[0].heldItems;

        for (size_t i = 1; i < elfs.size(); i++) {
            const std::unordered_set<char>* testItems = &elfs[i].heldItems;
            if (mostItems->size() < testItems->size()) {
                mostItems = testItems;
            }
        }
        char commonItem = 0;

        for (const auto& item : *mostItems) {
            bool common = true;

            for (auto elf : elfs) {
                common &= elf.heldItems.find(item) != elf.heldItems.end();
            }

            if (common) {
                commonItem = item;
                break;
            }
        }

        return commonItem;
    }

    const std::array<Rucksack, 3> elfs;

    const char duplicate;
    const uint32_t priority;
};

void test() {
    const Rucksack testRucksacks[6] = {
        Rucksack("vJrwpWtwJgWrhcsFMMfFFhFp"), Rucksack("jqHRNqRjqzjGDLGLrsFMfFZSrLrFZsSL"),
        Rucksack("PmmdzqPrVvPwwTWBwg"),       Rucksack("wMqvLMZHhHMvwLHjbvcjnnSBnvTQFn"),
        Rucksack("ttgJtRGJQctTZtZT"),         Rucksack("CrZsJsPPZsGzwwsLwLmpwMDw")};
    const char expectedDulicates[6] = {
        'p', 'L', 'P', 'v', 't', 's',
    };

    const uint32_t expectedPriorities[6] = {16, 38, 42, 22, 20, 19};

    for (int i = 0; i < 6; i++) {
        assert(testRucksacks[i].duplicate == expectedDulicates[i]);
        assert(testRucksacks[i].priority == expectedPriorities[i]);
    }

    const char expectedBadges[2] = {'r', 'Z'};
    const uint32_t expectedBadgesPriorities[2] = {18, 52};

    for (int i = 0; i < 2; i++) {
        ElfGroup eg(testRucksacks[(i * 3)], testRucksacks[(i * 3) + 1], testRucksacks[(i * 3) + 2]);
        assert(eg.duplicate == expectedBadges[i]);
        assert(eg.priority == expectedBadgesPriorities[i]);
    }
}

int main() {
    test();

    // Create a text string, which is used to output the text file
    std::string input;

    // Read from the text file
    std::ifstream inputFile("data/input.txt");

    uint32_t overallScore = 0;
    uint32_t badgeScore = 0;

    std::vector<Rucksack> rucksacksBuffer;
    while (std::getline(inputFile, input)) {
        rucksacksBuffer.emplace_back(input.c_str());
        overallScore += rucksacksBuffer.at(rucksacksBuffer.size() - 1).priority;

        if (rucksacksBuffer.size() == 3) {
            ElfGroup elfGroup(rucksacksBuffer[0], rucksacksBuffer[1], rucksacksBuffer[2]);
            badgeScore += elfGroup.priority;
            rucksacksBuffer.clear();
        }
    }

    std::cout << "Overall Score: " << overallScore << "\n";
    std::cout << "Badge Score: " << badgeScore << "\n";
    // Close the file
    inputFile.close();
}
