#include "fs.h"
// TODO: integrate LittleFS over Pico flash
// lfs_t lfs;
// lfs_config cfg = { ... flash read/prog/erase callbacks ... };

void fs_init(void) {}
bool fs_write(const char* path, const uint8_t* data, uint32_t len) { return false; }
int32_t fs_read(const char* path, uint8_t* buf, uint32_t len) { return -1; }
bool fs_exists(const char* path) { return false; }
bool fs_delete(const char* path) { return false; }

