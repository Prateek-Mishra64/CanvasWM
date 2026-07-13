#include "input.h"
#include "hevel.h"
#include "binding.h"


#include <xkbcommon/xkbcommon-keysyms.h>
#include <xkbcommon/xkbcommon.h>


static enum bind_symbol
normalize_mouse(uint32_t button,
                bool dragging,
                bool left_down,
                bool middle_down,
                bool right_down)
{
    if (dragging) {

        if (left_down && right_down)
            return MOUSE_LR_CHORD;

        if (left_down)
            return MOUSE_LEFT_DRAG;

        if (right_down)
            return MOUSE_RIGHT_DRAG;

        if (middle_down)
            return MOUSE_MIDDLE_DRAG;

        return SYMBOL_NONE;
    }

    if (left_down)
        return MOUSE_LEFT_CLICK;

    if (middle_down)
        return MOUSE_MIDDLE_CLICK;

    if (right_down)
        return MOUSE_RIGHT_CLICK;

    return SYMBOL_NONE;
}


struct input_state input;

/* Forward normalized input into the binding layer */
void
input_dispatch(enum bind_symbol symbol,
               bool held)
{
    if (symbol == SYMBOL_NONE)
        return;

    binding_dispatch(input.modifiers, symbol, held);
}


static void
input_key(enum bind_symbol key,
          bool held);

static void
input_mouse(enum bind_symbol mouse,
            bool held);

static void
input_gesture(enum bind_symbol gesture,
              bool held);

/* ---------- Keyboard ---------- */

static enum bind_symbol
normalize_keyboard(xkb_keysym_t key)
{
    switch (key) {

    case XKB_KEY_Return:  return INPUT_KEY_RETURN;
    case XKB_KEY_space:   return INPUT_KEY_SPACE;
    case XKB_KEY_comma:   return INPUT_KEY_COMMA;
    case XKB_KEY_period:  return INPUT_KEY_PERIOD;

    case XKB_KEY_a:
    case XKB_KEY_A: return INPUT_KEY_A;

    case XKB_KEY_b:
    case XKB_KEY_B: return INPUT_KEY_B;

    case XKB_KEY_c:
    case XKB_KEY_C: return INPUT_KEY_C;

    case XKB_KEY_d:
    case XKB_KEY_D: return INPUT_KEY_D;

    case XKB_KEY_e:
    case XKB_KEY_E: return INPUT_KEY_E;

    case XKB_KEY_f:
    case XKB_KEY_F: return INPUT_KEY_F;

    case XKB_KEY_g:
    case XKB_KEY_G: return INPUT_KEY_G;

    case XKB_KEY_h:
    case XKB_KEY_H: return INPUT_KEY_H;

    case XKB_KEY_i:
    case XKB_KEY_I: return INPUT_KEY_I;

    case XKB_KEY_j:
    case XKB_KEY_J: return INPUT_KEY_J;

    case XKB_KEY_k:
    case XKB_KEY_K: return INPUT_KEY_K;

    case XKB_KEY_l:
    case XKB_KEY_L: return INPUT_KEY_L;

    case XKB_KEY_m:
    case XKB_KEY_M: return INPUT_KEY_M;

    case XKB_KEY_n:
    case XKB_KEY_N: return INPUT_KEY_N;

    case XKB_KEY_o:
    case XKB_KEY_O: return INPUT_KEY_O;

    case XKB_KEY_p:
    case XKB_KEY_P: return INPUT_KEY_P;

    case XKB_KEY_q:
    case XKB_KEY_Q: return INPUT_KEY_Q;

    case XKB_KEY_r:
    case XKB_KEY_R: return INPUT_KEY_R;

    case XKB_KEY_s:
    case XKB_KEY_S: return INPUT_KEY_S;

    case XKB_KEY_t:
    case XKB_KEY_T: return INPUT_KEY_T;

    case XKB_KEY_u:
    case XKB_KEY_U: return INPUT_KEY_U;

    case XKB_KEY_v:
    case XKB_KEY_V: return INPUT_KEY_V;

    case XKB_KEY_w:
    case XKB_KEY_W: return INPUT_KEY_W;

    case XKB_KEY_x:
    case XKB_KEY_X: return INPUT_KEY_X;

    case XKB_KEY_y:
    case XKB_KEY_Y: return INPUT_KEY_Y;

    case XKB_KEY_z:
    case XKB_KEY_Z: return INPUT_KEY_Z;

    case XKB_KEY_0: return INPUT_KEY_0;
    case XKB_KEY_1: return INPUT_KEY_1;
    case XKB_KEY_2: return INPUT_KEY_2;
    case XKB_KEY_3: return INPUT_KEY_3;
    case XKB_KEY_4: return INPUT_KEY_4;
    case XKB_KEY_5: return INPUT_KEY_5;
    case XKB_KEY_6: return INPUT_KEY_6;
    case XKB_KEY_7: return INPUT_KEY_7;
    case XKB_KEY_8: return INPUT_KEY_8;
    case XKB_KEY_9: return INPUT_KEY_9;

    default:
        return SYMBOL_NONE;
    }
}

