#IFNDEF COMPUTER_H
#DEFINE COMPUTER_H

#include "ComputerState.hpp"

class Computer
{
public:
    Computer(size_t memory_size);

    load_memory(const char* filename);
    dump_memory(const char* filename);

    execute(size_t steps);

private:
    ComputerState state;
}


#ENDIF // header guard for COMPUTER_H
