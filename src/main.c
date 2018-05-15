#include "level-parser.h"
#include "game-engine.h"
#include "world.h"
#include "view.h"

#include <unistd.h>

#include <stdbool.h>
#include <error.h>
#include <errno.h>

static bool play_level(struct view *view, struct world *world_init)
{
	struct world world;

	world_copy(world_init, &world);
	view_new_level(view, &world);
	
	while (!ge_is_game_over(&world)) {
		enum view_key key = view_get_key(view);

		switch (key) {
		case VIEW_KEY_UP:
			ge_move(GE_DIR_UP, &world);
			break;
		case VIEW_KEY_DOWN:
			ge_move(GE_DIR_DN, &world);
			break;
		case VIEW_KEY_LEFT:
			ge_move(GE_DIR_LT, &world);
			break;
		case VIEW_KEY_RIGHT:
			ge_move(GE_DIR_RT, &world);
			break;
		case VIEW_KEY_SKIP:
			return true;
		case VIEW_KEY_RESTART:
			world_copy(world_init, &world);
			view_draw(view, &world);
			continue;
		case VIEW_KEY_QUIT:
			return false;
		}

		view_draw(view, &world);
	}
	
	return true;
}

static void panic(struct view *view, char *msg)
{
	view_cleanup(view);
	error(1, 0, "%s: see game.log for more details", msg);
}

int main()
{
	int ret;
	bool passed;
	struct world world;
	struct level_parser parser;
	struct view view;
	FILE *log_fp;

	log_fp = fopen("game.log", "w");
	if (log_fp == NULL)
		error(1, errno, "error opening game log file");
	
	log_set_fp(log_fp);

	passed = view_init(&view);
	if (!passed)
		panic(&view, "view initialization failed");
	
	lp_init(&parser, "levels.txt");

	passed = lp_open(&parser);
	if (!passed)
		panic(&view, "starting level parser failed");

	while (1) {
		passed = lp_next(&parser, &world);
		if (!passed)
			panic(&view, "parsing level failed");

		if (lp_stopped(&parser))
			break;

		if (!play_level(&view, &world))
			break;
		else
			sleep(1);
	}

	passed = lp_close(&parser);
	if (!passed)
		panic(&view, "closing level parser failed");

	view_cleanup(&view);

	return 0;
}
