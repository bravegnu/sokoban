#include "game-engine.h"
#include "world.h"

void ge_move(enum ge_dir dir, struct world *world)
{
	int next_row, next_col;
	int push_row, push_col;

	int next_tile;
	int push_tile;

	int row = world->worker_row;
	int col = world->worker_col;

	next_row = row;
	next_col = col;
	push_row = row;
	push_col = col;

	switch (dir) {
	case GE_DIR_UP:
		next_row -= 1;
		push_row -= 2;
		break;
	case GE_DIR_DN:
		next_row += 1;
		push_row += 2;
		break;
	case GE_DIR_RT:
		next_col += 1;
		push_col += 2;
		break;
	case GE_DIR_LT:
		next_col -= 1;
		push_col -= 2;
	}

	next_tile = world_get(world, next_row, next_col);
	if (next_tile & WORLD_TILE_INVALID)
		return;

	if (next_tile & WORLD_TILE_WALL)
		return;

	if (next_tile & WORLD_TILE_BOX) {
		push_tile = world_get(world, push_row, push_col);
		if (push_tile & WORLD_TILE_INVALID)
			return;

		if (push_tile & WORLD_TILE_WALL || push_tile & WORLD_TILE_BOX)
			return;

		world_push_box(world, next_row, next_col, push_row, push_col);
	}

	world_move_worker(world, next_row, next_col);
}

bool ge_is_game_over(struct world *world)
{
	int row, col;

	for (row = 0; row < WORLD_MAX_ROWS; row++) {
		for (col = 0; col < WORLD_MAX_COLS; col++) {
			int tile = world_get(world, row, col);

			if ((tile & WORLD_TILE_DOCK) && !(tile & WORLD_TILE_BOX))
				return false;
		}
	}

	return true;
}
