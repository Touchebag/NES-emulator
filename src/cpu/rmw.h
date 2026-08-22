case InstructionType::LDX: {
    reg_.x = current_instruction.val1_.value();

    // Set status
    setNegativeFlag(current_instruction.val1_.value());
    setZeroFlag(current_instruction.val1_.value());

    break;
}
