#include <gtest/gtest.h>

#include "cpu.h"
#include "memory.h"

using StatusFlag = Cpu::StatusFlag;

class InterpreterTestFixture : public ::testing::Test {
  public:
    void SetUp() override {
        cpu_ = Cpu{};
        memory_ = Memory{};

        current_mem_byte_ = 0;
        cpu_.setPc(static_cast<uint8_t>(0x00), static_cast<uint8_t>(0x00));
    }

    void addInstruction(std::vector<uint8_t> bytes) {
        for (auto b : bytes) {
            memory_.memory_[current_mem_byte_] = b;
            current_mem_byte_++;
        }
    }

    uint8_t peekMemoryAddress(uint8_t lo, uint8_t hi) {
        return *(memory_.getAddress(lo, hi));
    }

    void pokeMemoryAddress(uint8_t lo, uint8_t hi, uint8_t value) {
        *(memory_.getAddress(lo, hi)) = value;
    }

    void setStatusFlag(StatusFlag flag, bool value) {
        cpu_.setStatusFlag(flag, value);
    }

    void setRegisterA(uint8_t value) {
        cpu_.reg_.a = value;
    }

    void setRegisterX(uint8_t value) {
        cpu_.reg_.x = value;
    }

    void executeNextInstruction() {
        cpu_.executeInstruction(memory_);
    }

    uint16_t getPc() {
        return (cpu_.reg_.pc[1] << 8) | cpu_.reg_.pc[0];
    }

    Cpu cpu_;
    Memory memory_;
    int current_mem_byte_ = 0;
};

// BEQ
TEST_F(InterpreterTestFixture, test_0xF0) {
    setStatusFlag(StatusFlag::ZERO, false);

    addInstruction({0xF0, 0x12});
    addInstruction({0xF0, 0x12});

    EXPECT_EQ(getPc(), 0x0000);

    executeNextInstruction();
    EXPECT_EQ(getPc(), 0x0002);

    setStatusFlag(StatusFlag::ZERO, true);
    executeNextInstruction();
    EXPECT_EQ(getPc(), 0x0014);
}

// CMP immediate
TEST_F(InterpreterTestFixture, test_0xC9) {
    setRegisterA(0x59);
    addInstruction({0xC9, 0x4C});
    addInstruction({0xC9, 0x59});
    addInstruction({0xC9, 0xAD});

    executeNextInstruction();

    EXPECT_EQ(cpu_.getRegisters().a, 0x59);
    EXPECT_EQ(cpu_.getStatusFlag(StatusFlag::ZERO), false);
    EXPECT_EQ(cpu_.getStatusFlag(StatusFlag::NEGATIVE), false);
    EXPECT_EQ(cpu_.getStatusFlag(StatusFlag::CARRY), true);

    executeNextInstruction();

    EXPECT_EQ(cpu_.getRegisters().a, 0x59);
    EXPECT_EQ(cpu_.getStatusFlag(StatusFlag::ZERO), true);
    EXPECT_EQ(cpu_.getStatusFlag(StatusFlag::NEGATIVE), false);
    EXPECT_EQ(cpu_.getStatusFlag(StatusFlag::CARRY), true);

    executeNextInstruction();

    EXPECT_EQ(cpu_.getRegisters().a, 0x59);
    EXPECT_EQ(cpu_.getStatusFlag(StatusFlag::ZERO), false);
    EXPECT_EQ(cpu_.getStatusFlag(StatusFlag::NEGATIVE), true);
    EXPECT_EQ(cpu_.getStatusFlag(StatusFlag::CARRY), false);
}

// INX
TEST_F(InterpreterTestFixture, test_0xE8) {
    EXPECT_EQ(cpu_.getRegisters().x, 0x00);

    addInstruction({0xE8});
    addInstruction({0xE8});
    addInstruction({0xE8});

    executeNextInstruction();

    EXPECT_EQ(cpu_.getRegisters().x, 0x01);
    EXPECT_EQ(cpu_.getStatusFlag(StatusFlag::ZERO), false);
    EXPECT_EQ(cpu_.getStatusFlag(StatusFlag::NEGATIVE), false);

    setRegisterX(0xFE);

    executeNextInstruction();

    EXPECT_EQ(cpu_.getRegisters().x, 0xFF);
    EXPECT_EQ(cpu_.getStatusFlag(StatusFlag::ZERO), false);
    EXPECT_EQ(cpu_.getStatusFlag(StatusFlag::NEGATIVE), true);

    executeNextInstruction();

    EXPECT_EQ(cpu_.getRegisters().x, 0x00);
    EXPECT_EQ(cpu_.getStatusFlag(StatusFlag::ZERO), true);
    EXPECT_EQ(cpu_.getStatusFlag(StatusFlag::NEGATIVE), false);
}

