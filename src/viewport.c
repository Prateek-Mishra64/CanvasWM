#include "viewport.h"
#include "hevel.h"
#include "input.h"
#include "action.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

struct viewport_state viewport;
const struct canvas_origin *
viewport_origin(void)
{
    return &viewport.origin;
}

static void
viewport_update_screen(void)
{
    if (!compositor.current_screen)
        return;

    struct swc_rectangle *g =
        &compositor.current_screen->swc->geometry;

    viewport.screen.x = g->x;
    viewport.screen.y = g->y;
    viewport.screen.width = g->width;
    viewport.screen.height = g->height;

    viewport.origin.x =
        viewport.screen.x + viewport.screen.width / 2;

    viewport.origin.y =
        viewport.screen.y + viewport.screen.height / 2;
}

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
    viewport_update_screen();
    struct window *w, *tmp;
    struct swc_rectangle geometry;

    int32_t step;
    int32_t step_x;

    (void)data;

    if (!viewport.timer)
        return 0;

    if (viewport.request_x == 0 &&
        viewport.request_y == 0) {
        viewport.moving = false;

        viewport_stop();
        return 0;
    }
    

    step = viewport.request_x / scrollease;
    if (step == 0 && viewport.request_x != 0)
        step = viewport.request_x > 0 ? 1 : -1;
    if (step > scrollcap)
        step = scrollcap;
    if (step < -scrollcap)
        step = -scrollcap;

    step_x = viewport.request_y / scrollease;
    if (step_x == 0 && viewport.request_y != 0)
        step_x = viewport.request_y > 0 ? 1 : -1;
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

        if (!is_on_screen(&geometry))
            continue;

        swc_window_set_position(w->swc,
                                geometry.x + step,
                                geometry.y + step_x);
    }
    

    viewport.request_x   -= step;
    viewport.request_y -= step_x;
    
    if (viewport.request_x == 0 ||
        viewport.request_y == 0)
      {
          viewport_stop();
          return 0;
      }

    viewport_schedule();
    return 0;
}


bool
viewport_can_move_horizontally(void)
{
    return viewport.screen.width > 0;
}

void
viewport_set_active(bool active)
{
    viewport.moving = active;
}

bool
viewport_active(void)
{   
    return viewport.moving;
    
}

void
viewport_stop(void)
{
    viewport.request_x   = 0;
    viewport.request_y = 0;

    viewport.moving = false;
    viewport.mode = VIEWPORT_IDLE;

    }

void
viewport_push(int32_t dx,
              int32_t dy)
{
    viewport.request_x += dx;
    viewport.request_y += dy;
    viewport.moving = true;

    viewport_schedule();
}

void
viewport_follow_window(void)
{
    if (!window_is_moving())
        return;

    if (!compositor.current_screen)
        return;
    viewport_update_screen();

    int32_t x = input.cursor.x;
    int32_t y = input.cursor.y;

    struct canvas_screen *screen =
           &viewport.screen;

    if (y < move_scroll_edge_threshold) {

        viewport_push(0, move_scroll_speed);

    }
    else if (y > screen->height - move_scroll_edge_threshold) {

        viewport_push(0, -move_scroll_speed);

    }

    if (x < screen->x + move_scroll_edge_threshold) {

        viewport_push(move_scroll_speed, 0);

    }
    else if (x >
             screen->x +
             screen->width -
             move_scroll_edge_threshold) {

        viewport_push(-move_scroll_speed, 0);

    }
}


void
viewport_left(void)
{
    viewport.mode = VIEWPORT_KEYBOARD;
    viewport_push(scrollpx, 0);

}

void
viewport_right(void)
{
    viewport.mode = VIEWPORT_KEYBOARD;
    viewport_push(-scrollpx, 0);
}


void
viewport_top(void)
{
    viewport.mode = VIEWPORT_KEYBOARD;
    viewport_push(0, scrollpx);
}

void
viewport_down(void)
{

    viewport.mode = VIEWPORT_KEYBOARD;
    viewport_push(0, -scrollpx);
}


static int
viewport_sample_cursor(void *data)
{
    int32_t x, y;
    int32_t dx, dy;

    (void)data;

    if (!input.held) {
        viewport.moving = false;
        viewport.mode = VIEWPORT_IDLE;
        return 0;
    } 
    x = input.cursor.x;
    y = input.cursor.y;

    dx = x - viewport.cursor_prev_x;
    dy = y - viewport.cursor_prev_y;

    viewport.cursor_prev_x = x;
    viewport.cursor_prev_y = y;

    if (dx || dy)
        viewport_push(dx, dy);

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

    viewport.mode = VIEWPORT_PAN;
    if (!viewport.pan_timer)
        viewport.pan_timer =
            wl_event_loop_add_timer(compositor.evloop,
                                    viewport_sample_cursor,
                                    NULL);

    wl_event_source_timer_update(viewport.pan_timer,
                                 timerms);
}

const struct canvas_screen *
viewport_screen(void)
{
    return &viewport.screen;
}

int32_t
viewport_origin_x(void)
{
    return viewport.origin.x;
}

int32_t
viewport_origin_y(void)
{
    return viewport.origin.y;
}

bool
viewport_is_moving(void)
{
    return viewport.moving;
}
