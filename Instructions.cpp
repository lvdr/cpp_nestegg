#include <cstdio>
#include "Instructions.hpp"
#include "ComputerState.hpp"

Instructions::Instructions()
{
    for (size_t opcode = 0; opcode < 256; opcode++) {
        instruction_array[opcode] = std::make_pair(&get_operand_noop, &fail_unimplemented);
    }

    // ADC
    instruction_array[0x61] = std::make_pair(&get_operand_indirect_x, &execute_add_with_carry);
    instruction_array[0x65] = std::make_pair(&get_operand_zeropage, &execute_add_with_carry);
    instruction_array[0x69] = std::make_pair(&get_operand_immediate, &execute_add_with_carry);
    instruction_array[0x6d] = std::make_pair(&get_operand_absolute, &execute_add_with_carry);
    instruction_array[0x71] = std::make_pair(&get_operand_indirect_y, &execute_add_with_carry);
    instruction_array[0x75] = std::make_pair(&get_operand_zeropage_x, &execute_add_with_carry);
    instruction_array[0x79] = std::make_pair(&get_operand_absolute_y, &execute_add_with_carry);
    instruction_array[0x7d] = std::make_pair(&get_operand_absolute_x, &execute_add_with_carry);

    // AND
    instruction_array[0x29] = std::make_pair(&get_operand_immediate, &execute_and);
    instruction_array[0x25] = std::make_pair(&get_operand_zeropage, &execute_and);
    instruction_array[0x35] = std::make_pair(&get_operand_zeropage_x, &execute_and);
    instruction_array[0x2d] = std::make_pair(&get_operand_absolute, &execute_and);
    instruction_array[0x3d] = std::make_pair(&get_operand_absolute_x, &execute_and);
    instruction_array[0x39] = std::make_pair(&get_operand_absolute_y, &execute_and);
    instruction_array[0x21] = std::make_pair(&get_operand_indirect_x, &execute_and);
    instruction_array[0x31] = std::make_pair(&get_operand_indirect_y, &execute_and);

    // ORA
    instruction_array[0x09] = std::make_pair(&get_operand_immediate, &execute_ior);
    instruction_array[0x05] = std::make_pair(&get_operand_zeropage, &execute_ior);
    instruction_array[0x15] = std::make_pair(&get_operand_zeropage_x, &execute_ior);
    instruction_array[0x0d] = std::make_pair(&get_operand_absolute, &execute_ior);
    instruction_array[0x1d] = std::make_pair(&get_operand_absolute_x, &execute_ior);
    instruction_array[0x19] = std::make_pair(&get_operand_absolute_y, &execute_ior);
    instruction_array[0x01] = std::make_pair(&get_operand_indirect_x, &execute_ior);
    instruction_array[0x11] = std::make_pair(&get_operand_indirect_y, &execute_ior);

    // EOR
    instruction_array[0x49] = std::make_pair(&get_operand_immediate, &execute_xor);
    instruction_array[0x45] = std::make_pair(&get_operand_zeropage, &execute_xor);
    instruction_array[0x55] = std::make_pair(&get_operand_zeropage_x, &execute_xor);
    instruction_array[0x4d] = std::make_pair(&get_operand_absolute, &execute_xor);
    instruction_array[0x5d] = std::make_pair(&get_operand_absolute_x, &execute_xor);
    instruction_array[0x59] = std::make_pair(&get_operand_absolute_y, &execute_xor);
    instruction_array[0x41] = std::make_pair(&get_operand_indirect_x, &execute_xor);
    instruction_array[0x51] = std::make_pair(&get_operand_indirect_y, &execute_xor);

    instruction_array[0x24] = std::make_pair(&get_operand_zeropage, &execute_test_bits);
    instruction_array[0x2c] = std::make_pair(&get_operand_absolute, &execute_test_bits);

    // Branches: BPL, BMI, BVC, BVS, BCC, BCS, BEQ, BNE
    instruction_array[0x10] = std::make_pair(&get_operand_immediate, &execute_branch_on_plus);
    instruction_array[0x30] = std::make_pair(&get_operand_immediate, &execute_branch_on_minus);
    instruction_array[0x50] = std::make_pair(&get_operand_immediate, &execute_branch_on_overflow_clear);
    instruction_array[0x70] = std::make_pair(&get_operand_immediate, &execute_branch_on_overflow_set);
    instruction_array[0x90] = std::make_pair(&get_operand_immediate, &execute_branch_on_carry_clear);
    instruction_array[0xb0] = std::make_pair(&get_operand_immediate, &execute_branch_on_carry_set);
    instruction_array[0xd0] = std::make_pair(&get_operand_immediate, &execute_branch_on_not_equal);
    instruction_array[0xf0] = std::make_pair(&get_operand_immediate, &execute_branch_on_equal);

    instruction_array[0xee] = std::make_pair(&get_operand_noop, &execute_increment_memory_absolute);
    instruction_array[0xfe] = std::make_pair(&get_operand_noop, &execute_increment_memory_absolute_x);
    instruction_array[0xe6] = std::make_pair(&get_operand_noop, &execute_increment_memory_zeropage);
    instruction_array[0xf6] = std::make_pair(&get_operand_noop, &execute_increment_memory_zeropage_x);

    // CMP
    instruction_array[0xc1] = std::make_pair(&get_operand_indirect_x, &execute_compare_with_accumulator);
    instruction_array[0xc5] = std::make_pair(&get_operand_zeropage, &execute_compare_with_accumulator);
    instruction_array[0xc9] = std::make_pair(&get_operand_immediate, &execute_compare_with_accumulator);
    instruction_array[0xcd] = std::make_pair(&get_operand_absolute, &execute_compare_with_accumulator);
    instruction_array[0xd1] = std::make_pair(&get_operand_indirect_y, &execute_compare_with_accumulator);
    instruction_array[0xd5] = std::make_pair(&get_operand_zeropage_x, &execute_compare_with_accumulator);
    instruction_array[0xd9] = std::make_pair(&get_operand_absolute_y, &execute_compare_with_accumulator);
    instruction_array[0xdd] = std::make_pair(&get_operand_absolute_x, &execute_compare_with_accumulator);

    // CPX
    instruction_array[0xE0] = std::make_pair(&get_operand_immediate, &execute_compare_with_x);
    instruction_array[0xE4] = std::make_pair(&get_operand_zeropage, &execute_compare_with_x);
    instruction_array[0xEC] = std::make_pair(&get_operand_absolute, &execute_compare_with_x);

    // CPY
    instruction_array[0xC0] = std::make_pair(&get_operand_immediate, &execute_compare_with_y);
    instruction_array[0xC4] = std::make_pair(&get_operand_zeropage, &execute_compare_with_y);
    instruction_array[0xCC] = std::make_pair(&get_operand_absolute, &execute_compare_with_y);

    // NOP
    instruction_array[0xEA] = std::make_pair(&get_operand_noop, &execute_nop);

    // LDA
    instruction_array[0xa9] = std::make_pair(&get_operand_immediate, &execute_load_accumulator);
    instruction_array[0xa5] = std::make_pair(&get_operand_zeropage, &execute_load_accumulator);
    instruction_array[0xb5] = std::make_pair(&get_operand_zeropage_x, &execute_load_accumulator);
    instruction_array[0xad] = std::make_pair(&get_operand_absolute, &execute_load_accumulator);
    instruction_array[0xbd] = std::make_pair(&get_operand_absolute_x, &execute_load_accumulator);
    instruction_array[0xb9] = std::make_pair(&get_operand_absolute_y, &execute_load_accumulator);
    instruction_array[0xa1] = std::make_pair(&get_operand_indirect_x, &execute_load_accumulator);
    instruction_array[0xb1] = std::make_pair(&get_operand_indirect_y, &execute_load_accumulator);

    // LDX
    instruction_array[0xa2] = std::make_pair(&get_operand_immediate, &execute_load_x);
    instruction_array[0xa6] = std::make_pair(&get_operand_zeropage, &execute_load_x);
    instruction_array[0xb6] = std::make_pair(&get_operand_zeropage_y, &execute_load_x);
    instruction_array[0xae] = std::make_pair(&get_operand_absolute, &execute_load_x);
    instruction_array[0xbe] = std::make_pair(&get_operand_absolute_y, &execute_load_x);

    // LDY
    instruction_array[0xa0] = std::make_pair(&get_operand_immediate, &execute_load_y);
    instruction_array[0xa4] = std::make_pair(&get_operand_zeropage, &execute_load_y);
    instruction_array[0xb4] = std::make_pair(&get_operand_zeropage_y, &execute_load_y);
    instruction_array[0xac] = std::make_pair(&get_operand_absolute, &execute_load_y);
    instruction_array[0xbc] = std::make_pair(&get_operand_absolute_y, &execute_load_y);
}

