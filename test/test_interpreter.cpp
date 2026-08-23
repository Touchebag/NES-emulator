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
        return memory_.readAddress(lo, hi);
    }

    void pokeMemoryAddress(uint8_t lo, uint8_t hi, uint8_t value) {
        memory_.writeAddress(lo, hi, value);
    }

    void setStatusRegister(uint8_t value) {
        cpu_.reg_.p = value;
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

    void setRegisterY(uint8_t value) {
        cpu_.reg_.y = value;
    }

    void executeNextInstruction() {
        auto pc = cpu_.getRegisters().pc;
        auto current_instruction = ParsedInstruction(pc[0], pc[1]);

        // fprintf(stderr, "%s\n", current_instruction.toString().c_str());

        cpu_.executeInstruction(current_instruction);
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

// ADC
TEST_F(InterpreterTestFixture, test_0x69) {
    setStatusFlag(StatusFlag::ZERO, false);
    setStatusFlag(StatusFlag::NEGATIVE, false);
    setStatusFlag(StatusFlag::OVERFLOW, false);

    setRegisterA(0x00);

    addInstruction({0x69, 0x08});
    addInstruction({0x69, 0x18});
    addInstruction({0x69, 0x78});
    addInstruction({0x69, 0x07});
    addInstruction({0x69, 0x5E});

    setStatusFlag(StatusFlag::CARRY, false);

    executeNextInstruction();
    EXPECT_EQ(cpu_.getRegisters().a, 0x08);
    EXPECT_EQ(cpu_.getStatusFlag(StatusFlag::ZERO), false);
    EXPECT_EQ(cpu_.getStatusFlag(StatusFlag::NEGATIVE), false);
    EXPECT_EQ(cpu_.getStatusFlag(StatusFlag::OVERFLOW), false);

    setStatusFlag(StatusFlag::CARRY, true);

    executeNextInstruction();
    EXPECT_EQ(cpu_.getRegisters().a, 0x21);
    EXPECT_EQ(cpu_.getStatusFlag(StatusFlag::ZERO), false);
    EXPECT_EQ(cpu_.getStatusFlag(StatusFlag::NEGATIVE), false);
    EXPECT_EQ(cpu_.getStatusFlag(StatusFlag::OVERFLOW), false);

    setStatusFlag(StatusFlag::CARRY, true);

    executeNextInstruction();
    EXPECT_EQ(cpu_.getRegisters().a, 0x9A);
    EXPECT_EQ(cpu_.getStatusFlag(StatusFlag::ZERO), false);
    EXPECT_EQ(cpu_.getStatusFlag(StatusFlag::NEGATIVE), true);
    EXPECT_EQ(cpu_.getStatusFlag(StatusFlag::OVERFLOW), true);

    setStatusFlag(StatusFlag::CARRY, false);

    executeNextInstruction();
    EXPECT_EQ(cpu_.getRegisters().a, 0xA1);
    EXPECT_EQ(cpu_.getStatusFlag(StatusFlag::ZERO), false);
    EXPECT_EQ(cpu_.getStatusFlag(StatusFlag::NEGATIVE), true);
    EXPECT_EQ(cpu_.getStatusFlag(StatusFlag::OVERFLOW), false);

    setStatusFlag(StatusFlag::CARRY, true);

    executeNextInstruction();
    EXPECT_EQ(cpu_.getRegisters().a, 0x00);
    EXPECT_EQ(cpu_.getStatusFlag(StatusFlag::ZERO), true);
    EXPECT_EQ(cpu_.getStatusFlag(StatusFlag::NEGATIVE), false);
    EXPECT_EQ(cpu_.getStatusFlag(StatusFlag::OVERFLOW), false);
}

// AND
TEST_F(InterpreterTestFixture, test_0x29) {
    setStatusFlag(StatusFlag::ZERO, false);
    setStatusFlag(StatusFlag::NEGATIVE, false);

    setRegisterA(0x00);

    addInstruction({0x29, 0x08});
    addInstruction({0x29, 0x00});
    addInstruction({0x29, 0xE7});

    EXPECT_EQ(cpu_.getRegisters().a, 0x00);
    EXPECT_EQ(cpu_.getStatusFlag(StatusFlag::ZERO), false);
    EXPECT_EQ(cpu_.getStatusFlag(StatusFlag::NEGATIVE), false);

    setRegisterA(0x00);
    executeNextInstruction();
    EXPECT_EQ(cpu_.getRegisters().a, 0x00);
    EXPECT_EQ(cpu_.getStatusFlag(StatusFlag::ZERO), true);
    EXPECT_EQ(cpu_.getStatusFlag(StatusFlag::NEGATIVE), false);

    setRegisterA(0x46);
    executeNextInstruction();
    EXPECT_EQ(cpu_.getRegisters().a, 0x00);
    EXPECT_EQ(cpu_.getStatusFlag(StatusFlag::ZERO), true);
    EXPECT_EQ(cpu_.getStatusFlag(StatusFlag::NEGATIVE), false);

    setRegisterA(0xF9);
    executeNextInstruction();
    EXPECT_EQ(cpu_.getRegisters().a, 0xE1);
    EXPECT_EQ(cpu_.getStatusFlag(StatusFlag::ZERO), false);
    EXPECT_EQ(cpu_.getStatusFlag(StatusFlag::NEGATIVE), true);
}

// BCC
TEST_F(InterpreterTestFixture, test_0x90) {
    setStatusFlag(StatusFlag::CARRY, true);

    addInstruction({0x90, 0xE7});
    addInstruction({0x90, 0xE7});

    EXPECT_EQ(getPc(), 0x0000);

    executeNextInstruction();
    EXPECT_EQ(getPc(), 0x0002);

    setStatusFlag(StatusFlag::CARRY, false);
    executeNextInstruction();
    EXPECT_EQ(getPc(), 0xFFEB);
}

// BCS
TEST_F(InterpreterTestFixture, test_0xB0) {
    setStatusFlag(StatusFlag::CARRY, false);

    addInstruction({0xB0, 0x6F});
    addInstruction({0xB0, 0x6F});

    EXPECT_EQ(getPc(), 0x0000);

    executeNextInstruction();
    EXPECT_EQ(getPc(), 0x0002);

    setStatusFlag(StatusFlag::CARRY, true);
    executeNextInstruction();
    EXPECT_EQ(getPc(), 0x0073);
}

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
    EXPECT_EQ(getPc(), 0x0016);

    setPc(0xA9, 0x83);
    // Manually add instructions at pc
    pokeMemoryAddress(0xA9, 0x83, 0xF0);
    pokeMemoryAddress(0xAA, 0x83, 0x72);

    setStatusFlag(StatusFlag::ZERO, true);
    executeNextInstruction();
    EXPECT_EQ(getPc(), 0x841D);

    setPc(0xAB, 0x83);
    pokeMemoryAddress(0xAB, 0x83, 0xF0);
    pokeMemoryAddress(0xAC, 0x83, 0xFC);
    setStatusFlag(StatusFlag::ZERO, true);

    // Negative jump
    executeNextInstruction();
    EXPECT_EQ(getPc(), 0x83A9);
}

// BIT
TEST_F(InterpreterTestFixture, test_0x24) {
    setStatusFlag(StatusFlag::ZERO, false);
    setStatusFlag(StatusFlag::OVERFLOW, false);
    setStatusFlag(StatusFlag::NEGATIVE, false);

    pokeMemoryAddress(0x39, 0x00, 0x40);
    pokeMemoryAddress(0x46, 0x00, 0x82);

    addInstruction({0x24, 0x39});
    addInstruction({0x24, 0x46});

    setRegisterA(0x02);
    executeNextInstruction();

    EXPECT_EQ(cpu_.getStatusFlag(StatusFlag::ZERO), true);
    EXPECT_EQ(cpu_.getStatusFlag(StatusFlag::OVERFLOW), true);
    EXPECT_EQ(cpu_.getStatusFlag(StatusFlag::NEGATIVE), false);

    setRegisterA(0x02);
    executeNextInstruction();

    EXPECT_EQ(cpu_.getStatusFlag(StatusFlag::ZERO), false);
    EXPECT_EQ(cpu_.getStatusFlag(StatusFlag::OVERFLOW), false);
    EXPECT_EQ(cpu_.getStatusFlag(StatusFlag::NEGATIVE), true);
}

// BMI
TEST_F(InterpreterTestFixture, test_0x30) {
    setStatusFlag(StatusFlag::NEGATIVE, false);

    addInstruction({0x30, 0x49});
    addInstruction({0x30, 0x49});

    EXPECT_EQ(getPc(), 0x0000);

    executeNextInstruction();
    EXPECT_EQ(getPc(), 0x0002);

    setStatusFlag(StatusFlag::NEGATIVE, true);
    executeNextInstruction();
    EXPECT_EQ(getPc(), 0x004D);
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
    EXPECT_EQ(getPc(), 0x0041);
}

// BPL
TEST_F(InterpreterTestFixture, test_0x10) {
    setStatusFlag(StatusFlag::NEGATIVE, true);

    addInstruction({0x10, 0x26});
    addInstruction({0x10, 0x26});

    EXPECT_EQ(getPc(), 0x0000);

    executeNextInstruction();
    EXPECT_EQ(getPc(), 0x0002);

    setStatusFlag(StatusFlag::NEGATIVE, false);
    executeNextInstruction();
    EXPECT_EQ(getPc(), 0x002A);
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
                     static_cast<uint8_t>(StatusFlag::INTERRUPT) |
                     static_cast<uint8_t>(StatusFlag::BREAK) |
                     static_cast<uint8_t>(StatusFlag::UNUSED);

    executeNextInstruction();
    EXPECT_EQ(getPc(), 0x7D05);
    EXPECT_EQ(getStackPointer(), 0xFA);
    EXPECT_EQ(peekMemoryAddress(0xFD, 0x01), 0x12);
    EXPECT_EQ(peekMemoryAddress(0xFC, 0x01), 0x36);
    EXPECT_EQ(peekMemoryAddress(0xFB, 0x01), status);

    EXPECT_EQ(cpu_.getStatusFlag(StatusFlag::ZERO), true);
    EXPECT_EQ(cpu_.getStatusFlag(StatusFlag::CARRY), true);
    EXPECT_EQ(cpu_.getStatusFlag(StatusFlag::INTERRUPT), true);
    EXPECT_EQ(cpu_.getStatusFlag(StatusFlag::BREAK), false);
}

// BVC
TEST_F(InterpreterTestFixture, test_0x50) {
    setStatusFlag(StatusFlag::OVERFLOW, true);

    addInstruction({0x50, 0x33});
    addInstruction({0x50, 0x33});

    EXPECT_EQ(getPc(), 0x0000);

    executeNextInstruction();
    EXPECT_EQ(getPc(), 0x0002);

    setStatusFlag(StatusFlag::OVERFLOW, false);
    executeNextInstruction();
    EXPECT_EQ(getPc(), 0x0037);
}

// BVS
TEST_F(InterpreterTestFixture, test_0x70) {
    setStatusFlag(StatusFlag::OVERFLOW, false);

    addInstruction({0x70, 0x57});
    addInstruction({0x70, 0x57});

    EXPECT_EQ(getPc(), 0x0000);

    executeNextInstruction();
    EXPECT_EQ(getPc(), 0x0002);

    setStatusFlag(StatusFlag::OVERFLOW, true);
    executeNextInstruction();
    EXPECT_EQ(getPc(), 0x005B);
}

// CLC
TEST_F(InterpreterTestFixture, test_0x18) {
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

// CLD
TEST_F(InterpreterTestFixture, test_0xD8) {
    setStatusFlag(StatusFlag::DECIMAL, true);

    addInstruction({0xD8});
    addInstruction({0xD8});

    EXPECT_EQ(cpu_.getStatusFlag(StatusFlag::DECIMAL), true);

    executeNextInstruction();
    EXPECT_EQ(cpu_.getStatusFlag(StatusFlag::DECIMAL), false);

    // Ensure it doesn't change already cleared flag
    executeNextInstruction();
    EXPECT_EQ(cpu_.getStatusFlag(StatusFlag::DECIMAL), false);
}

// CLV
TEST_F(InterpreterTestFixture, test_0xB8) {
    setStatusFlag(StatusFlag::OVERFLOW, true);

    addInstruction({0xB8});
    addInstruction({0xB8});

    EXPECT_EQ(cpu_.getStatusFlag(StatusFlag::OVERFLOW), true);

    executeNextInstruction();
    EXPECT_EQ(cpu_.getStatusFlag(StatusFlag::OVERFLOW), false);

    // Ensure it doesn't change already cleared flag
    executeNextInstruction();
    EXPECT_EQ(cpu_.getStatusFlag(StatusFlag::OVERFLOW), false);
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

// CPY immediate
TEST_F(InterpreterTestFixture, test_0xC0) {
    setRegisterY(0x95);
    addInstruction({0xC0, 0x22});
    addInstruction({0xC0, 0x95});
    addInstruction({0xC0, 0xA9});

    executeNextInstruction();

    EXPECT_EQ(cpu_.getRegisters().y, 0x95);
    EXPECT_EQ(cpu_.getStatusFlag(StatusFlag::ZERO), false);
    EXPECT_EQ(cpu_.getStatusFlag(StatusFlag::NEGATIVE), false);
    EXPECT_EQ(cpu_.getStatusFlag(StatusFlag::CARRY), true);

    executeNextInstruction();

    EXPECT_EQ(cpu_.getRegisters().y, 0x95);
    EXPECT_EQ(cpu_.getStatusFlag(StatusFlag::ZERO), true);
    EXPECT_EQ(cpu_.getStatusFlag(StatusFlag::NEGATIVE), false);
    EXPECT_EQ(cpu_.getStatusFlag(StatusFlag::CARRY), true);

    executeNextInstruction();

    EXPECT_EQ(cpu_.getRegisters().y, 0x95);
    EXPECT_EQ(cpu_.getStatusFlag(StatusFlag::ZERO), false);
    EXPECT_EQ(cpu_.getStatusFlag(StatusFlag::NEGATIVE), true);
    EXPECT_EQ(cpu_.getStatusFlag(StatusFlag::CARRY), false);
}

// DEX
TEST_F(InterpreterTestFixture, test_0xCA) {
    setRegisterX(0xA9);

    addInstruction({0xCA});
    addInstruction({0xCA});
    addInstruction({0xCA});

    executeNextInstruction();

    EXPECT_EQ(cpu_.getRegisters().x, 0xA8);
    EXPECT_EQ(cpu_.getStatusFlag(StatusFlag::ZERO), false);
    EXPECT_EQ(cpu_.getStatusFlag(StatusFlag::NEGATIVE), true);

    setRegisterX(0x02);

    executeNextInstruction();

    EXPECT_EQ(cpu_.getRegisters().x, 0x01);
    EXPECT_EQ(cpu_.getStatusFlag(StatusFlag::ZERO), false);
    EXPECT_EQ(cpu_.getStatusFlag(StatusFlag::NEGATIVE), false);

    executeNextInstruction();

    EXPECT_EQ(cpu_.getRegisters().x, 0x00);
    EXPECT_EQ(cpu_.getStatusFlag(StatusFlag::ZERO), true);
    EXPECT_EQ(cpu_.getStatusFlag(StatusFlag::NEGATIVE), false);
}

// DEY
TEST_F(InterpreterTestFixture, test_0x88) {
    setRegisterY(0xA9);

    addInstruction({0x88});
    addInstruction({0x88});
    addInstruction({0x88});

    executeNextInstruction();

    EXPECT_EQ(cpu_.getRegisters().y, 0xA8);
    EXPECT_EQ(cpu_.getStatusFlag(StatusFlag::ZERO), false);
    EXPECT_EQ(cpu_.getStatusFlag(StatusFlag::NEGATIVE), true);

    setRegisterY(0x02);

    executeNextInstruction();

    EXPECT_EQ(cpu_.getRegisters().y, 0x01);
    EXPECT_EQ(cpu_.getStatusFlag(StatusFlag::ZERO), false);
    EXPECT_EQ(cpu_.getStatusFlag(StatusFlag::NEGATIVE), false);

    executeNextInstruction();

    EXPECT_EQ(cpu_.getRegisters().y, 0x00);
    EXPECT_EQ(cpu_.getStatusFlag(StatusFlag::ZERO), true);
    EXPECT_EQ(cpu_.getStatusFlag(StatusFlag::NEGATIVE), false);
}

// EOR
TEST_F(InterpreterTestFixture, test_0x49) {
    setStatusFlag(StatusFlag::ZERO, false);
    setStatusFlag(StatusFlag::NEGATIVE, false);

    setRegisterA(0x00);

    addInstruction({0x49, 0x08});
    addInstruction({0x49, 0x46});
    addInstruction({0x49, 0xE7});

    EXPECT_EQ(cpu_.getRegisters().a, 0x00);
    EXPECT_EQ(cpu_.getStatusFlag(StatusFlag::ZERO), false);
    EXPECT_EQ(cpu_.getStatusFlag(StatusFlag::NEGATIVE), false);

    setRegisterA(0x00);
    executeNextInstruction();
    EXPECT_EQ(cpu_.getRegisters().a, 0x08);
    EXPECT_EQ(cpu_.getStatusFlag(StatusFlag::ZERO), false);
    EXPECT_EQ(cpu_.getStatusFlag(StatusFlag::NEGATIVE), false);

    setRegisterA(0x46);
    executeNextInstruction();
    EXPECT_EQ(cpu_.getRegisters().a, 0x00);
    EXPECT_EQ(cpu_.getStatusFlag(StatusFlag::ZERO), true);
    EXPECT_EQ(cpu_.getStatusFlag(StatusFlag::NEGATIVE), false);

    setRegisterA(0x79);
    executeNextInstruction();
    EXPECT_EQ(cpu_.getRegisters().a, 0x9E);
    EXPECT_EQ(cpu_.getStatusFlag(StatusFlag::ZERO), false);
    EXPECT_EQ(cpu_.getStatusFlag(StatusFlag::NEGATIVE), true);
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

// INY
TEST_F(InterpreterTestFixture, test_0xC8) {
    EXPECT_EQ(cpu_.getRegisters().y, 0x00);

    addInstruction({0xC8});
    addInstruction({0xC8});
    addInstruction({0xC8});

    executeNextInstruction();

    EXPECT_EQ(cpu_.getRegisters().y, 0x01);
    EXPECT_EQ(cpu_.getStatusFlag(StatusFlag::ZERO), false);
    EXPECT_EQ(cpu_.getStatusFlag(StatusFlag::NEGATIVE), false);

    setRegisterY(0xFE);

    executeNextInstruction();

    EXPECT_EQ(cpu_.getRegisters().y, 0xFF);
    EXPECT_EQ(cpu_.getStatusFlag(StatusFlag::ZERO), false);
    EXPECT_EQ(cpu_.getStatusFlag(StatusFlag::NEGATIVE), true);

    executeNextInstruction();

    EXPECT_EQ(cpu_.getRegisters().y, 0x00);
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

// JMP indirect
TEST_F(InterpreterTestFixture, test_0x6C) {
    EXPECT_EQ(getPc(), 0x00);

    pokeMemoryAddress(0x12, 0x43, 0x53);
    pokeMemoryAddress(0x13, 0x43, 0x79);

    addInstruction({0x6C, 0x12, 0x43});

    executeNextInstruction();

    EXPECT_EQ(getPc(), 0x7953);
}

// JSR absolute
TEST_F(InterpreterTestFixture, test_0x20) {
    setPc(0x23, 0x74);

    pokeMemoryAddress(0x23, 0x74, 0x20);
    pokeMemoryAddress(0x24, 0x74, 0xA5);
    pokeMemoryAddress(0x25, 0x74, 0x94);

    executeNextInstruction();

    EXPECT_EQ(getPc(), 0x94A5);
    EXPECT_EQ(peekMemoryAddress(0xFD, 0x01), 0x74);
    EXPECT_EQ(peekMemoryAddress(0xFC, 0x01), 0x25);
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
TEST_F(InterpreterTestFixture, test_0xBD) {
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

// LDA absolute,Y
TEST_F(InterpreterTestFixture, test_0xB9) {
    EXPECT_EQ(cpu_.getRegisters().a, 0x00);
    EXPECT_EQ(cpu_.getRegisters().y, 0x00);

    pokeMemoryAddress(0x12, 0x54, 0x18);
    pokeMemoryAddress(0x15, 0x54, 0x00);
    pokeMemoryAddress(0xA3, 0x59, 0xB9);

    addInstruction({0xB9, 0x12, 0x54});
    addInstruction({0xB9, 0x12, 0x54});
    addInstruction({0xB9, 0xA0, 0x59});

    executeNextInstruction();

    EXPECT_EQ(cpu_.getRegisters().a, 0x18);
    EXPECT_EQ(cpu_.getStatusFlag(StatusFlag::ZERO), false);
    EXPECT_EQ(cpu_.getStatusFlag(StatusFlag::NEGATIVE), false);

    setRegisterY(0x03);
    executeNextInstruction();

    EXPECT_EQ(cpu_.getRegisters().a, 0x00);
    EXPECT_EQ(cpu_.getStatusFlag(StatusFlag::ZERO), true);
    EXPECT_EQ(cpu_.getStatusFlag(StatusFlag::NEGATIVE), false);

    executeNextInstruction();

    EXPECT_EQ(cpu_.getRegisters().a, 0xB9);
    EXPECT_EQ(cpu_.getStatusFlag(StatusFlag::ZERO), false);
    EXPECT_EQ(cpu_.getStatusFlag(StatusFlag::NEGATIVE), true);
}

// LDA indirect,Y
TEST_F(InterpreterTestFixture, test_0xB1) {
    EXPECT_EQ(cpu_.getRegisters().a, 0x00);
    EXPECT_EQ(cpu_.getRegisters().y, 0x00);

    pokeMemoryAddress(0x12, 0x00, 0x18);
    pokeMemoryAddress(0x13, 0x00, 0x24);
    pokeMemoryAddress(0x18, 0x24, 0x59);

    pokeMemoryAddress(0xA0, 0x00, 0x28);
    pokeMemoryAddress(0xA1, 0x00, 0x95);
    pokeMemoryAddress(0x2D, 0x95, 0xC7);

    addInstruction({0xB1, 0x12});
    addInstruction({0xB1, 0xA0});

    executeNextInstruction();

    EXPECT_EQ(cpu_.getRegisters().a, 0x59);
    EXPECT_EQ(cpu_.getStatusFlag(StatusFlag::ZERO), false);
    EXPECT_EQ(cpu_.getStatusFlag(StatusFlag::NEGATIVE), false);

    setRegisterY(0x05);
    executeNextInstruction();

    EXPECT_EQ(cpu_.getRegisters().a, 0xC7);
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

// LDY immediate
TEST_F(InterpreterTestFixture, test_0xA0) {
    EXPECT_EQ(cpu_.getRegisters().y, 0x00);

    addInstruction({0xA0, 0xA9});
    addInstruction({0xA0, 0x00});
    addInstruction({0xA0, 0x63});

    executeNextInstruction();

    EXPECT_EQ(cpu_.getRegisters().y, 0xA9);
    EXPECT_EQ(cpu_.getStatusFlag(StatusFlag::ZERO), false);
    EXPECT_EQ(cpu_.getStatusFlag(StatusFlag::NEGATIVE), true);

    executeNextInstruction();

    EXPECT_EQ(cpu_.getRegisters().y, 0x00);
    EXPECT_EQ(cpu_.getStatusFlag(StatusFlag::ZERO), true);
    EXPECT_EQ(cpu_.getStatusFlag(StatusFlag::NEGATIVE), false);

    executeNextInstruction();

    EXPECT_EQ(cpu_.getRegisters().y, 0x63);
    EXPECT_EQ(cpu_.getStatusFlag(StatusFlag::ZERO), false);
    EXPECT_EQ(cpu_.getStatusFlag(StatusFlag::NEGATIVE), false);
}

// LSR accumulator
TEST_F(InterpreterTestFixture, test_0x4A) {
    EXPECT_EQ(cpu_.getRegisters().a, 0x00);

    setRegisterA(0x59);

    addInstruction({0x4A});

    executeNextInstruction();
    EXPECT_EQ(cpu_.getRegisters().a, 0x2C);
}

// ORA
TEST_F(InterpreterTestFixture, test_0x09) {
    setStatusFlag(StatusFlag::ZERO, false);
    setStatusFlag(StatusFlag::NEGATIVE, false);

    setRegisterA(0x00);

    addInstruction({0x09, 0x09});
    addInstruction({0x09, 0xE7});
    addInstruction({0x09, 0x00});

    EXPECT_EQ(cpu_.getRegisters().a, 0x00);
    EXPECT_EQ(cpu_.getStatusFlag(StatusFlag::ZERO), false);
    EXPECT_EQ(cpu_.getStatusFlag(StatusFlag::NEGATIVE), false);

    setRegisterA(0x02);
    executeNextInstruction();
    EXPECT_EQ(cpu_.getRegisters().a, 0x0B);
    EXPECT_EQ(cpu_.getStatusFlag(StatusFlag::ZERO), false);
    EXPECT_EQ(cpu_.getStatusFlag(StatusFlag::NEGATIVE), false);

    setRegisterA(0x00);
    executeNextInstruction();
    EXPECT_EQ(cpu_.getRegisters().a, 0xE7);
    EXPECT_EQ(cpu_.getStatusFlag(StatusFlag::ZERO), false);
    EXPECT_EQ(cpu_.getStatusFlag(StatusFlag::NEGATIVE), true);

    setRegisterA(0x00);
    executeNextInstruction();
    EXPECT_EQ(cpu_.getRegisters().a, 0x00);
    EXPECT_EQ(cpu_.getStatusFlag(StatusFlag::ZERO), true);
    EXPECT_EQ(cpu_.getStatusFlag(StatusFlag::NEGATIVE), false);
}

// PHP absolute
TEST_F(InterpreterTestFixture, test_0x08) {
    setStatusRegister(0x00);
    EXPECT_EQ(cpu_.getRegisters().p, 0x00);

    setStatusFlag(StatusFlag::NEGATIVE, true);
    setStatusFlag(StatusFlag::DECIMAL, true);
    setStatusFlag(StatusFlag::ZERO, true);

    addInstruction({0x08});

    executeNextInstruction();

    // BREAK should always be pushed as 1
    EXPECT_EQ(peekMemoryAddress(0xFD, 0x01), static_cast<uint8_t>(StatusFlag::BREAK) |
                                             static_cast<uint8_t>(StatusFlag::NEGATIVE) |
                                             static_cast<uint8_t>(StatusFlag::DECIMAL) |
                                             static_cast<uint8_t>(StatusFlag::ZERO));
}

// PHA
TEST_F(InterpreterTestFixture, test_0x48) {
    EXPECT_EQ(cpu_.getRegisters().a, 0x00);

    setRegisterA(0xE3);

    addInstruction({0x48});
    addInstruction({0x48});

    executeNextInstruction();
    EXPECT_EQ(peekMemoryAddress(0xFE, 0x01), 0x00);

    executeNextInstruction();
    EXPECT_EQ(peekMemoryAddress(0xFD, 0x01), 0xE3);
}

// PLA
TEST_F(InterpreterTestFixture, test_0x68) {
    EXPECT_EQ(cpu_.getRegisters().a, 0x00);

    addInstruction({0x68});
    addInstruction({0x68});
    addInstruction({0x68});

    pokeMemoryAddress(0xFF, 0x01, 0x00);
    pokeMemoryAddress(0xFE, 0x01, 0xE9);
    pokeMemoryAddress(0xFD, 0x01, 0x64);
    setStackPointer(0xFC);

    executeNextInstruction();

    EXPECT_EQ(cpu_.getRegisters().a, 0x64);
    EXPECT_EQ(cpu_.getStatusFlag(StatusFlag::ZERO), false);
    EXPECT_EQ(cpu_.getStatusFlag(StatusFlag::NEGATIVE), false);

    executeNextInstruction();

    EXPECT_EQ(cpu_.getRegisters().a, 0xE9);
    EXPECT_EQ(cpu_.getStatusFlag(StatusFlag::ZERO), false);
    EXPECT_EQ(cpu_.getStatusFlag(StatusFlag::NEGATIVE), true);

    executeNextInstruction();

    EXPECT_EQ(cpu_.getRegisters().a, 0x00);
    EXPECT_EQ(cpu_.getStatusFlag(StatusFlag::ZERO), true);
    EXPECT_EQ(cpu_.getStatusFlag(StatusFlag::NEGATIVE), false);
}

// PLP
TEST_F(InterpreterTestFixture, test_0x28) {
    setStatusRegister(static_cast<uint8_t>(StatusFlag::BREAK));
    EXPECT_EQ(cpu_.getRegisters().p, static_cast<uint8_t>(StatusFlag::BREAK));

    pokeMemoryAddress(0xFE, 0x01, static_cast<uint8_t>(StatusFlag::INTERRUPT) |
                                  static_cast<uint8_t>(StatusFlag::OVERFLOW));

    pokeMemoryAddress(0xFD, 0x01, static_cast<uint8_t>(StatusFlag::NEGATIVE) |
                                  static_cast<uint8_t>(StatusFlag::DECIMAL) |
                                  static_cast<uint8_t>(StatusFlag::ZERO));
    setStackPointer(0xFC);

    addInstruction({0x28});
    addInstruction({0x28});

    executeNextInstruction();
    // BREAK should be untouched
    EXPECT_EQ(cpu_.getRegisters().p, static_cast<uint8_t>(StatusFlag::BREAK) |
                                     static_cast<uint8_t>(StatusFlag::NEGATIVE) |
                                     static_cast<uint8_t>(StatusFlag::DECIMAL) |
                                     static_cast<uint8_t>(StatusFlag::ZERO));

    setStatusRegister(static_cast<uint8_t>(StatusFlag::UNUSED));
    executeNextInstruction();
    // UNUSED should be untouched
    EXPECT_EQ(cpu_.getRegisters().p, static_cast<uint8_t>(StatusFlag::UNUSED) |
                                     static_cast<uint8_t>(StatusFlag::INTERRUPT) |
                                     static_cast<uint8_t>(StatusFlag::OVERFLOW));
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

// RTS
TEST_F(InterpreterTestFixture, test_0x60) {
    auto flags = cpu_.getRegisters().p;

    pokeMemoryAddress(0xFF, 0x01, 0x13);
    pokeMemoryAddress(0xFE, 0x01, 0x55);
    setStackPointer(0xFD);

    addInstruction({0x60});

    executeNextInstruction();
    EXPECT_EQ(getPc(), 0x1356);
    EXPECT_EQ(getStackPointer(), 0xFF);

    EXPECT_EQ(cpu_.getRegisters().p, flags);
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
    EXPECT_EQ(cpu_.getRegisters().a, 0x11);
    EXPECT_EQ(cpu_.getStatusFlag(StatusFlag::ZERO), false);
    EXPECT_EQ(cpu_.getStatusFlag(StatusFlag::NEGATIVE), false);
    EXPECT_EQ(cpu_.getStatusFlag(StatusFlag::CARRY), true);

    setRegisterA(0x5C);
    setStatusFlag(StatusFlag::CARRY, false);

    executeNextInstruction();
    EXPECT_EQ(cpu_.getRegisters().a, 0xFF);
    EXPECT_EQ(cpu_.getStatusFlag(StatusFlag::ZERO), false);
    EXPECT_EQ(cpu_.getStatusFlag(StatusFlag::NEGATIVE), true);
    EXPECT_EQ(cpu_.getStatusFlag(StatusFlag::CARRY), false);

    setRegisterA(0x5D);
    setStatusFlag(StatusFlag::CARRY, false);

    executeNextInstruction();
    EXPECT_EQ(cpu_.getRegisters().a, 0x00);
    EXPECT_EQ(cpu_.getStatusFlag(StatusFlag::ZERO), true);
    EXPECT_EQ(cpu_.getStatusFlag(StatusFlag::NEGATIVE), false);
    EXPECT_EQ(cpu_.getStatusFlag(StatusFlag::CARRY), true);
}

// SEC
TEST_F(InterpreterTestFixture, test_0x38) {
    setStatusFlag(Cpu::StatusFlag::CARRY, false);
    auto status_before = cpu_.getRegisters().p | static_cast<uint8_t>(Cpu::StatusFlag::CARRY);
    EXPECT_EQ(cpu_.getStatusFlag(Cpu::StatusFlag::CARRY), false);

    addInstruction({0x38});
    executeNextInstruction();

    EXPECT_EQ(cpu_.getStatusFlag(Cpu::StatusFlag::CARRY), true);
    EXPECT_EQ(status_before, cpu_.getRegisters().p);
}

// SED
TEST_F(InterpreterTestFixture, test_0xF8) {
    setStatusFlag(Cpu::StatusFlag::DECIMAL, false);
    auto status_before = cpu_.getRegisters().p | static_cast<uint8_t>(Cpu::StatusFlag::DECIMAL);
    EXPECT_EQ(cpu_.getStatusFlag(Cpu::StatusFlag::DECIMAL), false);

    addInstruction({0xF8});
    executeNextInstruction();

    EXPECT_EQ(cpu_.getStatusFlag(Cpu::StatusFlag::DECIMAL), true);
    EXPECT_EQ(status_before, cpu_.getRegisters().p);
}

// SEI
TEST_F(InterpreterTestFixture, test_0x78) {
    setStatusFlag(Cpu::StatusFlag::INTERRUPT, false);
    auto status_before = cpu_.getRegisters().p | static_cast<uint8_t>(Cpu::StatusFlag::INTERRUPT);
    EXPECT_EQ(cpu_.getStatusFlag(Cpu::StatusFlag::INTERRUPT), false);

    addInstruction({0x78});
    executeNextInstruction();

    EXPECT_EQ(cpu_.getStatusFlag(Cpu::StatusFlag::INTERRUPT), true);
    EXPECT_EQ(status_before, cpu_.getRegisters().p);
}

// STA absolute
TEST_F(InterpreterTestFixture, test_0x8D) {
    EXPECT_EQ(cpu_.getRegisters().a, 0x00);

    setRegisterA(0x5C);

    addInstruction({0x8D, 0x12, 0x14});

    EXPECT_EQ(peekMemoryAddress(0x12, 0x14), 0x00);
    executeNextInstruction();
    EXPECT_EQ(peekMemoryAddress(0x12, 0x14), 0x5C);
}

// STX zero page
TEST_F(InterpreterTestFixture, test_0x86) {
    EXPECT_EQ(cpu_.getRegisters().x, 0x00);

    setRegisterX(0x73);

    addInstruction({0x86, 0x25});

    EXPECT_EQ(peekMemoryAddress(0x25, 0x00), 0x00);
    executeNextInstruction();
    EXPECT_EQ(peekMemoryAddress(0x25, 0x00), 0x73);
}

// STX zero page, y
TEST_F(InterpreterTestFixture, test_0x96) {
    EXPECT_EQ(cpu_.getRegisters().x, 0x00);

    setRegisterX(0x12);
    setRegisterY(0x19);

    addInstruction({0x96, 0x27});

    EXPECT_EQ(peekMemoryAddress(0x40, 0x00), 0x00);
    executeNextInstruction();
    EXPECT_EQ(peekMemoryAddress(0x40, 0x00), 0x12);
}

// STY zero page, X
TEST_F(InterpreterTestFixture, test_0x94) {
    EXPECT_EQ(cpu_.getRegisters().x, 0x00);

    setRegisterY(0x54);
    setRegisterX(0x03);

    addInstruction({0x94, 0x17});

    EXPECT_EQ(peekMemoryAddress(0x1A, 0x00), 0x00);
    executeNextInstruction();
    EXPECT_EQ(peekMemoryAddress(0x1A, 0x00), 0x54);
}
