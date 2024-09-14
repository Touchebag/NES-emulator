#include <gtest/gtest.h>

#include "system.h"

#include "log.h"

using StatusFlag = Cpu::StatusFlag;

class InterpreterTestFixture : public ::testing::Test {
  public:
    InterpreterTestFixture() :
        cpu_(System::getInstance().get<Cpu>()),
        ppu_(System::getInstance().get<Ppu>()),
        memory_(System::getInstance().get<Memory>()) {
    }

    void SetUp() override {
        System::getInstance().resetComponents();
        System::getInstance().reset();

        cpu_    = System::getInstance().get<Cpu>();
        ppu_    = System::getInstance().get<Ppu>();
        memory_ = System::getInstance().get<Memory>();

        current_mem_byte_ = 0;
        setPc(0x00, 0x00);
    }

    void addInstruction(std::vector<uint8_t> bytes) {
        for (auto b : bytes) {
            memory_.memory_[current_mem_byte_] = b;
            current_mem_byte_++;
        }
    }

    uint8_t peekMemoryAddress(uint8_t lo, uint8_t hi) {
        uint16_t address = (hi << 8) | lo;
        return memory_.memory_[address];
    }

    void pokeMemoryAddress(uint8_t lo, uint8_t hi, uint8_t value) {
        uint16_t address = (hi << 8) | lo;
        memory_.memory_[address] = value;
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
        cpu_.executeInstruction();
    }

    void setPc(uint8_t lo, uint8_t hi) {
        cpu_.reg_.pc[0] = lo;
        cpu_.reg_.pc[1] = hi;
    }

    uint16_t getPc() {
        return (cpu_.reg_.pc[1] << 8) | cpu_.reg_.pc[0];
    }

    uint8_t getStackPointer() {
        return cpu_.reg_.sp;
    }

    void setStackPointer(uint8_t value) {
        cpu_.reg_.sp = value;
    }

    Cpu& cpu_;
    Ppu& ppu_;
    Memory& memory_;

    int current_mem_byte_ = 0x6000;
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

    setPc(0xA9, 0x83);
    // Manually add instructions at pc
    pokeMemoryAddress(0xA9, 0x83, 0xF0);
    pokeMemoryAddress(0xAA, 0x83, 0x72);

    setStatusFlag(StatusFlag::ZERO, true);
    executeNextInstruction();
    EXPECT_EQ(getPc(), 0x841B);

    setPc(0xAB, 0x83);
    pokeMemoryAddress(0xAB, 0x83, 0xF0);
    pokeMemoryAddress(0xAC, 0x83, 0xFC);
    setStatusFlag(StatusFlag::ZERO, true);

    // Negative jump
    executeNextInstruction();
    EXPECT_EQ(getPc(), 0x83A7);
}

// BNE
TEST_F(InterpreterTestFixture, test_0xD0) {
    setStatusFlag(StatusFlag::ZERO, true);

    addInstruction({0xD0, 0x3D});
    addInstruction({0xD0, 0x3D});

    EXPECT_EQ(getPc(), 0x0000);

    executeNextInstruction();
    EXPECT_EQ(getPc(), 0x0002);

    setStatusFlag(StatusFlag::ZERO, false);
    executeNextInstruction();
    EXPECT_EQ(getPc(), 0x003F);
}

// BRK
TEST_F(InterpreterTestFixture, test_0x00) {
    setStatusFlag(StatusFlag::ZERO, true);
    setStatusFlag(StatusFlag::CARRY, true);
    setPc(0x34, 0x12);

    pokeMemoryAddress(0xFE, 0xFF, 0x05);
    pokeMemoryAddress(0xFF, 0xFF, 0x7D);

    addInstruction({0x00});

    uint8_t status = static_cast<uint8_t>(StatusFlag::ZERO)  |
                     static_cast<uint8_t>(StatusFlag::CARRY) |
                     static_cast<uint8_t>(StatusFlag::BREAK);

    executeNextInstruction();
    EXPECT_EQ(getPc(), 0x7D05);
    EXPECT_EQ(getStackPointer(), 0xFC);
    EXPECT_EQ(peekMemoryAddress(0xFF, 0x01), 0x12);
    EXPECT_EQ(peekMemoryAddress(0xFE, 0x01), 0x36);
    EXPECT_EQ(peekMemoryAddress(0xFD, 0x01), status);
}

