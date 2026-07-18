#include "input.h"
#include "hevel.h"
#include "binding.h"
#include "viewport.h"
#include "window.h"


#include <xkbcommon/xkbcommon-keysyms.h>
#include <xkbcommon/xkbcommon.h>


static struct wl_event_source *cursor_timer;
static bool left_down;
static bool middle_down;
static bool right_down;
static bool dragging;
static bool cursor_position_raw(int32_t *, int32_t *);
bool interaction_held = false;
bool cursor_position(int32_t *, int32_t *);

struct input_state input;

static inline uint32_t
current_modifiers(void)
{
    return swc_get_modifiers();
}


static void
input_dispatch(enum input_symbol symbol,
               bool held)
{
    if (symbol == INPUT_NONE)
        return;
    input.symbol = symbol;
    input.held = held;
    binding_resolver();

}

void
input_initialize(void)
{
    cursor_timer =
        wl_event_loop_add_timer(compositor.evloop,
                                cursor_tick,
                                NULL);

    wl_event_source_timer_update(cursor_timer,
                                 timerms);
}

void
input_keyboard(xkb_keysym_t key,
               bool held)
{
    fprintf(stderr, "key: %u held=%d\n", key, held);
    fflush(stderr);
}

void
axis(void *data,
     uint32_t time,
     uint32_t axis,
     int32_t value120)
{
    (void)data;
    (void)time;
    uint32_t mods = current_modifiers();

    if (mods & SWC_MOD_LOGO) {

        if (value120 < 0)
            zoom_in();
        else
            zoom_out();

        return;
    }

}

void
button(void *data,
       uint32_t time,
       uint32_t button,
       uint32_t state)
{
    (void)data;

    bool held =
        state == WL_POINTER_BUTTON_STATE_PRESSED;

    /* Global interaction state. */
    input.held = held;

    uint32_t mods = current_modifiers();
    

    printf("[BUTTON] button=%u state=%u held=%d input.held=%d mods=%u\n",
           button,
           state,
           held,
           input.held,
           mods);

    /*
     * Hybrid mouse actions.
     */ 
    static bool resizing = false;

    if (button == BTN_LEFT) {

        /* Super + Shift + Left Drag -> Window Move */
        if ((mods & (SWC_MOD_LOGO | SWC_MOD_SHIFT))
            == (SWC_MOD_LOGO | SWC_MOD_SHIFT)) {

            printf("[MOVE] %s\n",
                   held ? "BEGIN" : "RELEASE");

            if (held)
                window_move_begin();

            return;
        }

        /* Super + Left Drag -> Viewport Pan */
        if (mods & SWC_MOD_LOGO) {

            printf("[PAN] %s\n",
                   held ? "BEGIN" : "RELEASE");

            if (held)
                viewport_begin_pan();
            viewport_stop();
            return;
        }
    }

    if (button == BTN_RIGHT &&
        (mods == SWC_MOD_LOGO)) {

        printf("[RESIZE] %s\n",
               held ? "BEGIN" : "RELEASE");

        if (held) {
            resizing = true; 
            window_resize(); }
        else if(resizing) {
            resizing = false;
            swc_window_end_resize(compositor.focused);
}
        
        swc_pointer_send_button(time, button, state);
        return;
    }

    /*
     * Normal mouse behaviour.
     */

    switch (button) {

    case BTN_LEFT:
        left_down = held;
        break;

    case BTN_MIDDLE:
        middle_down = held;
        break;

    case BTN_RIGHT:
        right_down = held;
        break;

    default:
        swc_pointer_send_button(time, button, state);
        return;
    }

    dragging =
        left_down ||
        middle_down ||
        right_down;

    printf("[POINTER] left=%d middle=%d right=%d dragging=%d\n",
           left_down,
           middle_down,
           right_down,
           dragging);

    swc_pointer_send_button(time, button, state);
}

int
cursor_tick(void *data)
{
    (void)data;

    int32_t x, y;
    struct screen *ns = NULL;

    if (!cursor_position_raw(&x, &y)) {
        wl_event_source_timer_update(cursor_timer, timerms);
        return 0;
    }

    /* Input owns the cursor state */
    input.cursor.x = x;
    input.cursor.y = y;
    window_update_focus();
    printf("swc=(%d,%d)\n", x, y);
    printf("input=(%d,%d)\n",
       input.cursor.x,
       input.cursor.y);

    wl_list_for_each(ns, &compositor.screens, link) {
        struct swc_rectangle *geom = &ns->swc->geometry;

        if (x >= geom->x &&
            x < geom->x + (int32_t)geom->width &&
            y >= geom->y &&
            y < geom->y + (int32_t)geom->height) {

            compositor.current_screen = ns;
            break;
        }
    }

    wl_event_source_timer_update(cursor_timer, timerms);
    return 0;
}

static bool
cursor_position_raw(int32_t *x, int32_t *y)
{
  int32_t fx, fy;

  if (!swc_cursor_position(&fx, &fy)) return false;
  *x = fx >> 8;
  *y = fy >> 8;
  return true;
}

bool
cursor_position(int32_t *x, int32_t *y)
{
  if (!cursor_position_raw(x, y)) return false;

  if (enable_zoom) {
    float zoom = swc_get_zoom();
    if (zoom != 1.0f && compositor.current_screen) {
      int32_t cx = compositor.current_screen->swc->geometry.x +
                   compositor.current_screen->swc->geometry.width / 2;
      int32_t cy = compositor.current_screen->swc->geometry.y +
                   compositor.current_screen->swc->geometry.height / 2;
      input.cursor.x = (int32_t)((*x - cx) / zoom) + cx;
      input.cursor.y = (int32_t)((*y - cy) / zoom) + cy;
    }
  }

  return true;
}


