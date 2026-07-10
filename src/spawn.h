#ifndef SPAWN_H
#define SPAWN_H

#include "hevel.h"

struct spawn_request {
  char command[512];

  int x;
  int y;

  int width;
  int height;

  pid_t pid;
  bool pending;

};

void
spawn_request_init(const char *command,
    int width,
    int height
    );


void
spawn_execute(void);


extern struct spawn_request spawn;

#endif