void Instructions::execute(uint8_t opcode, ComputerState &computer_state)
{
    increment_program_counter(computer_state);

    OperandFunction operand_function = instruction_array[opcode].first;
    InstructionFunction instruction_function = instruction_array[opcode].second;

    instruction_function(computer_state, operand_function(computer_state));
}

void Instructions::execute_nop(ComputerState &computer_state, uint8_t)
{
    return;
}

void Instructions::fail_unimplemented(ComputerState &computer_state, uint8_t)
{
    uint8_t opcode = computer_state.get_byte_from_memory(computer_state.get_program_counter() - 1);
    printf("Operation with opcode 0x%x not implemented\n", opcode);
    return;
}

void Instructions::execute_add_with_carry(ComputerState &computer_state, uint8_t operand)
{
    uint8_t carry = static_cast<uint8_t>(computer_state.get_status_flag(ComputerState::StatusFlag::CARRY));
    uint8_t accumulator = computer_state.get_accumulator();

    uint16_t sum = operand + carry + accumulator;

    bool new_carry = sum & (1 << 8);
    computer_state.set_status_flag(ComputerState::StatusFlag::CARRY, new_carry);

    sum &= 0xFF;

    bool byte_positive = !is_negative(operand);
    bool accumulator_positive = !is_negative(accumulator);
    bool sum_positive = !is_negative(sum);
    bool overflow = byte_positive == accumulator_positive && sum_positive != byte_positive;
    computer_state.set_status_flag(ComputerState::StatusFlag::OVERFLOW, overflow);

    computer_state.set_accumulator(sum);
}


