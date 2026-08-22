case InstructionType::LDX: {
    reg_.x = current_instruction.val1_.value();

    // Set status
    setNegativeFlag(current_instruction.val1_.value());
    setZeroFlag(current_instruction.val1_.value());

    // PRINT_INSTRUCTION("%x %s #%x", opcode, InstructionStringMap.at(current_instruction_data.type).c_str(), current_instruction.val1_.value())
    break;
}
