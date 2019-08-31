#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include <array>

class ComputerState;

class Instructions
{
public:
    Instructions();

    void execute(uint8_t opcode, ComputerState &computer_state);

    static void fail_unimplemented(ComputerState &computer_state);

    static void execute_nop(ComputerState &computer_state);

    static void execute_add_with_carry_immediate(ComputerState &computer_state);
    static void execute_add_with_carry_zeropage(ComputerState &computer_state);
    static void execute_add_with_carry_zeropage_x(ComputerState &computer_state);
    static void execute_add_with_carry_absolute(ComputerState &computer_state);
    static void execute_add_with_carry_absolute_x(ComputerState &computer_state);
    static void execute_add_with_carry_absolute_y(ComputerState &computer_state);
    static void execute_add_with_carry_indirect_x(ComputerState &computer_state);
    static void execute_add_with_carry_indirect_y(ComputerState &computer_state);

    static void execute_branch_on_carry_set(ComputerState& computer_state);
    static void execute_branch_on_carry_clear(ComputerState& computer_state);
    static void execute_branch_on_equal(ComputerState& computer_state);
    static void execute_branch_on_not_equal(ComputerState& computer_state);
    static void execute_branch_on_overflow_set(ComputerState& computer_state);
    static void execute_branch_on_overflow_clear(ComputerState& computer_state);
    static void execute_branch_on_plus(ComputerState& computer_state);
    static void execute_branch_on_minus(ComputerState& computer_state);

private:
    typedef void (*InstructionFunction)(ComputerState&);
    std::array<InstructionFunction, 256> instruction_array;

    static void add_with_carry(ComputerState &computer_state, uint8_t operand);

    static uint8_t get_immediate_byte(ComputerState &computer_state);
    static uint16_t get_immediate_word(ComputerState &computer_state);

    static uint8_t get_operand_absolute(ComputerState &computer_state);
    static uint8_t get_operand_absolute_x_indexed(ComputerState &computer_state);
    static uint8_t get_operand_absolute_y_indexed(ComputerState &computer_state);
    static uint8_t get_operand_indirect(ComputerState &computer_state);
    static uint8_t get_operand_x_indexed_indirect(ComputerState &computer_state);
    static uint8_t get_operand_indirect_y_indexed(ComputerState &computer_state);
    static uint8_t get_operand_zeropage(ComputerState &computer_state);
    static uint8_t get_operand_zeropage_x_indexed(ComputerState &computer_state);
    static uint8_t get_operand_zeropage_y_indexed(ComputerState &computer_state);

    static void increment_program_counter(ComputerState &computer_state);

    static bool is_negative(uint8_t byte);
};

#endif // header guard for INSTRUCTIONS_H
