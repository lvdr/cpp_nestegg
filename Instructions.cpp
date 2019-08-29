#include "Instructions.hpp"
#include "ComputerState.hpp"

Instructions::Instructions()
{
    // initialize array
    for (size_t opcode = 0; opcode < 256; opcode++) {
        instruction_array[opcode] = &execute_nop;
    }

    // Add instructions here, by default instructions do nothing
    instruction_array[0x69] = &execute_add_with_carry_immediate;
}

void Instructions::execute(uint8_t opcode, ComputerState &computer_state)
{
    increment_program_counter(computer_state);

    instruction_array[opcode](computer_state);
}

void Instructions::execute_nop(ComputerState &computer_state)
{
    return;
}


void Instructions::execute_add_with_carry_immediate(ComputerState &computer_state)
{
    uint8_t byte = get_immediate_byte(computer_state);
    add_with_carry(computer_state, byte);
}

void Instructions::add_with_carry(ComputerState &computer_state, uint8_t byte)
{
    uint8_t carry = static_cast<uint8_t>(computer_state.get_status_flag(ComputerState::StatusFlag::CARRY));
    uint8_t accumulator = computer_state.get_accumulator();

    uint16_t sum = byte + carry + accumulator;

    bool new_carry = sum & (1 << 8);
    computer_state.set_status_flag(ComputerState::StatusFlag::CARRY, new_carry);

    sum &= 0xFF;

    bool zero = sum == 0;
    computer_state.set_status_flag(ComputerState::StatusFlag::ZERO, zero);

    bool negative = is_negative(sum);
    computer_state.set_status_flag(ComputerState::StatusFlag::NEGATIVE, negative);

    bool byte_positive = !is_negative(byte);
    bool accumulator_positive = !is_negative(accumulator);
    bool sum_positive = !is_negative(sum);
    bool overflow = byte_positive == accumulator_positive && sum_positive != byte_positive;
    computer_state.set_status_flag(ComputerState::StatusFlag::OVERFLOW, overflow);

    computer_state.set_accumulator(sum);
}

uint8_t Instructions::get_immediate_byte(ComputerState &computer_state)
{
    uint8_t byte = computer_state.get_byte_from_memory(computer_state.get_program_counter());
    increment_program_counter(computer_state);

    return byte;
}

uint16_t Instructions::get_immediate_word(ComputerState &computer_state)
{
    uint16_t word = computer_state.get_word_from_memory(computer_state.get_program_counter());
    increment_program_counter(computer_state);
    increment_program_counter(computer_state);

    return word;
}

void Instructions::increment_program_counter(ComputerState &computer_state)
{
    computer_state.set_program_counter(computer_state.get_program_counter() + 1);
}

bool Instructions::is_negative(uint8_t byte)
{
    return (byte & (1 << 7)) == 127;
}

