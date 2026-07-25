#include "window.h"
#include "hevel.h"
#include "input.h"
#include "zoom.h"
#include "spawn.h"
#include "viewport.h"

struct window_move_state move_state;

static void fullscreen_active(struct window *window,
                              const struct swc_rectangle *screen);

static void fullscreen_passive(struct window *window);

void fullscreen_update(struct window *window);

struct window *
focused_window(void)
{
    struct window *w;

    wl_list_for_each(w, &compositor.windows, link) {
        if (w->swc == compositor.focused)
            return w;
    }

    return NULL;
}

static struct window *
window_from_swc(struct swc_window *swc);

static struct window *
window_from_swc(struct swc_window *swc)
{
    struct window *w;

    wl_list_for_each(w, &compositor.windows, link) {
        if (w->swc == swc)
            return w;
    }

    return NULL;
}

static bool
focus_frozen(void)
{
    bool frozen =
        window_is_moving() ||
        viewport_active() ||
        swc_get_zoom() != 1.0f;

    printf("move=%d pan=%d zoom=%f\n",
           window_is_moving(),
           viewport_active(),
           swc_get_zoom());

    fflush(stdout);

    return frozen;
}

void
window_update_focus(void)
{
    if (focus_frozen())
        return;

    if (swc_window_at(input.cursor.x, input.cursor.y))
        return;

    if (compositor.focused)
        focus_window(NULL, "pointer");
}


static void
windowentered(void *data)
{
    struct window *w = data;

    if (focus_frozen())
        return;

    /*
     * Passive immersion ignores pointer hover.
     * Clicks still work because they generate focus normally.
     */
    if (w->fullscreen.enabled &&
        !w->fullscreen.snapped)
        return;

    focus_window(w->swc, "pointer");
}


void
focus_window(struct swc_window *swc, const char *reason)
{
  const char *from = compositor.focused && compositor.focused->title
                         ? compositor.focused->title
                         : "";
  const char *to = swc && swc->title ? swc->title : "";

  if (compositor.focused == swc) return;
  printf("focus %p ('%s') -> %p ('%s') (%s)\n", (void *)compositor.focused,
         from, (void *)swc, to, reason);

  if (compositor.focused)
    swc_window_set_border(compositor.focused, inner_border_color_inactive,
                          inner_border_width, outer_border_color_inactive,
                          outer_border_width);

  swc_window_focus(swc);

 
  if (swc)
    swc_window_set_border(swc, inner_border_color_active, inner_border_width,
                          outer_border_color_active, outer_border_width);

  compositor.focused = swc;
  struct window *window = window_from_swc(swc);

  if (window &&
      window->fullscreen.enabled &&
      !window->fullscreen.snapped) {

      struct swc_rectangle screen =
        compositor.current_screen->swc->geometry;

      fullscreen_active(window, &screen);
}

  /* center the focused window: both axes in drag mode, vertical only in scroll
   * wheel mode, only when visible or jumping to it, else you can center
   * offscreen windows */

}

void
focus_window_reveal(struct swc_window *swc,
                    const char *reason)
{
    struct swc_rectangle window_geom;

    focus_window(swc, reason);

    const struct canvas_screen *screen = viewport_screen();
    const struct canvas_origin *origin = viewport_origin();

    if (!swc || !screen)
        return;

    /* Already in normal view. */
    if (swc_get_zoom() >= 1.0f)
        return;

    if (!swc_window_get_geometry(swc, &window_geom))
        return;

    if (window_geom.width == 0 ||
        window_geom.height == 0)
        return;

    int32_t window_center_x =
        window_geom.x +
        (int32_t)window_geom.width / 2;

    int32_t window_center_y =
        window_geom.y +
        (int32_t)window_geom.height / 2;

    int32_t screen_center_x =
                origin->x;

    int32_t screen_center_y =
                origin->y;

    viewport_push(screen_center_x - window_center_x,
                  screen_center_y - window_center_y);

    if (enable_zoom) {

        zoom.target = 1.0f;

        if (!zoom.timer) {
            zoom.timer =
                wl_event_loop_add_timer(compositor.evloop,
                                        zoom_tick,
                                        NULL);
        }

        if (zoom.timer)
            wl_event_source_timer_update(zoom.timer, 1);
    }
}


