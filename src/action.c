#include "action.h"

#include "spawn.h"
#include "../config.h"

void
action_execute(enum action action)
{
   printf("action %d\n", action);
   fflush(stdout);
  switch(action) {

    case ACTION_NONE:
      break;

    case ACTION_SPAWN:
      printf("action spawn %d\n", action);
      fflush(stdout);
      spawn_launch(term, 1000, 800);
      break;

    default:
      break;
  }
}
