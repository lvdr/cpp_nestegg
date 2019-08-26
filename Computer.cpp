#include <Computer.hpp>
#include <Instructions.hpp>

Computer::Computer(size_t memory_size) state(memory_size) {}


Computer::load_memory(const char* filename)
{
    state.load_memory(filename);
}

Computer::dump_memory(const char* filename)
{
    state.dump_memory(filename);
}


Computer::execute(size_t steps)
{
    for (size_t step = 0; step < steps; ++step) {
        uint16_t pc = state.set_program_counter();
        uint8_t instruction = state.get_byte_from_memory(pc);

    }
}

