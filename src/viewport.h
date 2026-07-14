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

struct viewport_state {

    struct wl_event_source *timer;
    struct wl_event_source *pan_timer;

    int32_t move_by_x;
    int32_t move_by_y;

    int32_t pending_x;
    int32_t pending_y;

    int32_t cursor_prev_x;
    int32_t cursor_prev_y;

    bool active;
};
extern struct viewport_state viewport;

#endif 
