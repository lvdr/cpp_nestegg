#ifndef COMPUTERSTATE_H
#define COMPUTERSTATE_H

#include <vector>
#include <cstdint>

class ComputerState;

#include "Instructions.hpp"

class ComputerState
{
public:
    enum class StatusFlag
    {
        CARRY     = 0,
        ZERO      = 1,
        INTERRUPT = 2,
        DECIMAL   = 3,
        BREAK     = 4,
        RESERVED  = 5,
        OVERFLOW  = 6,
        NEGATIVE  = 7
    };

    ComputerState(size_t memory_size);

    uint8_t get_accumulator();
    uint8_t get_x();
    uint8_t get_y();
    uint8_t get_status();
    uint8_t get_stack_pointer();
    uint16_t get_program_counter();

    void set_accumulator(uint8_t new_accumulator);
    void set_x(uint8_t new_x);
    void set_y(uint8_t new_y);
    void set_status(uint8_t new_status);
    void set_stack_pointer(uint8_t new_stack_pointer);
    void set_program_counter(uint16_t new_program_counter);

    bool get_status_flag(StatusFlag status_flag);
    void set_status_flag(StatusFlag status_flag, bool new_flag);

    uint8_t get_byte_from_memory(uint16_t index);
    uint16_t get_word_from_memory(uint16_t index);

    void set_byte_to_memory(uint16_t index, uint8_t byte);
    void set_word_to_memory(uint16_t index, uint16_t word);

    size_t memory_size();

    void load_memory(const char* filename);
    void dump_memory(const char* filename);

    void load_memory(std::vector<uint8_t> data);
    std::vector<uint8_t> dump_memory();

    void step(size_t steps);

private:
    Instructions instructions;

    uint8_t accumulator;
    uint8_t x;
    uint8_t y;
    uint8_t status;
    uint8_t stack_pointer;
    uint16_t program_counter;

    std::vector<uint8_t> memory;
};


#endif // header guard for COMPUTERSTATE_H
