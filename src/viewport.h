#ifndef VIEWPORT_H
#define VIEWPORT_H

#include "scroll.h"
#include <stdbool.h>

void
viewport_schedule(void);
void
viewport_stop(void);

int
viewport_tick(void *data);
int
viewport_drag_tick(void *data);

void
viewport_set_active(bool);
bool
viewport_active(void);

void
viewport_begin_navigation();
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
    struct wl_event_source *drag_timer;

    int32_t pending_x;
    int32_t pending_y;

    int32_t rem_x;
    int32_t rem_y;

    int32_t drag_last_x;
    int32_t drag_last_y;

    bool active;
};
extern struct viewport_state viewport;

#endif 
