#include <stddef.h>
#include <stdlib.h>
#include "binding.h"
#include "../config.h"

#include <linux/input-event-codes.h>
#include <xkbcommon/xkbcommon-keysyms.h>

#define MOD_SUPER SWC_MOD_LOGO


static void
binding_action(void *data,
               uint32_t time,
               uint32_t value,
               uint32_t state)
{
    (void)time;
    (void)value;
    (void)state;

    const struct binding *binding = data; 

    switch (binding->type) {
      case BIND_ACTION:
        action_execute(binding-> action);
        break;
      case BIND_EXEC:
        spawn_launch(binding->command, 1000, 800);
        break;
    }
}

void
binding_initialize(void)
{
    size_t i;

    for (i = 0; i < LENGTH(bindings); ++i) {

        swc_add_binding(
            SWC_BINDING_KEY,
            bindings[i].modifiers,
            bindings[i].key,
            binding_action,
            (void *)&bindings[i]);
    }
}

