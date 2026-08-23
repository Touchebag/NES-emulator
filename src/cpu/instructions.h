case InstructionType::ADC: {
    auto a = reg_.a;
    reg_.a = reg_.a + current_instruction.val1_.value() + (getStatusFlag(StatusFlag::CARRY) ? 1 : 0);

    setZeroFlag(reg_.a);
    setNegativeFlag(reg_.a);

    setStatusFlag(StatusFlag::OVERFLOW, (reg_.a ^ a) & (reg_.a ^ current_instruction.val1_.value()) & 0x80);
    // If wrapped around, set carry
    setStatusFlag(StatusFlag::CARRY, reg_.a < a);

    current_instruction.handlePageCross();

    break;
}

case InstructionType::AND: {
    reg_.a = reg_.a & current_instruction.val1_.value();

    setZeroFlag(reg_.a);
    setNegativeFlag(reg_.a);

    current_instruction.handlePageCross();

    break;
}

case InstructionType::BCC: {
    // If carry is not set
    if (!getStatusFlag(StatusFlag::CARRY)) {
        // Add extra cycle if branch taken
        current_instruction.cycles_++;
        current_instruction.handlePageCross();
        setPc(current_instruction.adjusted_lo_, current_instruction.adjusted_hi_);
    };

    break;
}

case InstructionType::BCS: {
    // If carry is set
    if (getStatusFlag(StatusFlag::CARRY)) {
        // Add extra cycle if branch taken
        current_instruction.cycles_++;
        current_instruction.handlePageCross();
        setPc(current_instruction.adjusted_lo_, current_instruction.adjusted_hi_);
    };

    break;
}

case InstructionType::BEQ: {
    // If latest operation was zero
    if (getStatusFlag(StatusFlag::ZERO)) {
        // Add extra cycle if branch taken
        current_instruction.cycles_++;
        current_instruction.handlePageCross();
        setPc(current_instruction.adjusted_lo_, current_instruction.adjusted_hi_);
    };

    break;
}

case InstructionType::BIT: {
    // ZERO is reasult of comparison with A
    setStatusFlag(StatusFlag::ZERO, (reg_.a & current_instruction.val1_.value()) == 0);

    // OVERFLOW and NEGATIVE is directly set from memory address
    setStatusFlag(StatusFlag::OVERFLOW, ((1 << 6) & current_instruction.val1_.value()) == (1 << 6));
    setStatusFlag(StatusFlag::NEGATIVE, ((1 << 7) & current_instruction.val1_.value()) == (1 << 7));
    break;
}

case InstructionType::BMI: {
    // If latest operation was negative
    if (getStatusFlag(StatusFlag::NEGATIVE)) {
        // Add extra cycle if branch taken
        current_instruction.cycles_++;
        current_instruction.handlePageCross();
        setPc(current_instruction.adjusted_lo_, current_instruction.adjusted_hi_);
    };

    break;
}

case InstructionType::BNE: {
    // If latest operation was non-zero
    if (!getStatusFlag(StatusFlag::ZERO)) {
        // Add extra cycle if branch taken
        current_instruction.cycles_++;
        current_instruction.handlePageCross();
        setPc(current_instruction.adjusted_lo_, current_instruction.adjusted_hi_);
    };

    break;
}

