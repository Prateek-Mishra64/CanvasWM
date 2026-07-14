#ifndef BINDING_H
#define BINDING_H

#include <stdint.h>
#include "action.h"
#include "input.h"
#include <stdbool.h>

enum binding_type {
    BIND_ACTION,
    BIND_EXEC,
};

struct binding {
    enum binding_type type;

    enum modifier modifiers;
    enum input_symbol symbol;

    union {
        enum action action;
        const char *command;
    };
};



bool
binding_resolver(void);
#endif
