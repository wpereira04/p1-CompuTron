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
        size_t* reg = icPtr; // register
        *irPtr = memory[*icPtr]; // may or may not be wrong
        *opCodePtr = *irPtr / 100;
        *opPtr = *irPtr % 100;
        //instruction counter to register
        //instructionRegister = memory [instructionCounter];
        //operationCode = instructionRegister / 100; // divide
        //operand = instructionRegister % 100; // remainder
        switch (int word{}; opCodeToCommand(*opCodePtr)) {
        case Command::read:
            word = inputs[inputIndex];
            memory[*opPtr] = word; // modified this might be assign *opPtr to word
            *icPtr = *icPtr + 1;
            // could be following line
            // *icPtr = *icPtr + 1;
            // Assign the value of 'word' to the memory location pointed to by 'opPtr'
            // Increment the instruction counter (icPtr) to point to the next instruction
            inputIndex++;
            break;
        case Command::write:
            // doesn't do anything. "writes to screen"
            // cout only allowed for dump, so this case
            // only increments the icPtr
            *icPtr = *icPtr + 1;
            //Dereference 'icPtr' to access the instruction counter and increment its value by 1
             // use the below cout if needed but comment before submission
            //std::cout << "Contents of " << std::setfill('0') << std::setw(2)
            //      << *opPtr << " : " << memory[*opPtr] << "\n";
            break;
        case Command::load:
            *acPtr = memory[*opPtr]; // this might be assign *opPtr to acPtr
            *icPtr = *icPtr + 1;
            // could be following line
            // *icPtr = *icPtr + 1;
            //Load the value from the memory location pointed to by 'opPtr' into the accumulator (acPtr)
            //Increment the instruction counter (icPtr) to point to the next instruction
            break;
        case Command::store:
            memory[*opPtr] = *acPtr;
            *icPtr = *icPtr + 1;
            // could be following line
            // *icPtr = *icPtr + 1;
            // Store the value in the accumulator (acPtr) into the memory location pointed to by 'opPtr'
            // Increment the instruction counter (icPtr) to move to the next instruction
            break;
        case Command::add:
            word = *acPtr + memory[*opPtr];
            if (validWord(word)) { 
                *acPtr = word;
                *icPtr = *icPtr + 1;
            }
            else {
                throw std::runtime_error("invalid_input");
            }
            // Add the value in the accumulator (acPtr) to the value in memory at the location pointed to by 'opPtr' and store the result in 'word'
            // If the result is valid, store it in the accumulator and increment the instruction counter
            // / If the result is invalid, throw a runtime error 
            break;
        case Command::subtract:
            word = *acPtr - memory[*opPtr];
            if (validWord(word)) {
                *acPtr = word;
                *icPtr = *icPtr + 1;
            }
            else {
                throw std::runtime_error("invalid_input");
            }
            // Subtract the value in memory at the location pointed to by 'opPtr' from the value in the accumulator (acPtr) and store the result in 'word'
            // If the result is valid, store it in the accumulator and increment the instruction counter
            // / If the result is invalid, throw a runtime error 
            break;
        case Command::multiply:
            word = *acPtr * memory[*opPtr];
            if (validWord(word)) {
                *acPtr = word;
                *icPtr = *icPtr + 1;
            }
            else {
                throw std::runtime_error("invalid_input");
            }
            // as above do it for multiplication
            break;
        case Command::divide:
            word = *acPtr / memory[*opPtr];
            if (validWord(word)) {
                *acPtr = word;
                *icPtr = *icPtr + 1;
            }
            else {
                throw std::runtime_error("invalid_input");
            }
            // as above do it for division
            break;
        case Command::branch:
            *icPtr = *opPtr;
            break;
        case Command::branchNeg:
            *acPtr < 0 ? *icPtr = *opPtr : ++(*icPtr);
            break;
        case Command::branchZero:
            *acPtr == 0 ? *icPtr = *opPtr : ++(*icPtr);
            break;
        case Command::halt:
            //delete icPtr;
            break;
        default:
            // any instruction required
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
    std::string sign = "";
    std::cout << "Registers\n"<<"accumulator";
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