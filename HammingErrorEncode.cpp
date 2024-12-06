// Inherit from Hamming Encode
// Encodes text from file to hamming code binary with errors
// Include errorEncodeStatic, which manipulates a specific bit
// Include errorEncodeRand, which manipulates rand bit per hamming code
// All output pushed to output file

#include <string>
#include <iostream>
#include <fstream>
#include <random>
#include <cstdlib>
#include <ctime>
#include "Hamming.h"


ErrorEncode::ErrorEncode(std::string file) : Encode(file) {

    encodeFile();
}
ErrorEncode::~ErrorEncode(){}

  
void ErrorEncode::encodeFile() {
    std::cout << "encodeFile() is called!" << std::endl;

    // Call base class processFile (encodes the original message)
    Encode::processFile();  

    // Now introduce errors into the encoded messages
    errorEncodeRand();  // Introduce random errors

    // Check if hammingCodeWithErrors is populated
    if (hammingCodeWithErrors.empty()) {
        std::cerr << "Error: hammingCodeWithErrors is empty. No errors introduced." << std::endl;
        return;
    }

    // Now output to errors.txt
    std::string outputFileName = fileName.substr(0, fileName.find_last_of('.')) + "_errors.txt";
    std::ofstream outputFile(outputFileName, std::ios::out | std::ios::trunc);
    if (!outputFile.is_open()) {
        std::cerr << "Error creating output file" << std::endl;
        return;
    }

    // Debugging: Only print the number of blocks with errors
    std::cout << "Number of blocks with errors: " << hammingCodeWithErrors.size() << std::endl;

    // Write the modified Hamming codes to the output file
    for (size_t i = 0; i < hammingCodeWithErrors.size(); i += 2) {
        if (i + 1 < hammingCodeWithErrors.size()) {
            const auto& code1 = hammingCodeWithErrors[i];
            const auto& code2 = hammingCodeWithErrors[i + 1];

            // Write the 7-bit codes with errors to the file
            for (int j = 0; j < 7; ++j) {
                outputFile << code1(0, j);  // First 7 bits
            }
            for (int j = 0; j < 7; ++j) {
                outputFile << code2(0, j);  // Next 7 bits
            }

            outputFile << "\n";  // Newline after each pair
        }
    }

    outputFile.close();
    std::cout << "Error encoding complete. Output written to " << outputFileName << ".\n";
}


// Introduces random errors in each Hamming code block
void ErrorEncode::errorEncodeRand() {
    auto& encodedMessages = this->getEncodedMessages();  // Reference to encoded messages

    hammingCodeWithErrors.clear();

    for (auto& encodedMessage : encodedMessages) {
        // Always introduce 1 error
        int bitPos = rand() % 7;  // Random bit position in the block
        encodedMessage(0, bitPos) = (encodedMessage(0, bitPos) == 0) ? 1 : 0;  // Flip the bit

        hammingCodeWithErrors.push_back(encodedMessage);  // Add the modified message
    }
}