#pragma once
#include <stdint.h>

#define MAX_TASKS 8

typedef void (*task_fn_t)(void);

typedef struct {
    task_fn_t fn;
    const char* name;
    uint8_t active;
} Task;

void scheduler_add(const char* name, task_fn_t fn);
void scheduler_run(void);
void scheduler_yield(void);