// JMP absolute
TEST_F(InterpreterTestFixture, test_0x4C) {
    EXPECT_EQ(getPc(), 0x00);

    addInstruction({0x4C, 0x34, 0x12});

    executeNextInstruction();

    EXPECT_EQ(getPc(), 0x1234);
}

// LDA immediate
TEST_F(InterpreterTestFixture, test_0xA9) {
    EXPECT_EQ(cpu_.getRegisters().a, 0x00);

    addInstruction({0xA9, 0x4C});
    addInstruction({0xA9, 0xBC});
    addInstruction({0xA9, 0x00});

    executeNextInstruction();

    EXPECT_EQ(cpu_.getRegisters().a, 0x4C);
    EXPECT_EQ(cpu_.getStatusFlag(StatusFlag::ZERO), false);
    EXPECT_EQ(cpu_.getStatusFlag(StatusFlag::NEGATIVE), false);

    executeNextInstruction();

    EXPECT_EQ(cpu_.getRegisters().a, 0xBC);
    EXPECT_EQ(cpu_.getStatusFlag(StatusFlag::ZERO), false);
    EXPECT_EQ(cpu_.getStatusFlag(StatusFlag::NEGATIVE), true);

    executeNextInstruction();

    EXPECT_EQ(cpu_.getRegisters().a, 0x00);
    EXPECT_EQ(cpu_.getStatusFlag(StatusFlag::ZERO), true);
    EXPECT_EQ(cpu_.getStatusFlag(StatusFlag::NEGATIVE), false);
}

// LDA absolute,X
TEST_F(InterpreterTestFixture, text_0xBD) {
    EXPECT_EQ(cpu_.getRegisters().a, 0x00);
    EXPECT_EQ(cpu_.getRegisters().x, 0x00);

    pokeMemoryAddress(0x12, 0x34, 0x18);
    pokeMemoryAddress(0x15, 0x34, 0x00);
    pokeMemoryAddress(0xA3, 0x59, 0xB9);

    addInstruction({0xBD, 0x12, 0x34});
    addInstruction({0xBD, 0x12, 0x34});
    addInstruction({0xBD, 0xA0, 0x59});

    executeNextInstruction();

    EXPECT_EQ(cpu_.getRegisters().a, 0x18);
    EXPECT_EQ(cpu_.getStatusFlag(StatusFlag::ZERO), false);
    EXPECT_EQ(cpu_.getStatusFlag(StatusFlag::NEGATIVE), false);

    setRegisterX(0x03);
    executeNextInstruction();

    EXPECT_EQ(cpu_.getRegisters().a, 0x00);
    EXPECT_EQ(cpu_.getStatusFlag(StatusFlag::ZERO), true);
    EXPECT_EQ(cpu_.getStatusFlag(StatusFlag::NEGATIVE), false);

    executeNextInstruction();

    EXPECT_EQ(cpu_.getRegisters().a, 0xB9);
    EXPECT_EQ(cpu_.getStatusFlag(StatusFlag::ZERO), false);
    EXPECT_EQ(cpu_.getStatusFlag(StatusFlag::NEGATIVE), true);
}

// LDX immediate
TEST_F(InterpreterTestFixture, test_0xA2) {
    EXPECT_EQ(cpu_.getRegisters().x, 0x00);

    addInstruction({0xA2, 0xAC});
    addInstruction({0xA2, 0x00});
    addInstruction({0xA2, 0x59});

    executeNextInstruction();

    EXPECT_EQ(cpu_.getRegisters().x, 0xAC);
    EXPECT_EQ(cpu_.getStatusFlag(StatusFlag::ZERO), false);
    EXPECT_EQ(cpu_.getStatusFlag(StatusFlag::NEGATIVE), true);

    executeNextInstruction();

    EXPECT_EQ(cpu_.getRegisters().x, 0x00);
    EXPECT_EQ(cpu_.getStatusFlag(StatusFlag::ZERO), true);
    EXPECT_EQ(cpu_.getStatusFlag(StatusFlag::NEGATIVE), false);

    executeNextInstruction();

    EXPECT_EQ(cpu_.getRegisters().x, 0x59);
    EXPECT_EQ(cpu_.getStatusFlag(StatusFlag::ZERO), false);
    EXPECT_EQ(cpu_.getStatusFlag(StatusFlag::NEGATIVE), false);
}

// STA absolute
TEST_F(InterpreterTestFixture, test_0x8D) {
    EXPECT_EQ(cpu_.getRegisters().a, 0x00);

    setRegisterA(0x5C);

    addInstruction({0x8D, 0x12, 0x34});

    EXPECT_EQ(peekMemoryAddress(0x12, 0x34), 0x00);
    executeNextInstruction();
    EXPECT_EQ(peekMemoryAddress(0x12, 0x34), 0x5C);
}
