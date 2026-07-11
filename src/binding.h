#ifndef BINDING_H
#define BINDING_H

#include <linux/input-event-codes.h>
#include <xkbcommon/xkbcommon-keysyms.h>
#include "action.h"
#include "hevel.h"

struct binding_type {
  BIND_ACTION,
  BIND_EXEC,
};

struct binding {
    uint32_t modifiers;
    xkb_keysym_t key;

    enum binding_type type;

    union {

      enum action action;
      const char *command;
    };
};

void
binding_initialize(void);


#endif
