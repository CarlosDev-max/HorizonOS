#pragma once
#include <stdint.h>
#define FLASH_SECTOR_SIZE 4096
#define FLASH_PAGE_SIZE   256
static inline void flash_range_erase(uint32_t a, size_t s)        { (void)a;(void)s; }
static inline void flash_range_program(uint32_t a,
    const uint8_t* d, size_t s)                                    { (void)a;(void)d;(void)s; }
