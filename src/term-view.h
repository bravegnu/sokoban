#ifndef TERM_VIEW_H
#define TERM_VIEW_H

#define TERM_VIEW(v) ((struct term_view *)(v))

struct term_view {
	/* Parent */
	struct view base;
};

bool term_view_init(struct term_view *term_view);

#endif
