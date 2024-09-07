case 0xF0: { // BEQ
    // Cycles first to help with page crossing
    cycles = 2;

    uint8_t lo = reg_.pc[0];
    uint8_t hi = reg_.pc[1];
    incPc(1);
    uint8_t tmp = readFromPc(memory);
    incPc(1);

    // If latest operation was zero
    if (getStatusFlag(StatusFlag::ZERO)) {
        // If highest bit is set (negative)
        if (tmp & 128) {
            tmp &= 127;
            // If no page crossing
            if (tmp <= lo) {
                lo -= tmp;
                cycles += 1;
            } else {
                //TODO Ugly as balls, neccessary because of underflow?
                tmp -= lo;
                lo = 256 - tmp;
                hi--;
                cycles += 2;
            };
        } else { // If positive
            // If no page crossing
            if (tmp <= (255 - lo)) {
                lo += tmp;
                cycles += 1;
            } else {
                lo = (lo + tmp) % 255;
                hi++;
                cycles += 2;
            };
        };
        setPc(lo, hi);
    };

    LOGV("%x BEQ %x", opcode, tmp)
    break;
}

case 0xD0: { // BNE
    // Cycles first to help with page crossing
    cycles = 2;

    uint8_t lo = reg_.pc[0];
    uint8_t hi = reg_.pc[1];
    incPc(1);
    uint8_t tmp = readFromPc(memory);
    incPc(1);

    // If latest operation was zero
    if (!getStatusFlag(StatusFlag::ZERO)) {
        // If highest bit is set (negative)
        if (tmp & 128) {
            tmp &= 127;
            // If no page crossing
            if (tmp <= lo) {
                lo -= tmp;
                cycles += 1;
            } else {
                //TODO Ugly as balls, neccessary because of underflow?
                tmp -= lo;
                lo = 256 - tmp;
                hi--;
                cycles += 2;
            };
        } else { // If positive
            // If no page crossing
            if (tmp <= (255 - lo)) {
                lo += tmp;
                cycles += 1;
            } else {
                lo = (lo + tmp) % 255;
                hi++;
                cycles += 2;
            };
        };
        setPc(lo, hi);
    };

    LOGV("%x BNE %x", opcode, tmp)
    break;
}

case 0x00: { // BRK
    uint8_t hi = reg_.pc[1];
    uint8_t lo = reg_.pc[0];

    // In case of lo overflow
    if (lo >= 0xFE) {
        hi++;
    }

    lo += 2;
    uint8_t status = reg_.p | static_cast<uint8_t>(StatusFlag::BREAK);

    pushStack(memory, hi);
    pushStack(memory, lo);
    pushStack(memory, status);

    lo = memory.readAddress(0xFE, 0xFF);
    hi = memory.readAddress(0xFF, 0xFF);
    setPc(lo, hi);

    cycles = 7;

    LOGV("%x BRK", opcode)
    break;
}

case 0xE8: { // INX
    incPc(1);
    reg_.x = (reg_.x + 1) % 256;

    // Set flags
    setNegativeFlag(reg_.x);
    setZeroFlag(reg_.x);

    // Number of cycles
    cycles = 2;

    LOGV("%x INX", opcode)
    break;
}

case 0x4C: { // JMP absolute
    incPc(1);
    uint8_t lo = readFromPc(memory);
    incPc(1);
    uint8_t hi = readFromPc(memory);

    setPc(lo, hi);

    cycles = 3;

    LOGV("%x JMP %x %x", opcode, lo, hi)
    break;
}
