#include <fstream>
#include <cassert>
#include "ComputerState.hpp"

ComputerState::ComputerState(size_t memory_size) : instructions()
{
    memory = std::vector<uint8_t>(memory_size);

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
    accumulator = new_accumulator;
}

void ComputerState::set_x(uint8_t new_x)
{
    x = new_x;
}

void ComputerState::set_y(uint8_t new_y)
{
    y = new_y;
}

void ComputerState::set_status(uint8_t new_status)
{
    status = new_status;
}

void ComputerState::set_stack_pointer(uint8_t new_stack_pointer)
{
    stack_pointer = new_stack_pointer;
}

void ComputerState::set_program_counter(uint16_t new_program_counter)
{
    program_counter = new_program_counter;
}


uint8_t ComputerState::get_status_flag(StatusFlag status_flag)
{
    uint8_t index = std::static_cast<uint8_t>(status_flag); 
    uint8_t flag = status >> index;

    return flag & 0x1;
}

uint8_t ComputerState::get_byte_from_memory(uint16_t index)
{
    return memory[index];
}

uint16_t ComputerState::get_word_from_memory(uint16_t index)
{
    uint16_t word;

    word = memory[index];
    word |= memory[index + 1] << 8;

    return word;
}

void ComputerState::set_byte_to_memory(uint16_t index, uint8_t byte)
{
    memory[index] = byte;
}

void ComputerState::set_word_to_memory(uint16_t index, uint16_t word)
{
    memory[index] = word & 0xFF;
    memory[index + 1] = (word >> 8) & 0xFF;
}

size_t ComputerState::memory_size()
{
    return memory.size();
}

void ComputerState::load_memory(const char* filename)
{
    std::ifstream file(filename, std::ios::binary);
    assert(!file.fail());

    file.seekg(0, std::ios::end);
    int size = file.tellg();
    assert(memory.size() >= size);
    file.seekg(0, std::ios::beg);

    file.read((char*) memory.data(), size);
}

void ComputerState::dump_memory(const char* filename)
{
    std::ofstream file(filename, std::ios::binary);
    assert(!file.fail());
    file.write((char*) memory.data(), memory.size());
}

void ComputerState::load_memory(std::vector<uint8_t> data)
{
    assert(data.size() <= memory.size());
    memory.assign(data.begin(), data.end());
}

std::vector<uint8_t> ComputerState::dump_memory()
{
    return memory;
}

void ComputerState::step(size_t steps)
{
    for (size_t step = 0; step < steps; ++step) {
        uint16_t pc = get_program_counter();
        uint8_t instruction = get_byte_from_memory(pc);
        instructions.execute(instruction, *this);
    }
}
