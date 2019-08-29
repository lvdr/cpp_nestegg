#include "Instructions.hpp"
#include "ComputerState.hpp"

Instructions::Instructions()
{
    // initialize array
    for (size_t opcode = 0; opcode < 256; opcode++) {
        instruction_array[opcode] = &execute_nop;
    }

    // Add instructions here, by default instructions do nothing
    instruction_array[0x69] = &execute_add_with_carry_immediate;
}

void Instructions::execute(uint8_t opcode, ComputerState &computer_state)
{
    increment_program_counter(computer_state);

    instruction_array[opcode](computer_state);
}

void Instructions::execute_nop(ComputerState &computer_state)
{
    return;
}


void Instructions::execute_add_with_carry_immediate(ComputerState &computer_state)
{
           
}


uint8_t Instructions::get_immediate_byte(ComputerState &computer_state)
{
    uint8_t byte = computer_state.get_byte_from_memory(computer_state.get_program_counter());
    increment_program_counter(computer_state);

    return byte;
}

uint16_t Instructions::get_immediate_word(ComputerState &computer_state)
{
    uint16_t word = computer_state.get_word_from_memory(computer_state.get_program_counter());
    increment_program_counter(computer_state);
    increment_program_counter(computer_state);

    return word;
}


void Instructions::increment_program_counter(ComputerState &computer_state)
{
    computer_state.set_program_counter(computer_state.get_program_counter() + 1);
}
