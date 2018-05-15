#ifndef GAME_ENGINE_H
#define GAME_ENGINE_H

#include "world.h"
#include <stdbool.h>

enum ge_dir {
	GE_DIR_UP,
	GE_DIR_DN,
	GE_DIR_LT,
	GE_DIR_RT,
};

/*
 * Moves the worker in the world, after validating the move.
 */
void ge_move(enum ge_dir dir, struct world *world);

/*
 * Returns true if the game is over, false otherwise.
 */
bool ge_is_game_over(struct world *world);

#endif