case InstructionType::BPL: {
    // If latest operation was positive
    if (!getStatusFlag(StatusFlag::NEGATIVE)) {
        // Add extra cycle if branch taken
        current_instruction.cycles_++;
        current_instruction.handlePageCross();
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

case InstructionType::BVC: {
    // If latest operation didn't overflow
    if (!getStatusFlag(StatusFlag::OVERFLOW)) {
        // Add extra cycle if branch taken
        current_instruction.cycles_++;
        current_instruction.handlePageCross();
        setPc(current_instruction.adjusted_lo_, current_instruction.adjusted_hi_);
    };

    break;
}

case InstructionType::BVS: {
    // If latest operation overflowed
    if (getStatusFlag(StatusFlag::OVERFLOW)) {
        // Add extra cycle if branch taken
        current_instruction.cycles_++;
        current_instruction.handlePageCross();
        setPc(current_instruction.adjusted_lo_, current_instruction.adjusted_hi_);
    };

    break;
}

case InstructionType::CLC: {
    setStatusFlag(StatusFlag::CARRY, false);

    break;
}

case InstructionType::CLD: {
    setStatusFlag(StatusFlag::DECIMAL, false);

    break;
}

case InstructionType::CLV: {
    setStatusFlag(StatusFlag::OVERFLOW, false);

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

    current_instruction.handlePageCross();

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

    current_instruction.handlePageCross();

    break;
}

case InstructionType::CPY: {
    uint8_t y = reg_.y;

    // 0 if current_instruction.val1_.value() > a
    setStatusFlag(StatusFlag::CARRY, current_instruction.val1_.value() <= y);

    y -= current_instruction.val1_.value();

    // Set flags
    setNegativeFlag(y);
    setZeroFlag(y);

    current_instruction.handlePageCross();

    break;
}

case InstructionType::DEX: {
    reg_.x = (reg_.x - 1) % 256;

    // Set flags
    setNegativeFlag(reg_.x);
    setZeroFlag(reg_.x);

    break;
}

case InstructionType::DEY: {
    reg_.y = (reg_.y - 1) % 256;

    // Set flags
    setNegativeFlag(reg_.y);
    setZeroFlag(reg_.y);

    break;
}

case InstructionType::EOR: {
    reg_.a = reg_.a ^ current_instruction.val1_.value();

    // Set flags
    setNegativeFlag(reg_.a);
    setZeroFlag(reg_.a);

    current_instruction.handlePageCross();

    break;
}

case InstructionType::INX: {
    reg_.x = (reg_.x + 1) % 256;

    // Set flags
    setNegativeFlag(reg_.x);
    setZeroFlag(reg_.x);

    break;
}

case InstructionType::INY: {
    reg_.y = (reg_.y + 1) % 256;

    // Set flags
    setNegativeFlag(reg_.y);
    setZeroFlag(reg_.y);

    break;
}

case InstructionType::JMP: {
    setPc(current_instruction.adjusted_lo_, current_instruction.adjusted_hi_);

    break;
}

case InstructionType::JSR: {
    uint8_t hi = current_instruction.pc_hi_;
    uint8_t lo = current_instruction.pc_lo_;

    // In case of lo overflow
    if (lo >= 0xFE) {
        hi++;
    }

    lo += 2;

    pushStack(hi);
    pushStack(lo);
    setPc(current_instruction.adjusted_lo_, current_instruction.adjusted_hi_);

    break;
}

case InstructionType::LDA: {
    reg_.a = current_instruction.val1_.value();

    // Set status
    setNegativeFlag(reg_.a);
    setZeroFlag(reg_.a);

    current_instruction.handlePageCross();

    break;
}

case InstructionType::LDX: {
    reg_.x = current_instruction.val1_.value();

    // Set status
    setNegativeFlag(current_instruction.val1_.value());
    setZeroFlag(current_instruction.val1_.value());

    current_instruction.handlePageCross();

    break;
}

case InstructionType::LDY: {
    reg_.y = current_instruction.val1_.value();

    // Set status
    setNegativeFlag(reg_.y);
    setZeroFlag(reg_.y);

    current_instruction.handlePageCross();

    break;
}

case InstructionType::LSR: {
    reg_.a = current_instruction.val1_.value() >> 1;

    break;
}

case InstructionType::NOP: {
    break;
}

case InstructionType::ORA: {
    reg_.a = reg_.a | current_instruction.val1_.value();

    setZeroFlag(reg_.a);
    setNegativeFlag(reg_.a);

    current_instruction.handlePageCross();

    break;
}

case InstructionType::PHA: {
    pushStack(reg_.a);
    break;
}

case InstructionType::PHP: {
    // BREAK is pushed as 1
    pushStack(reg_.p | static_cast<uint8_t>(StatusFlag::BREAK));
    break;
}

case InstructionType::PLA: {
    reg_.a = popStack();

    setStatusFlag(StatusFlag::ZERO, reg_.a == 0);
    setStatusFlag(StatusFlag::NEGATIVE, reg_.a & 128);
    break;
}

case InstructionType::PLP: {
    auto prev_p = reg_.p;
    reg_.p = popStack();

    // Restore untouched flags
    setStatusFlag(StatusFlag::UNUSED, prev_p & static_cast<uint8_t>(StatusFlag::UNUSED));
    setStatusFlag(StatusFlag::BREAK, prev_p & static_cast<uint8_t>(StatusFlag::BREAK));
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

case InstructionType::RTS: {
    uint8_t lo = popStack();
    uint8_t hi = popStack();

    setPc(lo, hi);
    incPc(1);

    break;
}

case InstructionType::SBC: {
    // Not of carry
    uint8_t c = getStatusFlag(StatusFlag::CARRY) ? 0 : 1;
    uint8_t a = reg_.a;

    uint8_t result = reg_.a - current_instruction.val1_.value() - c;

    // Underflow clears carry (inverser from ADC)
    setStatusFlag(StatusFlag::CARRY, result <= reg_.a);

    reg_.a = result;

    setNegativeFlag(reg_.a);
    setZeroFlag(reg_.a);

    setStatusFlag(StatusFlag::OVERFLOW, (reg_.a ^ a) & (reg_.a ^ ~(current_instruction.val1_.value())) & 0x80);

    current_instruction.handlePageCross();

    break;
}

case InstructionType::SEC: {
    setStatusFlag(StatusFlag::CARRY, true);
    break;
}

case InstructionType::SED: {
    setStatusFlag(StatusFlag::DECIMAL, true);
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

case InstructionType::STY: {
    System::get<Memory>().writeAddress(current_instruction.adjusted_lo_, current_instruction.adjusted_hi_, reg_.y);
    break;
}

case InstructionType::TAX: {
    reg_.x = reg_.a;

    // Set flags
    setNegativeFlag(reg_.x);
    setZeroFlag(reg_.x);

    break;
}
