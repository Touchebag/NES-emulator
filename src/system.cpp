#include "../inc/system.h"

int System::load_rom_image(std::string filepath) {
  int error_code = OK;

  error_code = System::open_rom(filepath);
  if (error_code != OK) {
    return error_code;
  };

  error_code = System::check_current_rom();
  if (error_code != OK) {
    return error_code;
  };

  error_code = System::parse_current_rom();
  if (error_code != OK) {
    return error_code;
  };

  return OK;
};

int System::init(std::string filepath) {
  load_rom_image(filepath);
  return OK;
};

int System::open_rom(std::string filepath) {
  // Get file size
  struct stat filestatus;
  stat(filepath.c_str(), &filestatus);
  int filesize = filestatus.st_size;

  current_rom = new BYTE[filesize];

  // Open file
  FILE *file = NULL;
  file = fopen(filepath.c_str(), "rb");

  if (file != NULL) {
    fread(current_rom, filesize, 1, file);
  } else {
    delete current_rom;
    return CANNOT_OPEN_FILE;
  }

  fclose(file);
  return OK;
};

int System::check_current_rom() {
  if (!((current_rom[0] == 'N') &&
        (current_rom[1] == 'E') &&
        (current_rom[2] == 'S'))) {
    return BAD_ROM_FILE;
  };

  if (current_rom[3] != 0x1A) {
    return BAD_ROM_FILE;
  }
  // TODO Add more checks
  // iNES, NES 2.0 etc.

  return OK;
};

int System::parse_current_rom() {
  // All flags
  // TODO parse more flags as needed
  int flags_6 = current_rom[6];

  prg_rom_blocks = current_rom[4];
  chr_rom_blocks = current_rom[5];

  // Array of pointers to the start of each PRG ROM block in current_rom
  prg_rom = new BYTE*[prg_rom_blocks];

  //Add 512 bytes to offset if the ROM has a trainer
  has_trainer = (flags_6 & 4);

  int trainer_offset = 0;
  if (has_trainer) {
    trainer_offset = 512;
  };

  // PRG ROM blocks
  for (int i = 0; i < prg_rom_blocks; i++) {
    // Calculate location of PRG ROM banks
    int tmp = 16 + trainer_offset + (i * 16384);
    prg_rom[i] = &current_rom[tmp];
  };

  // CHR ROM blocks
  for (int i = 0; i < chr_rom_blocks; i++) {
    // Calculate location of CHR ROM banks (offset by size of PRG ROM)
    int tmp = 16 + (prg_rom_blocks * 16384) + trainer_offset + (i * 16384);
    chr_rom[i] = &current_rom[tmp];
  };

  return OK;
};

