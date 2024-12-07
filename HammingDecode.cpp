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
    // Open the "out" file (test1_out.txt) and the "errors" file (test1_errors.txt)
    std::ifstream outFile(fileName.substr(0, fileName.find_last_of('.')) + "_out.txt", std::ios::in);
    if (!outFile.is_open()) {
        std::cerr << "Error opening file: " << fileName.substr(0, fileName.find_last_of('.')) + "_out.txt" << std::endl;
        return;
    }

    std::ifstream errorFile(fileName.substr(0, fileName.find_last_of('.')) + "_errors.txt", std::ios::in);
    if (!errorFile.is_open()) {
        std::cerr << "Error opening file: " << fileName.substr(0, fileName.find_last_of('.')) + "_errors.txt" << std::endl;
        outFile.close();
        return;
    }

    // Open the output file for decoded content (write only the errors to the decoded file)
    std::ofstream decodedFile(fileName.substr(0, fileName.find_last_of('.')) + "_decoded.txt", std::ios::out | std::ios::trunc);
    if (!decodedFile.is_open()) {
        std::cerr << "Error creating output file." << std::endl;
        outFile.close();
        errorFile.close();
        return;
    }

    std::string line;
    
    // Decode and print content of the "out" file (for terminal display)
    std::cout << "Decoding content of the out file:" << std::endl;
    while (std::getline(outFile, line)) {
        if (line.length() != 14) {
            std::cerr << "Error: Expected 14 bits per line in out file. Line has " << line.length() << " bits." << std::endl;
            continue;
        }

        auto [data1, data2] = parseAndCorrectBlock(line);
        if (data1.size() == 0 || data2.size() == 0) continue; 

        char decodedChar = combineDataAndConvertToChar(data1, data2);

        // Print binary and decoded character to the terminal
        std::string binaryString = "";
        for (int i = 0; i < 4; ++i) {
            binaryString += std::to_string(data1(0, i)); // Add data1
        }
        for (int i = 0; i < 4; ++i) {
            binaryString += std::to_string(data2(0, i)); // Add data2
        }
        
        std::cout << "Binary: " << binaryString << " -> ASCII: " << decodedChar << std::endl;
    }

    std::cout << "\nDecoding content of the errors file:" << std::endl;
    while (std::getline(errorFile, line)) {
        if (line.length() != 14) {
            std::cerr << "Error: Expected 14 bits per line in errors file. Line has " << line.length() << " bits." << std::endl;
            continue;
        }

        // Ensure the pair is empty if any block is corrupted
        auto [data1, data2] = parseAndCorrectBlock(line);
        if (data1.size() == 0 || data2.size() == 0) continue;  // Skip corrupted blocks

        // If data is valid, decode the character
        char decodedChar = combineDataAndConvertToChar(data1, data2);

        // Print binary and decoded character to the terminal
        std::string binaryString = "";
        for (int i = 0; i < 4; ++i) {
            binaryString += std::to_string(data1(0, i)); // Add data1
        }
        for (int i = 0; i < 4; ++i) {
            binaryString += std::to_string(data2(0, i)); // Add data2
        }

        std::cout << "Binary: " << binaryString << " -> ASCII: " << decodedChar << std::endl;

        // Write the decoded character to the output file (this is the only file written to)
        decodedFile << decodedChar;
    }

    outFile.close();
    errorFile.close();
    decodedFile.close();
    std::cout << "Decoding complete. Output written to " << fileName.substr(0, fileName.find_last_of('.')) + "_decoded.txt" << ".\n";
}



// Parse a line of binary text into a vector of integers
std::vector<int> Decode::parseLineToBits(const std::string& line) const {
    std::vector<int> bits;
    for (char c : line) {
        if (c == '0' || c == '1') {
            bits.push_back(c - '0');
        }
    }
    return bits;
}

// Extract the original 4-bit data from the corrected 7-bit block
Eigen::Matrix<int, 1, 4> Decode::extractData(const Eigen::Matrix<int, 1, 7>& receivedBlock) const {
    Eigen::Matrix<int, 1, 4> data;
    
    //Correctly extract data bits from positions 3, 5, 6, 7
    data(0, 0) = receivedBlock(0, 2); 
    data(0, 1) = receivedBlock(0, 4); 
    data(0, 2) = receivedBlock(0, 5); 
    data(0, 3) = receivedBlock(0, 6); 
    
    return data;
}


// Check parity and return the error position (1-based index); 0 if no error
int Decode::checkParity(const Eigen::Matrix<int, 1, 7>& block) const {
    Eigen::Matrix<int, 1, 3> parity = block * parityCheck.transpose();
    parity = parity.unaryExpr([](int x) { return x % 2; });  // Modulo 2

    // Combine syndrome bits into a single error position (1-based index)
    return (parity(0, 2) << 2) | (parity(0, 1) << 1) | parity(0, 0);
}


//Helper functions
Eigen::Matrix<int, 1, 7> Decode::correctBlock(const Eigen::Matrix<int, 1, 7>& block) const {
    Eigen::Matrix<int, 1, 7> correctedBlock = block;
    int errorPosition = checkParity(block);

    if (errorPosition > 0 && errorPosition <= 7) {
        // Correct the single-bit error
        correctedBlock(0, errorPosition - 1) ^= 1;
    } else if (errorPosition > 7) {
        // Log and skip blocks with multiple errors
        std::cerr << "Multiple errors detected in block: " << block << std::endl;
        return {};  // Return empty block
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

// Helper to parse and correct blocks, returning two 4-bit data matrices
std::pair<Eigen::Matrix<int, 1, 4>, Eigen::Matrix<int, 1, 4>> Decode::parseAndCorrectBlock(const std::string& line) const {
    std::vector<int> bits = parseLineToBits(line); // Parse line into bits
    if (bits.size() != 14) {
        std::cerr << "Error: Expected 14 bits per line. Line has " << bits.size() << " bits." << std::endl;
        return {}; // Return empty pair if invalid
    }

    // Split into two 7-bit blocks
    Eigen::Matrix<int, 1, 7> firstBlock, secondBlock;
    for (int i = 0; i < 7; ++i) firstBlock(0, i) = bits[i];
    for (int i = 7; i < 14; ++i) secondBlock(0, i - 7) = bits[i];

    // Check and correct parity for each block
    firstBlock = correctBlock(firstBlock);
    secondBlock = correctBlock(secondBlock);

    // If either block is empty (indicating multiple errors), return an empty pair
    if (firstBlock.size() == 0) std::cerr << "Corrupted first block detected." << std::endl;
    if (secondBlock.size() == 0) std::cerr << "Corrupted second block detected." << std::endl;

    // Extract data bits
    Eigen::Matrix<int, 1, 4> data1 = extractData(firstBlock);
    Eigen::Matrix<int, 1, 4> data2 = extractData(secondBlock);

    return std::make_pair(data1, data2);
}

char Decode::combineDataAndConvertToChar(const Eigen::Matrix<int, 1, 4>& data1, const Eigen::Matrix<int, 1, 4>& data2) const {
    if (data1.size() == 0 || data2.size() == 0) {
        std::cerr << "Invalid data detected. Skipping character conversion." << std::endl;
        return '\0';
    }

    int combinedData = 0;
    for (int i = 0; i < 4; ++i) {
        combinedData = (combinedData << 1) | data1(0, i); // Append data1
    }
    for (int i = 0; i < 4; ++i) {
        combinedData = (combinedData << 1) | data2(0, i); // Append data2
    }
    return static_cast<char>(combinedData);
}
