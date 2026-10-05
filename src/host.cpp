#include "host.h"

#include <wayfire/core.hpp>
#include <wayfire/view.hpp>
#include <wayfire/toplevel-view.hpp>
#include <wayfire/nonstd/wlroots-full.hpp>
#include <wayfire/geometry.hpp>

#include <src/core/core-impl.hpp>

#include <wayfire/canvas-input.hpp>

/* ============================================================================
 * Helpers
 * ========================================================================== */

//static wf::wayfire_toplevel_view
//host_get_toplevel(struct swc_window *window)
//{
    /*
     * First pass:
     *
     * struct swc_window is still retained by Lantern, so there is no
     * Wayfire <-> Lantern object mapping yet.
     *
     * Do not invent one here.
     *
     * This helper will be replaced once the window structure is migrated.
     */
 //   (void)window;
//    return nullptr;
//}

/* ============================================================================
 * Lifecycle
 * ========================================================================== */

bool
host_initialize(int argc, char **argv)
{
    return wf::canvas_initialize(argc, argv);
}

void
host_finalize(void)
{
    wf::canvas_shutdown();
}

/* ============================================================================
 * Cursor & Pointer
 * ========================================================================== */

bool
host_cursor_position(int32_t *x,
                     int32_t *y,
                     uint32_t *time)
{
    if (!x || !y)
        return false;

    auto position = wf::get_core().get_cursor_position();

    *x = static_cast<int32_t>(position.x);
    *y = static_cast<int32_t>(position.y);

    /*
     * Wayfire's current cursor-position API does not provide an event
     * timestamp. Keep the timestamp untouched for now.
     */
    if (time)
        *time = 0;

    return true;
}




void
host_pointer_send_button(uint32_t time,
                         uint32_t button,
                         uint32_t state)
{
    /*
     * Lantern currently receives pointer events through the
     * canvas input bridge. There is no need to synthesize another
     * Wayfire pointer event here.
     */
    (void)time;
    (void)button;
    (void)state;
}

void
host_pointer_send_axis(uint32_t time,
                       uint32_t axis,
                       int32_t value120)
{
    /*
     * Same reasoning as host_pointer_send_button().
     */
    (void)time;
    (void)axis;
    (void)value120;
}

uint32_t
host_get_modifiers(void)
{
    /*
     * Modifier state is currently supplied through the input bridge.
     * Keep this backend function inert until the SWC modifier interface
     * is removed.
     */
    return 0;
}

void
host_set_cursor(enum swc_cursor_kind kind)
{
    (void)kind;
}

void
host_set_cursor_mode(enum swc_cursor_mode mode)
{
    (void)mode;
}

void
host_set_cursor_image(enum swc_cursor_kind kind,
                      const uint32_t *argb8888,
                      uint32_t width,
                      uint32_t height,
                      int32_t hotspot_x,
                      int32_t hotspot_y)
{
    (void)kind;
    (void)argb8888;
    (void)width;
    (void)height;
    (void)hotspot_x;
    (void)hotspot_y;
}

void
host_clear_cursor_image(enum swc_cursor_kind kind)
{
    (void)kind;
}

/* ============================================================================
 * Overlay & Viewport
 * ========================================================================== */

void
host_overlay_set_box(int32_t x1,
                     int32_t y1,
                     int32_t x2,
                     int32_t y2,
                     uint32_t color,
                     uint32_t border_width)
{
    (void)x1;
    (void)y1;
    (void)x2;
    (void)y2;
    (void)color;
    (void)border_width;
}

void
host_overlay_clear(void)
{
}

float
host_get_zoom(void)
{
    return 1.0f;
}

void
host_set_zoom(float level)
{
    (void)level;
}

/* ============================================================================
 * Screen Management
 * ========================================================================== */

void
host_screen_set_handler(struct swc_screen *screen,
                        const struct swc_screen_handler *handler,
                        void *data)
{
    /*
     * SWC screen callbacks are part of the old host lifecycle.
     * Wayfire manages output lifecycle independently.
     */
    (void)screen;
    (void)handler;
    (void)data;
}

/* ============================================================================
 * Window Lifecycle
 * ========================================================================== */

void
host_window_set_handler(struct swc_window *window,
                        const struct swc_window_handler *handler,
                        void *data)
{
    /*
     * Old SWC callback mechanism. Do not recreate it on top of Wayfire.
     */
    (void)window;
    (void)handler;
    (void)data;
}

void
host_window_show(struct swc_window *window)
{
    /*
     * No direct replacement until Lantern's swc_window identity
     * is mapped to a Wayfire view.
     */
    (void)window;
}

void
host_window_hide(struct swc_window *window)
{
    (void)window;
}

void
host_window_close(struct swc_window *window)
{
    /*
     * Deliberately left alone for now because translating swc_window
     * into a Wayfire view requires the window-object migration.
     */
    (void)window;
}

void
host_window_focus(struct swc_window *window)
{
    auto view = host_get_toplevel(window);

    if (!view)
        return;

    wf::get_core().seat->focus_view(view);
}

