#include "input.h"
#include <stdint.h>

#define QSIZE 16
static InputEvent queue[QSIZE];
static int q_head = 0, q_tail = 0;

// Called from main_sim.c SDL event loop
void sim_input_push(InputEvent ev) {
    int next = (q_tail + 1) % QSIZE;
    if (next != q_head) { queue[q_tail] = ev; q_tail = next; }
}

void input_init(void) { q_head = q_tail = 0; }

InputEvent input_poll(void) {
    if (q_head == q_tail) return INPUT_NONE;
    InputEvent ev = queue[q_head];
    q_head = (q_head + 1) % QSIZE;
    return ev;
}
