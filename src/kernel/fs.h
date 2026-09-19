#pragma once
#include <stdint.h>
#include <stdbool.h>

void    fs_init(void);
bool    fs_write(const char* path, const uint8_t* data, uint32_t len);
int32_t fs_read(const char* path, uint8_t* buf, uint32_t len);
bool    fs_exists(const char* path);
bool    fs_delete(const char* path);
