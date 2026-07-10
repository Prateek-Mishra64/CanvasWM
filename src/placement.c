#include "hevel.h"
#include "placement.h"
#include "spawn.h"


#define PLACEMENT_OFFSET 40
#define PLACEMENT_MAX_TRIES 20

static const struct {
  int dx;
  int dy;
} placement_offsets[] = {
  {0, 0},
  {40, 40},
  {-40, -40},
  {40, -40},
  {-40, 40},
  {80, 80},
  {-80, -80},
  {80, -80},
  {-80, 80},
  {120, 120},
  {120, -120},
  {-120, 120},

};

static struct window *
placement_origin_occupant(const struct swc_rectangle *origin)
{
    struct window *w;
    struct swc_rectangle geometry;

    wl_list_for_each(w, &compositor.windows, link)
    {
        if (!swc_window_get_geometry(w->swc, &geometry))
            continue;

        /* Does this window cover the origin? */
        if (!(origin->x + (int32_t)origin->width  <= geometry.x ||
              origin->x >= geometry.x + (int32_t)geometry.width ||
              origin->y + (int32_t)origin->height <= geometry.y ||
              origin->y >= geometry.y + (int32_t)geometry.height))
        {
            return w;
        }
    }

    return NULL;
}

void
placement_compute(void)
{
  int screen_x;
  int screen_y;
  int screen_width;
  int screen_height;

  int center_x;
  int center_y;

  if (!compositor.current_screen)
    return;

  screen_x = compositor.current_screen->swc->geometry.x;
  screen_y = compositor.current_screen->swc->geometry.y;
  screen_width = compositor.current_screen->swc->geometry.width;
  screen_height = compositor.current_screen->swc->geometry.height;

  center_x = screen_x + screen_width / 2;
  center_y = screen_y + screen_height / 2;
  
  struct swc_rectangle candidate = spawn.geometry;

  candidate.x = center_x - candidate.width / 2;
  candidate.y = center_y - candidate.height / 2;

  struct window *occupant = placement_origin_occupant(&candidate);

    if (occupant) {
      struct swc_rectangle geom;

      if (swc_window_get_geometry(occupant->swc, &geom)) {
          candidate.x = geom.x + 40;
          candidate.y = geom.y + 40;
      }
  }

  spawn.geometry = candidate;
}


