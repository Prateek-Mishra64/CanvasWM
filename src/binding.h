#ifndef BINDING_H
#define BINDING_H

#include <stdint.h>
#include "action.h"
#include <stdbool.h>

enum modifier {
    MOD_NONE  = 0,

    MOD_SUPER = 1 << 0,
    MOD_SHIFT = 1 << 1,
    MOD_CTRL  = 1 << 2,
    MOD_ALT   = 1 << 3,
};

enum binding_type {
    BIND_ACTION,
    BIND_EXEC,
};

enum trackpad_gesture {
    TRACKPAD_NONE,
    TRACKPAD_PINCH_IN,
    TRACKPAD_PINCH_OUT,
    TRACKPAD_THREE_LEFT,
    TRACKPAD_THREE_RIGHT,
    TRACKPAD_THREE_UP,
    TRACKPAD_THREE_DOWN,
};


enum bind_symbol {
    /* Keyboard */
     
    SYMBOL_NONE = 0,
    INPUT_KEY_PERIOD,
    INPUT_KEY_COMMA,
    INPUT_KEY_RETURN,
    INPUT_KEY_SPACE,
    INPUT_KEY_A,
    INPUT_KEY_B,
    INPUT_KEY_C,
    INPUT_KEY_D,
    INPUT_KEY_E,
    INPUT_KEY_F,
    INPUT_KEY_G,
    INPUT_KEY_H,
    INPUT_KEY_I,
    INPUT_KEY_J,
    INPUT_KEY_K,
    INPUT_KEY_L,
    INPUT_KEY_M,
    INPUT_KEY_N,
    INPUT_KEY_O,
    INPUT_KEY_P,
    INPUT_KEY_Q,
    INPUT_KEY_R,
    INPUT_KEY_S,
    INPUT_KEY_T,
    INPUT_KEY_U,
    INPUT_KEY_V,
    INPUT_KEY_W,
    INPUT_KEY_X,
    INPUT_KEY_Y,
    INPUT_KEY_Z,
    INPUT_KEY_0,
    INPUT_KEY_1,
    INPUT_KEY_2,
    INPUT_KEY_3,
    INPUT_KEY_4,
    INPUT_KEY_5,
    INPUT_KEY_6,
    INPUT_KEY_7,
    INPUT_KEY_8,
    INPUT_KEY_9,

    /* Mouse */
    MOUSE_LEFT_CLICK,
    MOUSE_LEFT_DRAG,
      
    MOUSE_RIGHT_CLICK,
    MOUSE_RIGHT_DRAG,

    MOUSE_MIDDLE_CLICK,
    MOUSE_MIDDLE_DRAG,

    MOUSE_LR_CHORD,

    /* Gestures */
    GESTURE_PINCH_IN,
    GESTURE_PINCH_OUT,
    GESTURE_THREE_LEFT,
    GESTURE_THREE_RIGHT,
    GESTURE_THREE_UP,
    GESTURE_THREE_DOWN,

    MOUSE_SCROLL_UP,
    MOUSE_SCROLL_DOWN,

    MOUSE_SCROLL_LEFT,
    MOUSE_SCROLL_RIGHT,
};



struct binding {
    enum binding_type type;

    enum modifier modifiers;
    enum bind_symbol symbol;

    union {
        enum action action;
        const char *command;
    };
};

bool
binding_dispatch(enum modifier modifiers,
                 enum bind_symbol symbol,
                 bool held);
#endif
