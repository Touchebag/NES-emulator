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
}

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

  return OK;
};

int System::parse_current_rom() {
  prg_rom_blocks = current_rom[4];
  chr_rom_blocks = current_rom[5];
  return OK;
};

