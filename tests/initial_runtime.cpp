#include <numeric>
#include <algorithm>
#include "ComputerState.hpp"

#define CATCH_CONFIG_MAIN

#include "catch.hpp"

void run_and_test(ComputerState &comp, std::vector<uint8_t> results, int substeps = 1) {
    REQUIRE(comp.get_accumulator() == 0);
    for (size_t step = 0; step < results.size(); ++step) {
        INFO("On step " << step);
        comp.step(substeps);
        REQUIRE((int)comp.get_accumulator() == (int)results[step]);
    }
}

void test_immediate_operation(uint8_t opcode, std::vector<uint8_t> operands, std::vector<uint8_t> results, bool with_initialization = false) {

    if (with_initialization) {
        assert(operands.size() == 2*results.size());
    } else {
        assert(operands.size() == results.size());
    }

    ComputerState comp(1024);
    std::vector<uint8_t> initial_memory;
    for (uint8_t operand : operands) {
        if (with_initialization && initial_memory.size() % 4 == 0) {
            initial_memory.push_back(0xa9); // LDA imm
            initial_memory.push_back(operand);
        } else {
            initial_memory.push_back(opcode);
            initial_memory.push_back(operand);
        }
    }

    int substeps = 1;
    if (with_initialization) {
        substeps = 2;
    }

    comp.load_memory(initial_memory);
    run_and_test(comp, results, substeps);
}

void test_zeropage_operation(uint8_t opcode, std::vector<uint8_t> operands, std::vector<uint8_t> results) {
    // Build a vector of addresses from the end of the zeropage for operands
    std::vector<uint8_t> addresses(operands.size());
    uint8_t start_address = 0x100 - operands.size();
    std::iota(addresses.begin(), addresses.end(), start_address);
    // Shuffle addresses
    std::random_shuffle(addresses.begin(), addresses.end());

    std::vector<uint8_t> initial_memory(0x100);
    for (size_t i = 0; i < operands.size(); ++i) {
        initial_memory[i*2] = opcode;
        initial_memory[i*2+1] = addresses[i];
        initial_memory[addresses[i]] = operands[i];
    }

    ComputerState comp(1024);
    comp.load_memory(initial_memory);
    run_and_test(comp, results);
}

TEST_CASE( "NOP", "[base]" ) {
    ComputerState comp(1024);
    std::vector<uint8_t> initial_memory(10, 0xEA);
    comp.load_memory(initial_memory);
    comp.step(10);
    REQUIRE(comp.get_program_counter() == 10);
}


TEST_CASE( "ADC", "[opset]" ) {
    std::vector<uint8_t> operands({0x30, 0x70, 0xff, 0xff, 0x00});
    std::vector<uint8_t> results({0x30, 0xa0, 0x9f, 0x9f, 0xa0});
    {
        INFO("Testing add with immediate addressing");
        // nice
        test_immediate_operation(0x69, operands, results);
    }
    {
        INFO("Testing add with zeropage addressing");
        test_zeropage_operation(0x65, operands, results);
    }
}

TEST_CASE( "LDA", "[opset]" ) {
    std::vector<uint8_t> values(20);
    Catch::Generators::RandomIntegerGenerator<uint8_t> random_generator(0, 255);
    for (size_t i = 0; i < 20; ++i) {
        values[i] = random_generator.get();
        random_generator.next();
    }

    {
        INFO("Testing LDA with immediate addressing");
        test_immediate_operation(0xa9, values, values);
    }
    {
        INFO("Testing LDA with zeropage addressing");
        test_zeropage_operation(0xa5, values, values);
    }
}

TEST_CASE( "AND", "[opset]" ) {
    std::vector<uint8_t> values(20);
    std::vector<uint8_t> results(10);
    Catch::Generators::RandomIntegerGenerator<uint8_t> random_generator(0, 255);
    for (size_t i = 0; i < 20; ++i) {
        values[i] = random_generator.get();
        random_generator.next();
    }
    for (size_t i = 0; i < 10; ++i) {
        results[i] = values[2*i] & values[2*i+1];
    }

    {
        INFO("Testing AND with immediate addressing");
        test_immediate_operation(0x29, values, results, true);
    }
}
