#include "binding.h"
#include "action.h"
#include "spawn.h"
#include "input.h"
#include "../config.h"
#include <stdbool.h>


bool
binding_resolver(void)
{
    const struct binding *binding;

    for (size_t i = 0; i < LENGTH(bindings); ++i) {

        binding = &bindings[i];

        if (binding->modifiers != input.modifiers)
            continue;

        if (binding->symbol != input.symbol)
            continue;

        /*
         * Ignore held events for one-shot actions.
         * Navigation and drag actions will consume them.
         */

        switch (binding->type) {

        case BIND_ACTION:
            action_execute(binding->action, input.held);
            break;

        case BIND_EXEC:
            if (!input.held)
                spawn_launch(binding->command, 1000, 800);
            break;
        }

        return true;
    }
    return false;

}