/* ============================================================================
 * Window Geometry
 * ========================================================================== */

void
host_window_set_position(struct swc_window *window,
                         int32_t x,
                         int32_t y)
{
    auto view = host_get_toplevel(window);

    if (!view)
        return;

    view->move(x, y);
}

void
host_window_set_size(struct swc_window *window,
                     uint32_t width,
                     uint32_t height)
{
    /*
     * Deliberately deferred.
     *
     * Wayfire's resize/configure path has client-side negotiation semantics,
     * so we should not guess at the correct API here.
     */
    (void)window;
    (void)width;
    (void)height;
}

void
host_window_set_geometry(struct swc_window *window,
                         const struct swc_rectangle *geometry)
{
    if (!geometry)
        return;

    auto view = host_get_toplevel(window);

    if (!view)
        return;

    view->move(geometry->x, geometry->y);

    /*
     * Size is intentionally not applied yet.
     * See host_window_set_size().
     */
}

bool
host_window_get_geometry(const struct swc_window *window,
                         struct swc_rectangle *geometry)
{
    if (!geometry)
        return false;

    auto view = host_get_toplevel(const_cast<struct swc_window *>(window));

    if (!view)
        return false;

    auto g = view->get_geometry();

    geometry->x = g.x;
    geometry->y = g.y;
    geometry->width = g.width;
    geometry->height = g.height;

    return true;
}

pid_t
host_window_get_pid(struct swc_window *window)
{
    /*
     * Deferred until the swc_window -> Wayfire view mapping is removed.
     */
    (void)window;
    return -1;
}

struct swc_window *
host_window_at(int32_t x,
               int32_t y)
{
    /*
     * Wayfire can already locate the view under a compositor-space point.
     *
     * But returning struct swc_window * requires the old SWC identity.
     * Therefore the actual lookup is deferred until that identity is removed.
     */
    (void)x;
    (void)y;

    return nullptr;
}

/* ============================================================================
 * Window State
 * ========================================================================== */

void
host_window_set_stacked(struct swc_window *window)
{
    /*
     * Stacking semantics need the Wayfire view mapping.
     */
    (void)window;
}

void
host_window_set_tiled(struct swc_window *window)
{
    /*
     * Exact tile edges are currently part of Lantern's old SWC abstraction.
     * Do not guess at them here.
     */
    (void)window;
}

void
host_window_set_fullscreen(struct swc_window *window,
                           struct swc_screen *screen)
{
    /*
     * Requires both:
     *   swc_window -> Wayfire view
     *   swc_screen -> Wayfire output
     *
     * Leave untouched until those identities are migrated.
     */
    (void)window;
    (void)screen;
}

void
host_window_stack(struct swc_window *window,
                  int32_t direction)
{
    (void)window;
    (void)direction;
}

/* ============================================================================
 * Window Decorations
 * ========================================================================== */

void
host_window_set_border(struct swc_window *window,
                       uint32_t inner_border_color,
                       uint32_t inner_border_width,
                       uint32_t outer_border_color,
                       uint32_t outer_border_width)
{
    (void)window;
    (void)inner_border_color;
    (void)inner_border_width;
    (void)outer_border_color;
    (void)outer_border_width;
}

void
host_window_set_decor(struct swc_window *window,
                      const struct swc_decor *decor)
{
    (void)window;
    (void)decor;
}

/* ============================================================================
 * Interactive Operations
 * ========================================================================== */

void
host_window_begin_move(struct swc_window *window)
{
    /*
     * Lantern already owns move semantics.
     *
     * Do not invoke Wayfire's move plugin here.
     */
    (void)window;
}

void
host_window_end_move(struct swc_window *window)
{
    (void)window;
}

void
host_window_begin_resize(struct swc_window *window,
                         uint32_t edges)
{
    /*
     * Lantern already owns resize semantics.
     */
    (void)window;
    (void)edges;
}

void
host_window_end_resize(struct swc_window *window)
{
    (void)window;
}

/* ============================================================================
 * Input Bindings
 * ========================================================================== */

int
host_add_binding(enum swc_binding_type type,
                 uint32_t modifiers,
                 uint32_t value,
                 swc_binding_handler handler,
                 void *data)
{
    /*
     * Lantern's own binding system should eventually replace SWC bindings.
     * Do not re-create the old SWC registration layer.
     */
    (void)type;
    (void)modifiers;
    (void)value;
    (void)handler;
    (void)data;

    return 0;
}

void
host_remove_binding(enum swc_binding_type type,
                    uint32_t modifiers,
                    uint32_t value)
{
    (void)type;
    (void)modifiers;
    (void)value;
}

int
host_add_axis_binding(uint32_t modifiers,
                      uint32_t axis,
                      swc_axis_binding_handler handler,
                      void *data)
{
    (void)modifiers;
    (void)axis;
    (void)handler;
    (void)data;

    return 0;
}



/* ============================================================================
 * Miscellaneous
 * ========================================================================== */
