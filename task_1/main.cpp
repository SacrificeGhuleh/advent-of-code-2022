#include <iostream>
#include <cstdint>
#include <fstream>

int main()
{
    // Create a text string, which is used to output the text file
    std::string input;

    // Read from the text file
    std::ifstream inputFile("data/input.txt");

    // Use a while loop together with the getline() function to read the file line by line
    uint32_t accumulation = 0;
    uint32_t max_calories = 0;
    while (std::getline(inputFile, input))
    {
        // Output the text from the file

        try
        {
            // std::cout << std::stoi(input) << "\n";
            accumulation += std::stoi(input);
        }
        catch (std::exception const &ex)
        {
            max_calories = std::max(max_calories, accumulation);
            accumulation = 0;
        }
    }
    max_calories = std::max(max_calories, accumulation);
    accumulation = 0;

    std::cout << "Max calories: " << max_calories << "\n";

    // Close the file
    inputFile.close();
}