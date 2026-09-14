#pragma once
#include <stdint.h>
#include <stdbool.h>

// GPIO buttons used in Wokwi simulation.
// On real hardware the XPT2046 touch controller takes over.
#define BTN_LEFT   2   // navigate left
#define BTN_RIGHT  3   // navigate right
#define BTN_OK     4   // confirm / open
#define BTN_BACK   5   // back (optional)

typedef enum {
    INPUT_NONE = 0,
    INPUT_LEFT,
    INPUT_RIGHT,
    INPUT_UP,
    INPUT_DOWN,
    INPUT_SELECT,   // tap / confirm / A-equivalent
    INPUT_BACK,     // B-equivalent
} InputEvent;

void        input_init(void);
InputEvent  input_poll(void);   // non-blocking, call each frame