/* ---------- Trackpad ---------- */
static enum bind_symbol
normalize_trackpad(enum trackpad_gesture gesture)
{
    switch (gesture) {

    case TRACKPAD_PINCH_IN:
        return GESTURE_PINCH_IN;

    case TRACKPAD_PINCH_OUT:
        return GESTURE_PINCH_OUT;

    case TRACKPAD_THREE_LEFT:
        return GESTURE_THREE_LEFT;

    case TRACKPAD_THREE_RIGHT:
        return GESTURE_THREE_RIGHT;

    case TRACKPAD_THREE_UP:
        return GESTURE_THREE_UP;

    case TRACKPAD_THREE_DOWN:
        return GESTURE_THREE_DOWN;

    default:
        return SYMBOL_NONE;
    }
}

static enum bind_symbol
normalize_scroll(uint32_t axis, int32_t value120) {
  if (value120 == 0)
    return SYMBOL_NONE;

  switch (axis) {

    case WL_POINTER_AXIS_VERTICAL_SCROLL:
      return value120 < 0
        ? MOUSE_SCROLL_UP
        : MOUSE_SCROLL_DOWN;

    case WL_POINTER_AXIS_HORIZONTAL_SCROLL:
      return value120 < 0
        ? MOUSE_SCROLL_LEFT
        : MOUSE_SCROLL_RIGHT;
  }
    return SYMBOL_NONE;
}



void
axis(void *data, uint32_t time, uint32_t axis, int32_t value120)
{
  enum bind_symbol symbol;
  
  (void)data;
  (void)time;


  symbol = normalize_scroll(axis, value120);

if (symbol == SYMBOL_NONE) {
    swc_pointer_send_axis(time, axis, value120);
    return;
}

if (!binding_dispatch(input.modifiers, symbol, true))
    swc_pointer_send_axis(time, axis, value120);
 }


void
button(void *data,
       uint32_t time,
       uint32_t button,
       uint32_t state)
{
    (void)data;
    (void)time;

    bool held = (state == WL_POINTER_BUTTON_STATE_PRESSED);

    switch (button) {

    case BTN_LEFT:
        input.left_down = held;
        break;

    case BTN_MIDDLE:
        input.middle_down = held;
        break;

    case BTN_RIGHT:
        input.right_down = held;
        break;

    default:
        swc_pointer_send_button(time, button, state);
        return;
    }

    input.dragging =
        input.left_down ||
        input.middle_down ||
        input.right_down;

    enum bind_symbol symbol =
        normalize_mouse(button,
                        input.dragging,
                        input.left_down,
                        input.middle_down,
                        input.right_down);

    input_dispatch(symbol, held);

    if (symbol == SYMBOL_NONE)
        swc_pointer_send_button(time, button, state);
}

int
cursor_tick(void *data)
{
  (void)data;

  int32_t x, y;
  struct screen *ns = NULL;

  if (!cursor_position_raw(&x, &y)) {
    wl_event_source_timer_update(input.cursor_timer, timerms);
    return 0;
  } 

  wl_list_for_each(ns, &compositor.screens, link) {
    struct swc_rectangle *geom = &ns->swc->geometry;

    if (x >= geom->x && x < geom->x + (int32_t)geom->width && y >= geom->y &&
        y < geom->y + (int32_t)geom->height) {

      if (compositor.current_screen != ns) compositor.current_screen = ns;

      break;
    }

    }

  wl_event_source_timer_update(input.cursor_timer, timerms);
  return 0;
}

bool
cursor_position_raw(int32_t *x, int32_t *y)
{
  int32_t fx, fy;

  if (!swc_cursor_position(&fx, &fy)) return false;
  *x = wl_fixed_to_int(fx);
  *y = wl_fixed_to_int(fy);
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
      *x = (int32_t)((*x - cx) / zoom) + cx;
      *y = (int32_t)((*y - cy) / zoom) + cy;
    }
  }

  return true;
}


