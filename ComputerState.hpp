#IFNDEF COMPUTERSTATE_H
#DEFINE COMPUTERSTATE_H

class ComputerState
{
public:
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

    uint8_t get_byte_from_memory(uint16_t index);
    uint16_t get_word_from_memory(uint16_t index);

    void set_byte_to_memory(uint16_t index, uint8_t byte);
    void set_word_to_memory(uint16_t index, uint16_t word);

private:
    uint8_t accumulator;
    uint8_t x;
    uint8_t y;
    uint8_t status;
    uint8_t stack_pointer;
    uint16_t program_counter;

    std::vector<uint8_t> memory;
}


#ENDIF // header guard for COMPUTERSTATE_H
