// Read each line of a binary text file by 7 bits (ints) at a time
// Check the 7 bit's parity for errors; error correct as needed.
// Error message if too many errors in a block
// output decoded message to created file

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <bitset>
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

        // Ensure we have 14 bits (two 7-bit blocks)
        if (bits.size() != 14) {
            std::cerr << "Error: Expected 14 bits per line. Line has " << bits.size() << " bits." << std::endl;
            continue; // Skip the line if it doesn't have 14 bits
        }

        // Process each 7-bit block
        Eigen::Matrix<int, 1, 7> firstBlock, secondBlock;

        // Fill the first 7-bit block
        for (int i = 0; i < 7; ++i) {
            firstBlock(0, i) = bits[i];
        }

        // Fill the second 7-bit block
        for (int i = 7; i < 14; ++i) {
            secondBlock(0, i - 7) = bits[i];
        }

        // Check and correct parity for each block
        int errorPositionFirstBlock = checkParity(firstBlock);
        int errorPositionSecondBlock = checkParity(secondBlock);

        // If there are errors, correct them
        if (errorPositionFirstBlock > 0) {
            firstBlock(0, errorPositionFirstBlock - 1) ^= 1; // Correct the error
        }
        if (errorPositionSecondBlock > 0) {
            secondBlock(0, errorPositionSecondBlock - 1) ^= 1; // Correct the error
        }

        // Extract data bits from each 7-bit block
        Eigen::Matrix<int, 1, 4> data1 = extractData(firstBlock);
        Eigen::Matrix<int, 1, 4> data2 = extractData(secondBlock);

        // Combine the extracted data from both blocks to form the 8-bit value
        std::string combinedBinary;
        for (int i = 0; i < 4; ++i) {
            combinedBinary += std::to_string(data1(0, i));  // First block data
        }
        for (int i = 0; i < 4; ++i) {
            combinedBinary += std::to_string(data2(0, i));  // Second block data
        }

        // Debugging: print the combined binary and its ASCII value
        std::cout << "Combined 8-bit binary: " << combinedBinary << std::endl;

        // Convert the combined binary to an ASCII value
        int asciiValue = std::bitset<8>(combinedBinary).to_ulong();
        std::cout << "Decoded ASCII value: " << asciiValue << " (Char: " << static_cast<char>(asciiValue) << ")" << std::endl;

        // Write the decoded character to the output file
        outputFile << static_cast<char>(asciiValue);
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

    // If parity is zero (no error), set errorPosition to 0
    int errorPosition = parity(0, 0) * 1 + parity(0, 1) * 2 + parity(0, 2) * 4;

    std::cout << "Error position: " << errorPosition << std::endl;

    // If no error is detected (errorPosition == 0), then skip the error correction
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
char Decode::matrixToChar(const Eigen::Matrix<int, 1, 4>& data) {
    
    // Combine the 4 bits into a single value
    int charValue = data(0, 0) * 8 + data(0, 1) * 4 + data(0, 2) * 2 + data(0, 3);

    // Debugging: print the integer value before converting to char
    std::cout << "Decoded 4-bit value (int): " << charValue << std::endl;
    if (charValue >= 32 && charValue <= 126) {
        return static_cast<char>(charValue);
    } else {
        std::cerr << "Non-printable ASCII detected. Value: " << charValue << std::endl;
        return '!';  // Placeholder for non-printable characters
    }
};


//Helper functions
Eigen::Matrix<int, 1, 7> Decode::correctBlock(const Eigen::Matrix<int, 1, 7>& block) {
    Eigen::Matrix<int, 1, 7> correctedBlock = block;
    int errorPosition = checkParity(block);

    if (errorPosition > 0 && errorPosition <= 7) {
        correctedBlock(0, errorPosition - 1) ^= 1; // Correct the bit
    } else if (errorPosition > 7) {
        std::cerr << "Too many errors detected in block: " << block << std::endl;
    }

    return correctedBlock;
}


char Decode::byteToChar(const Eigen::Matrix<int, 1, 8>& data) const {
    int charValue = 0;
    for (int i = 0; i < 8; ++i) {
        charValue = (charValue << 1) | data(0, i);
    }

    return static_cast<char>(charValue);
}
