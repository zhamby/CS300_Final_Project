/*
 *
 *
*/

#ifndef HAMMING_H
#define HAMMING_H

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "Eigen/Dense"


class Hamming {

    public:    
        Hamming(std::string file);
        virtual ~Hamming();

        void printOut();
        
    protected:
        Eigen::Matrix<int, 7, 4> generator;
        Eigen::Matrix<int, 3, 7> parityCheck;
        std::string fileName;

        virtual void processFile() = 0;
};


class Decode : public Hamming {

    public:
        Decode(std::string file);
        ~Decode();
        
    private:
        void processFile() override;
        
        //Helper functions
        std::vector<int> parseLineToBits(const std::string& line);
        int checkParity(const Eigen::Matrix<int, 1, 7>& block) const;
        Eigen::Matrix<int, 1, 4> extractData(const Eigen::Matrix<int, 1, 7>& block);
        char matrixToChar(const Eigen::Matrix<int, 1, 4>& data);
        Eigen::Matrix<int, 1, 7> correctBlock(const Eigen::Matrix<int, 1, 7>& block);
        char byteToChar(const Eigen::Matrix<int, 1, 8>& data) const;
        char combineDataAndConvertToChar(const Eigen::Matrix<int, 1, 4>& data1, const Eigen::Matrix<int, 1, 4>& data2);
        std::pair<Eigen::Matrix<int, 1, 4>, Eigen::Matrix<int, 1, 4>> parseAndCorrectBlock(const std::string& line);
};  


class Encode : public Hamming {

    public:
        Encode(std::string file);
        ~Encode();

        //Function to encode a 4-bit message into a 7-bit Hamming code
        Eigen::Matrix<int, 1, 7> encodeMessage(const Eigen::Matrix<int, 1, 4>& message) const;

        //Print encoded msg
        void printEncodedMsg(const Eigen::Matrix<int, 1, 7>& encodedMessage) const;


    protected:
        void processFile() override;

        //Helper functions
        std::string getOriginalMessage(const std::string& fileName) const;
        std::pair<Eigen::Matrix<int, 1, 4>, Eigen::Matrix<int, 1, 4>> splitBinary(const Eigen::Matrix<int, 1, 8>& binary) const;
        Eigen::Matrix<int, 1, 8> charToBinary(char ch) const;

        //Getter for encoded msgs
        std::vector<Eigen::Matrix<int, 1, 7>>& getEncodedMessages();

    private:
        std::vector<Eigen::Matrix<int, 1, 7>> encodedMessages;
        
};

class ErrorEncode : public Encode  {

    public:
        ErrorEncode(std::string file);
        ~ErrorEncode();

        void encodeFile();

        // Introduces an error at a specific bit
        void errorEncodeStatic(size_t codeIndex, int bitPos);
        // Introduces a random error in each Hamming code block
        void errorEncodeRand();

    private:
        std::vector<Eigen::Matrix<int, 1, 7>> hammingCodeWithErrors;  // Stores encoded Hamming blocks with errors

};


#endif