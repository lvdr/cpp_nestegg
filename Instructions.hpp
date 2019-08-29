#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include <array>

class ComputerState;

class Instructions
{
public:
    Instructions();

    void execute(uint8_t opcode, ComputerState &computer_state);

    static void execute_nop(ComputerState &computer_state);
    static void execute_add_with_carry_immediate(ComputerState &computer_state);

private:
    typedef void (*InstructionFunction)(ComputerState&);
    std::array<InstructionFunction, 256> instruction_array;

    uint8_t get_immediate_byte(ComputerState &computer_state);
    uint16_t get_immediate_word(ComputerState &computer_state);

    void increment_program_counter(ComputerState &computer_state);
};

#endif // header guard for INSTRUCTIONS_H
