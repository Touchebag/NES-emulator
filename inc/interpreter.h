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
};

#endif
