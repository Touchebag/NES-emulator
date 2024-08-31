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