// CLC
TEST_F(InterpreterTestFixture, text_0x18) {
    setStatusFlag(StatusFlag::CARRY, true);

    addInstruction({0x18});
    addInstruction({0x18});

    EXPECT_EQ(cpu_.getStatusFlag(StatusFlag::CARRY), true);

    executeNextInstruction();
    EXPECT_EQ(cpu_.getStatusFlag(StatusFlag::CARRY), false);

    // Ensure it doesn't change already cleared flag
    executeNextInstruction();
    EXPECT_EQ(cpu_.getStatusFlag(StatusFlag::CARRY), false);
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

// CPX immediate
TEST_F(InterpreterTestFixture, test_0xE0) {
    setRegisterX(0xA2);
    addInstruction({0xE0, 0x34});
    addInstruction({0xE0, 0xA2});
    addInstruction({0xE0, 0xB6});

    executeNextInstruction();

    EXPECT_EQ(cpu_.getRegisters().x, 0xA2);
    EXPECT_EQ(cpu_.getStatusFlag(StatusFlag::ZERO), false);
    EXPECT_EQ(cpu_.getStatusFlag(StatusFlag::NEGATIVE), false);
    EXPECT_EQ(cpu_.getStatusFlag(StatusFlag::CARRY), true);

    executeNextInstruction();

    EXPECT_EQ(cpu_.getRegisters().x, 0xA2);
    EXPECT_EQ(cpu_.getStatusFlag(StatusFlag::ZERO), true);
    EXPECT_EQ(cpu_.getStatusFlag(StatusFlag::NEGATIVE), false);
    EXPECT_EQ(cpu_.getStatusFlag(StatusFlag::CARRY), true);

    executeNextInstruction();

    EXPECT_EQ(cpu_.getRegisters().x, 0xA2);
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

    pokeMemoryAddress(0x12, 0x54, 0x18);
    pokeMemoryAddress(0x15, 0x54, 0x00);
    pokeMemoryAddress(0xA3, 0x59, 0xB9);

    addInstruction({0xBD, 0x12, 0x54});
    addInstruction({0xBD, 0x12, 0x54});
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

// RTI
TEST_F(InterpreterTestFixture, test_0x40) {
    pokeMemoryAddress(0xFF, 0x01, 0x23);
    pokeMemoryAddress(0xFE, 0x01, 0x45);
    pokeMemoryAddress(0xFD, 0x01, static_cast<uint8_t>(StatusFlag::BREAK) | static_cast<uint8_t>(StatusFlag::ZERO));
    setStackPointer(0xFC);

    addInstruction({0x40});

    executeNextInstruction();
    EXPECT_EQ(getPc(), 0x2345);
    EXPECT_EQ(getStackPointer(), 0xFF);
    EXPECT_EQ(cpu_.getStatusFlag(StatusFlag::ZERO), true);
}

// SBC (indirect, X)
TEST_F(InterpreterTestFixture, test_0xE1) {
    pokeMemoryAddress(0x21, 0x00, 0x01);
    pokeMemoryAddress(0x22, 0x00, 0x74);
    pokeMemoryAddress(0x01, 0x74, 0x5C);
    setRegisterX(0x07);

    addInstruction({0xE1, 0x1A});
    addInstruction({0xE1, 0x1A});
    addInstruction({0xE1, 0x1A});

    setRegisterA(0x6D);
    setStatusFlag(StatusFlag::CARRY, true);

    executeNextInstruction();
    EXPECT_EQ(cpu_.getRegisters().a, 0x12);
    EXPECT_EQ(cpu_.getStatusFlag(StatusFlag::ZERO), false);
    EXPECT_EQ(cpu_.getStatusFlag(StatusFlag::NEGATIVE), false);
    EXPECT_EQ(cpu_.getStatusFlag(StatusFlag::CARRY), false);

    setRegisterA(0x5B);
    setStatusFlag(StatusFlag::CARRY, false);

    executeNextInstruction();
    EXPECT_EQ(cpu_.getRegisters().a, 0xFF);
    EXPECT_EQ(cpu_.getStatusFlag(StatusFlag::ZERO), false);
    EXPECT_EQ(cpu_.getStatusFlag(StatusFlag::NEGATIVE), true);
    EXPECT_EQ(cpu_.getStatusFlag(StatusFlag::CARRY), true);

    setRegisterA(0x5C);
    setStatusFlag(StatusFlag::CARRY, false);

    executeNextInstruction();
    EXPECT_EQ(cpu_.getRegisters().a, 0x00);
    EXPECT_EQ(cpu_.getStatusFlag(StatusFlag::ZERO), true);
    EXPECT_EQ(cpu_.getStatusFlag(StatusFlag::NEGATIVE), false);
    EXPECT_EQ(cpu_.getStatusFlag(StatusFlag::CARRY), false);
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
