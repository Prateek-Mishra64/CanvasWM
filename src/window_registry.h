#ifdef WINDOW_REGISTRY_H
#define WINDOW_REGISTRY_H

#include <stdint.h>

struct swc_window;

struct canvas_frame {
  uint32_t canvas_id;

  char title[256];

  int x;
  int y;

  int width;
  int height;
  
};

void window_registry_init(void);

struct canvas_frame * 
window_registry_create(struct swc_window *backend);

void
window_registry_destroy(struct swc_window *window);

struct canvas_frame *
window_registry_find(uint32_t, canvas_id);


#endif // WINDOW_REGISTRY_H
