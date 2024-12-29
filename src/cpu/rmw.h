case InstructionType::LDX: {
    auto tmp = READ_ARGUMENT();
    reg_.x = tmp;

    // Set status
    setNegativeFlag(tmp);
    setZeroFlag(tmp);

    LOGV("%x LDX #%x", opcode, c)
    break;
}
