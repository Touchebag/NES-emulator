case InstructionType::BCC: {
    // If carry is not set
    if (!getStatusFlag(StatusFlag::CARRY)) {
        // Add extra cycle if branch taken
        current_instruction.cycles_++;
        setPc(current_instruction.adjusted_lo_, current_instruction.adjusted_hi_);
    };

    break;
}

case InstructionType::BCS: {
    // If carry is set
    if (getStatusFlag(StatusFlag::CARRY)) {
        // Add extra cycle if branch taken
        current_instruction.cycles_++;
        setPc(current_instruction.adjusted_lo_, current_instruction.adjusted_hi_);
    };

    break;
}

case InstructionType::BEQ: {
    // If latest operation was zero
    if (getStatusFlag(StatusFlag::ZERO)) {
        // Add extra cycle if branch taken
        current_instruction.cycles_++;
        setPc(current_instruction.adjusted_lo_, current_instruction.adjusted_hi_);
    };

    break;
}

case InstructionType::BNE: {
    // If latest operation was non-zero
    if (!getStatusFlag(StatusFlag::ZERO)) {
        // Add extra cycle if branch taken
        current_instruction.cycles_++;
        setPc(current_instruction.adjusted_lo_, current_instruction.adjusted_hi_);
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

case InstructionType::CMP: {
    uint8_t a = reg_.a;

    // 0 if current_instruction.val1_.value() > a
    setStatusFlag(StatusFlag::CARRY, current_instruction.val1_.value() <= a);

    a -= current_instruction.val1_.value();

    // Set flags
    setNegativeFlag(a);
    setZeroFlag(a);

    break;
}

case InstructionType::CPX: {
    uint8_t x = reg_.x;

    // 0 if current_instruction.val1_.value() > a
    setStatusFlag(StatusFlag::CARRY, current_instruction.val1_.value() <= x);

    x -= current_instruction.val1_.value();

    // Set flags
    setNegativeFlag(x);
    setZeroFlag(x);

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
    setPc(current_instruction.adjusted_lo_, current_instruction.adjusted_hi_);

    break;
}

case InstructionType::JSR: {
    pushStack(reg_.pc[1]);
    pushStack(reg_.pc[0]);
    setPc(current_instruction.adjusted_lo_, current_instruction.adjusted_hi_);

    break;
}

case InstructionType::LDA: {
    reg_.a = current_instruction.val1_.value();

    // Set status
    setNegativeFlag(reg_.a);
    setZeroFlag(reg_.a);

    break;
}

case InstructionType::LDX: {
    reg_.x = current_instruction.val1_.value();

    // Set status
    setNegativeFlag(current_instruction.val1_.value());
    setZeroFlag(current_instruction.val1_.value());

    break;
}

case InstructionType::NOP: {
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

case InstructionType::SBC: {
    uint8_t c = getStatusFlag(StatusFlag::CARRY) ? 1 : 0;

    setStatusFlag(StatusFlag::CARRY, (current_instruction.val1_.value() + c) > reg_.a);

    reg_.a = reg_.a - current_instruction.val1_.value() + (c);

    uint8_t a = reg_.a;

    setNegativeFlag(a);
    setZeroFlag(a);

    break;
}

case InstructionType::SEC: {
    setStatusFlag(StatusFlag::CARRY, true);
    break;
}

case InstructionType::SEI: {
    setStatusFlag(StatusFlag::INTERRUPT, true);
    break;
}

case InstructionType::STA: {
    System::get<Memory>().writeAddress(current_instruction.adjusted_lo_, current_instruction.adjusted_hi_, reg_.a);
    break;
}

case InstructionType::STX: {
    System::get<Memory>().writeAddress(current_instruction.adjusted_lo_, current_instruction.adjusted_hi_, reg_.x);
    break;
}

