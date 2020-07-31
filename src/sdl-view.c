#include "world.h"
#include "view.h"
#include "log.h"

#include <assert.h>

#include <SDL.h>

#define TILE_SIZE 32

static char *tile_files[] = {
	"tiles/box.bmp",
	"tiles/box-docked.bmp",
	"tiles/dock.bmp",
	"tiles/floor.bmp",
	"tiles/wall.bmp",
	"tiles/worker.bmp",
};

bool view_init(struct view *view)
{
	int i;
	int ret;

	ret = SDL_Init(SDL_INIT_VIDEO);
	if (ret == -1) {
		log_error("initializing SDL failed: %s", SDL_GetError());
		return false;
	}

	for (i = 0; i < VIEW_TILE_MAX; i++) {
		view->tile_surfaces[i] = SDL_LoadBMP(tile_files[i]);
		if (view->tile_surfaces[i] == NULL) {
			log_error("loading bitmap '%s' failed: %s",
				  tile_files[i], SDL_GetError());
			return false;
		}
	}

	view->window = NULL;

	return true;
}

void view_cleanup(struct view *view)
{
	if (view->window != NULL)
		SDL_FreeSurface(view->window);

	SDL_Quit();
}

/*
 * Draws a tile on to the screen.
 */
static void view_draw_tile(struct view *view, int row, int col, int tile)
{
	SDL_Rect window_rect;
	int ret;

	window_rect.x = col * TILE_SIZE;
	window_rect.y = row * TILE_SIZE;
	window_rect.w = TILE_SIZE;
	window_rect.h = TILE_SIZE;

	ret = SDL_BlitSurface(view->tile_surfaces[tile],
			      NULL,
			      view->window,
			      &window_rect);
	assert(ret != -1);
}

bool view_new_level(struct view *view, struct world *world)
{
	if (view->window != NULL)
		SDL_FreeSurface(view->window);

	view->window = SDL_SetVideoMode(world->ncols * TILE_SIZE,
					 world->nrows * TILE_SIZE,
					 32,
					 SDL_SWSURFACE);
	if (view->window == NULL) {
		log_error("setting video mode failed: %s", SDL_GetError());
		return false;
	}

	SDL_WM_SetCaption("Sokoban!", NULL);

	view_draw(view, world);

	return true;
}

void view_draw(struct view *view, struct world *world)
{
	int row;
	int col;
	int ret;

	ret = SDL_FillRect(view->window, NULL,
			   SDL_MapRGB(view->window->format, 0, 0, 0));
	assert(ret != -1);

	for (row = 0; row < world->nrows; row++) {
		for (col = 0; col < world->ncols; col++) {
			int tile = world_get(world, row, col);

			if (tile & WORLD_TILE_WALL)
				view_draw_tile(view, row, col, VIEW_TILE_WALL);

			else if (tile & WORLD_TILE_WORKER)
				view_draw_tile(view, row, col, VIEW_TILE_WORKER);

			else if ((tile & WORLD_TILE_BOX) && (tile & WORLD_TILE_DOCK))
				view_draw_tile(view, row, col, VIEW_TILE_BOX_DOCKED);

			else if (tile & WORLD_TILE_BOX)
				view_draw_tile(view, row, col, VIEW_TILE_BOX);

			else if (tile & WORLD_TILE_DOCK)
				view_draw_tile(view, row, col, VIEW_TILE_DOCK);

			else
				view_draw_tile(view, row, col, VIEW_TILE_FLOOR);
		}
	}

	SDL_UpdateRect(view->window, 0, 0, 0, 0);
}

enum view_key view_get_key(struct view *view)
{
	SDL_Event event;
	SDL_KeyboardEvent *key_event;
	int ret;

	ret = SDL_WaitEvent(&event);
	if (ret == 0) {
		log_error("waiting for key event failed: %s", SDL_GetError());
		return VIEW_KEY_NONE;
	}

	if (event.type != SDL_KEYUP)
		return VIEW_KEY_NONE;

	key_event = &event.key;

	switch (key_event->keysym.sym) {
	case SDLK_UP:
		return VIEW_KEY_UP;
	case SDLK_DOWN:
		return VIEW_KEY_DOWN;
	case SDLK_LEFT:
		return VIEW_KEY_LEFT;
	case SDLK_RIGHT:
		return VIEW_KEY_RIGHT;
	case SDLK_s:
		return VIEW_KEY_SKIP;
	case SDLK_r:
		return VIEW_KEY_RESTART;
	case SDLK_q:
		return VIEW_KEY_QUIT;
	default:
		return VIEW_KEY_NONE;
	}
}
