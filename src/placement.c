#include "hevel.h"
#include "placement.h"
#include "spawn.h"

#define PLACEMENT_OFFSET 40
#define FULLSCREEN_SPAWN_OFFSET 40

static enum placement_policy
placement_decide(const struct swc_rectangle *origin,
                 struct window **occupant)
{
    struct window *w;
    struct swc_rectangle geometry;

    *occupant = NULL;

    wl_list_for_each(w, &compositor.windows, link) {

        if (!swc_window_get_geometry(w->swc, &geometry))
            continue;

        /*
         * The focused fullscreen window only establishes the temporary
         * origin. It should never become the cascade parent.
         */
        if (w->swc == compositor.focused &&
            swc_window_is_fullscreen(w->swc))
            continue;

        /* Does this window occupy the current origin? */
        if (!(origin->x + (int32_t)origin->width <= geometry.x ||
              origin->x >= geometry.x + (int32_t)geometry.width ||
              origin->y + (int32_t)origin->height <= geometry.y ||
              origin->y >= geometry.y + (int32_t)geometry.height)) {

            *occupant = w;
            return PLACEMENT_CASCADE;
        }
    }

    return PLACEMENT_ORIGIN;
}


static void
placement_compute_origin(struct swc_rectangle *origin)
{
    int screen_x;
    int screen_y;
    int screen_width;
    int screen_height;

    int center_x;
    int center_y;

    screen_x = compositor.current_screen->swc->geometry.x;
    screen_y = compositor.current_screen->swc->geometry.y;
    screen_width = compositor.current_screen->swc->geometry.width;
    screen_height = compositor.current_screen->swc->geometry.height;

    center_x = screen_x + screen_width / 2;
    center_y = screen_y + screen_height / 2;

    origin->x = center_x - (int32_t)origin->width / 2;
    origin->y = center_y - (int32_t)origin->height / 2;

    /*
     * If the focused window is fullscreen, temporarily move the origin
     * beside it.
     */
    if (compositor.focused &&
        swc_window_is_fullscreen(compositor.focused)) {

        struct swc_rectangle geometry;

        if (swc_window_get_geometry(compositor.focused, &geometry)) {
            origin->x =
                geometry.x +
                (int32_t)geometry.width +
                FULLSCREEN_SPAWN_OFFSET;

            origin->y = geometry.y;
        }
    }
}


void
placement_compute(void)
{
    if (!compositor.current_screen)
        return;

    struct swc_rectangle candidate = spawn.geometry;

    placement_compute_origin(&candidate);

    struct window *occupant = NULL;

    switch (placement_decide(&candidate, &occupant)) {

    case PLACEMENT_ORIGIN:
        break;

    case PLACEMENT_CASCADE: {
        struct swc_rectangle geometry;

        if (swc_window_get_geometry(occupant->swc, &geometry)) {
            candidate.x = geometry.x + PLACEMENT_OFFSET;
            candidate.y = geometry.y + PLACEMENT_OFFSET;
        }

        break;
    }

    }

    spawn.geometry = candidate;
}
