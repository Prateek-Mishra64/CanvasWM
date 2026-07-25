#ifndef WINDOW_H
#define WINDOW_H

#include <stdbool.h>
#include <swc.h>
#include <wayland-util.h>
#include <wayland-server.h>

struct screen;


void
window_update_focus(void);


void
focus_window(struct swc_window *swc, const char *reason);
bool
is_visible(struct swc_window *w);
bool
is_on_screen(struct swc_rectangle *window);
bool
is_acme(const struct swc_window *swc);

struct window *
focused_window(void);

bool 
window_is_moving(void);

int
window_move_tick(void *data);

void
focus_window_reveal(struct swc_window *swc,
                    const char *reason);

void
fullscreen_update(struct window *window);

void
window_move_begin(void);

void
window_end_move(void);

struct canvas_fullscreen {
    bool enabled;
    bool snapped;

    bool geometry_saved;
    struct swc_rectangle restore_geometry;
};

struct window {
    struct wl_list link;

    struct swc_window *swc;

    pid_t pid;

    bool sticky;
    struct canvas_fullscreen fullscreen;
};


void
newwindow(struct swc_window *swc);
void
newscreen(struct swc_screen *swc);

struct window_move_state {

    struct wl_event_source *timer;

    int32_t start_window_x;
    int32_t start_window_y;

    int32_t start_cursor_x;
    int32_t start_cursor_y;

    bool active;
};

extern struct window_move_state move_state;

void
window_toggle_sticky(void);

void
window_toggle_fullscreen(void);

void
compositor_quit(void);


void window_move(void);
void window_resize(void);

void
window_close(void);

void window_jump(struct swc_window *target);

void 
focus_next(void);

void 
focus_previous(void);


struct swc_window *
window_nearest(int32_t x,
               int32_t y);

#endif

