#ifndef SDL_VIEW_H
#define SDL_VIEW_H

#include <SDL.h>

struct view {
	SDL_Surface *tile_surfaces[VIEW_TILE_MAX];
	SDL_Surface *window;
};

#endif
