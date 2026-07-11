#include "spawn.h"
#include "hevel.h"
#include "placement.h"
#include "../config.h"

struct spawn_request spawn = {0};

void
spawn_launch(const char *command,
             int width,
             int height);

void
spawn_request_prepare(const char *command,
    int width,
    int height);

void
spawn_execute(void);

void
spawn_request_prepare(const char *command, int width, int height)
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

}

void
spawn_execute(void)
{
    printf("[spawn_execute]\n");
    fflush(stdout);
    if (!spawn.pending)
        return;

    placement_compute();

    pid_t pid = fork();

    printf("fork returned %d\n", pid);

    if (pid > 0) {
        spawn.pid = pid;

        printf("saved spawn pid = %d\n", spawn.pid);
        return;
    }

    if (pid == 0) {
        printf("child executing %s\n", spawn.argv[0]);

        execvp(spawn.argv[0], spawn.argv);

        perror("execvp");
        _exit(127);
    }

    perror("fork");
}

void
spawn_launch(const char *command,
             int width,
             int height)
{
    printf("[spawn_launch] %s\n", command);
    fflush(stdout);
    spawn_request_prepare(command, width, height);
    spawn_execute();
}
