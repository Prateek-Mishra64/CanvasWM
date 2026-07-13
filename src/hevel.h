#ifndef HEVEL_H
#define HEVEL_H

#define _POSIX_C_SOURCE 200809L

#include <inttypes.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wayland-server.h>
#include "input.h"

#ifdef __linux__
#include <linux/input-event-codes.h>
#else
#define BTN_LEFT 0x110
#define BTN_RIGHT 0x111
#define BTN_MIDDLE 0x112
#endif

#include <swc.h>
#include <xkbcommon/xkbcommon-keysyms.h>

#include "../config.h"
#include "nein_cursor.h"

#include "input.h"
#include "scroll.h"
#include "select.h"
#include "window.h"
#include "zoom.h"
#include "spawn.h"

typedef enum {
  MODE_NONE,
  MODE_KILL,
  MODE_SCROLL,
  MODE_MOVE,
  MODE_RESIZE,
  MODE_JUMP,
  MODE_SELECT,
} chord_mode;

struct window {
  struct swc_window *swc;
  struct wl_list link;

  pid_t pid;
  bool sticky;
};

struct screen {
  struct swc_screen *swc;
  struct wl_list link;
};

struct compositor_state {
  struct wl_display *display;
  struct wl_event_loop *evloop;
  struct wl_list windows;
  struct wl_list screens;
  struct screen *current_screen;
  struct swc_window *focused;
};
extern struct compositor_state compositor;

struct chord_state {
  chord_mode mode;
  bool left, middle, right;
  bool pending;
  bool forwarded;
  uint32_t button;
  uint32_t time;
  struct wl_event_source *timer;
};
extern struct chord_state chord;


struct zoom_state {
  float target;
  struct wl_event_source *timer;
};
extern struct zoom_state zoom;

struct sel_state {
  bool selecting;
  int32_t start_x, start_y;
  int32_t cur_x, cur_y;
  struct wl_event_source *timer;
};


//#############################################################################


extern struct sel_state sel;

extern bool focus_center;

extern const int timerms;

#endif
