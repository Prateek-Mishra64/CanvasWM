#include "hevel.h"
#include "binding.h"
#include "action.h"
#include "viewport.h"
#include "window.h"
#include "spawn.h"
#include "input.h"

#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>


struct compositor_state compositor = {0};
struct chord_state chord = {0};
struct zoom_state zoom = {0};
struct sel_state sel = {0};


/* TODO: clear this up
 * it does this because we modify this value from config
 * so it needs to be copied over, too lazy to change the name now */
bool focus_center = center_focus;

static void
maybe_enable_nein_cursor_theme(void)
{
  const struct nein_cursor_meta *arrow =
      &nein_cursor_metadata[NEIN_CURSOR_WHITEARROW];
  const struct nein_cursor_meta *box =
      &nein_cursor_metadata[NEIN_CURSOR_BOXCURSOR];
  const struct nein_cursor_meta *cross =
      &nein_cursor_metadata[NEIN_CURSOR_CROSSCURSOR];
  const struct nein_cursor_meta *sight =
      &nein_cursor_metadata[NEIN_CURSOR_SIGHTCURSOR];
  const struct nein_cursor_meta *up = &nein_cursor_metadata[NEIN_CURSOR_T];
  const struct nein_cursor_meta *down = &nein_cursor_metadata[NEIN_CURSOR_B];

  if (!cursor_theme || strcmp(cursor_theme, "nein") != 0) return;

  swc_set_cursor_mode(SWC_CURSOR_MODE_COMPOSITOR);
  swc_set_cursor_image(SWC_CURSOR_DEFAULT, &nein_cursor_data[arrow->offset],
                       arrow->width, arrow->height, arrow->hotspot_x,
                       arrow->hotspot_y);
  swc_set_cursor_image(SWC_CURSOR_BOX, &nein_cursor_data[box->offset],
                       box->width, box->height, box->hotspot_x, box->hotspot_y);
  swc_set_cursor_image(SWC_CURSOR_CROSS, &nein_cursor_data[cross->offset],
                       cross->width, cross->height, cross->hotspot_x,
                       cross->hotspot_y);
  swc_set_cursor_image(SWC_CURSOR_SIGHT, &nein_cursor_data[sight->offset],
                       sight->width, sight->height, sight->hotspot_x,
                       sight->hotspot_y);
  swc_set_cursor_image(SWC_CURSOR_UP, &nein_cursor_data[up->offset], up->width,
                       up->height, up->hotspot_x, up->hotspot_y);
  swc_set_cursor_image(SWC_CURSOR_DOWN, &nein_cursor_data[down->offset],
                       down->width, down->height, down->hotspot_x,
                       down->hotspot_y);

}

static void
newdevice(struct libinput_device *dev)
{
  (void)dev;
}


static const struct swc_manager manager = {
    .new_screen = newscreen,
    .new_window = newwindow,
    .new_device = newdevice,
};

#define REQUIRE_PRESS()                     \
    do {                                    \
        if (state != WL_KEYBOARD_KEY_STATE_PRESSED) \
            return;                         \
    } while (0)

static void
bind_quit(void *data, uint32_t time, uint32_t value, uint32_t state)
{
    (void)data;
    (void)time;
    (void)value;

    REQUIRE_PRESS();

    compositor_quit();
}

static void
spawn_terminal(void *data, uint32_t time, uint32_t value, uint32_t state)
{
    (void)data;
    (void)time;
    (void)value;

    REQUIRE_PRESS();

    spawn_launch("kitty");
}

static void
spawn_file_browser(void *data, uint32_t time, uint32_t value, uint32_t state)
{
    (void)data;
    (void)time;
    (void)value;

    REQUIRE_PRESS();

    spawn_launch("nautilus --new-window");
}


static void
spawn_browser(void *data, uint32_t time, uint32_t value, uint32_t state)
{
    (void)data;
    (void)time;
    (void)value;

    REQUIRE_PRESS();

    spawn_launch("librewolf --new-window");
}

static void
spawn_launcher(void *data, uint32_t time, uint32_t value, uint32_t state)
{
    (void)data;
    (void)time;
    (void)value;

    REQUIRE_PRESS();

    spawn_launch("rofi -show drun");
}

static void
bind_viewport_left(void *data, uint32_t time, uint32_t value, uint32_t state)
{
    (void)data;
    (void)time;
    (void)value;

    REQUIRE_PRESS();

    viewport_left();
}

static void
bind_viewport_right(void *data, uint32_t time, uint32_t value, uint32_t state)
{
    (void)data;
    (void)time;
    (void)value;

    REQUIRE_PRESS();

    viewport_right();
}

static void
bind_viewport_up(void *data, uint32_t time, uint32_t value, uint32_t state)
{
    (void)data;
    (void)time;
    (void)value;

    REQUIRE_PRESS();

    viewport_top();
}

static void
bind_viewport_down(void *data, uint32_t time, uint32_t value, uint32_t state)
{
    (void)data;
    (void)time;
    (void)value;

    REQUIRE_PRESS();

    viewport_down();
}

static void
bind_window_move(void *data,
                 uint32_t time,
                 uint32_t value,
                 uint32_t state)
{
    (void)data;
    (void)time;
    (void)value;

    if (state == WL_KEYBOARD_KEY_STATE_RELEASED)
        return;

    window_move_begin();
}

static void
bind_window_resize(void *data,
                   uint32_t time,
                   uint32_t value,
                   uint32_t state)
{
    (void)data;
    (void)time;
    (void)value;

    REQUIRE_PRESS();

    window_resize();
}

