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
  spawn.argv[0] = spawn.command;
  spawn.argv[1] = NULL;
  spawn.geometry.width = width;
  spawn.geometry.height = height;

  placement_compute();

  spawn.pending = true;
}

void
spawn_execute(void)
{
  pid_t pid;

  pid = fork();
  printf("spawn pid = %d\n", spawn.pid);
  fflush(stdout);
  if (pid > 0)
    spawn.pid = pid;

  if(pid == 0) {
    execvp(spawn.argv[0], spawn.argv);
    _exit(127);
  }
}

