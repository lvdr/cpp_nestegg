#include "Instructions.h"

static std::array<InstructionFunction, 256> instruction_array;

void register_instructions()
{
    // initialize array
    for (uint8_t opcode = 0; opcode < 256; opcode++) {
        instruction_array[opcode] = &execute_nop;
    }

    // Add instructions here, by default instructions do nothing
}

void execute_nop(ComputerState &computer_state)
{
    return;
}
