#include <stddef.h>
#include <stdlib.h>
#include "binding.h"
#include "hevel.h"
#include "spawn.h"

#include <xkbcommon/xkbcommon-keysyms.h>


static uint32_t
binding_modifires_to_backend(uint32_t modifiers)
{
    uint32_t swc_modifiers = 0;

    if (modifiers & MOD_SUPER)
        swc_modifiers |= SWC_MOD_LOGO;

    if (modifiers & MOD_SHIFT)
        swc_modifiers |= SWC_MOD_SHIFT;

    if (modifiers & MOD_CTRL)
        swc_modifiers |= SWC_MOD_CTRL;

    if (modifiers & MOD_ALT)
        swc_modifiers |= SWC_MOD_ALT;

    return swc_modifiers;
}

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

    action_execute(binding->action);
}


void
binding_initialize(void)
{
    for (size_t i = 0; i < LENGTH(bindings); ++i) {
        swc_add_binding(
            SWC_BINDING_KEY,
            binding_translate_modifiers(bindings[i].modifiers),
            bindings[i].key,
            binding_action,
            (void *)&bindings[i]);    
    }
}
