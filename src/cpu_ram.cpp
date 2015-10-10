#include "../inc/cpu_ram.h"

Cpu_ram::Cpu_ram(BYTE **prg_rom_pointer) {
  prg_rom = prg_rom_pointer;
};

Cpu_ram::~Cpu_ram() {
};

//TODO handle NULL pointer
BYTE Cpu_ram::read_ram(BYTE address_low, BYTE address_high) {
  BYTE *location = access_memory_byte(address_low, address_high);

  return *location;
};

void Cpu_ram::write_ram(BYTE address_low, BYTE address_high, BYTE value) {
  BYTE *location = access_memory_byte(address_low, address_high);
  *location = value;

  return;
};

BYTE* Cpu_ram::access_memory_byte(BYTE address_low, BYTE address_high) {
  int address = (address_high << 8) | address_low;
  BYTE *pointer = NULL;

  if (address < PPU_REGISTERS) {
    // System_ram
    address = address % 0x800;
    pointer = &(system_ram[address]);
  } else if (address < IO_REGISTERS) {
    // Ppu_registers
    address = (address - PPU_REGISTERS) % 0x8;
    pointer = &(ppu_registers[address]);
  } else if (address < EXPANSION_ROM) {
    // IO_registers
    address = (address - IO_REGISTERS) % 0x20;
    pointer = &(io_registers[address]);
  } else if (address < SRAM) {
    // Expansion_rom
    address = (address - EXPANSION_ROM);
    pointer = &(expansion_rom[address]);
  } else if (address < PRG_ROM_LOWER) {
    // SRAM
    address = (address - SRAM);
    pointer = &(sram[address]);
  } else if (address < PRG_ROM_UPPER) {
    // First PRG_ROM bank
    address = (address - PRG_ROM_LOWER);
    pointer = &(prg_rom_lower_bank[address]);
  } else if (address < 0x10000) {
    // Second PRG_ROM bank
    address = (address - PRG_ROM_UPPER);
    pointer = &(prg_rom_upper_bank[address]);
  };

  return pointer;
};

void Cpu_ram::load_prg_block(Block_slot slot, int block_index) {
  BYTE *block = prg_rom[block_index];

  if (slot == PRG_SLOT_LOWER) {
    prg_rom_lower_bank = block;
  } else {
    prg_rom_upper_bank = block;
  };
};
