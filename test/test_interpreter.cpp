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

    void setRegisterA(uint8_t value) {
        cpu_.reg_.a = value;
    }

    void executeNextInstruction() {
        cpu_.executeInstruction(memory_);
    }

    Cpu cpu_;
    Memory memory_;
    int current_mem_byte_ = 0;
};

// LDA immediate
TEST_F(InterpreterTestFixture, test_0xA9) {
    EXPECT_EQ(cpu_.getRegisters().a, 0);

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

// STA absolute
TEST_F(InterpreterTestFixture, test_0x8D) {
    EXPECT_EQ(cpu_.getRegisters().a, 0);

    setRegisterA(0x5C);

    addInstruction({0x8D, 0x12, 0x34});

    EXPECT_EQ(peekMemoryAddress(0x12, 0x34), 0x00);
    executeNextInstruction();
    EXPECT_EQ(peekMemoryAddress(0x12, 0x34), 0x5C);
}
