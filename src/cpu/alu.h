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

case InstructionType::STA: {
    System::get<Memory>().writeAddress(current_instruction.adjusted_lo_, current_instruction.adjusted_hi_, reg_.a);

    break;
}

case InstructionType::STX: {
    System::get<Memory>().writeAddress(current_instruction.adjusted_lo_, current_instruction.adjusted_hi_, reg_.x);

    break;
}

case InstructionType::LDA: {
    reg_.a = current_instruction.val1_.value();

    // Set status
    setNegativeFlag(reg_.a);
    setZeroFlag(reg_.a);

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
