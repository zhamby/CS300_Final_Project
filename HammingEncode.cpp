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


void Encode::processFile() {
    // Read the original message using the helper function
    std::string originalMessage = getOriginalMessage(fileName);
    if (originalMessage.empty()) {
        std::cerr << "Failed to read original message." << std::endl;
        return;
    }

    // Print the original message to the console
    std::cout << "Original Message: " << originalMessage << std::endl;

    // Remove the ".txt" extension from the original file name
    std::string outputFileName = fileName.substr(0, fileName.find_last_of('.')) + "_out.txt";
    
    std::ofstream outputFile(outputFileName, std::ios::out | std::ios::trunc);
    if (!outputFile.is_open()) {
        std::cerr << "Error creating output file" << std::endl;
        return;
    }

    // Process the input file character by character
    std::ifstream inputFile(fileName, std::ios::in);
    if (!inputFile.is_open()) {
        std::cerr << "Error opening file: " << fileName << std::endl;
        return;
    }

    std::string line;
    while (std::getline(inputFile, line)) {
        if (line.empty()) continue;

        // Process each character in the line
        for (char ch : line) {
            // Convert the character to an 8-bit binary representation
            Eigen::Matrix<int, 1, 8> binary = charToBinary(ch);

            // Split the binary into two 4-bit chunks
            auto [msg1, msg2] = splitBinary(binary);

            // Encode each 4-bit chunk
            Eigen::Matrix<int, 1, 7> encodedMsg1 = encodeMessage(msg1);
            Eigen::Matrix<int, 1, 7> encodedMsg2 = encodeMessage(msg2);

            // Write the encoded 14-bit message to the output file
            for (int j = 0; j < 7; ++j) {
                outputFile << encodedMsg1(0, j); // First 7 bits
            }
            for (int j = 0; j < 7; ++j) {
                outputFile << encodedMsg2(0, j); // Next 7 bits
            }

            outputFile << "\n"; // Newline after every 14-bit message
        }
    }

    inputFile.close();
    outputFile.close();
    std::cout << "Encoding complete. Output written to " + outputFileName + ".\n";
}


//Encode a 4-bit message into a 7-bit hamming code
Eigen::Matrix<int, 1, 7> Encode::encodeMessage(const Eigen::Matrix<int, 1, 4>& message) const {

    //Encode the message using the generator matrix
    Eigen::Matrix<int, 1, 7> encodedMessage = message * generator.transpose();

    //Ensure result is binary
    encodedMessage = encodedMessage.unaryExpr([](int x) { return x % 2; });

    return encodedMessage;
}


// Helper function to convert a character to its 8-bit binary representation
Eigen::Matrix<int, 1, 8> Encode::charToBinary(char ch) {
    int asciiValue = static_cast<unsigned char>(ch);
    Eigen::Matrix<int, 1, 8> binary;
    for (int i = 0; i < 8; ++i) {
        binary(0, 7 - i) = (asciiValue >> i) & 1; // Extract bits from the ASCII value
    }
    return binary;
}

// Helper function to split 8-bit binary into two 4-bit chunks
std::pair<Eigen::Matrix<int, 1, 4>, Eigen::Matrix<int, 1, 4>> Encode::splitBinary(const Eigen::Matrix<int, 1, 8>& binary) {
    Eigen::Matrix<int, 1, 4> msg1 = binary.block<1, 4>(0, 0); // Higher 4 bits
    Eigen::Matrix<int, 1, 4> msg2 = binary.block<1, 4>(0, 4); // Lower 4 bits
    return {msg1, msg2};
}


std::string Encode::getOriginalMessage(const std::string& fileName) {
    std::ifstream inputFile(fileName, std::ios::in);
    if (!inputFile.is_open()) {
        std::cerr << "Error opening file: " << fileName << std::endl;
        return "";
    }

    std::string originalMessage;
    std::string line;
    while (std::getline(inputFile, line)) {
        if (!line.empty()) {
            originalMessage += line;  // Append each character (line) to the message
        }
    }

    inputFile.close();
    return originalMessage;
}