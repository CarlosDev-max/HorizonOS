#include "fs.h"
#include "lfs.h"
#include "hardware/flash.h"
#include "hardware/sync.h"
#include "pico/stdlib.h"
#include <string.h>
#include <stdio.h>

// Use last 1 MB of 4 MB flash for filesystem
#define FS_FLASH_OFFSET  (3u * 1024u * 1024u)
#define FS_BLOCK_SIZE    FLASH_SECTOR_SIZE
#define FS_BLOCK_COUNT   (1024u * 1024u / FS_BLOCK_SIZE)
#define XIP_BASE_ADDR    0x10000000u

static lfs_t lfs;
static bool  lfs_mounted = false;

// ─── Flash callbacks ─────────────────────────────────────────────────────────

static int lfs_flash_read(const struct lfs_config* c,
                           lfs_block_t block, lfs_off_t off,
                           void* buf, lfs_size_t size) {
    (void)c;
    uint32_t addr = XIP_BASE_ADDR + FS_FLASH_OFFSET + block * FS_BLOCK_SIZE + off;
    memcpy(buf, (const void*)addr, size);
    return LFS_ERR_OK;
}

static int lfs_flash_prog(const struct lfs_config* c,
                           lfs_block_t block, lfs_off_t off,
                           const void* buf, lfs_size_t size) {
    (void)c;
    uint32_t addr = FS_FLASH_OFFSET + block * FS_BLOCK_SIZE + off;
    uint32_t irq  = save_and_disable_interrupts();
    flash_range_program(addr, (const uint8_t*)buf, size);
    restore_interrupts(irq);
    return LFS_ERR_OK;
}

static int lfs_flash_erase(const struct lfs_config* c, lfs_block_t block) {
    (void)c;
    uint32_t addr = FS_FLASH_OFFSET + block * FS_BLOCK_SIZE;
    uint32_t irq  = save_and_disable_interrupts();
    flash_range_erase(addr, FS_BLOCK_SIZE);
    restore_interrupts(irq);
    return LFS_ERR_OK;
}

static int lfs_flash_sync(const struct lfs_config* c) {
    (void)c;
    return LFS_ERR_OK;
}

static const struct lfs_config LFS_CFG = {
    .read  = lfs_flash_read,
    .prog  = lfs_flash_prog,
    .erase = lfs_flash_erase,
    .sync  = lfs_flash_sync,
    .read_size      = 1,
    .prog_size      = FLASH_PAGE_SIZE,
    .block_size     = FS_BLOCK_SIZE,
    .block_count    = FS_BLOCK_COUNT,
    .cache_size     = FLASH_PAGE_SIZE,
    .lookahead_size = 16,
    .block_cycles   = 500,
};

// ─── Public API ──────────────────────────────────────────────────────────────

void fs_init(void) {
    int err = lfs_mount(&lfs, &LFS_CFG);
    if (err) {
        printf("[fs] formatting...\n");
        lfs_format(&lfs, &LFS_CFG);
        err = lfs_mount(&lfs, &LFS_CFG);
    }
    lfs_mounted = (err == LFS_ERR_OK);
    printf("[fs] %s\n", lfs_mounted ? "mounted OK" : "FAILED");
}

bool fs_write(const char* path, const uint8_t* data, uint32_t len) {
    if (!lfs_mounted) return false;
    lfs_file_t f;
    int flags = LFS_O_WRONLY | LFS_O_CREAT | LFS_O_TRUNC;
    if (lfs_file_open(&lfs, &f, path, flags) < 0) return false;
    lfs_file_write(&lfs, &f, data, len);
    lfs_file_close(&lfs, &f);
    return true;
}

int32_t fs_read(const char* path, uint8_t* buf, uint32_t len) {
    if (!lfs_mounted) return -1;
    lfs_file_t f;
    if (lfs_file_open(&lfs, &f, path, LFS_O_RDONLY) < 0) return -1;
    int32_t n = (int32_t)lfs_file_read(&lfs, &f, buf, len);
    lfs_file_close(&lfs, &f);
    return n;
}

bool fs_exists(const char* path) {
    if (!lfs_mounted) return false;
    struct lfs_info info;
    return lfs_stat(&lfs, path, &info) == LFS_ERR_OK;
}

bool fs_delete(const char* path) {
    if (!lfs_mounted) return false;
    return lfs_remove(&lfs, path) == LFS_ERR_OK;
}
