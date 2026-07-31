#include "hevel.h"
#include "placement.h"
#include "window.h"
#include "viewport.h"

/*
 * Every window begins at the origin.
 *
 * The origin is the only fixed behaviour on the infinite canvas.
 *
 * Cascading is not a placement strategy. It is a safeguard that
 * prevents a newly spawned window from completely hiding another
 * window.
 */

#define PLACEMENT_OFFSET 40
#define FULLSCREEN_SPAWN_OFFSET 40

static void
placement_compute_origin(struct swc_rectangle *candidate)
{
    const struct canvas_origin *origin =
        viewport_origin();

    int center_x = origin->x;
    int center_y = origin->y;

    /* Default spawn origin: center of the current viewport. */
    candidate->x = origin->x - (int32_t)candidate->width / 2;

    candidate->y = origin->y - (int32_t)candidate->height / 2;

    /*
     * While a fullscreen window has focus, temporarily relocate the
     * origin beside it.
     */
    struct window *focused = NULL;
    struct window *w;

    wl_list_for_each(w, &compositor.windows, link) {
    if (w->swc == compositor.focused) {
        focused = w;
        break;
        }
    }

    if (focused && focused->fullscreen.enabled && focused->fullscreen.snapped) {
        struct swc_rectangle geometry;

        if (swc_window_get_geometry(compositor.focused, &geometry)) {
            candidate->x =
                geometry.x +
                (int32_t)geometry.width +
                FULLSCREEN_SPAWN_OFFSET;

            candidate->y = geometry.y;
        }
    }
}

static enum placement_policy
placement_decide(const struct swc_rectangle *candidate,
                 struct window **cascade_parent)
{
    struct window *w;
    struct swc_rectangle geometry;

    *cascade_parent = NULL;

    wl_list_for_each(w, &compositor.windows, link) {

        if (!swc_window_get_geometry(w->swc, &geometry))
            continue;

        /*
         * A focused fullscreen window only establishes the temporary
         * origin. It never becomes the cascade parent.
         */
        if (w->fullscreen.enabled)
            continue;

        bool fully_hidden =
            candidate->x <= geometry.x &&
            candidate->y <= geometry.y &&
            candidate->x + (int32_t)candidate->width >=
                geometry.x + (int32_t)geometry.width &&
            candidate->y + (int32_t)candidate->height >=
                geometry.y + (int32_t)geometry.height;

        if (!fully_hidden)
            continue;

        *cascade_parent = w;
        return PLACEMENT_CASCADE;
    }

    return PLACEMENT_ORIGIN;
}

void
placement_compute(struct swc_rectangle *geometry)
{
    if (!viewport_screen())
        return;
    
    struct swc_rectangle candidate = *geometry;

    placement_compute_origin(&candidate);

    struct window *cascade_parent = NULL;

    
    while (placement_decide(&candidate, &cascade_parent)
        == PLACEMENT_CASCADE) {

    struct swc_rectangle geometry;

    if (!cascade_parent)
        break;

    if (!swc_window_get_geometry(cascade_parent->swc, &geometry))
        break;

    candidate.x = geometry.x + PLACEMENT_OFFSET;
    candidate.y = geometry.y + PLACEMENT_OFFSET;
}


    *geometry = candidate;
}
