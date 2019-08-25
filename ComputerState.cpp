#include "ComputerState.h"

ComputerState::ComputerState(size_t memory_size)
{
    memory = std::vector<uint8_t>(size_t memory_size);

    uint8_t accumulator = 0;
    uint8_t x = 0;
    uint8_t y = 0;
    uint8_t status = 0;
    uint8_t stack_pointer = 0;
    uint16_t program_counter = 0;
}

uint8_t ComputerState::get_accumulator()
{
    return accumulator;
}

uint8_t ComputerState::get_x()
{
    return x;
}

uint8_t ComputerState::get_y()
{
    return y;
}

uint8_t ComputerState::get_status()
{
    return status;
}

uint8_t ComputerState::get_stack_pointer()
{
    return stack_pointer;
}

uint16_t ComputerState::get_program_counter()
{
    return program_counter;
}

void ComputerState::set_accumulator(uint8_t new_accumulator)
{

}
void ComputerState::set_x(uint8_t new_x)
{

}
void ComputerState::set_y(uint8_t new_y)
{

}
void ComputerState::set_status(uint8_t new_status)
{

}
void ComputerState::set_stack_pointer(uint8_t new_stack_pointer)
{

}
void ComputerState::set_program_counter(uint16_t new_program_counter)
{

}

uint8_t ComputerState::get_byte_from_memory(uint16_t index)
{

}
uint16_t ComputerState::get_word_from_memory(uint16_t index)
{

}

void ComputerState::set_byte_to_memory(uint16_t index, uint8_t byte)
{

}
void ComputerState::set_word_to_memory(uint16_t index, uint16_t word)
{

}
