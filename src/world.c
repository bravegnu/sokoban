#include "world.h"

void world_init(struct world *world)
{
	int row;
	int col;

	world->nrows = 0;
	world->ncols = 0;

	world->worker_row = 0;
	world->worker_col = 0;

	for (row = 0; row < WORLD_MAX_ROWS; row++)
		for (col = 0; col < WORLD_MAX_COLS; col++)
			world->tiles[row][col] = 0;
}

void world_dump(struct world *world)
{
	int row;
	int col;

	for (row = 0; row < world->nrows; row++) {
		for (col = 0; col < world->ncols; col++) {
			char tile = world->tiles[row][col];

			if (tile & WORLD_TILE_WALL)
				putchar('#');

			else if (tile & WORLD_TILE_WORKER)
				putchar('@');

			else if (tile & WORLD_TILE_DOCK)
				putchar('.');

			else if (tile & WORLD_TILE_BOX)
				putchar('$');

			else
				putchar(' ');
		}
		putchar('\n');
	}
	putchar('\n');
}

int world_get(struct world *world, int row, int col)
{
	if (row >= WORLD_MAX_ROWS || col >= WORLD_MAX_COLS)
		return WORLD_TILE_INVALID;

	return world->tiles[row][col];
}

void world_move_worker(struct world *world, int to_row, int to_col)
{
	world->tiles[world->worker_row][world->worker_col] &= ~WORLD_TILE_WORKER;

	world->worker_row = to_row;
	world->worker_col = to_col;
	world->tiles[to_row][to_col] |= WORLD_TILE_WORKER;
}

void world_push_box(struct world *world,
		    int from_row, int from_col,
		    int to_row, int to_col)
{
	world->tiles[from_row][from_col] &= ~WORLD_TILE_BOX;
	world->tiles[to_row][to_col] |= WORLD_TILE_BOX;
}

void world_copy(struct world *from, struct world *to)
{
	memcpy(to, from, sizeof(struct world));
}
