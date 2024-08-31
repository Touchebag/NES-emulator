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
