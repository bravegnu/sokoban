CPPFLAGS = -DLOG_USE_COLOR
objs = main.o game-engine.o level-parser.o world.o log.o

-include .config

all: check-config main

help:
	@echo 'Usage:'
	@echo '    $ make sdl-config or make ncurses-config'
	@echo '    $ make'

main: $(objs)

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
	rm -f *.o
	rm -f main