bool
is_visible(struct swc_window *w)
{   
  const struct canvas_screen *screen =
                    viewport_screen();

  const struct canvas_origin *origin =
                    viewport_origin();

  struct swc_rectangle wgeom;
  swc_window_get_geometry(w, &wgeom);

  int32_t left =
    origin->x - screen->width / 2;

  int32_t right =
    origin->x + screen->width / 2;

  int32_t top =
    origin->y - screen->height / 2;

  int32_t bottom =
    origin->y + screen->height / 2;

  bool h =
    wgeom.x + (int32_t)wgeom.width > left &&
    wgeom.x < right;

  bool v =
    wgeom.y + (int32_t)wgeom.height > top &&
    wgeom.y < bottom;
  return h && v;
}

/* hacky sorta, only works for vertical cuz of this */
bool
is_on_screen(struct swc_rectangle *window)
{
  const struct canvas_screen *screen =
                    viewport_screen();

  const struct canvas_origin *origin =
                    viewport_origin();

  int32_t left =
    origin->x - screen->width / 2;

  int32_t right =
    origin->x + screen->width / 2;

  return window->x + (int32_t)window->width > left &&
       window->x < right;

}

bool
is_acme(const struct swc_window *swc)
{
  return swc && swc->app_id && strcmp(swc->app_id, "acme") == 0;
}

void
windowdestroy(void *data)
{
  struct window *w = data;
  if (compositor.focused == w->swc) focus_window(NULL, "destroy");
  wl_list_remove(&w->link);
  free(w);
}

static const struct swc_window_handler windowhandler = {
    .destroy = windowdestroy,
    .entered = windowentered,
};

void
newwindow(struct swc_window *swc)
{
  struct window *w;

  w = malloc(sizeof(*w));
  if (!w)
      return;

  memset(w, 0, sizeof(*w));
  w->swc = swc;
  w->pid = 0;
  w->sticky = false;
  printf("NEW WINDOW: enabled=%d snapped=%d geometry_saved=%d\n",
       w->fullscreen.enabled,
       w->fullscreen.snapped,
       w->fullscreen.geometry_saved);

  wl_list_insert(&compositor.windows, &w->link);
  swc_window_set_handler(swc, &windowhandler, w);
  swc_window_set_stacked(swc);
  swc_window_set_border(swc, inner_border_color_inactive, inner_border_width,
                        outer_border_color_inactive, outer_border_width);
  w->pid = swc_window_get_pid(swc);
  
  if (spawn.pending && w->pid == spawn.pid) {
      swc_window_set_geometry(swc, &spawn.geometry);
      spawn.pending = false;
    }

  swc_window_show(swc);
  printf("window '%s'\n", swc->title ? swc->title : "");
  focus_window(swc, "new_window");
}

void
screendestroy(void *data)
{
  struct screen *s = data;
  wl_list_remove(&s->link);
  free(s);
}

static const struct swc_screen_handler screenhandler = {
    .destroy = screendestroy,
};

void
newscreen(struct swc_screen *swc)
{
  struct screen *s;

  s = malloc(sizeof(*s));
  if (!s) return;
  s->swc = swc;
  wl_list_insert(&compositor.screens, &s->link);
  if (!compositor.current_screen)
    compositor.current_screen = s;
  swc_screen_set_handler(swc, &screenhandler, s);
  printf("screen %dx%d\n", swc->geometry.width, swc->geometry.height);

}

void
compositor_quit(void)
{
    wl_display_terminate(compositor.display);
}


void
window_toggle_sticky(void)
{
    struct window *w = focused_window();

    if (!w)
        return;

    w->sticky = !w->sticky;
}

static void
fullscreen_active(struct window *window,
                  const struct swc_rectangle *screen)
{
    if (!window || !screen)
        return;

    if (window->fullscreen.snapped)
        return;

    window->fullscreen.snapped = true;

    swc_window_set_geometry(window->swc, screen);

    swc_window_set_fullscreen(
        window->swc,
        compositor.current_screen->swc);

    swc_window_focus(window->swc);
    viewport_update_screen();
}



static void
fullscreen_passive(struct window *window)
{
    if (!window)
        return;

    if (!window->fullscreen.snapped)
        return;

    window->fullscreen.snapped = false;

    swc_window_set_fullscreen(window->swc, NULL);

    swc_window_set_geometry(
        window->swc,
        &window->fullscreen.restore_geometry);

    swc_window_set_stacked(window->swc);

    viewport_update_screen();
}


void
fullscreen_update(struct window *window)
{
    /*
    * Synchronize the current immersion state with the viewport after a
    * viewport transition. This keeps Active and Passive immersion states
    * consistent with viewport interaction and ensures the canvas recognizes
    * the new interaction rules before the next frame.
    */
    if (!window)
        return;

    if (!window->fullscreen.enabled)
        return;

    if (!window->fullscreen.snapped)
        return;

    if (!viewport.moving)
        return;

    fullscreen_passive(window);
}


