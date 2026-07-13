#ifndef INPUT_H
#define INPUT_H

#include <stdbool.h>
#include <stdint.h>
#include "binding.h"
#include <xkbcommon/xkbcommon-keysyms.h>
#include <xkbcommon/xkbcommon.h>



struct input_state {

    /* Current modifier state */
    enum modifier modifiers;

    struct wl_event_source *cursor_timer;

    /* Button state */
    bool left_down;
    bool middle_down;
    bool right_down;

    bool dragging;
};

extern struct input_state input;


void
input_keyboard(xkb_keysym_t key,
               bool held);

void
input_modifiers(enum modifier modifiers);

void
input_trackpad(enum trackpad_gesture gesture,
               bool held);

void
button(void *data, uint32_t time, uint32_t b, uint32_t state);
void
axis(void *data, uint32_t time, uint32_t axis, int32_t value120);
bool
cursor_position(int32_t *x, int32_t *y);
bool
cursor_position_raw(int32_t *x, int32_t *y);
int
cursor_tick(void *data);

/* Dispatch into binding layer */
void input_dispatch(enum bind_symbol symbol,
                    bool held);
#endif
