#include "viewport.h"
#include "hevel.h"
#include "input.h"
#include "action.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

struct viewport_state viewport;

static int viewport_tick(void *data);

static void
viewport_schedule(void)
{
    if (!viewport.timer)
        viewport.timer =
            wl_event_loop_add_timer(compositor.evloop,
                                    viewport_tick,
                                    NULL);

    wl_event_source_timer_update(viewport.timer, 1);
}



static int
viewport_tick(void *data)
{
    struct window *w, *tmp;
    struct swc_rectangle geometry;

    int32_t step;
    int32_t step_x;

    (void)data;

    if (!viewport.timer)
        return 0;

    if (!viewport.active &&
        viewport.pending_x == 0 &&
        viewport.pending_y == 0) {
        viewport_stop();
        return 0;
    }

    step = viewport.pending_x / scrollease;
    if (step == 0 && viewport.pending_x != 0)
        step = viewport.pending_x > 0 ? 1 : -1;
    if (step > scrollcap)
        step = scrollcap;
    if (step < -scrollcap)
        step = -scrollcap;

    step_x = viewport.pending_y / scrollease;
    if (step_x == 0 && viewport.pending_y != 0)
        step_x = viewport.pending_y > 0 ? 1 : -1;
    if (step_x > scrollcap)
        step_x = scrollcap;
    if (step_x < -scrollcap)
        step_x = -scrollcap;

    wl_list_for_each_safe(w, tmp, &compositor.windows, link) {

        if (!w->swc)
            continue;

        if (w->sticky)
            continue;

        if (window_is_moving() &&
            w->swc == compositor.focused)
            continue;

        if (!swc_window_get_geometry(w->swc, &geometry))
            continue;

        if (!is_on_screen(&geometry,
                          compositor.current_screen))
            continue;

        swc_window_set_position(w->swc,
                                geometry.x + step_x,
                                geometry.y + step);
    }

    viewport.pending_y   -= step;
    viewport.pending_x -= step_x;

    viewport_schedule();

    return 0;
}


bool
viewport_can_move_horizontally(void)
{
    return compositor.current_screen &&
           compositor.current_screen->swc->geometry.width > 0;
}

void
viewport_set_active(bool active)
{
    viewport.active = active;
}

bool
viewport_active(void)
{
    return viewport.active;
}

void
viewport_stop(void)
{
    viewport.pending_y   = 0;
    viewport.pending_x = 0;

    viewport.active = false;

    if (viewport.pan_timer) {
        wl_event_source_remove(viewport.pan_timer);
        viewport.pan_timer = NULL;
    }
}

void
viewport_push(int32_t dx,
              int32_t dy)
{
    viewport.pending_x += dx;
    viewport.pending_y   += dy;

    viewport_schedule();
}

void
viewport_follow_window(void)
{
    if (!window_is_moving())
        return;

    if (!compositor.current_screen)
        return;

    int32_t x = input.cursor.x;
    int32_t y = input.cursor.y;

    struct swc_rectangle *screen =
        &compositor.current_screen->swc->geometry;

    viewport.active = false;

    if (y < move_scroll_edge_threshold) {

        viewport.active = true;
        viewport_push(0, move_scroll_speed);

    }
    else if (y > screen->height - move_scroll_edge_threshold) {

        viewport.active = true;
        viewport_push(0, -move_scroll_speed);

    }

    if (x < screen->x + move_scroll_edge_threshold) {

        viewport.active = true;
        viewport_push(move_scroll_speed, 0);

    }
    else if (x >
             screen->x +
             screen->width -
             move_scroll_edge_threshold) {

        viewport.active = true;
        viewport_push(-move_scroll_speed, 0);

    }
}


void
viewport_left(void)
{
    viewport.move_by_x = -scrollpx;
    viewport.move_by_y = 0;
    viewport.active = true;

    viewport_schedule();}

void
viewport_right(void)
{
    viewport.move_by_x = scrollpx;
    viewport.move_by_y = 0;
    viewport.active = true;

    viewport_schedule();
}


void
viewport_top(void)
{
    viewport.move_by_x = 0;
    viewport.move_by_y = -scrollpx;
    viewport.active = true;

    viewport_schedule();
}

void
viewport_down(void)
{
    viewport.move_by_x = 0;
    viewport.move_by_y = scrollpx;
    viewport.active = true;

    viewport_schedule();
}


static int
viewport_sample_cursor(void *data)
{
    int32_t x, y;
    int32_t dx, dy;

    (void)data;

    if (!viewport.active)
        return 0;

    x = input.cursor.x;
    y = input.cursor.y;

    dx = x - viewport.cursor_prev_x;
    dy = y - viewport.cursor_prev_y;

    viewport.cursor_prev_x = x;
    viewport.cursor_prev_y = y;

    if (dx || dy)
        viewport_push(-dx, -dy);

    wl_event_source_timer_update(viewport.pan_timer,
                                 timerms);

    return 0;
}

void
viewport_begin_pan(void)
{
    int32_t x, y;

    x = input.cursor.x;
    y = input.cursor.y;
    
    viewport.cursor_prev_x = x;
    viewport.cursor_prev_y = y;

    viewport.active = true;

    if (!viewport.pan_timer)
        viewport.pan_timer =
            wl_event_loop_add_timer(compositor.evloop,
                                    viewport_sample_cursor,
                                    NULL);

    wl_event_source_timer_update(viewport.pan_timer,
                                 timerms);
}