void
window_toggle_fullscreen(void)
{
    struct window *window = focused_window();
    struct swc_rectangle screen;

    if (!window)
        return;

    if (!compositor.current_screen)
        return;

    screen = compositor.current_screen->swc->geometry;

    /*
     * Leave immersion.
     */
    if (window->fullscreen.enabled) {

        window->fullscreen.enabled = false;
        window->fullscreen.snapped = false;

        swc_window_set_geometry(
            window->swc,
            &window->fullscreen.restore_geometry);

        swc_window_set_stacked(window->swc);
        viewport_update_screen();

        return;
    }

    /*
     * Enter immersion.
     */
    if (!window->fullscreen.geometry_saved) {

        swc_window_get_geometry(
            window->swc,
            &window->fullscreen.restore_geometry);

        window->fullscreen.geometry_saved = true;
    }

    window->fullscreen.enabled = true;

    fullscreen_active(window, &screen);
}


static void
window_schedule_move(void)
{
    if (!move_state.timer)
        move_state.timer =
            wl_event_loop_add_timer(compositor.evloop,
                                    window_move_tick,
                                    NULL);

    wl_event_source_timer_update(move_state.timer,
                                 timerms);
}

void
window_move_begin(void)
{
    printf("[MOVE] begin\n");
    fflush(stdout);
    int32_t x, y;
    struct swc_rectangle geometry;

    if (!compositor.focused)
        return;
    
    struct window *window = focused_window();

    if (window && window->fullscreen.enabled)
        return;

    x = input.cursor.x;
    y = input.cursor.y;
    
    if (!swc_window_get_geometry(compositor.focused, &geometry))
        return;

    move_state.start_cursor_x = x;
    move_state.start_cursor_y = y;

    move_state.start_window_x = geometry.x;
    move_state.start_window_y = geometry.y;

    move_state.active = true;
    
    window_schedule_move();
      
}


void
window_resize(void)
{

    struct window *window = focused_window();

    if (window && window->fullscreen.enabled)
        return;
    
    if (!input.held)
        swc_window_end_resize(compositor.focused);

    if (compositor.focused)
        swc_window_begin_resize(
            compositor.focused,
            SWC_WINDOW_EDGE_RIGHT |
            SWC_WINDOW_EDGE_BOTTOM);
}


void
window_close(void)
{
    if (!compositor.focused)
        return;

    swc_window_close(compositor.focused);
}


struct swc_window *
window_nearest(int32_t x,
               int32_t y)
{
    struct window *closest = NULL;
    struct window *w;
    struct swc_rectangle geom;

    int64_t mindist = INT64_MAX;

    wl_list_for_each(w, &compositor.windows, link) {

        if (!w->swc)
            continue;

        if (!swc_window_get_geometry(w->swc, &geom))
            continue;

        if (w->swc == compositor.focused)
            continue;

        int64_t dx = (int64_t)x - geom.x;
        int64_t dy = (int64_t)y - geom.y;
        int64_t dist = dx * dx + dy * dy;

        if (dist < mindist) {
            closest = w;
            mindist = dist;
        }
    }

    return closest ? closest->swc : NULL;
}


void
window_jump(struct swc_window *target)
{
    if (!target)
        return;

    focus_window_reveal(target, "jump");
}


int
window_move_tick(void *data)
{
    if (!input.held) {
    window_end_move();
    return 0;
    }
    printf("[MOVE] tick\n");
    fflush(stdout);
    int32_t x, y;
    struct swc_rectangle geometry;


    (void)data;

    if (!move_state.active)
        return 0;

    if (!compositor.focused)
        return 0;

    printf("cursor %d %d\n",
       input.cursor.x,
       input.cursor.y);

    x = input.cursor.x;
    y = input.cursor.y;


    /* Smooth window movement */
    if (swc_window_get_geometry(compositor.focused, &geometry)) {

        int32_t target_x =
            move_state.start_window_x +
            (x - move_state.start_cursor_x);

        int32_t target_y =
            move_state.start_window_y +
            (y - move_state.start_cursor_y);

        int32_t new_x =
            geometry.x +
            (int32_t)((target_x - geometry.x) * move_ease_factor);

        int32_t new_y =

            geometry.y +
            (int32_t)((target_y - geometry.y) * move_ease_factor);

        swc_window_set_position(compositor.focused,
                                new_x,
                                new_y);
    }

    viewport_follow_window();

    window_schedule_move();

    return 0;
}

bool
window_is_moving(void)
{
    return move_state.active;
}


void
window_end_move(void)
{
    move_state.active = false;
}


