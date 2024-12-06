// Read each line of a binary text file by 7 bits (ints) at a time
// Check the 7 bit's parity for errors; error correct as needed.
// Error message if too many errors in a block
// output decoded message to created file

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "Eigen/Dense"
#include "Hamming.h"


Decode::Decode(std::string file) : Hamming(file) {

    processFile();

}

Decode::~Decode() {}

void Decode::processFile() {
    std::ifstream inputFile(fileName, std::ios::in);
    if (!inputFile.is_open()) {
        std::cerr << "Error opening file: " << fileName << std::endl;
        return;
    }

    // Remove the ".txt" extension from the original file name
    std::string outputFileName = fileName.substr(0, fileName.find_last_of('.')) + "_decoded.txt";
    
    std::ofstream outputFile(outputFileName, std::ios::out | std::ios::trunc);
    if (!outputFile.is_open()) {
        std::cerr << "Error creating output file." << std::endl;
        inputFile.close();
        return;
    }



    std::string line;
    while (std::getline(inputFile, line)) {
        std::vector<int> bits = parseLineToBits(line);


        // Debugging: print parsed bits
        std::cout << "Parsed bits: ";
        for (int bit : bits) {
            std::cout << bit;
        }
        std::cout << std::endl;

        // Ensure that we have exactly 14 bits per line (two 7-bit blocks)
        if (bits.size() != 14) {
            std::cerr << "Error: Expected 14 bits per line. Line has " << bits.size() << " bits." << std::endl;
            continue; // Skip the line if it doesn't have exactly 14 bits
        }

        // Process each 7-bit block
        for (size_t i = 0; i < 14; i += 7) {
            Eigen::Matrix<int, 1, 7> receivedBlock;
            for (int j = 0; j < 7; ++j) {
                receivedBlock(0, j) = bits[i + j];
            }

            // Debugging: print received 7-bit block
            std::cout << "Received block: ";
            for (int j = 0; j < 7; ++j) {
                std::cout << receivedBlock(0, j);
            }
            std::cout << std::endl;

            // Decode the 7-bit block
            int errorPosition = checkParity(receivedBlock);
            if (errorPosition > 0) {
                if (errorPosition > 7) {
                    std::cerr << "Too many errors detected in block: " << receivedBlock << std::endl;
                    continue;
                }
                // Correct the error
                receivedBlock(0, errorPosition - 1) ^= 1;
            }

            // Extract the original 4-bit data
            Eigen::Matrix<int, 1, 4> decodedData = extractData(receivedBlock);

            // Debugging: print extracted 4-bit data
            std::cout << "Extracted 4-bit data: ";
            for (int i = 0; i < 4; ++i) {
                std::cout << decodedData(0, i);
            }
            std::cout << std::endl;

            // Convert 4-bit block back to character
            char decodedChar = matrixToChar(decodedData);

            std::cout << "Decoded Char: " << decodedChar << std::endl; // Debugging output
            
            outputFile << decodedChar;
        }
    }

    inputFile.close();
    outputFile.close();
    std::cout << "Decoding complete. Output written to " << outputFileName << ".\n";
}


    // Parse a line of binary text into a vector of integers
std::vector<int> Decode::parseLineToBits(const std::string& line) {
    std::vector<int> bits;
    for (char c : line) {
        if (c == '0' || c == '1') {
            bits.push_back(c - '0');
        }
    }
    return bits;
}

// Check parity and return the error position (1-based index); 0 if no error
int Decode::checkParity(const Eigen::Matrix<int, 1, 7>& block) const {
    Eigen::Matrix<int, 1, 3> parity = block * parityCheck.transpose();
    parity = parity.unaryExpr([](int x) { return x % 2; });

    int errorPosition = parity(0, 0) * 1 + parity(0, 1) * 2 + parity(0, 2) * 4; //multiply by error position
    return errorPosition;
}

// Extract the original 4-bit data from the corrected 7-bit block
Eigen::Matrix<int, 1, 4> Decode::extractData(const Eigen::Matrix<int, 1, 7>& receivedBlock) {
    Eigen::Matrix<int, 1, 4> data;
    
    // Correctly extract data bits from positions 3, 5, 6, 7
    data(0, 0) = receivedBlock(0, 2); // 3rd position
    data(0, 1) = receivedBlock(0, 4); // 5th position
    data(0, 2) = receivedBlock(0, 5); // 6th position
    data(0, 3) = receivedBlock(0, 6); // 7th position
    
    return data;
}

// Convert a 4-bit Eigen::Matrix to a character
char Decode::matrixToChar(const Eigen::Matrix<int, 1, 4>& data) const {
    
    // Combine the 4 bits into a single value
    int charValue = data(0, 0) * 8 + data(0, 1) * 4 + data(0, 2) * 2 + data(0, 3);

    // Debugging: print the integer value before converting to char
    std::cout << "Decoded 4-bit value (int): " << charValue << std::endl;

    // Check if value corresponds to printable ASCII characters (between 32 and 126)
    if (charValue >= 32 && charValue <= 126) {
        return static_cast<char>(charValue);
    } else {
        return '\0';  // Return null character if not a printable ASCII character
    }
};