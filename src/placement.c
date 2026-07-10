#include "hevel.h"
#include "placement.h"
#include "spawn.h"


void
placement_reserve(void)
{
  if (!compositor.current_screen)
    return;
  spawn.x = compositor.current_screen->swc->geometry.x + compositor.current_screen->swc->geometry.width / 2;
  spawn.y = compositor.current_screen->swc->geometry.y + compositor.current_screen->swc->geometry.width / 2;

}

