#include "scheduler.h"
#include "pico/stdlib.h"

static Task tasks[MAX_TASKS];
static uint8_t task_count = 0;

void scheduler_add(const char* name, task_fn_t fn) {
    if (task_count >= MAX_TASKS) return;
    tasks[task_count].fn = fn;
    tasks[task_count].name = name;
    tasks[task_count].active = 1;
    task_count++;
}

void scheduler_run(void) {
    while (1) {
        for (uint8_t i = 0; i < task_count; i++) {
            if (tasks[i].active && tasks[i].fn) {
                tasks[i].fn();
            }
        }
    }
}

void scheduler_yield(void) {
    // cooperative: just return to scheduler loop
    tight_loop_contents();
}

