case InstructionType::LDX: {
    auto tmp = READ_ARGUMENT();
    reg_.x = tmp;

    // Set status
    setNegativeFlag(tmp);
    setZeroFlag(tmp);

    PRINT_INSTRUCTION("%x %s #%x", opcode, InstructionStringMap.at(current_instruction_data.type).c_str(), tmp)
    break;
}
