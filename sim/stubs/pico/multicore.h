#pragma once
typedef void (*multicore_entry_fn_t)(void);
static inline void multicore_launch_core1(multicore_entry_fn_t fn) { (void)fn; }
