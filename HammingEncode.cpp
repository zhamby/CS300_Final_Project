// Reads standard ASCII text from a file
// Every char is converted into 2 Hamming codes; 4 bits of data per code; 14 bits overall per char
// Output is written to a newly created output file

#include <iostream>
#include "Eigen/Dense"
#include "Hamming.h"



Encode::Encode(std::string file) : Hamming(file) {}
Encode::~Encode() {}


Eigen::Matrix<int, 1, 7> Encode::encodeMessage(Eigen::Matrix<int, 1, 4> message) {
    //Multiply the message by the generator matrix
    Eigen::Matrix<int, 1, 7> encodedMessage = message * generator;

    //Apply modulo 2 
    encodedMessage = encodedMessage.unaryExpr([](int x) { return x % 2; });

    return encodedMessage;
}


void Encode::printEncodedMsg(Eigen::Matrix<int, 1, 7> encodedMessage) {
    std::cout << "Encoded Message: " << encodedMessage << std::endl;
}
