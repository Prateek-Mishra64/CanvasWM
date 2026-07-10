#include "spawn.h"
#include "hevel.h"
#include "placement.h"

struct spawn_request spawn = {0};
void
spawn_request_init(const char *command, int width, int height)
{
  snprintf(spawn.command, 
      sizeof(spawn.command), 
      "%s",
      command);
  spawn.geometry.width = width;
  spawn.geometry.height = height;

  placement_compute();
}

void
spawn_execute(void)
{
  pid_t pid;

  pid = fork();

  if(pid == 0) {
    execlp(term,
        term,
        term_flag,
        select_term_app_id,
        NULL);
    _exit(127);
  }
}