static void
bind_window_close(void *data,
                  uint32_t time,
                  uint32_t value,
                  uint32_t state)
{
    (void)data;
    (void)time;
    (void)value;

    REQUIRE_PRESS();

    window_close();
}

static void
bind_window_fullscreen(void *data,
                       uint32_t time,
                       uint32_t value,
                       uint32_t state)
{
    (void)data;
    (void)time;
    (void)value;

    REQUIRE_PRESS();

    window_toggle_fullscreen();
}

static void
bind_window_sticky(void *data,
                   uint32_t time,
                   uint32_t value,
                   uint32_t state)
{
    (void)data;
    (void)time;
    (void)value;

    REQUIRE_PRESS();

    window_toggle_sticky();
}

static void
exit_handler(void)
{
    fprintf(stderr, "[DEBUG] atexit() called\n");
    fflush(stderr);
}

static void
debug_signal(int sig)
{
    fprintf(stderr, "[DEBUG] received signal %d\n", sig);
    fflush(stderr);

    signal(sig, SIG_DFL);
    raise(sig);
}

static void
sig(int s)
{
    fprintf(stderr, "[DEBUG] sig() called with %d\n", s);
    fflush(stderr);

    wl_display_terminate(compositor.display);
}


int
main(void)
{
  struct wl_event_loop *evloop;
  const char *sock;

  wl_list_init(&compositor.windows);
  wl_list_init(&compositor.screens);

  compositor.current_screen = NULL;
  compositor.display = wl_display_create();
  if (!compositor.display) {
    fprintf(stderr, "cannot create display\n");
    return 1;
  }

  evloop = wl_display_get_event_loop(compositor.display);
  compositor.evloop = evloop;

  if (!swc_initialize(compositor.display, evloop, &manager)) {
    fprintf(stderr, "cannot initialize swc\n");
    return 1;
  }
  

  maybe_enable_nein_cursor_theme();
  input_initialize();


  /* we can bind mouse buttons using SWC_MOD_ANY */
  /* Quit */
  swc_add_binding(
    SWC_BINDING_KEY,
    SWC_MOD_LOGO | SWC_MOD_SHIFT,
    XKB_KEY_q,
    bind_quit,
    NULL);

/* Spawn */
  swc_add_binding(
    SWC_BINDING_KEY,
    SWC_MOD_LOGO,
    XKB_KEY_t,
    spawn_terminal,
    NULL);


  swc_add_binding(
    SWC_BINDING_KEY,
    SWC_MOD_LOGO,
    XKB_KEY_f,
    spawn_browser,
    NULL);

  swc_add_binding(
    SWC_BINDING_KEY,
    SWC_MOD_LOGO,
    XKB_KEY_g,
    spawn_file_browser,
    NULL);

  swc_add_binding(
    SWC_BINDING_KEY,
    SWC_MOD_LOGO,
    XKB_KEY_space,
    spawn_launcher,
    NULL);


/* Viewport navigation */
  swc_add_binding(
    SWC_BINDING_KEY,
    SWC_MOD_LOGO,
    XKB_KEY_h,
    bind_viewport_left,
    NULL);

  swc_add_binding(
    SWC_BINDING_KEY,
    SWC_MOD_LOGO,
    XKB_KEY_j,
    bind_viewport_down,
    NULL);

  swc_add_binding(
    SWC_BINDING_KEY,
    SWC_MOD_LOGO,
    XKB_KEY_k,
    bind_viewport_up,
    NULL);

  swc_add_binding(
    SWC_BINDING_KEY,
    SWC_MOD_LOGO,
    XKB_KEY_l,
    bind_viewport_right,
    NULL);

/* Window */
  swc_add_binding(
    SWC_BINDING_KEY,
    SWC_MOD_LOGO,
    XKB_KEY_period,
    bind_window_fullscreen,
    NULL);

  swc_add_binding(
    SWC_BINDING_KEY,
    SWC_MOD_LOGO,
    XKB_KEY_c,
    bind_window_close,
    NULL);

  swc_add_binding(
    SWC_BINDING_KEY,
    SWC_MOD_LOGO,
    XKB_KEY_x,
    bind_window_sticky,
    NULL);


  swc_add_binding(SWC_BINDING_BUTTON,
                SWC_MOD_ANY,
                BTN_LEFT,
                button,
                NULL);

  swc_add_binding(SWC_BINDING_BUTTON,
                SWC_MOD_ANY,
                BTN_MIDDLE,
                button,
                NULL);

  swc_add_binding(SWC_BINDING_BUTTON,
                SWC_MOD_ANY,
                BTN_RIGHT,
                button,
                NULL);
   
  swc_add_axis_binding(SWC_MOD_ANY, 0, axis, NULL);
  swc_add_axis_binding(SWC_MOD_ANY, 1, axis, NULL);
  sock = wl_display_add_socket_auto(compositor.display);
  if (!sock) {
    fprintf(stderr, "cannot add socket\n");
    return 1;
  }

  printf("%s\n", sock);
  setenv("WAYLAND_DISPLAY", sock, 1);

    atexit(exit_handler);

  signal(SIGTERM, sig);
  signal(SIGINT, sig);

  signal(SIGABRT, debug_signal);
  signal(SIGSEGV, debug_signal);
  signal(SIGBUS, debug_signal);
  signal(SIGILL, debug_signal);
  signal(SIGFPE, debug_signal);
  signal(SIGPIPE, debug_signal);
  signal(SIGQUIT, debug_signal);

  viewport_update_screen();

  wl_display_run(compositor.display);

  swc_finalize();
  wl_display_destroy(compositor.display);

  return 0;

}
  
