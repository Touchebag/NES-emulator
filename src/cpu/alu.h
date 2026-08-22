case InstructionType::CMP: {
    uint8_t a = reg_.a;

    // 0 if current_instruction.val1_.value() > a
    setStatusFlag(StatusFlag::CARRY, current_instruction.val1_.value() <= a);

    a -= current_instruction.val1_.value();

    // Set flags
    setNegativeFlag(a);
    setZeroFlag(a);

    // PRINT_INSTRUCTION("%x %s #%x", opcode, InstructionStringMap.at(current_instruction_data.type).c_str(), current_instruction.val1_.value())
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

    // PRINT_INSTRUCTION("%x %s #%x", opcode, InstructionStringMap.at(current_instruction_data.type).c_str(), current_instruction.val1_.value())
    break;
}

case InstructionType::STA: {
    System::get<Memory>().writeAddress(current_instruction.val1_.value(), current_instruction.val2_.value(), reg_.a);

    // PRINT_INSTRUCTION("%x %s %x %x %x",
    //         opcode, InstructionStringMap.at(current_instruction_data.type).c_str(),
    //         System::get<Memory>().readAddress(reg_.pc[0] - 1, reg_.pc[1]), // Hi
    //         System::get<Memory>().readAddress(reg_.pc[0] - 2, reg_.pc[1]), // Lo
    //         reg_.a)
    break;
}

case InstructionType::LDA: {
    reg_.a = current_instruction.val1_.value();

    // Set status
    setNegativeFlag(reg_.a);
    setZeroFlag(reg_.a);

    // PRINT_INSTRUCTION("%x %s #%x", opcode, InstructionStringMap.at(current_instruction_data.type).c_str(), tmp)
    break;
}

case InstructionType::SBC: {
    uint8_t c = getStatusFlag(StatusFlag::CARRY) ? 1 : 0;

    setStatusFlag(StatusFlag::CARRY, (current_instruction.val1_.value() + c) > reg_.a);

    reg_.a = reg_.a - current_instruction.val1_.value() + (c);

    uint8_t a = reg_.a;

    setNegativeFlag(a);
    setZeroFlag(a);

    // PRINT_INSTRUCTION("%x %s %x", opcode, InstructionStringMap.at(current_instruction_data.type).c_str(), tmp);
    break;
}
