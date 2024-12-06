// Inherit from Hamming Encode
// Encodes text from file to hamming code binary with errors
// Include errorEncodeStatic, which manipulates a specific bit
// Include errorEncodeRand, which manipulates rand bit per hamming code
// All output pushed to output file

#include <string>
#include <iostream>
#include <fstream>
#include <random>
#include "Hamming.h"


ErrorEncode::ErrorEncode(std::string file) : Encode(file) {

    encodeFile();
}
ErrorEncode::~ErrorEncode(){}

  
void ErrorEncode::encodeFile() {
    std::cout << "ErrorEncode::processFile() is called!" << std::endl;
    Encode::processFile();  // Call base class processFile

    // Now introduce errors into the encoded messages
    errorEncodeRand();  // Introduce random errors

    // Now output to errors.txt
    std::string outputFileName = fileName.substr(0, fileName.find_last_of('.')) + "_errors.txt";
    std::ofstream outputFile(outputFileName, std::ios::out | std::ios::trunc);
    if (!outputFile.is_open()) {
        std::cerr << "Error creating output file" << std::endl;
        return;
    }

    // Check if the encoded messages with errors exist
    std::cout << "Writing errors to " << outputFileName << std::endl;
    const auto& hammingWithErrors = hammingCodeWithErrors;
    for (size_t i = 0; i < hammingWithErrors.size(); i += 2) {
        const auto& code1 = hammingWithErrors[i];
        const auto& code2 = hammingWithErrors[i + 1];

        // Write the 7-bit codes with errors
        for (int j = 0; j < 7; ++j) {
            outputFile << code1(0, j);  // First 7 bits
        }
        for (int j = 0; j < 7; ++j) {
            outputFile << code2(0, j);  // Next 7 bits
        }

        outputFile << "\n";
    }

    outputFile.close();
    std::cout << "Error encoding complete. Output written to " << outputFileName << ".\n";
}

// Introduces a static error by flipping a specific bit in a given code block
void ErrorEncode::errorEncodeStatic(size_t codeIndex, int bitPos) {
    const auto& encodedMessages = this->getEncodedMessages();  // Use getter to access encodedMessages
    if (codeIndex >= encodedMessages.size() || bitPos < 0 || bitPos >= 7) {
        std::cerr << "Invalid code index or bit position" << std::endl;
        return;
    }
    hammingCodeWithErrors = encodedMessages; // Copy the encoded messages
    hammingCodeWithErrors[codeIndex](0, bitPos) ^= 1; // Flip the specific bit
}

// Introduces a random error in each Hamming code block
void ErrorEncode::errorEncodeRand() {
    // Access the encoded messages without const reference
    auto& encodedMessages = this->getEncodedMessages();  // Now it's a reference

    hammingCodeWithErrors = encodedMessages; // Copy the encoded messages

    std::cout << "Introducing random errors..." << std::endl;

    // Iterate through each encoded message and introduce a random error
    for (auto& encodedMessage : encodedMessages) {
        // Randomly introduce an error (flip a bit in each encoded message)
        int bitPos = rand() % 7;  // Random bit position to flip
        encodedMessage(0, bitPos) = (encodedMessage(0, bitPos) == 0) ? 1 : 0;  // Flip the bit
        hammingCodeWithErrors.push_back(encodedMessage);  // Store it with errors
    }

    std::cout << "Random errors introduced." << std::endl;
}