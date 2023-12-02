#include <iostream>
#include <fstream>
#include <random>
#include <set>

int main() {
    // Define the range of random numbers
    const int minNumber = 1;
    const int maxNumber = 10000000;

    // Create a random number generator
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distribution(minNumber, maxNumber);

    // Create a set to store unique random numbers
    std::set<int> uniqueNumbers;

    // Generate unique random numbers
    while (uniqueNumbers.size() < 10000000) {
        int randomNumber = distribution(gen);
        uniqueNumbers.insert(randomNumber);
    }

    // Write the unique random numbers to a file
    std::ofstream outputFile("random_numbers.txt");
    if (outputFile.is_open()) {
        for (const int number : uniqueNumbers) {
            outputFile << number << "\n";
        }
        outputFile.close();
        std::cout << "Random numbers written to 'random_numbers.txt'" << std::endl;
    } else {
        std::cerr << "Unable to open the file for writing." << std::endl;
        return 1;
    }

    return 0;
}
