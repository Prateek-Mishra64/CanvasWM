#ifndef BINDING_H
#define BINDING_H

#include <stdint.h>
#include <linux/input-event-codes.h>
#include <xkbcommon/xkbcommon.h>
#include "action.h"

enum modifier {
    MOD_NONE  = 0,
    MOD_SUPER = 1 << 0,
    MOD_SHIFT = 1 << 1,
    MOD_CTRL  = 1 << 2,
    MOD_ALT   = 1 << 3,
};


struct binding {
    enum modifier modifiers;
    xkb_keysym_t key;
    enum action action;
};

void
binding_initialize(void);

#endif
