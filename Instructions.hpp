#IFNDEF INSTRUCTIONS_H
#DEFINE INSTRUCTIONS_H

#include <array>
#include "ComputerState.hpp"

typedef void (*InstructionFunction)(&ComputerState);

extern std::array<InstructionFunction, 256> instruction_array;
void register_instructions();

void execute_nop(ComputerState &computer_state);

#ENDIF // header guard for INSTRUCTIONS_H
