#ifndef VIEW_H
#define VIEW_H

enum {
	VIEW_TILE_BOX,
	VIEW_TILE_BOX_DOCKED,
	VIEW_TILE_DOCK,
	VIEW_TILE_FLOOR,
	VIEW_TILE_WALL,
	VIEW_TILE_WORKER,

	VIEW_TILE_MAX
};

#if defined(CONFIG_SDL)
#include "sdl-view.h"
#elif defined(CONFIG_NCURSES)
#include "term-view.h"
#endif

#include <stdbool.h>

enum view_key {
	VIEW_KEY_UP,
	VIEW_KEY_DOWN,
	VIEW_KEY_LEFT,
	VIEW_KEY_RIGHT,
	VIEW_KEY_QUIT,
	VIEW_KEY_SKIP,
	VIEW_KEY_RESTART,
	VIEW_KEY_NONE
};

/*
 * Initializes the view.
 *
 * Returns true on success and false on failure.
 */
bool view_init(struct view *view);

/*
 * Destorys the view.
 */
void view_cleanup(struct view *view);

/*
 * Draws the world, on the screen.
 */
void view_draw(struct view *view, struct world *world);

/*
 * Waits for input from user and returns a enum view_key.
 *
 * In an invalid key is pressed VIEW_KEY_NONE is returned.
 */
enum view_key view_get_key(struct view *view);

/*
 * Initializes the screen for a new level.
 *
 * Return true on success and false on failure.
 */
bool view_new_level(struct view *view, struct world *world);

#endif
