// Reads standard ASCII text from a file
// Every char is converted into 2 Hamming codes; 4 bits of data per code; 14 bits overall per char
// Output is written to a newly created output file
#include <iostream>
#include "Eigen/Dense"
#include "Hamming.h"


//Encode class constructor
Encode::Encode(std::string file) : Hamming(file) {

    processFile();

}

Encode::~Encode(){}

//Encode a 4-bit message into a 7-bit hamming code
Eigen::Matrix<int, 1, 7> Encode::encodeMessage(const Eigen::Matrix<int, 1, 4>& message) const {

    //Encode the message using the generator matrix
    Eigen::Matrix<int, 1, 7> encodedMessage = message * generator.transpose();

    //Ensure result is binary
    encodedMessage = encodedMessage.unaryExpr([](int x) { return x % 2; });

    return encodedMessage;
}


void Encode::printEncodedMsg(Eigen::Matrix<int, 1, 7> encodedMessage) {
    std::cout << "\nEncoded Message: " << encodedMessage;
}

//Process input file, encode characters, and write to output file
void Encode::processFile() {
    std::ifstream inputFile(fileName, std::ios::in);
    if (!inputFile.is_open()) {
        std::cerr << "Error opening file: " << fileName << std::endl;
        return;
    }

    std::ofstream outputFile(fileName + "_out.txt", std::ios::out | std::ios::trunc);
    if (!outputFile.is_open()) {
        std::cerr << "Error creating output file" << std::endl;
        return;
    }

    char ch;
    while (inputFile.get(ch)) {
        //Convert char to binary in an Eigen matrix
        int asciiValue = static_cast<unsigned char>(ch); 

        Eigen::Matrix<int, 1, 8> binary;
        for (int i = 0; i < 8; ++i) {
            binary(0, 7 - i) = (asciiValue >> i) & 1; //Extract bits from the ASCII value
        }

        //Split into two 4-bit chunks
        Eigen::Matrix<int, 1, 4> msg1 = binary.block<1, 4>(0, 0); //Higher chunk
        Eigen::Matrix<int, 1, 4> msg2 = binary.block<1, 4>(0, 4); //Lower chunk

        //Encode each 4-bit message
        Eigen::Matrix<int, 1, 7> encodedMsg1 = encodeMessage(msg1);
        Eigen::Matrix<int, 1, 7> encodedMsg2 = encodeMessage(msg2);

        std::cout << "\nMessage 1: ";
        printEncodedMsg(encodedMsg1);
        std::cout << "\nMessage 2: ";
        printEncodedMsg(encodedMsg2);
        std::cout << std::endl;

        //Write encoded messages to output
        for (int i = 0; i < 7; ++i) {
            outputFile << encodedMsg1(0, i);
        }

        for (int i = 0; i < 7; ++i) {
            outputFile << encodedMsg2(0, i);
        }
        outputFile << "\n"; //New line for each character
    }

    inputFile.close();
    outputFile.close();
    std::cout << "Encoding complete. Output written to " + fileName + "_out.txt.\n";
}
