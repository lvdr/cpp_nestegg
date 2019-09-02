#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include <array>
#include <utility>

class ComputerState;

class Instructions
{
public:
    Instructions();

    void execute(uint8_t opcode, ComputerState &computer_state);

    static void fail_unimplemented(ComputerState &computer_state, uint8_t operand);

    static void execute_nop(ComputerState &computer_state, uint8_t operand);

    static void execute_add_with_carry(ComputerState &computer_state, uint8_t operand);
    static void execute_and(ComputerState &computer_state, uint8_t operand);
    static void execute_ior(ComputerState &computer_state, uint8_t operand);
    static void execute_xor(ComputerState &computer_state, uint8_t operand);
    static void execute_compare_with_accumulator(ComputerState &state, uint8_t operand);
    static void execute_compare_with_x(ComputerState &state, uint8_t operand);
    static void execute_compare_with_y(ComputerState &state, uint8_t operand);

    static void execute_branch_on_carry_set(ComputerState& computer_state, uint8_t offset);
    static void execute_branch_on_carry_clear(ComputerState& computer_state, uint8_t offset);
    static void execute_branch_on_equal(ComputerState& computer_state, uint8_t offset);
    static void execute_branch_on_not_equal(ComputerState& computer_state, uint8_t offset);
    static void execute_branch_on_overflow_set(ComputerState& computer_state, uint8_t offset);
    static void execute_branch_on_overflow_clear(ComputerState& computer_state, uint8_t offset);
    static void execute_branch_on_plus(ComputerState& computer_state, uint8_t offset);
    static void execute_branch_on_minus(ComputerState& computer_state, uint8_t offset);

    static uint8_t get_operand_noop(ComputerState &ComputerState);
    static uint8_t get_operand_immediate(ComputerState &ComputerState);
    static uint8_t get_operand_absolute(ComputerState &computer_state);
    static uint8_t get_operand_absolute_x(ComputerState &computer_state);
    static uint8_t get_operand_absolute_y(ComputerState &computer_state);
    static uint8_t get_operand_indirect(ComputerState &computer_state);
    static uint8_t get_operand_indirect_x(ComputerState &computer_state);
    static uint8_t get_operand_indirect_y(ComputerState &computer_state);
    static uint8_t get_operand_zeropage(ComputerState &computer_state);
    static uint8_t get_operand_zeropage_x(ComputerState &computer_state);
    static uint8_t get_operand_zeropage_y(ComputerState &computer_state);

    static void execute_load_accumulator(ComputerState& computer_state, uint8_t operand);
    static void execute_load_x(ComputerState& computer_state, uint8_t operand);
    static void execute_load_y(ComputerState& computer_state, uint8_t operand);

    static void execute_test_bits(ComputerState& computer_state, uint8_t operand);

private:
    typedef uint8_t (*OperandFunction)(ComputerState&);
    typedef void (*InstructionFunction)(ComputerState&, uint8_t);
    std::array<std::pair<OperandFunction, InstructionFunction>, 256> instruction_array;

    static uint8_t get_immediate_byte(ComputerState &computer_state);
    static uint16_t get_immediate_word(ComputerState &computer_state);

    static void increment_program_counter(ComputerState &computer_state);

    static bool is_negative(uint8_t byte);
};

#endif // header guard for INSTRUCTIONS_H
