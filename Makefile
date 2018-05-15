CPPFLAGS = -DLOG_USE_COLOR
objs = src/main.o src/game-engine.o src/level-parser.o src/world.o src/log.o

-include .config

all: check-config src/main
	cp src/main sokoban

help:
	@echo 'Usage:'
	@echo '    $ make sdl-config or make ncurses-config'
	@echo '    $ make'

src/main: $(objs)

check-config:
	@test -e ".config" || (echo "*** Source tree not configured! ***"; exit 1)

sdl-config:
	cp configs/sdl.config .config

ncurses-config:
	cp configs/ncurses.config .config

distclean: clean
	find . -name *~ -delete
	rm -f .config
	rm -f game.log

clean:
	rm -f src/*.o
	rm -f src/main
	rm -f sokoban


