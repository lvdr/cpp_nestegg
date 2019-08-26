#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include <array>
#include "ComputerState.hpp"

class Instructions
{
public:
    Instructions();

    void execute(uint8_t opcode, ComputerState &computer_state);
    static void execute_nop(ComputerState &computer_state);

private:
    typedef void (*InstructionFunction)(ComputerState&);
    std::array<InstructionFunction, 256> instruction_array;
};

#endif // header guard for INSTRUCTIONS_H
