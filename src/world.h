#ifndef WORLD_H
#define WORLD_H

#include <string.h>

#define WORLD_MAX_ROWS 32
#define WORLD_MAX_COLS 32

struct world
{
	unsigned char tiles[WORLD_MAX_ROWS][WORLD_MAX_COLS];

	unsigned nrows;
	unsigned ncols;

	unsigned worker_row;
	unsigned worker_col;
};

enum {
	WORLD_TILE_WORKER = (1 << 0),
	WORLD_TILE_WALL = (1 << 1),
	WORLD_TILE_DOCK = (1 << 2),
	WORLD_TILE_BOX = (1 << 3),

	WORLD_TILE_INVALID = (1 << 4)
};

/*
 * Initializes the world.
 */
void world_init(struct world *world);

/*
 * Prints the world to stdout.
 */
void world_dump(struct world *world);

/*
 * Returns the tile at the specified position.
 *
 * The tile is a bit-field, set using WORLD_TILE_*. If the row or col
 * is out of range, the bit WORLD_TILE_INVALID is set.
 */
int world_get(struct world *world, int row, int col);

/*
 * Moves the worker to the specified row and col.
 *
 * Validation of the move should be done by the caller.
 */
void world_move_worker(struct world *world, int to_row, int to_col);

/*
 * Moves a box from specified position to another position.
 */
void world_push_box(struct world *world,
		    int from_row, int from_col,
		    int to_row, int to_col);

/*
 * Creates a copy of the world.
 */
void world_copy(struct world *from, struct world *to);
		
#endif
