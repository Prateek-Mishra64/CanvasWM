#ifndef CONFIG_H
#define CONFIG_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "src/binding.h"
#include "src/input.h"
#include "src/action.h"
#define LENGTH(x) (sizeof(x) / sizeof((x)[0]))

#define ACTION_BIND(modifier, bind, act) \
{                                        \
    .modifiers = (modifier),             \
    .symbol = (bind),                    \
    .type = BIND_ACTION,                 \
    .action = (act),                     \
}

#define EXEC_BIND(modifier, bind, cmd) \
{                                      \
    .modifiers = (modifier),           \
    .symbol = (bind),                  \
    .type = BIND_EXEC,                 \
    .command = (cmd),                  \
}

static const float zoom_step = 0.15f;
static const float zoom_min  = 0.25f;
static const float zoom_max  = 4.0f;

static const uint32_t background_color = 0xff242933;

static const uint32_t outer_border_color_inactive = 0xffffffea;
static const uint32_t inner_border_color_inactive = 0xffddbd8c;

static const uint32_t outer_border_color_active = 0xffffffea;
static const uint32_t inner_border_color_active = 0xffc99043;

static const uint32_t outer_border_width = 2;
static const uint32_t inner_border_width = 1;

static const uint32_t select_box_color = 0xffffffff;
static const uint32_t select_box_border = 2;
/*############################# EDIT CANVAS SYSTEM BINDS HERE #############################################*/
static const struct binding bindings[] = {

    ACTION_BIND(MOD_SUPER, INPUT_KEY_X, ACTION_WINDOW_FULLSCREEN_TOGGLE),
    ACTION_BIND(MOD_SUPER, INPUT_KEY_COMMA, ACTION_WINDOW_STICKY_TOGGLE),

    /* Viewport navigation */
    ACTION_BIND(MOD_SUPER, INPUT_KEY_J, ACTION_VIEWPORT_LEFT),
    ACTION_BIND(MOD_SUPER, INPUT_KEY_K, ACTION_VIEWPORT_RIGHT),
    ACTION_BIND(MOD_SUPER, INPUT_KEY_I, ACTION_VIEWPORT_UP),
    ACTION_BIND(MOD_SUPER, INPUT_KEY_M, ACTION_VIEWPORT_DOWN),

    /* Window jump */
    ACTION_BIND(MOD_SUPER, INPUT_KEY_O, ACTION_WINDOW_JUMP),

    /* Zoom */
    ACTION_BIND(MOD_SUPER, MOUSE_SCROLL_UP, ACTION_ZOOM_IN),
    ACTION_BIND(MOD_SUPER, MOUSE_SCROLL_DOWN, ACTION_ZOOM_OUT),

    /* Mouse */
    ACTION_BIND(MOD_SUPER, MOUSE_LEFT_DRAG, ACTION_VIEWPORT_NAVIGATE),
    ACTION_BIND(MOD_SUPER | MOD_SHIFT,
                MOUSE_LEFT_DRAG,
                ACTION_WINDOW_MOVE),

    ACTION_BIND(MOD_SUPER, MOUSE_RIGHT_DRAG, ACTION_WINDOW_RESIZE),

    ACTION_BIND(MOD_SUPER | MOD_SHIFT,
                INPUT_KEY_Q,
                ACTION_QUIT),

    EXEC_BIND(MOD_SUPER, INPUT_KEY_T, "kitty"),
    EXEC_BIND(MOD_SUPER, INPUT_KEY_F, "librewolf"),
    EXEC_BIND(MOD_SUPER, INPUT_KEY_G, "nautilus --new-window"),
    EXEC_BIND(MOD_SUPER, INPUT_KEY_SPACE, "rofi -show drun"),

}; 

/*
 * - "swc"  : use swc's built-in cursor, client cursors allowed, no per-chord
 * cursor
 * - "nein" : use the plan 9 cursor set, client cursors blocked, per chord
 * cursors
 */
static const char *const cursor_theme = "swc";

/* recommended st-wl/hst (stock st-wl has some issues) or havoc
 * but anything will work just fine */
static const char *const term = "kitty";

/* a flag for your terminal emulator to setup a windowid
 * - for st-wl: -w
 * - for havoc: -i
 * - for everything else: idk
 */

/* gui programs take over the geometry of the terminal, broken for xwayland */

/* define a list of terminals that you use */

static const int chord_click_timeout_ms = 250;

static const int32_t move_scroll_edge_threshold = 80;
static const int32_t move_scroll_speed = 16;
static const float move_ease_factor = 0.30f;

static const int timerms = 16;

static const int scrollpx = 64;
static const int scrollease = 4;
static const int scrollcap = 64;

/* scroll chord mode:
 * - true  : drag mouse to scroll in any direction
 * - false : use scroll wheel for vertical scrolling only
 */
static const bool scroll_drag_mode = true;

/* enable zoom feature:
 * - when enabled: scroll wheel controls zoom when in drag scroll mode
 * broken for multiple monitors
 */
static const bool enable_zoom = true;

/* whether or not to center the window.
 * in drag mode, it centers on both axis
 * otherwise on the vertical axis
 */
static const bool center_focus = true;

/* customizable 2-1 chord
 * avaliable options:
 * - sticky: make window not move when scroll
 * - fullscreen: make a window take entire screen
 * - jump: switch focus to the closest window
 */
static const char *const custom_chord = "fullscreen";

#endif
