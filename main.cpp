#include <iostream>
#include <vector>
#include <string>
#include "Hamming.h"
#include "Eigen/Dense"

int main() {
    // Vector to store Hamming objects
    std::vector<Hamming> hammingObjects;

/*
    // Populate 15 filenames and create Hamming objects
    for (int i = 1; i <= 15; ++i) {
        std::string filename = "test" + std::to_string(i) + ".txt";
        hammingObjects.emplace_back(filename); // Construct Hamming object with the filename
    }

    //Hamming object testing
    for (const auto& hamming : hammingObjects) {
        hamming.print(); //testing
    }
*/
    Hamming hammingCode;
    hammingCode.printOut();

    return 0;
}
