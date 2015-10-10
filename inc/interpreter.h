#ifndef INTERPRETER
#define INTERPRETER

#include "utility.h"
#include "cpu_ram.h"

class Interpreter {
  public:
    /* Fetches and executes the next instruction according to the current
     * address in pc.
     */
    int execute_instruction();

    /* Sets the pc register to the specified value
     *
     * Arguments are the lowwe and the upper byte
     */
    void set_pc(BYTE low, BYTE high);

    // Increments the pc register by "number"
    void inc_pc(int number);

    // Constructor and destructor. Takes the pointers to the systems registers
    // and the memory object
    Interpreter(Registers*, Cpu_ram*);
    ~Interpreter();

  private:
    // Pointer to the registers and memory from system class
    Registers *reg;
    Cpu_ram *memory;

    /* Sets (or clears) a bit in the p register
     *
     * Arguments are which bit and the value of it
     * (true = 1, false = 0)
     */
    void set_status_bit(Status_flag bit, bool value);

    /* These functions automatically sets or clears the bits in the p register
     * dependant on the value (i.e. the result of the operation)
     */
    void set_negative_bit(BYTE value);
    void set_zero_bit(BYTE value);

    // Fetches the byte stored at the address in pc
    BYTE read_from_pc();
};

#endif
