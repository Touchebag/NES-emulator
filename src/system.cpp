
#include "../inc/system.h"

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
    return -1;
  }

  fclose(file);
  return 0;
};
