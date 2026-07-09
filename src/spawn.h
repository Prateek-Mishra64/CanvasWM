#ifndef SPAWN_H
#define SPAWN_H



struct spawn_request {
  const char *command;

  int width;
  int height;

};

void
spawn_request_init(const char *command,
    int width,
    int height);

void
spawn_execute(void);


extern struct spawn_request spawn;

#endif

