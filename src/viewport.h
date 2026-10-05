#ifndef VIEWPORT_H
#define VIEWPORT_H

#include <stdbool.h>
#include <stdint.h>

void
viewport_stop(void);

void
viewport_set_active(bool);
bool
viewport_active(void);

void
viewport_follow_window(void);

void
viewport_update_screen(void);

void
viewport_begin_pan(void);
void 
viewport_left(void);
void 
viewport_right(void);
void 
viewport_top(void);
void 
viewport_down(void);
void 
viewport_center(void);

bool 
viewport_active(void);
void
viewport_push( int32_t dx,
              int32_t dy);
bool
viewport_can_move_horizontally(void);

enum viewport_mode {
    VIEWPORT_IDLE,
    VIEWPORT_KEYBOARD,
    VIEWPORT_WINDOW_FOLLOW,
    VIEWPORT_PAN,
    VIEWPORT_GESTURE,
};

struct lantern_screen {
    int32_t x;
    int32_t y;

    uint32_t width;
    uint32_t height;
};

const struct lantern_screen *
viewport_screen(void);

struct lantern_origin {
    int32_t x;
    int32_t y;
};

const struct lantern_origin *
viewport_origin(void);

int32_t
viewport_origin_x(void);

int32_t
viewport_origin_y(void);

bool
viewport_is_moving(void);

struct viewport_state {
    
    struct lantern_origin origin;
    struct lantern_screen screen;

    /* Current movement request */
    int32_t request_x;
    int32_t request_y;

    bool moving;
    enum viewport_mode mode;

    /* Pan state */
    int32_t cursor_prev_x;
    int32_t cursor_prev_y;

    struct wl_event_source *timer;
    struct wl_event_source *pan_timer;
};

extern struct viewport_state viewport;

#endif 
