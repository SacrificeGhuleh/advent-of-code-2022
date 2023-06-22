#include <array>
#include <cassert>
#include <cstdint>
#include <cstring>
#include <fstream>
#include <iostream>
#include <regex>
#include <unordered_set>
#include <vector>

const std::regex rgx("^(\\d+)-(\\d+),(\\d+)-(\\d+)$");

std::ostream& operator<<(std::ostream& os, const std::smatch& v) {
    os << "[";
    for (auto ii = v.begin(); ii != v.end(); ++ii) {
        os << " " << *ii;
    }
    os << "]";
    return os;
}

struct ElfPair {
    ElfPair(const std::pair<size_t, size_t>& e1, const std::pair<size_t, size_t>& e2)
        : e1{e1}, e2{e2}, subset{isSubset(e1, e2)}, overlap{isOverlap(e1, e2)} {}

    const std::pair<size_t, size_t> e1;
    const std::pair<size_t, size_t> e2;
    const bool subset;
    const bool overlap;

    static bool isSubset(const std::pair<size_t, size_t>& e1,
                         const std::pair<size_t, size_t>& e2,
                         bool recurse = true) {
        bool ret = e2.first >= e1.first && e2.second <= e1.second;
        if (recurse) {
            ret |= isSubset(e2, e1, false);
        }
        return ret;
    }

    static bool isOverlap(const std::pair<size_t, size_t>& e1,
                          const std::pair<size_t, size_t>& e2,
                          bool recurse = true) {
        bool ret = e1.first <= e2.first && e2.first <= e1.second;
        if (recurse) {
            ret |= isOverlap(e2, e1, false);
        }
        return ret | isSubset(e1, e2);
    }
};

void test() {
    ElfPair elfPairs[] = {
        ElfPair({2, 4}, {6, 8}), ElfPair({2, 3}, {4, 5}), ElfPair({5, 7}, {7, 9}),
        ElfPair({2, 8}, {3, 7}), ElfPair({6, 6}, {4, 6}), ElfPair({2, 6}, {4, 8}),
    };

    bool isSubset[] = {
        false, false, false, true, true, false,
    };

    bool isOverlap[] = {
        false, false, true, true, true, true,
    };

    for (int i = 0; i < 6; i++) {
        assert(elfPairs[i].subset == isSubset[i]);
        assert(elfPairs[i].overlap == isOverlap[i]);
    }

    std::smatch matches;
    std::string str = "2-8,3-7";

    if (std::regex_search(str, matches, rgx)) {
        ElfPair elfPair({std::stoi(matches[1].str()), std::stoi(matches[2].str())},
                        {std::stoi(matches[3].str()), std::stoi(matches[4].str())});

        std::cout << "Matches: " << matches << "\n";

        std::cout << "elfPair.e1.first " << elfPair.e1.first << "\n";
        std::cout << "elfPair.e1.second " << elfPair.e1.second << "\n";
        std::cout << "elfPair.e2.first " << elfPair.e2.first << "\n";
        std::cout << "elfPair.e2.second " << elfPair.e2.second << "\n";

        assert(elfPair.e1.first == 2);
        assert(elfPair.e1.second == 8);
        assert(elfPair.e2.first == 3);
        assert(elfPair.e2.second == 7);

        assert(elfPair.subset == true);
    }
}

int main() {
    test();

    // Create a text string, which is used to output the text file
    std::string input;

    // Read from the text file
    std::ifstream inputFile("data/input.txt");

    std::smatch matches;

    size_t subsets = 0;
    size_t overlaps = 0;

    while (std::getline(inputFile, input)) {
        if (std::regex_search(input, matches, rgx)) {
            ElfPair elfPair({std::stoi(matches[1].str()), std::stoi(matches[2].str())},
                            {std::stoi(matches[3].str()), std::stoi(matches[4].str())});
            subsets += elfPair.subset;
            overlaps += elfPair.overlap;
        }
    }
    // Close the file
    inputFile.close();

    std::cout << "Number of subsets: " << subsets << "\n";
    std::cout << "Number of overlaps: " << overlaps << "\n";
}
