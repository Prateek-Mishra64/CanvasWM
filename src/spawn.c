#include "spawn.h"
#include "hevel.h"
#include "placement.h"
#include "../config.h"

struct spawn_request spawn = {0};
void
spawn_request_init(const char *command, int width, int height)
{
  memset(&spawn.geometry, 0, sizeof(spawn.geometry));
  snprintf(spawn.command, 
      sizeof(spawn.command), 
      "%s",
      command);
  spawn.argv[0] = spawn.command;
  spawn.argv[1] = NULL;
  spawn.geometry.width = width;
  spawn.geometry.height = height;

  spawn.pending = true;
  placement_compute();

}

void
spawn_execute(void)
{
  pid_t pid;
  pid = fork();

  printf("fork returned %d\n", pid);

  if (pid > 0) {
      spawn.pid = pid;
      printf("saved spawn pid = %d\n", spawn.pid);
  }

  if (pid == 0) {
      printf("child executing %s\n", spawn.argv[0]);
      execvp(spawn.argv[0], spawn.argv);

      perror("execvp");
      _exit(127);
}

}

void
spawn_terminal_request(int width, int height)
{
  spawn_request_init(term, width, height);
}

