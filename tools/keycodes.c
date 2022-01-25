#include <ncurses.h>

int main()
{
	initscr();
	keypad(stdscr, TRUE);
	int c;
	c = getch();
	printf("%i", c);
	endwin();
	return 0;
}
