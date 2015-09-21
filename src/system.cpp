
#include "../inc/system.h"

int System::open_rom(std::string filepath) {
  // Get file size
  struct stat filestatus;
  stat(filepath.c_str(), &filestatus);
  int filessize = filestatus.st_size;

  // Open file
  FILE *file = NULL;
  fopen(filepath.c_str(), "rb");

  fclose(file);

  return 0;
};
