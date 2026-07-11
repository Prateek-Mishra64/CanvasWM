#include "action.h"

#include "spawn.h"
#include "../config.h"

void
action_execute(enum action action)
{
  switch(action) {

    case ACTION_NONE:
      break;

    case ACTION_SPAWN:
      spawn_launch(term, 1000, 800);
      break;

    default:
      break;
  }
}
