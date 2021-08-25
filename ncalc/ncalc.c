#include <ncurses.h>

int main()
{
	initscr();
	keypad(stdscr, TRUE);
	noecho();
}
