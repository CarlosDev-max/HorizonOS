#pragma once
#include <stdint.h>
#include <stddef.h>
#define LFS_ERR_OK   0
#define LFS_O_RDONLY 1
#define LFS_O_WRONLY 2
#define LFS_O_CREAT  4
#define LFS_O_TRUNC  8
typedef uint32_t lfs_block_t, lfs_off_t, lfs_size_t;
typedef struct { void *r,*p,*e,*s; uint32_t read_size,prog_size,block_size,block_count,cache_size,lookahead_size,block_cycles; } lfs_config;
typedef struct { int x; } lfs_t, lfs_file_t;
struct lfs_info { int type; char name[256]; };
static inline int lfs_mount(lfs_t* l, const lfs_config* c)  { (void)l;(void)c; return -1; }
static inline int lfs_format(lfs_t* l, const lfs_config* c) { (void)l;(void)c; return -1; }
static inline int lfs_file_open(lfs_t* l, lfs_file_t* f, const char* p, int fl) { (void)l;(void)f;(void)p;(void)fl; return -1; }
static inline int lfs_file_read(lfs_t* l, lfs_file_t* f, void* b, lfs_size_t s) { (void)l;(void)f;(void)b;(void)s; return -1; }
static inline int lfs_file_write(lfs_t* l, lfs_file_t* f, const void* b, lfs_size_t s) { (void)l;(void)f;(void)b;(void)s; return -1; }
static inline int lfs_file_close(lfs_t* l, lfs_file_t* f)  { (void)l;(void)f; return 0; }
static inline int lfs_stat(lfs_t* l, const char* p, struct lfs_info* i) { (void)l;(void)p;(void)i; return -1; }
static inline int lfs_remove(lfs_t* l, const char* p)       { (void)l;(void)p; return -1; }