void Instructions::execute_and(ComputerState &computer_state, uint8_t operand) {
    uint8_t result = computer_state.get_accumulator() & operand;
    computer_state.set_accumulator(result);
}

void Instructions::execute_ior(ComputerState &computer_state, uint8_t operand) {
    uint8_t result = computer_state.get_accumulator() | operand;
    computer_state.set_accumulator(result);
}

void Instructions::execute_xor(ComputerState &computer_state, uint8_t operand) {
    uint8_t result = computer_state.get_accumulator() ^ operand;
    computer_state.set_accumulator(result);
}

void Instructions::execute_test_bits(ComputerState &computer_state, uint8_t operand)
{
    uint8_t accumulator = computer_state.get_accumulator();
    uint8_t result = accumulator & operand;

    computer_state.set_status_flag(ComputerState::StatusFlag::NEGATIVE, result & (1 << 7));
    computer_state.set_status_flag(ComputerState::StatusFlag::OVERFLOW, result & (1 << 6));
    computer_state.set_status_flag(ComputerState::StatusFlag::ZERO, result == 0);
}

void Instructions::execute_compare_with_accumulator(ComputerState& computer_state, uint8_t operand)
{
    uint16_t result = computer_state.get_accumulator() - operand;
    computer_state.set_status_flag(ComputerState::StatusFlag::CARRY, result & (1 << 8));
    computer_state.set_status_flag(ComputerState::StatusFlag::ZERO, result == 0);
    computer_state.set_status_flag(ComputerState::StatusFlag::NEGATIVE, result & (1 << 7));
}

void Instructions::execute_compare_with_x(ComputerState& computer_state, uint8_t operand)
{
    uint16_t result = computer_state.get_x() - operand;
    computer_state.set_status_flag(ComputerState::StatusFlag::CARRY, result & (1 << 8));
    computer_state.set_status_flag(ComputerState::StatusFlag::ZERO, result == 0);
    computer_state.set_status_flag(ComputerState::StatusFlag::NEGATIVE, result & (1 << 7));
}

void Instructions::execute_compare_with_y(ComputerState& computer_state, uint8_t operand)
{
    uint16_t result = computer_state.get_y() - operand;
    computer_state.set_status_flag(ComputerState::StatusFlag::CARRY, result & (1 << 8));
    computer_state.set_status_flag(ComputerState::StatusFlag::ZERO, result == 0);
    computer_state.set_status_flag(ComputerState::StatusFlag::NEGATIVE, is_negative(result));
}


void Instructions::execute_branch_on_carry_set(ComputerState &computer_state, uint8_t offset)
{
    if (computer_state.get_status_flag(ComputerState::StatusFlag::CARRY)) {
        uint16_t new_program_counter = computer_state.get_program_counter();
        new_program_counter += static_cast<int8_t>(offset);
        computer_state.set_program_counter(new_program_counter);
    }
}

void Instructions::execute_branch_on_carry_clear(ComputerState &computer_state, uint8_t offset)
{
    if (!computer_state.get_status_flag(ComputerState::StatusFlag::CARRY)) {
        uint16_t new_program_counter = computer_state.get_program_counter();
        new_program_counter += static_cast<int8_t>(offset);
        computer_state.set_program_counter(new_program_counter);
    }
}

void Instructions::execute_branch_on_equal(ComputerState &computer_state, uint8_t offset)
{
    if (computer_state.get_status_flag(ComputerState::StatusFlag::ZERO)) {
        uint16_t new_program_counter = computer_state.get_program_counter();
        new_program_counter += static_cast<int8_t>(offset);
        computer_state.set_program_counter(new_program_counter);
    }
}

void Instructions::execute_branch_on_not_equal(ComputerState &computer_state, uint8_t offset)
{
    if (!computer_state.get_status_flag(ComputerState::StatusFlag::ZERO)) {
        uint16_t new_program_counter = computer_state.get_program_counter();
        new_program_counter += static_cast<int8_t>(offset);
        computer_state.set_program_counter(new_program_counter);
    }
}


