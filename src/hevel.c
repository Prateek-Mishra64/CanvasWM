#include "hevel.h"
#include "binding.h"

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


static bool
canvas_raw_key(xkb_keysym_t sym,
               uint32_t modifiers,
               bool held)
{
    input.modifiers = modifiers;

    input_keyboard(sym, held);

    return true;
}

static const struct swc_manager manager = {
    .new_screen = newscreen,
    .new_window = newwindow,
    .new_device = newdevice,
};



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
  swc_set_raw_key_handler(canvas_raw_key);


  /* we can bind mouse buttons using SWC_MOD_ANY */
  swc_add_binding(SWC_BINDING_BUTTON, SWC_MOD_ANY, BTN_LEFT, button, NULL);
  swc_add_binding(SWC_BINDING_BUTTON, SWC_MOD_ANY, BTN_MIDDLE, button, NULL);
  swc_add_binding(SWC_BINDING_BUTTON, SWC_MOD_ANY, BTN_RIGHT, button, NULL);
  
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

  fprintf(stderr, "[DEBUG] BEFORE wl_display_run()\n");
  fflush(stderr);

  wl_display_run(compositor.display);

  fprintf(stderr, "[DEBUG] AFTER wl_display_run()\n");
  fflush(stderr);

  fprintf(stderr, "[DEBUG] BEFORE swc_finalize()\n");
  fflush(stderr);

  swc_finalize();
  fprintf(stderr, "[DEBUG] AFTER swc_finalize()\n");
  fflush(stderr);

  fprintf(stderr, "[DEBUG] BEFORE wl_display_destroy()\n");
  fflush(stderr);

  wl_display_destroy(compositor.display);

  fprintf(stderr, "[DEBUG] AFTER wl_display_destroy()\n");
  fflush(stderr);

  fprintf(stderr, "[DEBUG] RETURNING FROM MAIN\n");
  fflush(stderr);

  return 0;

}
  
