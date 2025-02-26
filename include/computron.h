////////////////////////////////////////////////////////
//
// ECE 3574, P1, Walter Pereira Cruz
// File name: computron.h
// Description: Contains the declarations for the computron
// functions and variables
//				
// Date:        02/26/2025
//

#ifndef COMPUTRON_H
#define COMPUTRON_H​
#include <iostream>
#include <array>
#include <vector>
#include <string>
#include <stdexcept>
#include <cmath>

// used as parameters for implementation
constexpr size_t memorySize{ 100 };
constexpr int minWord{ -9999 };
constexpr int maxWord{ 9999 };

enum class Command {
    read = 10, write,
    load = 20, store,
    add = 30, subtract, divide, multiply,
    branch = 40, branchNeg, branchZero, halt
};
// loads instructions from a file into the provided memory
void load_from_file(std::array<int, memorySize>& memory, const std::string& filename);

// executes instructions from a provided memory using the provided ptrs.
void execute(std::array<int, memorySize>& memory, int* const acPtr,
    size_t* const icPtr, int* const irPtr,
    size_t* const opCodePtr, size_t* const opPtr,
    const std::vector<int>& inputs);
// prints the information of the memory and variables in a readable way
void dump(std::array<int, memorySize>& memory, int accumulator,
    size_t instructionCounter, size_t instructionRegister,
    size_t operationCode, size_t operand);
// checks if a word is valid according to minWord and maxWord.
bool validWord(int word);
// unused
void output(std::string label, int width, int value, bool sign);

#endif