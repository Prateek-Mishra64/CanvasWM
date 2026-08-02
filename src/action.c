#include "action.h"
#include "window.h"
#include "binding.h"
#include "input.h"
#include "viewport.h"
#include "zoom.h"
#include "host.h"



#include "spawn.h"
#include "../config.h"

void
action_execute(enum action action,
               bool held)
{
    switch (action) {

    case ACTION_VIEWPORT_NAVIGATE:
        if (held)
            viewport_begin_pan();
        break;


    case ACTION_VIEWPORT_LEFT:
        if (held)
            viewport_left();
        break;

    case ACTION_VIEWPORT_RIGHT:
        if (held)
            viewport_right();
        break;

    case ACTION_VIEWPORT_UP:
        if (held)
            viewport_top();
        break;

    case ACTION_VIEWPORT_DOWN:
        if (held)
            viewport_down();
        break;

    case ACTION_WINDOW_MOVE:
        if (held)
            window_move_begin();
        break;

    case ACTION_WINDOW_RESIZE:
        if (held)
            window_resize();
        break;

    case ACTION_WINDOW_CLOSE:
        if (!held)
            window_close();
        break;

    case ACTION_WINDOW_FULLSCREEN_TOGGLE:
        if (!held)
            window_toggle_fullscreen();
        break;

    case ACTION_WINDOW_STICKY_TOGGLE:
        if (!held)
            window_toggle_sticky();
        break;

    case ACTION_WINDOW_JUMP: {
        if (!held) {
          int32_t x = input.cursor.x;
          int32_t y = input.cursor.y;

          window_jump(window_nearest(x, y));        }
        break;
      }

//    case ACTION_FOCUS_NEXT:
  //      if (!held)
    //        focus_next();
      //  break;

//    case ACTION_FOCUS_PREVIOUS:
  //      if (!held)
    //        focus_previous();
      //  break;

    case ACTION_ZOOM_IN:
        if (!held)
            zoom_in();
        break;

    case ACTION_ZOOM_OUT:
        if (!held)
            zoom_out();
        break;

    case ACTION_ZOOM_RESET:
        if (!held)
            zoom_reset();
        break;

    case ACTION_QUIT:
        if (!held)
            wl_display_terminate(compositor.display);
        break;

    default:
        break;
    }
}
