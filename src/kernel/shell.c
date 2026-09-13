#include "shell.h"
#include "pico/stdlib.h"
#include "scheduler.h"
#include <stdio.h>
#include <string.h>

#define SHELL_BUF 64
static char buf[SHELL_BUF];
static uint8_t pos = 0;

static void shell_exec(const char* cmd) {
    if (strcmp(cmd, "help") == 0) {
        printf("HorizonOS shell
Commands: help, tasks, mem, reboot
");
    } else if (strcmp(cmd, "reboot") == 0) {
        printf("Rebooting...
");
        watchdog_reboot(0, 0, 0);
    } else {
        printf("Unknown: %s
", cmd);
    }
}

void shell_init(void) {
    printf("
HorizonOS v0.1 shell ready. Type \"help\".
> ");
}

void shell_tick(void) {
    int c = getchar_timeout_us(0);
    if (c == PICO_ERROR_TIMEOUT) return;
    if (c == r || c == n) {
        buf[pos] = 0;
        printf("
");
        if (pos > 0) shell_exec(buf);
        pos = 0;
        printf("> ");
    } else if (c == 127 && pos > 0) {
        pos--;
        printf(" ");
    } else if (pos < SHELL_BUF - 1) {
        buf[pos++] = c;
        putchar(c);
    }
}

