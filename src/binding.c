#include "binding.h"
#include "action.h"
#include "spawn.h"
#include "../config.h"
#include <stdbool.h>

bool
binding_dispatch(enum modifier modifiers,
                 enum bind_symbol symbol,
                 bool held)
{
    const struct binding *binding;

    for (size_t i = 0; i < LENGTH(bindings); ++i) {

        binding = &bindings[i];

        if (binding->modifiers != modifiers)
            continue;

        if (binding->symbol != symbol)
            continue;

        /*
         * Ignore held events for one-shot actions.
         * Navigation and drag actions will consume them.
         */

        switch (binding->type) {

        case BIND_ACTION:
            action_execute(binding->action, held);
            break;

        case BIND_EXEC:
            if (!held)
                spawn_launch(binding->command, 1000, 800);
            break;
        }

        return false;
    }
}
