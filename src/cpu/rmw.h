case InstructionType::LDX: {
    auto tmp = READ_ARGUMENT();
    reg_.x = tmp;

    // Set status
    setNegativeFlag(tmp);
    setZeroFlag(tmp);

    PRINT_INSTRUCTION("%x LDX #%x", opcode, tmp)
    break;
}
