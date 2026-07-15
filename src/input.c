#include "input.h"
#include "hevel.h"
#include "binding.h"
#include "viewport.h"


#include <xkbcommon/xkbcommon-keysyms.h>
#include <xkbcommon/xkbcommon.h>


static struct wl_event_source *cursor_timer;
static bool left_down;
static bool middle_down;
static bool right_down;
static bool dragging;
static bool cursor_position_raw(int32_t *, int32_t *);
bool cursor_position(int32_t *, int32_t *);

struct input_state input;

static void normalize_keyboard(xkb_keysym_t key, bool held);
static void normalize_trackpad(enum input_trackpad_gesture gesture, bool held);
static void normalize_mouse(bool dragging,
                            bool left,
                            bool middle,
                            bool right,
                            bool held);
static void normalize_scroll(uint32_t axis,
                             int32_t value120);

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
input_keyboard(xkb_keysym_t key,
               bool held)
{
    fprintf(stderr, "key: %u held=%d\n", key, held);
    fflush(stderr);
    normalize_keyboard(key, held);
}

void
input_trackpad(enum input_trackpad_gesture gesture,
               bool held)
{
    normalize_trackpad(gesture, held);
}

void
input_modifiers(enum modifier modifiers)
{
    input.modifiers = modifiers;
}



static void
normalize_mouse(bool dragging,
                bool left_down,
                bool middle_down,
                bool right_down,
                bool held)
{
    if (dragging) {

        if (left_down && right_down) {
            input_dispatch(MOUSE_LR_CHORD, held);
            return;
        }

        if (left_down) {
            input_dispatch(MOUSE_LEFT_DRAG, held);
            return;
        }

        if (right_down) {
            input_dispatch(MOUSE_RIGHT_DRAG, held);
            return;
        }

        if (middle_down) {
            input_dispatch(MOUSE_MIDDLE_DRAG, held);
            return;
        }

        return;
    }

    if (left_down) {
        input_dispatch(MOUSE_LEFT_CLICK, held);
        return;
    }

    if (middle_down) {
        input_dispatch(MOUSE_MIDDLE_CLICK, held);
        return;
    }

    if (right_down) {
        input_dispatch(MOUSE_RIGHT_CLICK, held);
        return;
    }
}
/* ---------- Keyboard ---------- */

static void
normalize_keyboard(xkb_keysym_t key,
                   bool held)
{
    switch (key) {

    case XKB_KEY_Return:
        input_dispatch(INPUT_KEY_RETURN, held);
        return;

    case XKB_KEY_space:
        input_dispatch(INPUT_KEY_SPACE, held);
        return;

    case XKB_KEY_comma:
        input_dispatch(INPUT_KEY_COMMA, held);
        return;

    case XKB_KEY_period:
        input_dispatch(INPUT_KEY_PERIOD, held);
        return;

    case XKB_KEY_a:
    case XKB_KEY_A:
        input_dispatch(INPUT_KEY_A, held);
        return;

    case XKB_KEY_b:
    case XKB_KEY_B:
        input_dispatch(INPUT_KEY_B, held);
        return;

    case XKB_KEY_c:
    case XKB_KEY_C:
        input_dispatch(INPUT_KEY_C, held);
        return;

    case XKB_KEY_d:
    case XKB_KEY_D:
        input_dispatch(INPUT_KEY_D, held);
        return;

    case XKB_KEY_e:
    case XKB_KEY_E:
        input_dispatch(INPUT_KEY_E, held);
        return;

    case XKB_KEY_f:
    case XKB_KEY_F:
        input_dispatch(INPUT_KEY_F, held);
        return;

    case XKB_KEY_g:
    case XKB_KEY_G:
        input_dispatch(INPUT_KEY_G, held);
        return;

    case XKB_KEY_h:
    case XKB_KEY_H:
        input_dispatch(INPUT_KEY_H, held);
        return;

    case XKB_KEY_i:
    case XKB_KEY_I:
        input_dispatch(INPUT_KEY_I, held);
        return;

    case XKB_KEY_j:
    case XKB_KEY_J:
        input_dispatch(INPUT_KEY_J, held);
        return;

    case XKB_KEY_k:
    case XKB_KEY_K:
        input_dispatch(INPUT_KEY_K, held);
        return;

    case XKB_KEY_l:
    case XKB_KEY_L:
        input_dispatch(INPUT_KEY_L, held);
        return;

    case XKB_KEY_m:
    case XKB_KEY_M:
        input_dispatch(INPUT_KEY_M, held);
        return;

    case XKB_KEY_n:
    case XKB_KEY_N:
        input_dispatch(INPUT_KEY_N, held);
        return;

    case XKB_KEY_o:
    case XKB_KEY_O:
        input_dispatch(INPUT_KEY_O, held);
        return;

    case XKB_KEY_p:
    case XKB_KEY_P:
        input_dispatch(INPUT_KEY_P, held);
        return;

    case XKB_KEY_q:
    case XKB_KEY_Q:
        input_dispatch(INPUT_KEY_Q, held);
        return;

    case XKB_KEY_r:
    case XKB_KEY_R:
        input_dispatch(INPUT_KEY_R, held);
        return;

    case XKB_KEY_s:
    case XKB_KEY_S:
        input_dispatch(INPUT_KEY_S, held);
        return;

    case XKB_KEY_t:
    case XKB_KEY_T:
        input_dispatch(INPUT_KEY_T, held);
        return;

    case XKB_KEY_u:
    case XKB_KEY_U:
        input_dispatch(INPUT_KEY_U, held);
        return;

    case XKB_KEY_v:
    case XKB_KEY_V:
        input_dispatch(INPUT_KEY_V, held);
        return;

    case XKB_KEY_w:
    case XKB_KEY_W:
        input_dispatch(INPUT_KEY_W, held);
        return;

    case XKB_KEY_x:
    case XKB_KEY_X:
        input_dispatch(INPUT_KEY_X, held);
        return;

    case XKB_KEY_y:
    case XKB_KEY_Y:
        input_dispatch(INPUT_KEY_Y, held);
        return;

    case XKB_KEY_z:
    case XKB_KEY_Z:
        input_dispatch(INPUT_KEY_Z, held);
        return;

    case XKB_KEY_0:
        input_dispatch(INPUT_KEY_0, held);
        return;

    case XKB_KEY_1:
        input_dispatch(INPUT_KEY_1, held);
        return;

    case XKB_KEY_2:
        input_dispatch(INPUT_KEY_2, held);
        return;

    case XKB_KEY_3:
        input_dispatch(INPUT_KEY_3, held);
        return;

    case XKB_KEY_4:
        input_dispatch(INPUT_KEY_4, held);
        return;

    case XKB_KEY_5:
        input_dispatch(INPUT_KEY_5, held);
        return;

    case XKB_KEY_6:
        input_dispatch(INPUT_KEY_6, held);
        return;

    case XKB_KEY_7:
        input_dispatch(INPUT_KEY_7, held);
        return;

    case XKB_KEY_8:
        input_dispatch(INPUT_KEY_8, held);
        return;

    case XKB_KEY_9:
        input_dispatch(INPUT_KEY_9, held);
        return;

    default:
        return;
    }
}

