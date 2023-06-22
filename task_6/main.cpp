#include <iostream>
#include <cstring>
#include <cassert>
#include <unordered_set>
#include <fstream>
#include <cstdint>
#include <array>
#include <vector>
#include <deque>

template <class T>
std::ostream &printer(std::ostream &os, const T &v)
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

template <class T>
std::ostream &operator<<(std::ostream &os, const std::deque<T> &v)
{
    return printer(os, v);
}

template <class T>
std::ostream &operator<<(std::ostream &os, const std::unordered_set<T> &v)
{
    return printer(os, v);
}

size_t getMarkerPosition(const std::string &str)
{
    size_t idx = 0;
    std::deque<char> kvi;
    std::unordered_set<char> set;

    for (const auto &ch : str)
    {
        ++idx;
        kvi.emplace_back(ch);
        const bool dulicate = (set.find(ch) != set.end());

        if (dulicate)
        {
            char poped;
            do
            {
                poped = kvi.front();
                kvi.pop_front();
                set.erase(poped);
            } while (poped != ch);
        }

        if (kvi.size() == 4)
        {
            std::cout << "Found at idx " << idx << " " << kvi << "\n";
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

void test()
{
    assert(getMarkerPosition("mjqjpqmgbljsphdztnvjfqwrcgsmlb") == 7);
    assert(getMarkerPosition("bvwbjplbgvbhsrlpgdmjqwftvncz") == 5);
    assert(getMarkerPosition("nppdvjthqldpwncqszvftbrmjlhg") == 6);
    assert(getMarkerPosition("nznrnfrfntjfmvfwmzdfjlvtqnbhcprsg") == 10);
    assert(getMarkerPosition("zcfzfwzzqfrljwzlrfnpqdbhtmscgvjw") == 11);
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
        getMarkerPosition(input);
        // imput
    }
    // Close the file
    inputFile.close();
}
