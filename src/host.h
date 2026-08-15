#ifndef HOST_H
#define HOST_H

#ifdef __cplusplus
extern "C" {
#endif

#define _POSIX_C_SOURCE 200809L

#include <stdbool.h>
#include <stdint.h>
#include <sys/types.h>

#include <wayland-server.h>
#include <swc.h>

/* ============================================================================
 * Lifecycle
 * ========================================================================== */

bool
host_initialize(int argc, char **argv);

void
host_finalize(void);

/* ============================================================================
 * Cursor & Pointer
 * ========================================================================== */

bool
host_cursor_position(int32_t *x,
                     int32_t *y);

void
host_pointer_send_button(uint32_t time,
                         uint32_t button,
                         uint32_t state);

void
host_pointer_send_axis(uint32_t time,
                       uint32_t axis,
                       int32_t value120);

uint32_t
host_get_modifiers(void);

void
host_set_cursor(enum swc_cursor_kind kind);

void
host_set_cursor_mode(enum swc_cursor_mode mode);

void
host_set_cursor_image(enum swc_cursor_kind kind,
                      const uint32_t *argb8888,
                      uint32_t width,
                      uint32_t height,
                      int32_t hotspot_x,
                      int32_t hotspot_y);

void
host_clear_cursor_image(enum swc_cursor_kind kind);

/* ============================================================================
 * Overlay & Viewport
 * ========================================================================== */

void
host_overlay_set_box(int32_t x1,
                     int32_t y1,
                     int32_t x2,
                     int32_t y2,
                     uint32_t color,
                     uint32_t border_width);

void
host_overlay_clear(void);

float
host_get_zoom(void);

void
host_set_zoom(float zoom);

/* ============================================================================
 * Screen Management
 * ========================================================================== */

void
host_screen_set_handler(struct swc_screen *screen,
                        const struct swc_screen_handler *handler,
                        void *data);

/* ============================================================================
 * Window Lifecycle
 * ========================================================================== */

void
host_window_set_handler(struct swc_window *window,
                        const struct swc_window_handler *handler,
                        void *data);

void
host_window_show(struct swc_window *window);

void
host_window_hide(struct swc_window *window);

void
host_window_close(struct swc_window *window);

void
host_window_focus(struct swc_window *window);

/* ============================================================================
 * Window Geometry
 * ========================================================================== */

void
host_window_set_position(struct swc_window *window,
                         int32_t x,
                         int32_t y);

void
host_window_set_size(struct swc_window *window,
                     uint32_t width,
                     uint32_t height);

void
host_window_set_geometry(struct swc_window *window,
                         const struct swc_rectangle *geometry);

bool
host_window_get_geometry(const struct swc_window *window,
                         struct swc_rectangle *geometry);

pid_t
host_window_get_pid(struct swc_window *window);

struct swc_window *
host_window_at(int32_t x,
               int32_t y);

/* ============================================================================
 * Window State
 * ========================================================================== */

void
host_window_set_stacked(struct swc_window *window);

void
host_window_set_tiled(struct swc_window *window);

void
host_window_set_fullscreen(struct swc_window *window,
                           struct swc_screen *screen);

void
host_window_stack(struct swc_window *window,
                  int32_t direction);

/* ============================================================================
 * Window Decorations
 * ========================================================================== */

void
host_window_set_border(struct swc_window *window,
                       uint32_t inner_border_color,
                       uint32_t inner_border_width,
                       uint32_t outer_border_color,
                       uint32_t outer_border_width);

void
host_window_set_decor(struct swc_window *window,
                      const struct swc_decor *decor);

/* ============================================================================
 * Interactive Operations
 * ========================================================================== */

void
host_window_begin_move(struct swc_window *window);

void
host_window_end_move(struct swc_window *window);

void
host_window_begin_resize(struct swc_window *window,
                         uint32_t edges);

void
host_window_end_resize(struct swc_window *window);

/* ============================================================================
 * Input Bindings
 * ========================================================================== */

int
host_add_binding(enum swc_binding_type type,
                 uint32_t modifiers,
                 uint32_t value,
                 swc_binding_handler handler,
                 void *data);

void
host_remove_binding(enum swc_binding_type type,
                    uint32_t modifiers,
                    uint32_t value);

int
host_add_axis_binding(uint32_t modifiers,
                      uint32_t axis,
                      swc_axis_binding_handler handler,
                      void *data);

#ifdef __cplusplus
}
#endif

#endif /* HOST_H */
