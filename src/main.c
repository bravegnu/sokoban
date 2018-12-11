#include "level-parser.h"
#include "game-engine.h"
#include "world.h"
#include "view.h"
#include "sdl-view.h"
#include "term-view.h"

#include <unistd.h>

#include <stdbool.h>
#include <error.h>
#include <errno.h>

/*
 * Runs game loop for one level.
 */
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

/*
 * Terminates, with message.
 */
static void error_exit(char *msg)
{
	error(1, 0, "%s: see game.log for more details", msg);
}

/*
 * Destroys the UI and terminates, with message.
 */
static void panic(struct view *view, char *msg)
{
	view_cleanup(view);
	error_exit(msg);
}

int term_main()
{
	struct term_view term_view;
	bool passed;

	passed = term_view_init(&term_view);
	if (!passed)
		error_exit("view initialization failed");

	game_main(VIEW(&term_view));
}

int sdl_main()
{
	struct sdl_view sdl_view;
	bool passed;

	passed = sdl_view_init(&sdl_view);
	if (!passed)
		error_exit("view initialization failed");

	game_main(VIEW(&sdl_view));
}

int game_main(struct view *view)
{
	int ret;
	bool passed;
	struct world world;
	struct level_parser parser;
	FILE *log_fp;

	log_fp = fopen("game.log", "w");
	if (log_fp == NULL)
		error(1, errno, "error opening game log file");

	log_set_fp(log_fp);

	lp_init(&parser, "levels.txt");

	passed = lp_open(&parser);
	if (!passed)
		panic(view, "starting level parser failed");

	while (1) {
		passed = lp_next(&parser, &world);
		if (!passed)
			panic(view, "parsing level failed");

		if (lp_stopped(&parser))
			break;

		if (!play_level(view, &world))
			break;

		sleep(1);
	}

	passed = lp_close(&parser);
	if (!passed)
		panic(view, "closing level parser failed");

	view_cleanup(view);

	return 0;
}

int main(int argc, char *argv[])
{
	if (argc != 2) {
		fprintf(stderr, "Usage: sokoban sdl|term\n");
		return 1;
	}

	if (strcmp(argv[1], "sdl") == 0) {
		sdl_main();
	} else if (strcmp(argv[1], "term") == 0) {
		term_main();
	} else {
		fprintf(stderr, "Unsupported view: %s\n", argv[1]);
	}
}
