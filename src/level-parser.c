#include "level-parser.h"
#include "log.h"

#include <stdio.h>
#include <error.h>
#include <errno.h>
#include <stdbool.h>
#include <string.h>

void lp_init(struct level_parser *parser, const char *filename)
{
	parser->filename = filename;
}

/*
 * Returns true if level is consistent, false otherwise.
 */
static bool lp_level_is_valid(struct level_parser *parser)
{
	if (parser->worker_count != 1)
		return false;

	if (parser->box_count != parser->dock_count)
		return false;

	return true;
}

/*
 * Processes a col in the current row.
 *
 * Updates parser and world state accordingly. Returns true if the
 * parsing was successful, false otherwise.
 */
static bool lp_parse_col(struct level_parser *parser, char ch, struct world *world)
{
	char *tile;
	bool valid;

	if (parser->curr_row >= WORLD_MAX_ROWS
	    || parser->curr_col >= WORLD_MAX_COLS) {
		log_warn("too large: supported level rows: %d, cols: %d",
			  WORLD_MAX_ROWS, WORLD_MAX_COLS);
		return false;
	}

	tile = &world->tiles[parser->curr_row][parser->curr_col];
	valid = false;

	if (ch == '#') {
		*tile |= WORLD_TILE_WALL;
		valid = true;
	}

	if (ch == '@' || ch == '+') {
		*tile |= WORLD_TILE_WORKER;
		parser->worker_count++;
		world->worker_row = parser->curr_row;
		world->worker_col = parser->curr_col;
		valid = true;
	}

	if (ch == '.' || ch == '*' || ch == '+') {
		*tile |= WORLD_TILE_DOCK;
		parser->dock_count++;
		valid = true;
	}

	if (ch == '$' || ch == '*') {
		*tile |= WORLD_TILE_BOX;
		parser->box_count++;
		valid = true;
	}

	if (ch == ' ')
		valid = true;

	if (!valid) {
		log_warn("invalid char '%c' in level", ch);
		return false;
	}

	return true;
}

static bool is_end_of_level(char *line)
{
	return line[0] == '\n';
}

static bool is_end_of_row(char ch)
{
	return ch == '\n';
}

static bool is_comment(char *line)
{
	return line[0] == ';';
}

/*
 * Process a row, within a level.
 *
 * Updates parser and world state accordingly. Returns true if the
 * parsing was successful, false otherwise.
 */
static bool lp_parse_row(struct level_parser *parser, char *line, struct world *world)
{
	int i;

	for (i = 0; line[i] != '\0'; i++) {
		bool passed;
		
		if (is_end_of_row(line[i])) {
			if (parser->curr_col > world->ncols)
				world->ncols = parser->curr_col;

			parser->curr_col = 0;
			parser->curr_row++;
			return true;
		}

		passed = lp_parse_col(parser, line[i], world);
		if (!passed)
			return passed;

		parser->curr_col++;
	}

	return true;
}

bool lp_open(struct level_parser *parser)
{
	parser->fp = fopen(parser->filename, "r");
	if (parser->fp == NULL) {
		log_error("error opening levels file '%s': %s",
			  parser->filename, strerror(errno));
		return false;
	}

	parser->stopped = false;
	return true;
}

/*
 * Setup the parser and world state for processing a new level.
 */
static void lp_init_level(struct level_parser *parser, struct world *world)
{
	world_init(world);
	
	parser->curr_row = 0;
	parser->curr_col = 0;
	parser->done = false;
	parser->parse_error = false;

	parser->worker_count = 0;
	parser->box_count = 0;
	parser->dock_count = 0;
}

/*
 * Processes a line from the levels file.
 *
 * Skips comments. Handles EOF and errors. Returns true on success,
 * false otherwise.
 */
static bool lp_do_line(struct level_parser *parser, char *line, struct world *world)
{
	bool passed;

	if (line == NULL) {
		if (ferror(parser->fp)) {
			log_error("error reading from file: %s", strerror(errno));
			return false;
		}

		if (parser->curr_row != 0) {
			log_error("unexpected EOF, parsing file: %s", strerror(errno));
			return false;
		}

		parser->stopped = true;
		return true;
	}

	if (line[strlen(line) - 1] != '\n') {
		log_warn("line too long in level");
		return false;
	}

	if (is_comment(line))
		return true;

	if (is_end_of_level(line)) {
		world->nrows = parser->curr_row;
		parser->done = true;
		return true;
	}

	passed = lp_parse_row(parser, line, world);
	if (!passed) {
		parser->parse_error = true;
		return true;
	}

	return true;
}

bool lp_next(struct level_parser *parser, struct world *world)
{
	char line[64];
	char *retp;
	bool passed;
	
	lp_init_level(parser, world);

	while (1) {
		retp = fgets(line, sizeof(line), parser->fp);
		passed = lp_do_line(parser, retp, world);
		if (!passed)
			return passed;

		if (parser->stopped)
			return true;

		if (parser->done) {
			if (parser->parse_error || !lp_level_is_valid(parser)) {
				lp_init_level(parser, world);
				continue;
			}
			return true;
		}
	}

	return true;
}

bool lp_stopped(struct level_parser *parser)
{
	return parser->stopped;
}

bool lp_close(struct level_parser *parser)
{
	int ret;

	ret = fclose(parser->fp);
	if (ret == -1) {
		log_error("error closing levels file '%s': %s",
			  parser->filename, strerror(errno));
		return false;
	}

	return true;
}
