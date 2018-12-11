#ifndef SDL_VIEW_H
#define SDL_VIEW_H

#include <SDL.h>

#define SDL_VIEW(v) ((struct sdl_view *)(v))

struct sdl_view {
	/* Parent */
	struct view base;
	
	SDL_Surface *tile_surfaces[VIEW_TILE_MAX];
	SDL_Surface *window;
};

bool sdl_view_init(struct sdl_view *sdl_view);

#endif
