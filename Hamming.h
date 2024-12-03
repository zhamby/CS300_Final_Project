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
        ~Hamming();

        void printOut();
        void writeToFile();

    protected:
        Eigen::Matrix<int, 7, 4> generator;
        Eigen::Matrix<int, 3, 7> parityCheck;
        std::string fileName = "";

};


class Decode : public Hamming {

    public:
        Decode();
        ~Decode();

        bool errorCheck(int errors);

    private:
        int errors = 0;

};


class Encode : public Hamming {

    public:
        Encode();
        ~Encode();



};

class ErrorEncode : public Encode  {

    public:
        ErrorEncode();
        ~ErrorEncode();


}


#endif