/* ---------- Trackpad ---------- */
static void
normalize_trackpad(enum input_trackpad_gesture gesture,
                   bool held)
{
    switch (gesture) {

    case TRACKPAD_PINCH_IN:
        input_dispatch(GESTURE_PINCH_IN, held);
        return;

    case TRACKPAD_PINCH_OUT:
        input_dispatch(GESTURE_PINCH_OUT, held);
        return;

    case TRACKPAD_THREE_LEFT:
        input_dispatch(GESTURE_THREE_LEFT, held);
        return;

    case TRACKPAD_THREE_RIGHT:
        input_dispatch(GESTURE_THREE_RIGHT, held);
        return;

    case TRACKPAD_THREE_UP:
        input_dispatch(GESTURE_THREE_UP, held);
        return;

    case TRACKPAD_THREE_DOWN:
        input_dispatch(GESTURE_THREE_DOWN, held);
        return;

    case TRACKPAD_NONE:
    default:
        return;
    }
}

static void
normalize_scroll(uint32_t axis,
                 int32_t value120)
{
    if (value120 == 0)
        return;

    switch (axis) {

    case WL_POINTER_AXIS_VERTICAL_SCROLL:
        input_dispatch(
            value120 < 0 ? MOUSE_SCROLL_UP
                         : MOUSE_SCROLL_DOWN,
            true);
        return;

    case WL_POINTER_AXIS_HORIZONTAL_SCROLL:
        input_dispatch(
            value120 < 0 ? MOUSE_SCROLL_LEFT
                         : MOUSE_SCROLL_RIGHT,
            true);
        return;
    }
}

void
axis(void *data,
     uint32_t time,
     uint32_t axis,
     int32_t value120)
{
    (void)data;
    (void)time;

    if (input.modifiers & SWC_MOD_LOGO) {

        if (value120 < 0)
            zoom_in();
        else
            zoom_out();

        return;
    }

    normalize_scroll(axis, value120);
}

void
button(void *data,
       uint32_t time,
       uint32_t button,
       uint32_t state)
{
    (void)data;
    (void)time;

    bool held =
        state == WL_POINTER_BUTTON_STATE_PRESSED;
    
    printf("mods=%u button=%u held=%d\n",
           input.modifiers,
           button,
           held);
    /*
     * Hybrid mouse actions.
     */

    if (button == BTN_LEFT) {

        /* Super + Shift + Left Drag -> Window Move */
        if ((input.modifiers & (MOD_SUPER | MOD_SHIFT))
            == (MOD_SUPER | MOD_SHIFT)) {

            if (held)
                window_move_begin();

            return;
        }

        /* Super + Left Drag -> Viewport Pan */
        if (input.modifiers & MOD_SUPER) {

            if (held)
                viewport_begin_pan();

            else
                viewport_stop();

            return;
        }
    }

    if (button == BTN_RIGHT) {

        /* Super + Right Drag -> Resize */
        if ((input.modifiers & MOD_SUPER) && held) {
            window_resize();
            return;
        }
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

    normalize_mouse(dragging,
                    left_down,
                    middle_down,
                    right_down,
                    held);
}


static int
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
  *x = fx;
  *y = fy;
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


