/*
 *
 *
*/

#ifndef HAMMING_H
#define HAMMING_H

#include <iostream>
#include <string>
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

        virtual void processFile();
    
};


class Decode : public Hamming {

    public:
        Decode();
        ~Decode();

        bool errorCheck(int errors);

    private:
        int errors = 0;
        void processFile() override;

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
        ErrorEncode();
        ~ErrorEncode();

    private:
        void processFile() override;
};


#endif