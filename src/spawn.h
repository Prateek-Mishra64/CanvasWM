#ifndef SPAWN_H
#define SPAWN_H

#include "hevel.h"
#define SPAWN_MAX_ARGS 32

struct spawn_request {
  char command[512];
  char *argv[SPAWN_MAX_ARGS];

  struct swc_rectangle geometry;

  pid_t pid;
  bool pending;

};

void
spawn_terminal_request(int width, int height);

void
spawn_request_init(const char *command,
    int width,
    int height
    );


void
spawn_execute(void);


extern struct spawn_request spawn;

#endif

