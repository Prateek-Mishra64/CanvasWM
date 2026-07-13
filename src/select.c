#include "select.h"
#include "hevel.h"
#include "input.h"
#include "spawn.h"
#include "window.h"
#include "../config.h"

int
select_tick(void *data)
{
  int32_t x, y;

  (void)data;
  if (!sel.selecting) return 0;

  if (cursor_position(&x, &y)) {
    sel.cur_x = x;
    sel.cur_y = y;
    swc_overlay_set_box(sel.start_x, sel.start_y, x, y, select_box_color,
                        select_box_border);
  }

  wl_event_source_timer_update(sel.timer, timerms);
  return 0;
}

