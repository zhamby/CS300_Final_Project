#include "Eigen/Dense"
#include <iostream>
#include <vector>
#include <string>
#include "Hamming.h"
#include "Eigen/Dense"

int main() {
    // Vector to store Hamming objects
    std::vector<Hamming> hammingObjects;



    // Populate 15 filenames and create Hamming objects
    for (int i = 1; i <= 15; ++i) {
        std::string fileName = "test" + std::to_string(i) + ".txt";
        hammingObjects.emplace_back(fileName); // Construct Hamming object with the filename
    }

    //Hamming object testing
    for (const auto& hamming : hammingObjects) {
        hamming.printOut(); //testing
    }



    return 0;
}
