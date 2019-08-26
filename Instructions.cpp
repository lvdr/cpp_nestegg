#include "Instructions.hpp"
#include "ComputerState.hpp"

Instructions::Instructions()
{
    // initialize array
    for (size_t opcode = 0; opcode < 256; opcode++) {
        instruction_array[opcode] = &execute_nop;
    }

    // Add instructions here, by default instructions do nothing
}

void Instructions::execute(uint8_t opcode, ComputerState &computer_state)
{
    instruction_array[opcode](computer_state);
}

void Instructions::execute_nop(ComputerState &computer_state)
{
    uint16_t ip = computer_state.get_program_counter();
    computer_state.set_program_counter(ip + 1);
    return;
}
