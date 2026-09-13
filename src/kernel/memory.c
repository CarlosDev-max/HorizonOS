#include "memory.h"
#include <string.h>

// Simple bump allocator over a static pool
// 520KB SRAM on Pico 2W — reserve 64KB for OS pool
#define MEM_POOL_SIZE (64 * 1024)
static uint8_t pool[MEM_POOL_SIZE];
static uint32_t pool_ptr = 0;

void mem_init(void) { pool_ptr = 0; }

void* mem_alloc(size_t size) {
    size = (size + 3) & ~3; // align to 4 bytes
    if (pool_ptr + size > MEM_POOL_SIZE) return NULL;
    void* p = &pool[pool_ptr];
    pool_ptr += size;
    return p;
}

void mem_free(void* ptr) {
    // bump allocator: no individual free
    // TODO: replace with tlsf or similar
    (void)ptr;
}

void mem_stats(uint32_t* used, uint32_t* free) {
    *used = pool_ptr;
    *free = MEM_POOL_SIZE - pool_ptr;
}

