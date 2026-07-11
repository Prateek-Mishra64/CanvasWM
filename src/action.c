#include "action.h"
#include "window.h"

#include "spawn.h"
#include "../config.h"

void
action_execute(enum action action)
{
    switch (action) {

    case ACTION_NONE:
        break;

    case ACTION_WINDOW_STICKY_TOGGLE:
        window_toggle_sticky();
        break;

    case ACTION_WINDOW_FULLSCREEN_TOGGLE:
        window_toggle_fullscreen();
        break;

    case ACTION_WINDOW_CLOSE:
        compositor_quit();
        break;

    case ACTION_QUIT:
        wl_display_terminate(compositor.display);
        break;

    default:
        break;
    }
}
