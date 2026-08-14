case InstructionType::CMP: {
    uint8_t tmp = READ_ARGUMENT();

    uint8_t a = reg_.a;

    // 0 if tmp > a
    setStatusFlag(StatusFlag::CARRY, tmp <= a);

    a -= tmp;

    // Set flags
    setNegativeFlag(a);
    setZeroFlag(a);

    PRINT_INSTRUCTION("%x %s #%x", opcode, InstructionStringMap.at(current_instruction_data.type).c_str(), tmp)
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

    PRINT_INSTRUCTION("%x %s #%x", opcode, InstructionStringMap.at(current_instruction_data.type).c_str(), tmp)
    break;
}

case InstructionType::STA: {
    WRITE_ARGUMENT(reg_.a);

    PRINT_INSTRUCTION("%x %s %x %x %x",
            opcode, InstructionStringMap.at(current_instruction_data.type).c_str(),
            System::get<Memory>().readAddress(reg_.pc[0] - 1, reg_.pc[1]), // Hi
            System::get<Memory>().readAddress(reg_.pc[0] - 2, reg_.pc[1]), // Lo
            reg_.a)
    break;
}

case InstructionType::LDA: {
    auto tmp = READ_ARGUMENT();
    reg_.a = tmp;

    // Set status
    setNegativeFlag(tmp);
    setZeroFlag(tmp);

    PRINT_INSTRUCTION("%x %s #%x", opcode, InstructionStringMap.at(current_instruction_data.type).c_str(), tmp)
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

    PRINT_INSTRUCTION("%x %s %x", opcode, InstructionStringMap.at(current_instruction_data.type).c_str(), tmp);
    break;
}
