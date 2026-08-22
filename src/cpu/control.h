case InstructionType::BEQ: {
    // If latest operation was zero
    if (getStatusFlag(StatusFlag::ZERO)) {
        // Add extra cycle if branch taken
        current_instruction.cycles_++;

        auto [new_lo, new_hi, page_cross] = calculateRelativeJump(
                current_instruction.adjusted_lo_,
                current_instruction.adjusted_hi_,
                current_instruction.val1_.value());

        setPc(new_lo, new_hi);
        if (page_cross) {
            current_instruction.cycles_++;
        }
    };

    break;
}

case InstructionType::BNE: {
    // If latest operation was non-zero
    if (!getStatusFlag(StatusFlag::ZERO)) {
        // Add extra cycle if branch taken
        current_instruction.cycles_++;

        auto [new_lo, new_hi, page_cross] = calculateRelativeJump(
                current_instruction.adjusted_lo_,
                current_instruction.adjusted_hi_,
                current_instruction.val1_.value());

        setPc(new_lo, new_hi);
        if (page_cross) {
            current_instruction.cycles_++;
        }
    };

    break;
}

case InstructionType::BRK: {
    uint8_t hi = current_instruction.pc_hi_;
    uint8_t lo = current_instruction.pc_lo_;

    // In case of lo overflow
    if (lo >= 0xFE) {
        hi++;
    }

    lo += 2;

    pushStack(hi);
    pushStack(lo);
    // Break only exists in the pushed flags
    pushStack(reg_.p | static_cast<uint8_t>(StatusFlag::BREAK));

    // Interrupt is not pushed
    setStatusFlag(StatusFlag::INTERRUPT, true);

    auto& memory = System::get<Memory>();
    lo = memory.readAddress(0xFE, 0xFF);
    hi = memory.readAddress(0xFF, 0xFF);
    setPc(lo, hi);

    break;
}

case InstructionType::CLC: {
    setStatusFlag(StatusFlag::CARRY, false);

    break;
}

case InstructionType::INX: {
    reg_.x = (reg_.x + 1) % 256;

    // Set flags
    setNegativeFlag(reg_.x);
    setZeroFlag(reg_.x);

    break;
}

case InstructionType::JMP: {
    setPc(current_instruction.val1_.value(), current_instruction.val2_.value());

    break;
}

case InstructionType::RTI: {
    uint8_t p  = popStack();
    uint8_t lo = popStack();
    uint8_t hi = popStack();

    setPc(lo, hi);
    reg_.p = p & ~(static_cast<uint8_t>(StatusFlag::BREAK));

    break;
}

case InstructionType::SEI: {
    setStatusFlag(StatusFlag::INTERRUPT, true);
    break;
}
