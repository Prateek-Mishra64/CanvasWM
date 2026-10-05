#include "spawn.h"
#include "lantern.h"
#include "placement.h"
#include "../config.h"
#include "host.h"

struct spawn_request spawn = {0};

void
spawn_launch(const char *command);

void
spawn_launch(const char *command)
{
    pid_t pid = fork();

    if (pid == 0) {
        execl("/bin/sh",
              "sh",
              "-c",
              command,
              (char *)NULL);

        perror("execl");
        _exit(127);
    }

    if (pid < 0)
        perror("fork");
}
