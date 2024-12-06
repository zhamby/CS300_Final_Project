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


void Encode::processFile() {
    std::ifstream inputFile(fileName, std::ios::in);
    if (!inputFile.is_open()) {
        std::cerr << "Error opening file: " << fileName << std::endl;
        return;
    }

    // Remove the ".txt" extension from the original file name
    std::string outputFileName = fileName.substr(0, fileName.find_last_of('.')) + "_out.txt";
    
    std::ofstream outputFile(outputFileName, std::ios::out | std::ios::trunc);
    if (!outputFile.is_open()) {
        std::cerr << "Error creating output file" << std::endl;
        return;
    }

    char ch;
    std::vector<Eigen::Matrix<int, 1, 7>> encodedMessages; // Vector to store encoded messages

    // Process the input file character by character
    while (inputFile.get(ch)) {
        // Convert the character to an 8-bit binary representation
        int asciiValue = static_cast<unsigned char>(ch);
        Eigen::Matrix<int, 1, 8> binary;
        for (int i = 0; i < 8; ++i) {
            binary(0, 7 - i) = (asciiValue >> i) & 1; // Extract bits from the ASCII value
        }

        // Split into two 4-bit chunks
        Eigen::Matrix<int, 1, 4> msg1 = binary.block<1, 4>(0, 0); // Higher 4 bits
        Eigen::Matrix<int, 1, 4> msg2 = binary.block<1, 4>(0, 4); // Lower 4 bits

        // Encode each 4-bit message into a 7-bit encoded message
        Eigen::Matrix<int, 1, 7> encodedMsg1 = encodeMessage(msg1);
        Eigen::Matrix<int, 1, 7> encodedMsg2 = encodeMessage(msg2);

        // Store encoded messages
        encodedMessages.push_back(encodedMsg1);
        encodedMessages.push_back(encodedMsg2);
    }

    // Output the encoded 7-bit messages in pairs of 7 bits
    int messageCount = 0;
    for (size_t i = 0; i < encodedMessages.size(); i += 2) {
        // Ensure that we don't go out of bounds
        if (i + 1 >= encodedMessages.size()) break;

        Eigen::Matrix<int, 1, 7> encodedMsg1 = encodedMessages[i];
        Eigen::Matrix<int, 1, 7> encodedMsg2 = encodedMessages[i + 1];

        // Output one line per pair of encoded 7-bit messages
        for (int j = 0; j < 7; ++j) {
            outputFile << encodedMsg1(0, j); // Write first 7-bit block
        }
        for (int j = 0; j < 7; ++j) {
            outputFile << encodedMsg2(0, j); // Write second 7-bit block
        }

        outputFile << "\n"; // Add a newline after every two 7-bit blocks (14 bits)
        
    } 

    inputFile.close();
    outputFile.close();

    std::cout << "Encoding complete. Output written to " + outputFileName + ".\n";
}
