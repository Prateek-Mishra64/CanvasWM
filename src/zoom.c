#include "zoom.h"
#include "canvas.h"
#include "host.h"


int
zoom_tick(void *data)
{
  (void)data;

  float current = host_get_zoom();
  float target = zoom.target;
  float diff = target - current;

  /* Stop if close enough */
  if (diff > -0.01f && diff < 0.01f) {
    host_set_zoom(target);
    return 0;
  }

  /* Ease toward target */
  float step = diff / 4.0f;
  if (step > 0 && step < 0.01f) step = 0.01f;
  if (step < 0 && step > -0.01f) step = -0.01f;

  host_set_zoom(current + step);

  /* Continue animation */
  wl_event_source_timer_update(zoom.timer, timerms);
  return 0;
}

void
zoom_in(void)
{
    zoom.target += zoom_step;

    if (zoom.target > 1.0f)
        zoom.target = 1.0f;

    if (!zoom.timer)
        zoom.timer =
            wl_event_loop_add_timer(compositor.evloop, zoom_tick, NULL);

    wl_event_source_timer_update(zoom.timer, timerms);
}

void
zoom_out(void)
{
    zoom.target -= zoom_step;

    if (zoom.target < zoom_min)
        zoom.target = zoom_min;

    if (!zoom.timer)
        zoom.timer =
            wl_event_loop_add_timer(compositor.evloop, zoom_tick, NULL);

    wl_event_source_timer_update(zoom.timer, timerms);
}

void
zoom_reset(void)
{
    zoom.target = 1.0f;

    if (!zoom.timer)
        zoom.timer =
            wl_event_loop_add_timer(compositor.evloop, zoom_tick, NULL);

    wl_event_source_timer_update(zoom.timer, timerms);
}
