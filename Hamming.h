/* Colton Criswell and Zach Hamby
 * Final Project - CS-300
 */

#ifndef HAMMING_H
#define HAMMING_H

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "Eigen/Dense"

/**
 * @class Hamming
 * @brief Base class for handling Hamming codes.
 * 
 * This class provides common functionality for both encoding and decoding Hamming codes.
 */
class Hamming {

    public:
        /**
         * @brief Constructor for Hamming class.
         * @param file The name of the file to process.
         */
        Hamming(std::string file);
        
        /**
         * @brief Destructor for Hamming class.
         */
        virtual ~Hamming();

        /**
         * @brief Prints the generator and parity check matrices.
         */
        void printOut() const;
        
    protected:
        Eigen::Matrix<int, 7, 4> generator;  ///< Hamming generator matrix (7x4)
        Eigen::Matrix<int, 3, 7> parityCheck; ///< Hamming parity check matrix (3x7)
        std::string fileName; ///< Name of the file being processed

        /**
         * @brief Pure virtual method for processing files.
         * @note This method must be overridden in derived classes.
         */
        virtual void processFile() = 0;
};

/**
 * @class Decode
 * @brief Derived class for decoding Hamming codes.
 * 
 * This class handles the decoding of files using the Hamming algorithm, and checks for errors using the parity bits.
 */
class Decode : public Hamming {

    public:
        /**
         * @brief Constructor for Decode class.
         * @param file The name of the file to decode.
         */
        Decode(std::string file);
        
        /**
         * @brief Destructor for Decode class.
         */
        ~Decode();
        
    private:
        /**
         * @brief Processes the file for decoding.
         */
        void processFile() override;
        
        /**
         * @brief Parses a line of binary text into a vector of integers.
         * @param line The binary string to parse.
         * @return A vector of integers representing the binary data.
         */
        std::vector<int> parseLineToBits(const std::string& line) const;

        /**
         * @brief Checks the parity of a 7-bit block.
         * @param block The 7-bit block to check.
         * @return The error position (1-based) or 0 if no error.
         */
        int checkParity(const Eigen::Matrix<int, 1, 7>& block) const;

        /**
         * @brief Extracts the 4-bit data from a corrected 7-bit block.
         * @param block The 7-bit block to extract data from.
         * @return A 4-bit data matrix.
         */
        Eigen::Matrix<int, 1, 4> extractData(const Eigen::Matrix<int, 1, 7>& block) const;

        /**
         * @brief Converts a 4-bit matrix to a character.
         * @param data The 4-bit matrix.
         * @return The decoded character.
         */
        char matrixToChar(const Eigen::Matrix<int, 1, 4>& data) const;

        /**
         * @brief Corrects a 7-bit block by fixing errors if necessary.
         * @param block The 7-bit block to correct.
         * @return The corrected block.
         */
        Eigen::Matrix<int, 1, 7> correctBlock(const Eigen::Matrix<int, 1, 7>& block) const;

        /**
         * @brief Converts a byte (8-bit data) to a character.
         * @param data The 8-bit data matrix.
         * @return The decoded character.
         */
        char byteToChar(const Eigen::Matrix<int, 1, 8>& data) const;

        /**
         * @brief Combines two 4-bit data matrices and converts them to a character.
         * @param data1 The first 4-bit data matrix.
         * @param data2 The second 4-bit data matrix.
         * @return The combined character.
         */
        char combineDataAndConvertToChar(const Eigen::Matrix<int, 1, 4>& data1, const Eigen::Matrix<int, 1, 4>& data2) const;

        /**
         * @brief Parses and corrects a line of data.
         * @param line The line to parse.
         * @return A pair of 4-bit matrices representing the corrected data.
         */
        std::pair<Eigen::Matrix<int, 1, 4>, Eigen::Matrix<int, 1, 4>> parseAndCorrectBlock(const std::string& line) const;
};

/**
 * @class Encode
 * @brief Derived class for encoding Hamming codes.
 * 
 * This class handles the encoding of files using the Hamming algorithm.
 */
class Encode : public Hamming {

    public:
        /**
         * @brief Constructor for Encode class.
         * @param file The name of the file to encode.
         */
        Encode(std::string file);
        
        /**
         * @brief Destructor for Encode class.
         */
        ~Encode();

        /**
         * @brief Encodes a 4-bit message into a 7-bit Hamming code.
         * @param message The 4-bit message to encode.
         * @return A 7-bit encoded message.
         */
        Eigen::Matrix<int, 1, 7> encodeMessage(const Eigen::Matrix<int, 1, 4>& message) const;

        /**
         * @brief Prints the encoded message.
         * @param encodedMessage The 7-bit encoded message.
         */
        void printEncodedMsg(const Eigen::Matrix<int, 1, 7>& encodedMessage) const;

    protected:
        /**
         * @brief Processes the file for encoding.
         */
        void processFile() override;

        /**
         * @brief Retrieves the original message from a file.
         * @param fileName The name of the file containing the message.
         * @return The original message.
         */
        std::string getOriginalMessage(const std::string& fileName) const;

        /**
         * @brief Splits an 8-bit binary matrix into two 4-bit parts.
         * @param binary The 8-bit binary matrix.
         * @return A pair of 4-bit matrices.
         */
        std::pair<Eigen::Matrix<int, 1, 4>, Eigen::Matrix<int, 1, 4>> splitBinary(const Eigen::Matrix<int, 1, 8>& binary) const;

        /**
         * @brief Converts a character to its binary representation.
         * @param ch The character to convert.
         * @return An 8-bit binary matrix.
         */
        Eigen::Matrix<int, 1, 8> charToBinary(char ch) const;

        /**
         * @brief Getter for encoded messages.
         * @return A reference to the vector of encoded messages.
         */
        std::vector<Eigen::Matrix<int, 1, 7>>& getEncodedMessages();

    private:
        std::vector<Eigen::Matrix<int, 1, 7>> encodedMessages;  ///< Stores the encoded Hamming blocks
};

/**
 * @class ErrorEncode
 * @brief Derived class for encoding Hamming codes with introduced errors.
 * 
 * This class encodes messages and introduces random errors in the encoded blocks.
 */
class ErrorEncode : public Encode {

    public:
        /**
         * @brief Constructor for ErrorEncode class.
         * @param file The name of the file to encode.
         */
        ErrorEncode(std::string file);
        
        /**
         * @brief Destructor for ErrorEncode class.
         */
        ~ErrorEncode();

        /**
         * @brief Encodes the file and introduces errors.
         */
        void encodeFile();

        /**
         * @brief Introduces a random error in each Hamming code block.
         */
        void errorEncodeRand();

    private:
        std::vector<Eigen::Matrix<int, 1, 7>> hammingCodeWithErrors;  ///< Stores Hamming blocks with errors
};

#endif
