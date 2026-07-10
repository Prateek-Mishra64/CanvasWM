#include "window.h"
#include "hevel.h"
#include "input.h"
#include "scroll.h"
#include "zoom.h"
#include "spawn.h"

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

  /* zoom to default size when focusing a window */
  if (enable_zoom && swc && swc_get_zoom() != 1.0f) {
    zoom.target = 1.0f;
    if (!zoom.timer)
      zoom.timer = wl_event_loop_add_timer(compositor.evloop, zoom_tick, NULL);
    if (zoom.timer) wl_event_source_timer_update(zoom.timer, 1);
  }

  if (swc)
    swc_window_set_border(swc, inner_border_color_active, inner_border_width,
                          outer_border_color_active, outer_border_width);

  compositor.focused = swc;

  /* center the focused window: both axes in drag mode, vertical only in scroll
   * wheel mode, only when visible or jumping to it, else you can center
   * offscreen windows */
  if (focus_center == true && swc && compositor.current_screen &&
      (is_visible(compositor.focused, compositor.current_screen) ||
       chord.mode == MODE_JUMP)) {
    struct swc_rectangle window_geom;

    if (swc_window_get_geometry(swc, &window_geom)) {
      /* skip if window has no size yet (not configured by client) */
      if (window_geom.width == 0 || window_geom.height == 0) return;

      int32_t window_center_x = window_geom.x + (int32_t)window_geom.width / 2;
      int32_t window_center_y = window_geom.y + (int32_t)window_geom.height / 2;
      int32_t screen_center_x =
          compositor.current_screen->swc->geometry.x +
          (int32_t)compositor.current_screen->swc->geometry.width / 2;
      int32_t screen_center_y =
          compositor.current_screen->swc->geometry.y +
          (int32_t)compositor.current_screen->swc->geometry.height / 2;

      /* in drag mode: center on both axes; in scroll wheel mode: vertical only
       */
      int32_t scroll_delta_x =
          scroll_drag_mode ? (screen_center_x - window_center_x) : 0;
      int32_t scroll_delta_y = screen_center_y - window_center_y;

      if (scroll_delta_x != 0 || scroll_delta_y != 0) {
        /* stop scroll before auto-scroll */
        scroll_stop();

        scroll.pending_px = scroll_delta_y;
        scroll.pending_px_x = scroll_delta_x;
        scroll.rem = 0;
        scroll.rem_x = 0;
        scroll.auto_scrolling = true;

        if (!scroll.timer) {
          scroll.timer =
              wl_event_loop_add_timer(compositor.evloop, scroll_tick, NULL);
        }
        wl_event_source_timer_update(scroll.timer, timerms);
      }
    }
  }
}

bool
is_visible(struct swc_window *w, struct screen *screen)
{
  struct swc_rectangle *geom = &screen->swc->geometry;
  struct swc_rectangle wgeom;
  swc_window_get_geometry(w, &wgeom);

  bool h = wgeom.x + (int32_t)wgeom.width > geom->x &&
           wgeom.x < geom->x + (int32_t)geom->width;
  bool v = wgeom.y + (int32_t)wgeom.height > geom->y &&
           wgeom.y < geom->y + (int32_t)geom->height;

  return h && v;
}

/* hacky sorta, only works for vertical cuz of this */
bool
is_on_screen(struct swc_rectangle *window, struct screen *screen)
{
  struct swc_rectangle *geom = &screen->swc->geometry;

  return window->x + (int32_t)window->width > geom->x &&
         window->x < geom->x + (int32_t)geom->width;
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
};

void
newwindow(struct swc_window *swc)
{
  struct window *w;

  w = malloc(sizeof(*w));
  if (!w) return;
  w->swc = swc;
  w->pid = 0;
  w->sticky = false;

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
  swc_screen_set_handler(swc, &screenhandler, s);
  printf("screen %dx%d\n", swc->geometry.width, swc->geometry.height);

  if (!input.cursor_timer)
    input.cursor_timer =
        wl_event_loop_add_timer(compositor.evloop, cursor_tick, NULL);
  if (input.cursor_timer)
    wl_event_source_timer_update(input.cursor_timer, timerms);
}


