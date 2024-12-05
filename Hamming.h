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
        void processFile() override;

    private:
    //helper functions
        std::vector<int> parseLineToBits(const std::string& line);
        int checkParity(const Eigen::Matrix<int, 1, 7>& block) const;
        Eigen::Matrix<int, 1, 4> extractData(const Eigen::Matrix<int, 1, 7>& block) const;
        char matrixToChar(const Eigen::Matrix<int, 1, 4>& data) const;
};


class Encode : public Hamming {

    public:
        Encode(std::string file);

        //Function to encode a 4-bit message into a 7-bit Hamming code
        Eigen::Matrix<int, 1, 7> encodeMessage(const Eigen::Matrix<int, 1, 4>& message) const;

        //Print encoded msg
        void printEncodedMsg(Eigen::Matrix<int, 1, 7> encodedMessage);

    private:
        void processFile() override;
};

class ErrorEncode : public Encode  {

    public:
        ErrorEncode(std::string file);
        ~ErrorEncode();

    private:
        void processFile() override;
};


#endif