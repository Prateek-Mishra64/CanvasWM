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

static bool
placement_overlaps(const struct swc_rectangle *candidate);


static bool
placement_overlaps(const struct swc_rectangle *candidate) 
{
  struct window *w;
  struct swc_rectangle geometry;
  
  wl_list_for_each(w, &compositor.windows, link)
  {

  if(!swc_window_get_geometry(w->swc, &geometry))
    continue;
// this is the conversion to make data stable and to remove redundancy
  int32_t candidate_right =
    candidate->x + (int32_t) candidate->width;
  int32_t candidate_bottom =
    candidate->y + (int32_t) candidate->height;

  int32_t geometry_right =
    geometry.x + (int32_t) geometry.width;
  int32_t geometry_bottom =
    geometry.y + (int32_t) geometry.height;

  
  if (!(candidate_right <= geometry.x ||
      candidate->x >= geometry_right ||
      candidate_bottom <= geometry.y ||
      candidate->y >= geometry_bottom)) {
    return true;
}
}


  return false;

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

  for (size_t i = 0;
      i < sizeof(placement_offsets) / sizeof(placement_offsets[0]);
      i++) {
    candidate.x = center_x - candidate.width / 2 + placement_offsets[i].dx;
    candidate.y = center_y - candidate.height / 2 + placement_offsets[i].dy;
  
  if (!placement_overlaps(&candidate)) {
    spawn.geometry = candidate;
    return;
  }

}

  candidate.x = center_x - candidate.width / 2;
  candidate.y = center_y - candidate.height / 2;

  spawn.geometry = candidate;


}


