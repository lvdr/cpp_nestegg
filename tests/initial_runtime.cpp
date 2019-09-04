#include <numeric>
#include <algorithm>
#include "ComputerState.hpp"

#define CATCH_CONFIG_MAIN

#include "catch.hpp"
enum Offset {
    None = 0,
    X = 1,
    Y = 2
};

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

void test_zeropage_operation(uint8_t opcode, std::vector<uint8_t> operands, std::vector<uint8_t> results, bool with_initialization = false, Offset offset = Offset::None) {

    if (with_initialization) {
        assert(operands.size() == 2*results.size());
    } else {
        assert(operands.size() == results.size());
    }

    // Build a vector of addresses from the end of the zeropage for operands
    std::vector<uint8_t> addresses(operands.size());
    uint8_t start_address = 0x100 - operands.size();
    std::iota(addresses.begin(), addresses.end(), start_address);
    // Shuffle addresses
    std::random_shuffle(addresses.begin(), addresses.end());

    ComputerState comp(1024);

    uint8_t base_offset = 0;

    if (offset == Offset::X || offset == Offset::Y) {
        base_offset = start_address;

        for (size_t i = 0; i < addresses.size(); ++i) {
            addresses[i] -= base_offset;
        }

        if (offset == Offset::X) {
            comp.set_x(base_offset);
        } else {
            comp.set_y(base_offset);
        }
    }

    std::vector<uint8_t> initial_memory(0x100);

    int substeps = 1;
    if (with_initialization) {
        substeps = 2;
    }

    for (size_t i = 0; i < operands.size(); i += substeps) {
        if (with_initialization) {
            initial_memory[i*2] = 0xa9; // LDA IMM
            initial_memory[i*2+1] = operands[i];
            initial_memory[i*2+2] = opcode;
            initial_memory[i*2+3] = addresses[i+1];
            initial_memory[addresses[i+1] + base_offset] = operands[i+1];
        } else {
            initial_memory[i*2] = opcode;
            initial_memory[i*2+1] = addresses[i];
            initial_memory[addresses[i] + base_offset] = operands[i];
        }
    }

    comp.load_memory(initial_memory);
    run_and_test(comp, results, substeps);
}

void test_absolute_operation(uint8_t opcode, std::vector<uint8_t> operands, std::vector<uint8_t> results, bool with_initialization = false, Offset offset = Offset::None) {

    if (with_initialization) {
        assert(operands.size() == 2*results.size());
    } else {
        assert(operands.size() == results.size());
    }
    // Build a vector of addresses from memory for operands
    std::vector<uint16_t> addresses(operands.size());
    uint8_t start_address = 0x400 - operands.size();
    std::iota(addresses.begin(), addresses.end(), start_address);
    // Shuffle addresses
    std::random_shuffle(addresses.begin(), addresses.end());

    ComputerState comp(0x800);

    uint8_t base_offset = 0;

    if (offset == Offset::X || offset == Offset::Y) {
        base_offset = start_address;

        for (size_t i = 0; i < addresses.size(); ++i) {
            addresses[i] -= base_offset;
        }

        if (offset == Offset::X) {
            comp.set_x(base_offset);
        } else {
            comp.set_y(base_offset);
        }
    }

    std::vector<uint8_t> initial_memory(0x100);
    int substeps = 1;
    if (with_initialization) {
        substeps = 2;
    }

    for (size_t i = 0; i < results.size(); ++i) {
        if (with_initialization) {
            initial_memory[i*5] = 0xa9; // LDA Imm
            initial_memory[i*5+1] = operands.at(i*2);
            initial_memory[i*5+2] = opcode;
            initial_memory[i*5+3] = addresses.at(i) & 0xff;
            initial_memory[i*5+4] = (addresses.at(i) >> 8) & 0xff;
            initial_memory[addresses.at(i) + base_offset] = operands[i*2+1];
        } else {
            initial_memory[i*3] = opcode;
            initial_memory[i*3+1] = addresses[i] & 0xff;
            initial_memory[i*3+2] = (addresses[i] >> 8) & 0xff;
            initial_memory[addresses[i] + base_offset] = operands[i];
        }
    }

    comp.load_memory(initial_memory);
    run_and_test(comp, results, substeps);
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
    {
        INFO("Testing add with zeropage X addressing");
        test_zeropage_operation(0x75, operands, results, false, Offset::X);
    }
    {
        INFO("Testing add with absolute addressing");
        test_absolute_operation(0x6d, operands, results);
    }
    {
        INFO("Testing add with absolute X addressing");
        test_absolute_operation(0x7d, operands, results, false, Offset::X);
    }
    {
        INFO("Testing add with absolute Y addressing");
        test_absolute_operation(0x79, operands, results, false, Offset::Y);
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
    {
        INFO("Testing LDA with zeropage X addressing");
        test_zeropage_operation(0xb5, values, values, false, Offset::X);
    }
    {
        INFO("Testing LDA with absolute addressing");
        test_absolute_operation(0xad, values, values);
    }
    {
        INFO("Testing LDA with absolute X addressing");
        test_absolute_operation(0xbd, values, values, false, Offset::X);
    }
    {
        INFO("Testing LDA with absolute Y addressing");
        test_absolute_operation(0xb9, values, values, false, Offset::Y);
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
    {
        INFO("Testing AND with zeropage addressing");
        test_zeropage_operation(0x25, values, results, true);
    }
    {
        INFO("Testing AND with zeropage X addressing");
        test_zeropage_operation(0x35, values, results, true, Offset::X);
    }
    {
        INFO("Testing AND with absolute addressing");
        test_absolute_operation(0x2d, values, results, true);
    }
    {
        INFO("Testing AND with absolute X addressing");
        test_absolute_operation(0x3d, values, results, true, Offset::X);
    }
    {
        INFO("Testing AND with absolute Y addressing");
        test_absolute_operation(0x39, values, results, true, Offset::Y);
    }
}
