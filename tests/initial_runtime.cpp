#include "ComputerState.hpp"

#define CATCH_CONFIG_MAIN

#include "catch.hpp"

TEST_CASE( "Initial smoketest", "[base]" ) {
    ComputerState comp(200);
    comp.step(10);
    REQUIRE(comp.get_program_counter() == 10);
}