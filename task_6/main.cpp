#include <array>
#include <cassert>
#include <cstdint>
#include <cstring>
#include <deque>
#include <fstream>
#include <iostream>
#include <unordered_set>
#include <vector>

template <class T>
std::ostream& printer(std::ostream& os, const T& v) {
    os << "[";
    for (auto ii = v.begin(); ii != v.end(); ++ii) {
        os << " "
           << "\"" << *ii << "\"";
    }
    os << "]";
    return os;
}

template <class T>
std::ostream& operator<<(std::ostream& os, const std::deque<T>& v) {
    return printer(os, v);
}

template <class T>
std::ostream& operator<<(std::ostream& os, const std::unordered_set<T>& v) {
    return printer(os, v);
}

size_t getUniqueSubstrPosition(const std::string& str, size_t substrLen) {
    size_t idx = 0;
    std::deque<char> kvi;
    std::unordered_set<char> set;

    for (const auto& ch : str) {
        ++idx;
        kvi.emplace_back(ch);
        const bool dulicate = (set.find(ch) != set.end());

        if (dulicate) {
            char poped;
            do {
                poped = kvi.front();
                kvi.pop_front();
                set.erase(poped);
            } while (poped != ch);
        }

        if (kvi.size() == substrLen) {
            std::cout << substrLen << " unique chars found at idx " << idx << " " << kvi << "\n";
            break;
        }

        set.insert(ch);

        // std::cout << "Kurent char: " << ch << "\n";
        // std::cout << "Kurent kvi: " << kvi << "\n";
        // std::cout << "Kurent set: " << set << "\n";
        // std::cout << "Dulicate found: " << (dulicate ? "tru" : "fals") << "\n";
        // std::cout << "------------\n";
    }

    return idx;
}

void test() {
    assert(getUniqueSubstrPosition("mjqjpqmgbljsphdztnvjfqwrcgsmlb", 4) == 7);
    assert(getUniqueSubstrPosition("bvwbjplbgvbhsrlpgdmjqwftvncz", 4) == 5);
    assert(getUniqueSubstrPosition("nppdvjthqldpwncqszvftbrmjlhg", 4) == 6);
    assert(getUniqueSubstrPosition("nznrnfrfntjfmvfwmzdfjlvtqnbhcprsg", 4) == 10);
    assert(getUniqueSubstrPosition("zcfzfwzzqfrljwzlrfnpqdbhtmscgvjw", 4) == 11);

    assert(getUniqueSubstrPosition("mjqjpqmgbljsphdztnvjfqwrcgsmlb", 14) == 19);
    assert(getUniqueSubstrPosition("bvwbjplbgvbhsrlpgdmjqwftvncz", 14) == 23);
    assert(getUniqueSubstrPosition("nppdvjthqldpwncqszvftbrmjlhg", 14) == 23);
    assert(getUniqueSubstrPosition("nznrnfrfntjfmvfwmzdfjlvtqnbhcprsg", 14) == 29);
    assert(getUniqueSubstrPosition("zcfzfwzzqfrljwzlrfnpqdbhtmscgvjw", 14) == 26);
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
        getUniqueSubstrPosition(input, 4);
        getUniqueSubstrPosition(input, 14);
        // imput
    }
    // Close the file
    inputFile.close();
}
