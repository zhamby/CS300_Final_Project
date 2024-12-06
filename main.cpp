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
    Eigen::Matrix<int, 1, 4> message1, message2, message3, message4;

    //Examples
    message1 << 1, 0, 1, 0;  
    message2 << 0, 1, 1, 1;  
    message3 << 1, 1, 0, 0;  
    message4 << 0, 0, 1, 0;  


    Eigen::Matrix<int, 1, 7> encodedMessage1 = encoder.encodeMessage(message1);
    Eigen::Matrix<int, 1, 7> encodedMessage2 = encoder.encodeMessage(message2);
    Eigen::Matrix<int, 1, 7> encodedMessage3 = encoder.encodeMessage(message3);
    Eigen::Matrix<int, 1, 7> encodedMessage4 = encoder.encodeMessage(message4);


    std::cout << "1: ";
    encoder.printEncodedMsg(encodedMessage1);
    std::cout << "2: ";
    encoder.printEncodedMsg(encodedMessage2);
    std::cout << "3: ";
    encoder.printEncodedMsg(encodedMessage3);
    std::cout << "4: ";
    encoder.printEncodedMsg(encodedMessage4);
*/