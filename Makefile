CPPFLAGS = -DLOG_USE_COLOR
objs = src/main.o src/game-engine.o src/level-parser.o src/world.o src/log.o

# SDL Options

LDLIBS += -L/usr/lib/x86_64-linux-gnu -lSDL
CPPFLAGS += -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT
objs += src/sdl-view.o

# ncurses Options

objs += src/term-view.o
LDLIBS += -lncurses

all: src/main
	cp src/main sokoban

src/main: $(objs)

distclean: clean
	find . -name *~ -delete
	rm -f game.log

clean:
	rm -f src/*.o
	rm -f src/main
	rm -f sokoban


