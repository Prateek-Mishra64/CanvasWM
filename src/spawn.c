#include "spawn.h"
#include "hevel.h"

struct spawn_request spawn = {0};
void
spawn_request_init(const char *command, int width, int height)
{
  spawn.command = command;
  spawn.width = width;
  spawn.height = height;
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

