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

class Decode : public Hamming {
public:
    Decode(std::string file) : Hamming(file) {}

    void processFile() override {
        std::ifstream inputFile(fileName, std::ios::in);
        if (!inputFile.is_open()) {
            std::cerr << "Error opening file: " << fileName << std::endl;
            return;
        }

        std::ofstream outputFile(fileName + "_out.txt", std::ios::out | std::ios::trunc);
        if (!outputFile.is_open()) {
            std::cerr << "Error creating output file." << std::endl;
            inputFile.close();
            return;
        }

        std::string line;
        while (std::getline(inputFile, line)) {

            std::vector<int> bits = parseLineToBits(line);

            // Process each 7-bit block
            for (size_t i = 0; i + 7 <= bits.size(); i += 7) {
                Eigen::Matrix<int, 1, 7> receivedBlock;
                for (int j = 0; j < 7; ++j) {
                    receivedBlock(0, j) = bits[i + j];
                }

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

                // Convert 4-bit block back to character
                char decodedChar = matrixToChar(decodedData);
                outputFile << decodedChar;
            }
        }

        inputFile.close();
        outputFile.close();
        std::cout << "Decoding complete. Output written to 'decoded_output.txt'.\n";
    }

private:
    // Parse a line of binary text into a vector of integers
    std::vector<int> parseLineToBits(const std::string& line) {
        std::vector<int> bits;
        for (char c : line) {
            if (c == '0' || c == '1') {
                bits.push_back(c - '0');
            }
        }
        return bits;
    }

    // Check parity and return the error position (1-based index); 0 if no error
    int checkParity(const Eigen::Matrix<int, 1, 7>& block) const {
        Eigen::Matrix<int, 1, 3> parity = block * parityCheck.transpose();
        parity = parity.unaryExpr([](int x) { return x % 2; });

        int errorPosition = parity(0, 0) * 1 + parity(0, 1) * 2 + parity(0, 2) * 4; //multiply by error position
        return errorPosition;
    }

    // Extract the original 4-bit data from the corrected 7-bit block
    Eigen::Matrix<int, 1, 4> extractData(const Eigen::Matrix<int, 1, 7>& block) const {
        Eigen::Matrix<int, 1, 4> data;
        data(0, 0) = block(0, 2);
        data(0, 1) = block(0, 4);
        data(0, 2) = block(0, 5);
        data(0, 3) = block(0, 6);
        return data;
    }

    // Convert a 4-bit Eigen::Matrix to a character
    char matrixToChar(const Eigen::Matrix<int, 1, 4>& data) const {
        int value = 0;
        for (int i = 0; i < 4; ++i) {
            value = (value << 1) | data(0, i);
        }
        return static_cast<char>(value);
    }


};