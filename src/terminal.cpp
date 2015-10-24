#include "terminal.h"
#include <curses.h>

Terminal::Terminal() : width(-1), height(-1) {
	getmaxyx(stdscr, height, width);
}
