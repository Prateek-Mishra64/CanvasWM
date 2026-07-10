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

