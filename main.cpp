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
        std::string fileName = "test" + std::to_string(i) + ".txt";
        hammingObjects.emplace_back(fileName); // Construct Hamming object with the filename
    }

    //Hamming object testing
    for (const auto& hamming : hammingObjects) {
        hamming.printOut(); //testing
    }
*/

 


    std::string eFileName = "test1.txt";
    std::string dFileName = "test1_out.txt";
 
    //Encoding testing
    Encode encoder(eFileName);  
    Decode decoder(dFileName);  


    return 0;
}




    /*  
        "HELLO" ASCII values: H = 72, E = 69, L = 76, L = 76, O = 79
        Hamming codes for HELLO
        10011001110000
        10011000100101
        10011000111100
        10011000111100
        10011001111111
    */