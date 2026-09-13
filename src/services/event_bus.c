#include "event_bus.h"
#include <string.h>

#define MAX_HANDLERS 16

typedef struct {
    EventType       type;
    event_handler_t handler;
} Subscription;

static Subscription subs[MAX_HANDLERS];
static uint8_t sub_count = 0;

void event_bus_subscribe(EventType type, event_handler_t handler) {
    if (sub_count >= MAX_HANDLERS) return;
    subs[sub_count].type = type;
    subs[sub_count].handler = handler;
    sub_count++;
}

void event_bus_publish(const Event* ev) {
    for (uint8_t i = 0; i < sub_count; i++) {
        if (subs[i].type == ev->type && subs[i].handler) {
            subs[i].handler(ev);
        }
    }
}

void event_bus_tick(void) {
    Event ev = { .type = EVT_TICK };
    event_bus_publish(&ev);
}

