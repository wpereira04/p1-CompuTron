////////////////////////////////////////////////////////
//
// ECE 3574, P1, Walter Pereira Cruz
// File name: test.cpp
// Description: Contains the tests for the computron
// functions.
//				
// Date:        02/26/2025
//
#define CATCH_CONFIG_MAIN
#include "catch2/catch.hpp"
#include "computron.h"

TEST_CASE("Tests for the CompuTron Project") {
	std::array<int, memorySize> memory{ 0 };
    int accumulator{ 0 };
    size_t instructionCounter{ 0 };
    int instructionRegister{ 0 };
    size_t operationCode{ 0 };
    size_t operand{ 0 };
    // inputs not declared here to change for each test section
    
    SECTION("Testing read, write, load, and store", "execute()") {
        const std::vector<int> inputs{ 5, -6 }; // both positive and negative values
        memory[0] = 1008; // read, 00
        memory[1] = 1009; // read, 01
        memory[2] = 1100; // write does nothing, 02
        memory[3] = 2008; // load 5, 03
        memory[4] = 2110; // store 5 at 10, 04
        memory[5] = 2009; // load -6, 05
        memory[6] = 2111; // store -6 at 11, 06
        memory[7] = 4300; // halt, 07
        REQUIRE_NOTHROW(execute(memory, &accumulator, &instructionCounter,
            &instructionRegister, &operationCode, &operand, inputs));
        // checks if changed variables and memory locations are correct
        REQUIRE(accumulator == -6);
        REQUIRE(instructionCounter == 7);
        REQUIRE(instructionRegister == 4300);
        REQUIRE(operationCode == 43);
        REQUIRE(memory[8] == 5);
        REQUIRE(memory[9] == -6);
        REQUIRE(memory[10] == 5);
        REQUIRE(memory[11] == -6);
        REQUIRE_NOTHROW(dump(memory,accumulator, instructionCounter,
            instructionRegister, operationCode, operand));
    }
    SECTION("Testing add, subtract, multiply, and divide", "execute()") {
        const std::vector<int> inputs{ 5, -6, 7, 8 }; // both positive and negative values
        memory[0] = 1017; // read, 00
        memory[1] = 1018; // read, 01
        memory[2] = 1019; // read, 02
        memory[3] = 1020; // read, 03

        memory[4] = 2017; // load 17, 04
        memory[5] = 3018; // add 18, 05
        memory[6] = 2117; // store ac at 17, 06
        
        memory[7] = 2018; // load 18, 07
        memory[8] = 3119; // subtract 19, 08
        memory[9] = 2118; // store ac at 18, 09

        memory[10] = 2019; // load 19, 10
        memory[11] = 3320; // multiply 20, 11
        memory[12] = 2119; // store ac at 19, 12

        memory[13] = 2020; // load 20, 13
        memory[14] = 3217; // divide 21, 14
        memory[15] = 2120; // store ac at 20, 15

        memory[16] = 4300; // halt, 16
        REQUIRE_NOTHROW(execute(memory, &accumulator, &instructionCounter,
            &instructionRegister, &operationCode, &operand, inputs));
        // checks if changed variables and memory locations are correct
        REQUIRE(accumulator == -8);
        REQUIRE(instructionCounter == 16);
        REQUIRE(instructionRegister == 4300);
        REQUIRE(operationCode == 43);
        REQUIRE(memory[17] == -1);
        REQUIRE(memory[18] == -13);
        REQUIRE(memory[19] == 56);
        REQUIRE(memory[20] == -8);
        REQUIRE_NOTHROW(dump(memory, accumulator, instructionCounter,
            instructionRegister, operationCode, operand));
    }
    SECTION("Testing branch, branchNeg, and branchZero", "execute()") {
        const std::vector<int> inputs{ 5, 1 };
        memory[0] = 1012; // read, 00
        memory[1] = 1013; // read, 01
        memory[2] = 2012; // load, 02
        memory[3] = 4005; // branch to 5, 03

        memory[4] = 3113; // subtract by 13, 04
        memory[5] = 3112; // subtract by 12, 05
        memory[6] = 4208; // branchZero to 08, 06

        memory[7] = 3012; // add 12, 07
        memory[8] = 3113; // subtract 13, 08
        memory[9] = 4111; // branchNeg to 11, 09

        memory[10] = 3312; // multiply by 00, 10
        memory[11] = 4300; // halt, 16
        REQUIRE_NOTHROW(execute(memory, &accumulator, &instructionCounter,
            &instructionRegister, &operationCode, &operand, inputs));
        // checks if changed variables
        REQUIRE(accumulator == -1);
        REQUIRE(instructionCounter == 11);
        REQUIRE(instructionRegister == 4300);
        REQUIRE(operationCode == 43);
        REQUIRE_NOTHROW(dump(memory, accumulator, instructionCounter,
            instructionRegister, operationCode, operand));
    }
    SECTION("Testing throws of load from file", "load_from_file()") {
        // throw for invalid file
        REQUIRE_THROWS(load_from_file(memory, "nonexistentFile.txt"));
    }
    SECTION("Testing throws", "execute()") {
        const std::vector<int> inputs{ 5000, 6000};
        const std::vector<int> inputs2{ -10000, 1 };
        memory[0] = 1012; // read, 00
        memory[1] = 1013; // read, 01
        memory[2] = 2012; // load, 02
        // first throw
        memory[3] = 3013; // add, 03
        memory[4] = 4300; // halt, 04
        REQUIRE_THROWS(execute(memory, &accumulator, &instructionCounter,
            &instructionRegister, &operationCode, &operand, inputs));
        // reset
        accumulator = 0;
        instructionCounter = 0;
        // next throw
        memory[3] = 3113; // subtract by 13, 03
        memory[4] = 3113; // repeat, 04
        memory[5] = 3113; // repeat, 05, causes throw
        memory[6] = 4300;
        REQUIRE_THROWS(execute(memory, &accumulator, &instructionCounter,
            &instructionRegister, &operationCode, &operand, inputs));
        // reset
        accumulator = 0;
        instructionCounter = 0;
        memory[3] = 3213; // divide, 03
        memory[4] = 4300; // halt, 04
        memory[5] = 0; // clear mem
        memory[6] = 0; // clear mem
        REQUIRE_THROWS(execute(memory, &accumulator, &instructionCounter,
            &instructionRegister, &operationCode, &operand, inputs2));
        // reset
        accumulator = 0;
        instructionCounter = 0;
        memory[3] = 3313; // multiply, 03
        memory[4] = 4300; // halt, 04
        REQUIRE_THROWS(execute(memory, &accumulator, &instructionCounter,
            &instructionRegister, &operationCode, &operand, inputs2));

    }
}