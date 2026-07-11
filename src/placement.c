#include "hevel.h"
#include "placement.h"
#include "spawn.h"

#define PLACEMENT_OFFSET 40
#define FULLSCREEN_SPAWN_OFFSET 40

static void
placement_compute_origin(struct swc_rectangle *origin)
{
    int screen_x = compositor.current_screen->swc->geometry.x;
    int screen_y = compositor.current_screen->swc->geometry.y;
    int screen_width = compositor.current_screen->swc->geometry.width;
    int screen_height = compositor.current_screen->swc->geometry.height;

    int center_x = screen_x + screen_width / 2;
    int center_y = screen_y + screen_height / 2;

    /* Default origin: screen center */
    origin->x = center_x - (int32_t)origin->width / 2;
    origin->y = center_y - (int32_t)origin->height / 2;

    /* If the focused window is fullscreen, move the origin beside it. */
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

    printf("\n========== ORIGIN ==========\n");
    printf("Origin : (%d,%d) %ux%u\n",
           origin->x,
           origin->y,
           origin->width,
           origin->height);

    if (compositor.focused) {
        printf("Focused: '%s'%s\n",
               compositor.focused->title
                   ? compositor.focused->title
                   : "<untitled>",
               swc_window_is_fullscreen(compositor.focused)
                   ? " [fullscreen]"
                   : "");
    }

    printf("============================\n");
}

static enum placement_policy
placement_decide(const struct swc_rectangle *origin,
                 struct window **occupant)
{
    struct window *w;
    struct swc_rectangle geometry;

    *occupant = NULL;

    printf("\n------ placement_decide ------\n");

    wl_list_for_each(w, &compositor.windows, link)
    {
        if (!swc_window_get_geometry(w->swc, &geometry))
            continue;

        printf("Window '%s'\n",
               w->swc->title ? w->swc->title : "<untitled>");

        printf("    Geometry : (%d,%d) %ux%u\n",
               geometry.x,
               geometry.y,
               geometry.width,
               geometry.height);

        /* Ignore the focused fullscreen window. */
        if (w->swc == compositor.focused &&
            swc_window_is_fullscreen(w->swc)) {

            printf("    -> skipped (focused fullscreen)\n");
            continue;
        }

        bool overlap =
            !(origin->x + (int32_t)origin->width <= geometry.x ||
              origin->x >= geometry.x + (int32_t)geometry.width ||
              origin->y + (int32_t)origin->height <= geometry.y ||
              origin->y >= geometry.y + (int32_t)geometry.height);

        printf("    Overlap : %s\n",
               overlap ? "YES" : "NO");

        if (overlap) {
            printf("    ==> CASCADE PARENT\n");

            *occupant = w;
            return PLACEMENT_CASCADE;
        }
    }

    printf("No overlapping window.\n");
    printf("------------------------------\n");

    return PLACEMENT_ORIGIN;
}

void
placement_compute(void)
{
    if (!compositor.current_screen)
        return;

    struct swc_rectangle candidate = spawn.geometry;

    placement_compute_origin(&candidate);

    printf("Candidate before policy : (%d,%d) %ux%u\n",
           candidate.x,
           candidate.y,
           candidate.width,
           candidate.height);

    struct window *occupant = NULL;

    switch (placement_decide(&candidate, &occupant)) {

    case PLACEMENT_ORIGIN:
        printf("Placement policy : ORIGIN\n");
        break;

    case PLACEMENT_CASCADE: {
        struct swc_rectangle geometry;

        printf("Placement policy : CASCADE\n");

        if (swc_window_get_geometry(occupant->swc, &geometry)) {
            candidate.x = geometry.x + PLACEMENT_OFFSET;
            candidate.y = geometry.y + PLACEMENT_OFFSET;

            printf("Cascade parent geometry : (%d,%d) %ux%u\n",
                   geometry.x,
                   geometry.y,
                   geometry.width,
                   geometry.height);
        }

        break;
    }

    }

    printf("Final spawn geometry : (%d,%d) %ux%u\n",
           candidate.x,
           candidate.y,
           candidate.width,
           candidate.height);

    printf("=========================================\n\n");

    spawn.geometry = candidate;
}
