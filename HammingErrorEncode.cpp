// Inherit from Hamming Encode
// Encodes text from file to hamming code binary with errors
// Include errorEncodeStatic, which manipulates a specific bit
// Include errorEncodeRand, which manipulates rand bit per hamming code
// All output pushed to output file

#include <string>
#include "Hamming.h"


ErrorEncode::ErrorEncode(std::string file) : Encode(file) {}
ErrorEncode::~ErrorEncode(){}

  
void ErrorEncode::processFile() {}