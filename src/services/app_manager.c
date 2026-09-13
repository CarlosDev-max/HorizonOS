#include "app_manager.h"
#include <string.h>

#define MAX_APPS 8
static AppDef apps[MAX_APPS];
static uint8_t app_count = 0;

void app_manager_register(const char* name, app_launch_fn_t launch) {
    if (app_count >= MAX_APPS) return;
    apps[app_count].name = name;
    apps[app_count].launch = launch;
    app_count++;
}

void app_manager_launch(const char* name) {
    for (uint8_t i = 0; i < app_count; i++) {
        if (strcmp(apps[i].name, name) == 0) {
            if (apps[i].launch) apps[i].launch();
            return;
        }
    }
}

