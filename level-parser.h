#ifndef LEVEL_PARSER_H
#define LEVEL_PARSER_H

#include "world.h"

#include <stdio.h>
#include <stdbool.h>

struct level_parser
{
	/* Private */
	const char *filename;
	unsigned curr_row;
	unsigned curr_col;
	FILE *fp;

	int box_count;
	int dock_count;
	int worker_count;

	bool done;
	bool parse_error;
	bool stopped;
};

/*
 * Initializes the level parser.
 */
void lp_init(struct level_parser *parser, const char *filename);

/*
 * Starts parsing file.
 *
 * Returns true on success, false otherwise.
 */
bool lp_open(struct level_parser *parser);

/*
 * Gets the next level from the file.
 *
 * Returns true on success, and false on failure. If EOF occurs, true
 * is returned. On success, the caller should check, if we have hit
 * EOF, by invoking lp_stopped().
 */
bool lp_next(struct level_parser *parser, struct world *world);

/*
 * Returns true if EOF is reached, false otherwise.
 */
bool lp_stopped(struct level_parser *parser);

/*
 * Cleans up of parser.
 *
 * Returns true on success, false on failure.
 */
bool lp_close(struct level_parser *parser);

#endif
