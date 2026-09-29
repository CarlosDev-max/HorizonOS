#pragma once
#include <stdint.h>
typedef void* spi_inst_t;
#define spi0 ((spi_inst_t*)0)
static inline void spi_init(spi_inst_t* s, uint32_t b)            { (void)s;(void)b; }
static inline int  spi_write_blocking(spi_inst_t* s,
    const uint8_t* b, size_t l)                                    { (void)s;(void)b;(void)l; return 0; }
