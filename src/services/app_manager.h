#pragma once

typedef void (*app_launch_fn_t)(void);

typedef struct {
    const char*    name;
    app_launch_fn_t launch;
} AppDef;

void app_manager_register(const char* name, app_launch_fn_t launch);
void app_manager_launch(const char* name);

