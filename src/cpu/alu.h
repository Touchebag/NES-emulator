case InstructionType::CMP: {
    uint8_t tmp = READ_ARGUMENT();

    uint8_t a = reg_.a;

    // 0 if tmp > a
    setStatusFlag(StatusFlag::CARRY, tmp <= a);

    a -= tmp;

    // Set flags
    setNegativeFlag(a);
    setZeroFlag(a);

    LOGV("%x CMP #%x", opcode, c)
    break;
}

case InstructionType::CPX: {
    uint8_t tmp = READ_ARGUMENT();

    uint8_t x = reg_.x;

    // 0 if tmp > a
    setStatusFlag(StatusFlag::CARRY, tmp <= x);

    x -= tmp;

    // Set flags
    setNegativeFlag(x);
    setZeroFlag(x);

    LOGV("%x CPX #%x", opcode, c)
    break;
}

case InstructionType::STA: {
    WRITE_ARGUMENT(reg_.a);

    LOGV("%x STA %x %x %x", opcode, hi, lo, reg_.a)
    break;
}

case InstructionType::LDA: {
    auto tmp = READ_ARGUMENT();
    reg_.a = tmp;

    // Set status
    setNegativeFlag(tmp);
    setZeroFlag(tmp);

    LOGV("%x LDA #%x", opcode, tmp)
    break;
}

case InstructionType::SBC: {
    auto tmp = READ_ARGUMENT();

    uint8_t c = getStatusFlag(StatusFlag::CARRY) ? 1 : 0;

    setStatusFlag(StatusFlag::CARRY, (tmp + c) > reg_.a);

    reg_.a = reg_.a - tmp + (c);

    uint8_t a = reg_.a;

    setNegativeFlag(a);
    setZeroFlag(a);

    LOGV("%x SBC %x", opcode, val);
    break;
}
