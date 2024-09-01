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

    LOGV("%x BEQ %i", opcode, tmp)
    break;
}
