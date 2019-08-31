#include <cstdio>
#include "Instructions.hpp"
#include "ComputerState.hpp"

Instructions::Instructions()
{
    for (size_t opcode = 0; opcode < 256; opcode++) {
        instruction_array[opcode] = &fail_unimplemented;
    }

    instruction_array[0x61] = &execute_add_with_carry_indirect_x;
    instruction_array[0x65] = &execute_add_with_carry_zeropage;
    instruction_array[0x69] = &execute_add_with_carry_immediate;
    instruction_array[0x6d] = &execute_add_with_carry_absolute;
    instruction_array[0x71] = &execute_add_with_carry_indirect_y;
    instruction_array[0x75] = &execute_add_with_carry_zeropage_x;
    instruction_array[0x79] = &execute_add_with_carry_absolute_y;
    instruction_array[0x7d] = &execute_add_with_carry_absolute_x;

    instruction_array[0x10] = &execute_branch_on_plus;
    instruction_array[0x30] = &execute_branch_on_minus;
    instruction_array[0x50] = &execute_branch_on_overflow_clear;
    instruction_array[0x70] = &execute_branch_on_overflow_set;
    instruction_array[0x90] = &execute_branch_on_carry_clear;
    instruction_array[0xb0] = &execute_branch_on_carry_set;
    instruction_array[0xd0] = &execute_branch_on_not_equal;
    instruction_array[0xf0] = &execute_branch_on_equal;

    instruction_array[0xEA] = &execute_nop;
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

void Instructions::fail_unimplemented(ComputerState &computer_state)
{
    uint8_t opcode = computer_state.get_byte_from_memory(computer_state.get_program_counter() - 1);
    printf("Operation with opcode 0x%x not implemented\n", opcode);
    return;
}

void Instructions::execute_add_with_carry_immediate(ComputerState &computer_state)
{
    uint8_t operand = get_immediate_byte(computer_state);
    add_with_carry(computer_state, operand);
}

void Instructions::execute_add_with_carry_zeropage(ComputerState &computer_state)
{
    uint8_t operand = get_operand_zeropage(computer_state);
    add_with_carry(computer_state, operand);
}

void Instructions::execute_add_with_carry_zeropage_x(ComputerState &computer_state)
{
    uint8_t operand = get_operand_zeropage_x_indexed(computer_state);
    add_with_carry(computer_state, operand);
}

void Instructions::execute_add_with_carry_absolute(ComputerState &computer_state)
{
    uint8_t operand = get_operand_absolute(computer_state);
    add_with_carry(computer_state, operand);
}

void Instructions::execute_add_with_carry_absolute_x(ComputerState &computer_state)
{
    uint8_t operand = get_operand_absolute_x_indexed(computer_state);
    add_with_carry(computer_state, operand);
}

void Instructions::execute_add_with_carry_absolute_y(ComputerState &computer_state)
{
    uint8_t operand = get_operand_absolute_y_indexed(computer_state);
    add_with_carry(computer_state, operand);
}

void Instructions::execute_add_with_carry_indirect_x(ComputerState &computer_state)
{
    uint8_t operand = get_operand_x_indexed_indirect(computer_state);
    add_with_carry(computer_state, operand);
}

void Instructions::execute_add_with_carry_indirect_y(ComputerState &computer_state)
{
    uint8_t operand = get_operand_indirect_y_indexed(computer_state);
    add_with_carry(computer_state, operand);
}

void Instructions::add_with_carry(ComputerState &computer_state, uint8_t operand)
{
    uint8_t carry = static_cast<uint8_t>(computer_state.get_status_flag(ComputerState::StatusFlag::CARRY));
    uint8_t accumulator = computer_state.get_accumulator();

    uint16_t sum = operand + carry + accumulator;

    bool new_carry = sum & (1 << 8);
    computer_state.set_status_flag(ComputerState::StatusFlag::CARRY, new_carry);

    sum &= 0xFF;

    bool zero = sum == 0;
    computer_state.set_status_flag(ComputerState::StatusFlag::ZERO, zero);

    bool negative = is_negative(sum);
    computer_state.set_status_flag(ComputerState::StatusFlag::NEGATIVE, negative);

    bool byte_positive = !is_negative(operand);
    bool accumulator_positive = !is_negative(accumulator);
    bool sum_positive = !is_negative(sum);
    bool overflow = byte_positive == accumulator_positive && sum_positive != byte_positive;
    computer_state.set_status_flag(ComputerState::StatusFlag::OVERFLOW, overflow);

    computer_state.set_accumulator(sum);
}

void Instructions::execute_branch_on_carry_set(ComputerState &computer_state)
{
    uint8_t offset = get_immediate_byte(computer_state);
    if (computer_state.get_status_flag(ComputerState::StatusFlag::CARRY)) {
        uint16_t new_program_counter = computer_state.get_program_counter();
        new_program_counter += static_cast<int8_t>(offset);
    }
}

void Instructions::execute_branch_on_carry_clear(ComputerState &computer_state)
{
    uint8_t offset = get_immediate_byte(computer_state);
    if (!computer_state.get_status_flag(ComputerState::StatusFlag::CARRY)) {
        uint16_t new_program_counter = computer_state.get_program_counter();
        new_program_counter += static_cast<int8_t>(offset);
    }
}

void Instructions::execute_branch_on_equal(ComputerState &computer_state)
{
    uint8_t offset = get_immediate_byte(computer_state);
    if (computer_state.get_status_flag(ComputerState::StatusFlag::ZERO)) {
        uint16_t new_program_counter = computer_state.get_program_counter();
        new_program_counter += static_cast<int8_t>(offset);
    }
}

void Instructions::execute_branch_on_not_equal(ComputerState &computer_state)
{
    uint8_t offset = get_immediate_byte(computer_state);
    if (!computer_state.get_status_flag(ComputerState::StatusFlag::ZERO)) {
        uint16_t new_program_counter = computer_state.get_program_counter();
        new_program_counter += static_cast<int8_t>(offset);
    }
}


void Instructions::execute_branch_on_overflow_set(ComputerState &computer_state)
{
    uint8_t offset = get_immediate_byte(computer_state);
    if (computer_state.get_status_flag(ComputerState::StatusFlag::OVERFLOW)) {
        uint16_t new_program_counter = computer_state.get_program_counter();
        new_program_counter += static_cast<int8_t>(offset);
    }
}

void Instructions::execute_branch_on_overflow_clear(ComputerState &computer_state)
{
    uint8_t offset = get_immediate_byte(computer_state);
    if (!computer_state.get_status_flag(ComputerState::StatusFlag::OVERFLOW)) {
        uint16_t new_program_counter = computer_state.get_program_counter();
        new_program_counter += static_cast<int8_t>(offset);
    }
}


void Instructions::execute_branch_on_plus(ComputerState &computer_state)
{
    uint8_t offset = get_immediate_byte(computer_state);
    if (!computer_state.get_status_flag(ComputerState::StatusFlag::NEGATIVE)) {
        uint16_t new_program_counter = computer_state.get_program_counter();
        new_program_counter += static_cast<int8_t>(offset);
    }
}

void Instructions::execute_branch_on_minus(ComputerState &computer_state)
{
    uint8_t offset = get_immediate_byte(computer_state);
    if (computer_state.get_status_flag(ComputerState::StatusFlag::NEGATIVE)) {
        uint16_t new_program_counter = computer_state.get_program_counter();
        new_program_counter += static_cast<int8_t>(offset);
    }
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

uint8_t Instructions::get_operand_absolute(ComputerState &computer_state)
{
    uint16_t address = get_immediate_word(computer_state);
    return computer_state.get_byte_from_memory(address);
}

uint8_t Instructions::get_operand_absolute_x_indexed(ComputerState &computer_state)
{
    uint16_t address = get_immediate_word(computer_state) + computer_state.get_x();
    return computer_state.get_byte_from_memory(address);
}

uint8_t Instructions::get_operand_absolute_y_indexed(ComputerState &computer_state)
{
    uint16_t address = get_immediate_word(computer_state) + computer_state.get_y();
    return computer_state.get_byte_from_memory(address);
}

uint8_t Instructions::get_operand_indirect(ComputerState &computer_state)
{
    uint8_t address_of_pointer = get_immediate_word(computer_state);
    uint8_t address = computer_state.get_word_from_memory(address_of_pointer);
    return computer_state.get_byte_from_memory(address);
}

uint8_t Instructions::get_operand_x_indexed_indirect(ComputerState &computer_state)
{
    uint8_t address_of_pointer = get_immediate_word(computer_state) + computer_state.get_x();
    uint8_t address = computer_state.get_word_from_memory(address_of_pointer);
    return computer_state.get_byte_from_memory(address);
}

uint8_t Instructions::get_operand_indirect_y_indexed(ComputerState &computer_state)
{
    uint8_t address_of_pointer = get_immediate_word(computer_state);
    uint8_t address = computer_state.get_word_from_memory(address_of_pointer) + computer_state.get_y();
    return computer_state.get_byte_from_memory(address);
}

uint8_t Instructions::get_operand_zeropage(ComputerState &computer_state)
{
    uint8_t index = get_immediate_byte(computer_state);
    return computer_state.get_byte_from_memory(index);
}

uint8_t Instructions::get_operand_zeropage_x_indexed(ComputerState &computer_state)
{
    uint8_t address = get_immediate_byte(computer_state) + computer_state.get_x();
    return computer_state.get_byte_from_memory(address);
}

uint8_t Instructions::get_operand_zeropage_y_indexed(ComputerState &computer_state)
{
    uint8_t address = get_immediate_byte(computer_state) + computer_state.get_y();
    return computer_state.get_byte_from_memory(address);
}


void Instructions::increment_program_counter(ComputerState &computer_state)
{
    computer_state.set_program_counter(computer_state.get_program_counter() + 1);
}

bool Instructions::is_negative(uint8_t byte)
{
    return (byte & (1 << 7)) == 127;
}

