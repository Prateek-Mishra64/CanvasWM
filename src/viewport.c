#include "viewport.h"
#include "hevel.h"
#include "window.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

struct viewport_state viewport;

void
viewport_schedule(void)
{
    if (!viewport.timer)
        viewport.timer =
            wl_event_loop_add_timer(compositor.evloop,
                                    viewport_tick,
                                    NULL);

    wl_event_source_timer_update(viewport.timer, 1);
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

    viewport.rem_x   = 0;
    viewport.rem_y = 0;

    viewport.active = false;

    if (viewport.drag_timer) {
        wl_event_source_remove(viewport.drag_timer);
        viewport.drag_timer = NULL;
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

int
viewport_tick(void *data)
{
    struct window *w, *tmp;
    struct swc_rectangle geometry;

    int32_t rem   = viewport.pending_y;
    int32_t rem_x = viewport.pending_x;

    int32_t step;
    int32_t step_x;

    (void)data;

    if (!viewport.timer)
        return 0;

    if (!viewport.active &&
        rem == 0 &&
        rem_x == 0) {
        viewport_stop();
        return 0;
    }

    step = rem / scrollease;
    if (step == 0 && rem != 0)
        step = rem > 0 ? 1 : -1;
    if (step > scrollcap)
        step = scrollcap;
    if (step < -scrollcap)
        step = -scrollcap;

    step_x = rem_x / scrollease;
    if (step_x == 0 && rem_x != 0)
        step_x = rem_x > 0 ? 1 : -1;
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

void
viewport_left(void)
{
    viewport_push(-scrollpx, 0);
}

void
viewport_right(void)
{
    viewport_push(scrollpx, 0);
}

void
viewport_top(void)
{
    viewport_push(0, -scrollpx);
}

void
viewport_down(void)
{
    viewport_push(0, scrollpx);
}


int
viewport_drag_tick(void *data)
{
    int32_t x, y;
    int32_t dx, dy;

    (void)data;

    if (!viewport.active)
        return 0;

    if (!cursor_position(&x, &y)) {
        wl_event_source_timer_update(viewport.drag_timer,
                                     timerms);
        return 0;
    }

    dx = x - viewport.drag_last_x;
    dy = y - viewport.drag_last_y;

    viewport.drag_last_x = x;
    viewport.drag_last_y = y;

    if (dx || dy)
        viewport_push(-dx, -dy);

    wl_event_source_timer_update(viewport.drag_timer,
                                 timerms);

    return 0;
}

void
viewport_begin_navigation(void)
{
    int32_t x, y;

    if (!cursor_position(&x, &y))
        return;

    viewport.drag_last_x = x;
    viewport.drag_last_y = y;

    viewport.active = true;

    if (!viewport.drag_timer)
        viewport.drag_timer =
            wl_event_loop_add_timer(compositor.evloop,
                                    viewport_drag_tick,
                                    NULL);

    wl_event_source_timer_update(viewport.drag_timer,
                                 timerms);
}

