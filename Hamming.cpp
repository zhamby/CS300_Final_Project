/*
 *   
 *
*/

#include <iostream>
#include "Eigen/Dense"
#include "Hamming.h"

Hamming::Hamming() : generator(7,4), parityCheck(3,7) {

    generator << 1, 1, 0, 1,
                 1, 0, 1, 1,
                 1, 0, 0, 0,
                 0, 1, 1, 1,
                 0, 1, 0, 0,
                 0, 0, 1, 0,
                 0, 0, 0, 1;

                

    parityCheck << 1, 0, 1, 0, 1, 0, 1,
                   0, 1, 1, 0, 0, 1, 1,
                   0, 0, 0, 1, 1, 1, 1;
}

Hamming::~Hamming(){}


void Hamming::printOut() {
    std::cout << "Generator Matrix:\n" << generator << "\n";
    std::cout << "Parity Check Matrix:\n" << parityCheck << "\n";
}