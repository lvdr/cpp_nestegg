#include <Computer.hpp>
#include <Instructions.hpp>

Computer::Computer(size_t memory_size) : state(memory_size), instructions() {}


void Computer::load_memory(std::vector<uint8_t> data)
{
    state.load_memory(data);
}

std::vector<uint8_t> Computer::dump_memory()
{
    return state.dump_memory();
}


void Computer::step(size_t steps)
{
    for (size_t step = 0; step < steps; ++step) {
        uint16_t pc = state.get_program_counter();
        uint8_t instruction = state.get_byte_from_memory(pc);
        instructions.execute(instruction, state);
    }
}

