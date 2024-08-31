#include <gtest/gtest.h>

#include "cpu.h"
#include "memory.h"

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

    Cpu cpu_;
    Memory memory_;
    int current_mem_byte_ = 0;
};

// LDA immediate
TEST_F(InterpreterTestFixture, test_0xA9) {
    EXPECT_EQ(cpu_.getRegisters().a, 0);

    addInstruction({0xA9, 0xBC});
    cpu_.executeInstruction(memory_);

    EXPECT_EQ(cpu_.getRegisters().a, 0xBC);
}
