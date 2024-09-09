case 0xC9: { // CMP immediate
    incPc(1);
    uint8_t c = readFromPc();
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

    LOGV("%x CMP #%x", opcode, c)
    break;
}

case 0xE0: { // CMX immediate
    incPc(1);
    uint8_t c = readFromPc();
    incPc(1);

    // Compare
    uint8_t x = reg_.x;
    uint8_t tmp = x - c;

    // Set flags
    setNegativeFlag(tmp);
    setZeroFlag(tmp);
    // 0 if c > a
    setStatusFlag(StatusFlag::CARRY, c <= x);

    // Set cycles
    cycles = 2;

    LOGV("%x CMP #%x", opcode, c)
    break;
}

case 0x8D: { // STA absolute
    incPc(1);
    uint8_t lo = readFromPc();
    incPc(1);
    uint8_t hi = readFromPc();
    incPc(1);

    memory.writeAddress(lo, hi, reg_.a);

    // Number of cycles
    cycles = 3;

    LOGV("%x STA %x %x %x", opcode, hi, lo, reg_.a)
    break;
}

case 0xA9: { // LDA immediate
    incPc(1);
    uint8_t c = readFromPc();
    reg_.a = c;
    incPc(1);

    // Set status
    setNegativeFlag(c);
    setZeroFlag(c);

    // Number of cycles
    cycles = 2;

    LOGV("%x LDA #%x", opcode, c)
    break;
}

case 0xBD: { // LDA absolute,X
    // Cycles first for easier adding of potential page-crossing cycle
    cycles = 4;

    incPc(1);
    uint8_t lo = readFromPc();
    incPc(1);
    uint8_t hi = readFromPc();
    incPc(1);
    uint8_t x = reg_.x;

    auto [val, page_cross] = absoluteX(lo, hi, x);
    reg_.a = val;

    if (page_cross) {
        cycles++;
    }

    // Set status
    setNegativeFlag(val);
    setZeroFlag(val);

    LOGV("%x LDA %x %x %x", opcode, hi, lo, x)
    break;
}

case 0xE1: { // SBC (indirect,X)
    incPc(1);
    uint8_t addr = readFromPc();
    incPc(1);

    auto [val, _] = indirectX(addr, reg_.x);

    uint8_t c = getStatusFlag(StatusFlag::CARRY) ? 1 : 0;

    setStatusFlag(StatusFlag::CARRY, (val + c) > reg_.a);

    reg_.a = reg_.a - val + (c);

    uint8_t a = reg_.a;

    setNegativeFlag(a);
    setZeroFlag(a);

    cycles = 6;

    LOGV("%x SBC %x", opcode, val);
    break;
}
