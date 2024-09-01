case 0xC9: { // CMP immediate
    incPc(1);
    uint8_t c = readFromPc(memory);
    incPc(1);

    // Compare
    uint8_t a = reg_.a;
    uint8_t tmp = a - c;

    // Set flags
    setNegativeFlag(tmp);
    setZeroFlag(tmp);
    // 0 if c > a
    setStatusFlag(StatusFlag::CARRY, c <= a);

    // Set cycles
    cycles = 2;

    LOGV("%x CMP #%i", opcode, c)
    break;
}

case 0x8D: { // STA absolute
    incPc(1);
    uint8_t lo = readFromPc(memory);
    incPc(1);
    uint8_t hi = readFromPc(memory);
    incPc(1);

    memory.writeAddress(lo, hi, reg_.a);

    // Number of cycles
    cycles = 3;

    LOGV("%x STA %i %i %i", opcode, hi, lo, reg_.a)
    break;
}

case 0xA9: { // LDA immediate
    incPc(1);
    uint8_t c = readFromPc(memory);
    reg_.a = c;
    incPc(1);

    // Set status
    setNegativeFlag(c);
    setZeroFlag(c);

    // Number of cycles
    cycles = 2;

    LOGV("%x LDA #%i", opcode, c)
    break;
}

case 0xBD: { // LDA absolute,X
    // Cycles first for easier adding of potential page-crossing cycle
    cycles = 4;

    incPc(1);
    uint8_t lo = readFromPc(memory);
    incPc(1);
    uint8_t hi = readFromPc(memory);
    incPc(1);
    uint8_t x = reg_.x;

    if (x < (256 - lo)) {
        // If no page crossing, continue as normal
        lo += x;
    } else {
        // else add extra cycle(s)
        lo = (lo + x) % 255;
        hi++;
        cycles++;
    };

    // Store to A
    uint8_t a = memory.readAddress(lo, hi);
    reg_.a = a;

    // Set status
    setNegativeFlag(a);
    setZeroFlag(a);

    LOGV("%x LDA %i %i %i %i", opcode, hi, lo, x, y)
    break;
}
