////////////////////////////////////////////////////////
//
// ECE 3574, P1, Walter Pereira Cruz
// File name: main.cpp
// Description: Contains the main function which runs
// an instruction set from a file "p1.txt".
//				
// 
// Date:        02/26/2025
//

#include "computron.h"
int main() {
    std::array<int, memorySize> memory{ 0 };
    int accumulator{ 0 };
    size_t instructionCounter{ 0 };
    int instructionRegister{ 0 };
    size_t operationCode{ 0 };
    size_t operand{ 0 };
    const std::vector<int> inputs{ 4, 5 };

    load_from_file(memory, "p1.txt");
    execute(memory, &accumulator,
        &instructionCounter, &instructionRegister,
        &operationCode, &operand, inputs);

    dump(memory, accumulator,
        instructionCounter, instructionRegister,
        operationCode, operand);
}