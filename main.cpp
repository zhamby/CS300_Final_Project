#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include "Hamming.h"
#include "Eigen/Dense"

int main() {
    for (int i = 1; i <= 2; ++i) {
        // Generate Filename
        std::string fileName = "test" + std::to_string(i);
        
        //Create objects to test Encode, Error Encode, and Decoding
        std::cout << "________ 1. Start Encoder ________\n";
        Encode encoder1(fileName + ".txt");  

        std::cout << "_____ 2. Start Error Encoder _____\n";
        ErrorEncode errorEncode2(fileName + ".txt"); //also calls Encode processFile()

        std::cout << "________ 3. Start Decoder ________\n";
        Decode decode3(fileName + "_out.txt");
        
        std::cout << "____ 4. Start Decoder (ERROR) ____\n";
        Decode decode4(fileName + "_e_out.txt");
    }
    return 0;
}
