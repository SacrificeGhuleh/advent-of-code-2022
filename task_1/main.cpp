#include <cstdint>
#include <fstream>
#include <iostream>
#include <set>

struct Elf {
    Elf(uint32_t cals, size_t i) : calories{cals}, idx{i} {}

    bool operator<(const Elf& elf) const { return calories > elf.calories; }

    uint32_t calories = 0;
    size_t idx = 0;
};

int main() {
    // Create a text string, which is used to output the text file
    std::string input;

    // Read from the text file
    std::ifstream inputFile("data/input.txt");

    // Use a while loop together with the getline() function to read the file line by line
    uint32_t accumulation = 0;
    uint32_t maxCalories = 0;
    size_t idx = 0;

    std::set<Elf> elfs;

    while (std::getline(inputFile, input)) {
        // Output the text from the file

        try {
            accumulation += static_cast<uint32_t>(std::stoi(input));
        } catch (std::exception const& ex) {
            elfs.insert(Elf(accumulation, idx));
            maxCalories = std::max(maxCalories, accumulation);
            accumulation = 0;
            ++idx;
        }
    }

    elfs.insert(Elf(accumulation, idx));
    maxCalories = std::max(maxCalories, accumulation);
    accumulation = 0;
    ++idx;

    size_t count = 0;
    uint32_t accumulationOfTopThree = 0;
    for (auto const& elf : elfs) {
        if (++count > 3) {
            break;
        }
        accumulationOfTopThree += elf.calories;
    }

    std::cout << "Max calories: " << maxCalories << "\n";
    std::cout << "Sum of top three: " << accumulationOfTopThree << "\n";

    // Close the file
    inputFile.close();
}