#ifndef INPUT_H
#define INPUT_H

#include <stdbool.h>
#include <stdint.h>
#include "binding.h"


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
button(void *data, uint32_t time, uint32_t b, uint32_t state);
void
axis(void *data, uint32_t time, uint32_t axis, int32_t value120);
void
click_cancel(void);
bool
cursor_position(int32_t *x, int32_t *y);
bool
cursor_position_raw(int32_t *x, int32_t *y);
int
cursor_tick(void *data);


/* Raw event entry points */
void input_keyboard(...);
void input_mouse_button(...);
void input_mouse_motion(...);
void input_mouse_scroll(...);
void input_trackpad(...);

/* Normalization helpers */
enum bind_symbol input_normalize_key(...);
enum bind_symbol input_normalize_mouse(...);
enum bind_symbol input_normalize_gesture(...);

enum modifier input_normalize_modifiers(...);

/* Dispatch into binding layer */
void input_dispatch(enum bind_symbol symbol,
                    bool held);
#endif
