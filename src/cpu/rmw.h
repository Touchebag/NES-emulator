case 0xA2: { // LDX immediate
    incPc(1);
    uint8_t c = readFromPc();
    reg_.x = c;
    incPc(1);

    // Set status
    setNegativeFlag(c);
    setZeroFlag(c);

    // Number of cycles
    cycles = 2;

    LOGV("%x LDX #%x", opcode, c)
    break;
}
