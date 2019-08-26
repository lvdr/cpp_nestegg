#ifndef COMPUTER_H
#define COMPUTER_H

#include "ComputerState.hpp"
#include "Instructions.hpp"

class Computer
{
public:
    Computer(size_t memory_size);

    void load_memory(std::vector<uint8_t> data);
    std::vector<uint8_t> dump_memory();

    void step(size_t steps);

private:
    ComputerState state;
    Instructions instructions;
};


#endif // header guard for COMPUTER_H
