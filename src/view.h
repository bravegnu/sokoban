#ifndef VIEW_H
#define VIEW_H

#include <stdbool.h>

#define VIEW(v) ((struct view *)(v))

struct view {
	void (*cleanup)(struct view *view);
	void (*draw)(struct view *view, struct world *world);
	enum view_key (*get_key)(struct view *view);
	bool (*new_level)(struct view *view, struct world *world);
};

enum {
	VIEW_TILE_BOX,
	VIEW_TILE_BOX_DOCKED,
	VIEW_TILE_DOCK,
	VIEW_TILE_FLOOR,
	VIEW_TILE_WALL,
	VIEW_TILE_WORKER,

	VIEW_TILE_MAX
};

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
 * Destorys the view.
 */
static inline void view_cleanup(struct view *view)
{
	view->cleanup(view);
}

/*
 * Draws the world, on the screen.
 */
static inline void view_draw(struct view *view, struct world *world)
{
	view->draw(view, world);
}

/*
 * Waits for input from user and returns a enum view_key.
 *
 * In an invalid key is pressed VIEW_KEY_NONE is returned.
 */
static inline enum view_key view_get_key(struct view *view)
{
	return view->get_key(view);
}

/*
 * Initializes the screen for a new level.
 *
 * Return true on success and false on failure.
 */
static inline bool view_new_level(struct view *view, struct world *world)
{
	return view->new_level(view, world);
}

#endif
