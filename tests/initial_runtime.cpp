#include "ComputerState.hpp"

#define CATCH_CONFIG_MAIN

#include "catch.hpp"

TEST_CASE( "Initial smoketest", "[base]" ) {
    ComputerState comp(1024);
    std::vector<uint8_t> initial_memory(10, 0xEA);
    comp.load_memory(initial_memory);
    comp.step(10);
    REQUIRE(comp.get_program_counter() == 10);
}

TEST_CASE( "ADC immediate", "[base]" ) {
    ComputerState comp(1024);
    // Nice
    std::vector<uint8_t> initial_memory({0x69, 0x30, 0x69, 0x70,
                                         0x69, 0xff, 0x69, 0xff});
    comp.load_memory(initial_memory);
    comp.step(1);
    REQUIRE(comp.get_accumulator() == 0x30);
    REQUIRE(comp.get_status_flag(ComputerState::StatusFlag::CARRY) == false);
    comp.step(1);
    REQUIRE(comp.get_accumulator() == 0xa0);
    REQUIRE(comp.get_status_flag(ComputerState::StatusFlag::CARRY) == false);
    comp.step(1);
    REQUIRE(comp.get_accumulator() == 0x9f);
    REQUIRE(comp.get_status_flag(ComputerState::StatusFlag::CARRY) == true);
    comp.step(1);
    REQUIRE(comp.get_accumulator() == 0x9f);
    REQUIRE(comp.get_status_flag(ComputerState::StatusFlag::CARRY) == true);
}

TEST_CASE( "ADC zeropage", "[base]" ) {
    ComputerState comp(1024);
    // Nice
    std::vector<uint8_t> initial_memory({0x65, 8, 0x65, 10,
                                         0x65, 9, 0x65, 9,
                                         0x30, 0xff, 0x70});
    comp.load_memory(initial_memory);
    comp.step(1);
    REQUIRE(comp.get_accumulator() == 0x30);
    REQUIRE(comp.get_status_flag(ComputerState::StatusFlag::CARRY) == false);
    comp.step(1);
    REQUIRE(comp.get_accumulator() == 0xa0);
    REQUIRE(comp.get_status_flag(ComputerState::StatusFlag::CARRY) == false);
    comp.step(1);
    REQUIRE(comp.get_accumulator() == 0x9f);
    REQUIRE(comp.get_status_flag(ComputerState::StatusFlag::CARRY) == true);
    comp.step(1);
    REQUIRE(comp.get_accumulator() == 0x9f);
    REQUIRE(comp.get_status_flag(ComputerState::StatusFlag::CARRY) == true);
}