void Instructions::execute_branch_on_overflow_set(ComputerState &computer_state, uint8_t offset)
{
    if (computer_state.get_status_flag(ComputerState::StatusFlag::OVERFLOW)) {
        uint16_t new_program_counter = computer_state.get_program_counter();
        new_program_counter += static_cast<int8_t>(offset);
        computer_state.set_program_counter(new_program_counter);
    }
}

void Instructions::execute_branch_on_overflow_clear(ComputerState &computer_state, uint8_t offset)
{
    if (!computer_state.get_status_flag(ComputerState::StatusFlag::OVERFLOW)) {
        uint16_t new_program_counter = computer_state.get_program_counter();
        new_program_counter += static_cast<int8_t>(offset);
        computer_state.set_program_counter(new_program_counter);
    }
}


void Instructions::execute_branch_on_plus(ComputerState &computer_state, uint8_t offset)
{
    if (!computer_state.get_status_flag(ComputerState::StatusFlag::NEGATIVE)) {
        uint16_t new_program_counter = computer_state.get_program_counter();
        new_program_counter += static_cast<int8_t>(offset);
        computer_state.set_program_counter(new_program_counter);
    }
}

void Instructions::execute_branch_on_minus(ComputerState &computer_state, uint8_t offset)
{
    if (computer_state.get_status_flag(ComputerState::StatusFlag::NEGATIVE)) {
        uint16_t new_program_counter = computer_state.get_program_counter();
        new_program_counter += static_cast<int8_t>(offset);
        computer_state.set_program_counter(new_program_counter);
    }
}

void Instructions::execute_increment_memory_absolute(ComputerState& computer_state, uint8_t unused)
{
    uint16_t address = get_immediate_word();
    uint8_t byte = computer_state.get_byte_from_memory(address);
    computer_state.set_byte_to_memory(address, byte + 1);
}

void Instructions::execute_increment_memory_absolute_x(ComputerState& computer_state, uint8_t unused)
{
    uint16_t address = get_immediate_word();
    uint8_t offset = computer_state.get_x();
    uint8_t byte = computer_state.get_byte_from_memory(address + offset);
    computer_state.set_byte_to_memory(address + offset, byte + 1);
}

void Instructions::execute_increment_memory_zeropage(ComputerState& computer_state, uint8_t unused)
{
    uint16_t address = get_immediate_byte();
    uint8_t byte = computer_state.get_byte_from_memory(address);
    computer_state.set_byte_to_memory(address, byte + 1);
}

void Instructions::execute_increment_memory_zeropage_x(ComputerState& computer_state, uint8_t unused)
{
    uint16_t address = get_immediate_byte();
    uint8_t offset = computer_state.get_x();
    uint8_t byte = computer_state.get_byte_from_memory(address + offset);
    computer_state.set_byte_to_memory(address + offset, byte + 1);
}


void Instructions::execute_load_accumulator(ComputerState& computer_state, uint8_t operand)
{
    computer_state.set_accumulator(operand);
}

void Instructions::execute_load_x(ComputerState& computer_state, uint8_t operand)
{
    computer_state.set_x(operand);
}

void Instructions::execute_load_y(ComputerState& computer_state, uint8_t operand)
{
    computer_state.set_y(operand);
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

uint8_t Instructions::get_operand_noop(ComputerState&)
{
    return 0;
}

uint8_t Instructions::get_operand_immediate(ComputerState &computer_state) {
    return get_immediate_byte(computer_state);
}

uint8_t Instructions::get_operand_absolute(ComputerState &computer_state)
{
    uint16_t address = get_immediate_word(computer_state);
    return computer_state.get_byte_from_memory(address);
}

uint8_t Instructions::get_operand_absolute_x(ComputerState &computer_state)
{
    uint16_t address = get_immediate_word(computer_state) + computer_state.get_x();
    return computer_state.get_byte_from_memory(address);
}

uint8_t Instructions::get_operand_absolute_y(ComputerState &computer_state)
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

uint8_t Instructions::get_operand_indirect_x(ComputerState &computer_state)
{
    uint8_t address_of_pointer = get_immediate_word(computer_state) + computer_state.get_x();
    uint8_t address = computer_state.get_word_from_memory(address_of_pointer);
    return computer_state.get_byte_from_memory(address);
}

uint8_t Instructions::get_operand_indirect_y(ComputerState &computer_state)
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

uint8_t Instructions::get_operand_zeropage_x(ComputerState &computer_state)
{
    uint8_t address = get_immediate_byte(computer_state) + computer_state.get_x();
    return computer_state.get_byte_from_memory(address);
}

uint8_t Instructions::get_operand_zeropage_y(ComputerState &computer_state)
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

