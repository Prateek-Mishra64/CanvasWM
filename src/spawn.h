#ifndef SPAWN_H
#define SPAWN_H

#include "canvas.h"
#define SPAWN_MAX_ARGS 32

struct spawn_request {
  char command[512];
  char *argv[SPAWN_MAX_ARGS];

  struct swc_rectangle geometry;

  pid_t pid;
  bool pending;

};


void
spawn_launch(const char *command);


extern struct spawn_request spawn;

#endif

