#pragma once
#include <stdint.h>

typedef enum {
    EVT_TOUCH,
    EVT_APP_LAUNCH,
    EVT_APP_EXIT,
    EVT_WIFI_CONNECTED,
    EVT_WIFI_DISCONNECTED,
    EVT_TICK,
} EventType;

typedef struct {
    EventType type;
    int32_t   data[2];
} Event;

typedef void (*event_handler_t)(const Event* ev);

void event_bus_subscribe(EventType type, event_handler_t handler);
void event_bus_publish(const Event* ev);
void event_bus_tick(void);

