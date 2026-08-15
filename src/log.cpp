#include "log.h"

#include "cpu/instruction_table.h"

#include <stdexcept>

#ifdef NESTEST_OUTPUT
void print_nestest_output(NestestData data) {
    fprintf(stderr, "%02X%02X  ", data.pc_hi, data.pc_lo);
    fprintf(stderr, "%02X ", data.opcode);

    // Optional arguments
    if (data.arg1) {
        fprintf(stderr, "%02X ", data.arg1.value());

        if (data.arg2) {
            fprintf(stderr, "%02X", data.arg2.value());
        } else {
            fprintf(stderr, "  ");
        }
    } else {
        fprintf(stderr, "  ");
    }
    fprintf(stderr, "  ");

    fprintf(stderr, "%s ", data.name.c_str());

    switch (data.mode) {
        case AddressingMode::RELATIVE:
            fprintf(stderr, "RELATIVE");
            break;
        case AddressingMode::IMMEDIATE:
            fprintf(stderr, "#$%02X                       ", data.arg1.value());
            break;
        case AddressingMode::ABSOLUTE:
            fprintf(stderr, "$%02X%02X                       ", data.arg2.value(), data.arg1.value());
            break;
        case AddressingMode::ABSOLUTE_X:
            fprintf(stderr, "$%02X%02X,X @ %02X%02X = %02X        ",
                    data.arg2.value(),
                    data.arg1.value(),
                    data.address_hi,
                    data.address_lo,
                    data.address_val);
            break;
        case AddressingMode::INDIRECT_X:
            fprintf(stderr, "INDIRECT_X");
            break;
        case AddressingMode::NONE:
            fprintf(stderr, "NONE");
            break;
        default:
            throw std::invalid_argument("Unknown addressing mode. This should never happen");
            break;
    }

    fprintf(stderr, "A:%02X X:%02X Y:%02X P:%02X SP:%02X ", data.reg_a, data.reg_x, data.reg_y, data.reg_p, data.sp);

    fprintf(stderr, "PPU:YYY,XXX ");
    fprintf(stderr, "CYC:%i", data.cycles);

    fprintf(stderr, "\n");
    fflush(stderr);
}
#endif
