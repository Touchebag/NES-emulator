case InstructionType::BEQ: {
    uint8_t lo = reg_.pc[0];
    uint8_t hi = reg_.pc[1];

    uint8_t tmp = READ_ARGUMENT();

    // If latest operation was zero
    if (getStatusFlag(StatusFlag::ZERO)) {
        // Add extra cycle if branch taken
        cycles++;

        auto [new_lo, new_hi, page_cross] = calculateRelativeJump(lo, hi, tmp);
        setPc(new_lo, new_hi);
        if (page_cross) { cycles++; }
    };

    LOGV("%x BEQ %x", opcode, tmp)
    break;
}

case InstructionType::BNE: {
    uint8_t lo = reg_.pc[0];
    uint8_t hi = reg_.pc[1];

    uint8_t tmp = READ_ARGUMENT();

    // If latest operation was non-zero
    if (!getStatusFlag(StatusFlag::ZERO)) {
        // Add extra cycle if branch taken
        cycles++;

        auto [new_lo, new_hi, page_cross] = calculateRelativeJump(lo, hi, tmp);
        setPc(new_lo, new_hi);
        if (page_cross) { cycles++; }
    };

    LOGV("%x BNE %x", opcode, tmp)
    break;
}

case InstructionType::BRK: {
    uint8_t hi = reg_.pc[1];
    uint8_t lo = reg_.pc[0];

    // In case of lo overflow
    if (lo >= 0xFE) {
        hi++;
    }

    lo += 2;
    uint8_t status = reg_.p | static_cast<uint8_t>(StatusFlag::BREAK);

    pushStack(hi);
    pushStack(lo);
    pushStack(status);

    auto& memory = System::get<Memory>();
    lo = memory.readAddress(0xFE, 0xFF);
    hi = memory.readAddress(0xFF, 0xFF);
    setPc(lo, hi);

    LOGV("%x BRK", opcode)
    break;
}

case InstructionType::CLC: {
    incPc(1);

    setStatusFlag(StatusFlag::CARRY, false);

    break;
}

case InstructionType::INX: {
    incPc(1);
    reg_.x = (reg_.x + 1) % 256;

    // Set flags
    setNegativeFlag(reg_.x);
    setZeroFlag(reg_.x);

    LOGV("%x INX", opcode)
    break;
}

case InstructionType::JMP: {
    incPc(1);
    uint8_t lo = readFromPc();
    incPc(1);
    uint8_t hi = readFromPc();

    setPc(lo, hi);

    LOGV("%x JMP %x %x", opcode, lo, hi)
    break;
}

case InstructionType::RTI: {
    uint8_t p  = popStack();
    uint8_t lo = popStack();
    uint8_t hi = popStack();

    setPc(lo, hi);
    reg_.p = p & ~(static_cast<uint8_t>(StatusFlag::BREAK));

    LOGV("%x RTI", opcode)
    break;
}
