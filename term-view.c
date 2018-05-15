#include "world.h"
#include "view.h"
#include "log.h"

#include <ncurses.h>

static struct {
	char ch;
	int color;
	bool bold;
} tile_map[] = {
	[VIEW_TILE_BOX] = { '@', COLOR_MAGENTA, false },
	[VIEW_TILE_BOX_DOCKED] = { '@', COLOR_MAGENTA, true },
	[VIEW_TILE_DOCK] = { '.', COLOR_WHITE, false },
	[VIEW_TILE_FLOOR] = { ' ', COLOR_BLACK, false },
	[VIEW_TILE_WALL] = { '#', COLOR_YELLOW, false },
	[VIEW_TILE_WORKER] = { '$', COLOR_YELLOW, true },
};

bool view_init(struct view *view)
{
	int i;
	WINDOW *win;
	
	win = initscr();
	if (win == NULL) {
		log_error("error initializing ncurses\n");
		return false;
	}
	
	raw();
	noecho();
	keypad(win, true);
	curs_set(0);

	if (has_colors() == FALSE) {
		log_error("terminal does not support colors\n");
		return false;
	}

	start_color();
	for (i = 0; i < VIEW_TILE_MAX; i++) {
		/* Pair numbering starts from 1. */
		init_pair(i + 1, tile_map[i].color, COLOR_BLACK);
	}
	
	return true;
}

void view_cleanup(struct view *view)
{
	endwin();
}

bool view_new_level(struct view *view, struct world *world)
{
	view_draw(view, world);
}

static void view_draw_tile(struct view *view, int row, int col, int tile)
{
	attron(COLOR_PAIR(tile + 1));
	if (tile_map[tile].bold)
		attron(A_BOLD);

	mvaddch(row, col, tile_map[tile].ch);

	attroff(COLOR_PAIR(tile + 1));
	attroff(A_BOLD);
}

void view_draw(struct view *view, struct world *world)
{
	int row;
	int col;

	clear();

	for (row = 0; row < WORLD_MAX_ROWS; row++) {
		for (col = 0; col < WORLD_MAX_COLS; col++) {
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
		}
	}

	refresh();
}

enum view_key view_get_key(struct view *view)
{
	int key = getch();

	switch (key) {
	case KEY_UP:
		return VIEW_KEY_UP;
	case KEY_DOWN:
		return VIEW_KEY_DOWN;
	case KEY_LEFT:
		return VIEW_KEY_LEFT;
	case KEY_RIGHT:
		return VIEW_KEY_RIGHT;
	case 's':
		return VIEW_KEY_SKIP;
	case 'r':
		return VIEW_KEY_RESTART;
	case 'q':
		return VIEW_KEY_QUIT;
	default:
		return VIEW_KEY_NONE;
	}
}
