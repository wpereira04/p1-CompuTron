////////////////////////////////////////////////////////
//
// ECE 3574, P1, Walter Pereira Cruz
// File name: computron.cpp
// Description: Contains the implementations for the computron
// functions
//				
// Date:        02/26/2025
//
#include "computron.h"
#include <fstream>
#include <iomanip>

void load_from_file(std::array<int, memorySize>& memory, const std::string& filename) {
    constexpr int sentinel{ -99999 };// terminates reading after -99999
    size_t i{ 0 };
    std::string line;
    int instruction;
    std::ifstream inputFile(filename);
    if (!inputFile) throw std::runtime_error("invalid_input");
        // throw runtime_error exception with string "invalid_input"
    while (std::getline(inputFile, line)) {
        instruction = std::stoi(line);
        if (instruction == sentinel)
            break;
        if (validWord(instruction)) {
            memory[i] = instruction;
            i++;
        }
        else {
            throw std::runtime_error("invalid_input");
        }
        // Check if the instruction is valid using the validWord function
        // If the instruction is valid, store it in memory at position 'i' and increment 'i'
        // If the instruction is invalid, throw a runtime error with the message "invalid_input"
    }
    inputFile.close();
}

// returns the command type based on the provided opCode
Command opCodeToCommand(size_t opCode) {
    switch (opCode) {
    case 10: return Command::read;
    case 11: return Command::write;
    case 20: return Command::load;
    case 21: return Command::store;
    case 30: return Command::add;
    case 31: return Command::subtract;
    case 32: return Command::divide;
    case 33: return Command::multiply;
    case 40: return Command::branch;
    case 41: return Command::branchNeg;
    case 42: return Command::branchZero;
    default: return Command::halt;
    };

}

void execute(std::array<int, memorySize>& memory,
    int* const acPtr, size_t* const icPtr,
    int* const irPtr, size_t* const opCodePtr,
    size_t* const opPtr,
    const std::vector<int>& inputs) {
    size_t inputIndex{ 0 }; // Tracks input
    do {
        *irPtr = memory[*icPtr];
        *opCodePtr = *irPtr / 100;
        *opPtr = *irPtr % 100;
        
        switch (int word{}; opCodeToCommand(*opCodePtr)) {
        case Command::read:
            word = inputs[inputIndex];
            memory[*opPtr] = word;
            *icPtr = *icPtr + 1;
            inputIndex++;
            break;
        case Command::write:
            // doesn't do anything. "writes to screen"
            // cout only allowed for dump, so this case
            // only increments the icPtr
            *icPtr = *icPtr + 1;
            break;
        case Command::load:
            // loads a value from the memory to the accumulator
            *acPtr = memory[*opPtr];
            *icPtr = *icPtr + 1;
            break;
        case Command::store:
            // stores a value from the accumulator to the memory
            memory[*opPtr] = *acPtr;
            *icPtr = *icPtr + 1;
            break;
        case Command::add:
            // adds accumulator value and a value in memory, checks if the word is valid
            // and sets accumulator to the word if valid.
            word = *acPtr + memory[*opPtr];
            if (validWord(word)) { 
                *acPtr = word;
                *icPtr = *icPtr + 1;
            }
            else {
                throw std::runtime_error("invalid_input");
            }
            break;
        case Command::subtract:
            // subtracts accumulator value and a value in memory, checks if the word is valid
            // and sets accumulator to the word if valid.
            word = *acPtr - memory[*opPtr];
            if (validWord(word)) {
                *acPtr = word;
                *icPtr = *icPtr + 1;
            }
            else {
                throw std::runtime_error("invalid_input");
            }
            break;
        case Command::multiply:
            // multiplies accumulator value and a value in memory, checks if the word is valid
            // and sets accumulator to the word if valid.
            word = *acPtr * memory[*opPtr];
            if (validWord(word)) {
                *acPtr = word;
                *icPtr = *icPtr + 1;
            }
            else {
                throw std::runtime_error("invalid_input");
            }
            break;
        case Command::divide:
            // divides accumulator value and a value in memory, checks if the word is valid
            // and sets accumulator to the word if valid. Will also throw if dividing by 0
            if (memory[*opPtr] == 0) throw std::runtime_error("dividing by zero");
            word = *acPtr / memory[*opPtr];
            if (validWord(word)) {
                *acPtr = word;
                *icPtr = *icPtr + 1;
            }
            else {
                throw std::runtime_error("invalid_input");
            }
            break;
        case Command::branch:
            // branches the instructionCounter to the given operand
            *icPtr = *opPtr;
            break;
        case Command::branchNeg:
            // branches the instructionCounter to the given operand
            // if accumulator is negative
            *acPtr < 0 ? *icPtr = *opPtr : ++(*icPtr);
            break;
        case Command::branchZero:
            // branches the instructionCounter to the given operand
            // if accumulator is zero
            *acPtr == 0 ? *icPtr = *opPtr : ++(*icPtr);
            break;
        case Command::halt:
            // ends the program
            break;
        default:
            break;
        };
        // You may modify the below while condition if required
    } while (opCodeToCommand(*opCodePtr) != Command::halt);
};

bool validWord(int word) {
    return word >= minWord && word <= maxWord;
};

void dump(std::array<int, memorySize>& memory, int accumulator,
    size_t instructionCounter, size_t instructionRegister,
    size_t operationCode, size_t operand) {
    int col;
    int row{ 0 }; // must be initialized
    // prints register dump
    // keeps track of the sign of a data word
    std::string sign = "";
    std::cout << "Registers\n" << "accumulator";
    if (accumulator >= 0) sign = "+";
    else sign = "-";
    std::cout << std::setw(11)<< std::setfill(' ') << sign << std::setw(4) << std::setfill('0') << abs(accumulator) << '\n'
        << "instructionCounter   " << std::setw(2) << std::setfill('0') << instructionCounter << '\n'
        << "instructionRegister  +" << instructionRegister << '\n'
        << "operationCode        " << operationCode << '\n'
        << "operand              " << std::setw(2) << std::setfill('0') << operand << "\nMemory:\n";
    for (size_t i = 0; i < 10; i++) {
        std::cout << "    " << i;
    }
    std::cout << '\n';
    // prints the memory dump
    for (size_t i = 0; i < memorySize; i++) {
        // uses modulous operator and division to find row and column and ensure table prints properly
        col = i % 10;
        if (row < floor(i / 10) * 10 || i == 0) std::cout << std::setw(2) << floor(i / 10) * 10 << " ";
        row = floor(i / 10) * 10;
        if (memory[i] >= 0) std::cout << "+";
        else std::cout << "-";
        // - should be printed with memory[i] value
        std::cout << std::setw(4) << std::setfill('0') << abs(memory[i]);
        if (col == 9 || i + 1 == memorySize) {
            std::cout << '\n';
        }
    }
};