#ifndef SPAWN_H
#define SPAWN_H

#include "hevel.h"

struct spawn_request {
  char command[512];

  struct swc_rectangle geometry;

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

