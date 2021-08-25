#include <ncurses.h>

int main()
{
	initscr();
	int c;
	c = getch();
	printf("%i", c);
	endwin();
	return 0;
}
