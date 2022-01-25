#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
	bool exit = false, opf = false;
	int key, row, col, a, b, suma = 0, sumb = 0, res = 0, op = 0;

	initscr();
	getmaxyx(stdscr, row, col);
	noecho();
	curs_set(0);

	WINDOW * w = newwin(row, col, 0, 0);
	keypad(w, true);

	while(!exit)
	{
		wclear(w);
		box(w, 0, 0);
		mvwprintw(w, 0, 1, " ncalc ");
		mvwprintw(w, 1, 4, "%i", suma);
		mvwprintw(w, 2, 4, "%i", sumb);
		mvwprintw(w, 3, 4, "%i", res);
		switch(op)
		{
			case 0:
				mvwprintw(w, 2, 2, " ");
				break;
			case 1:
				mvwprintw(w, 2, 2, "+");
				break;
			case 2:
				mvwprintw(w, 2, 2, "-");
				break;
			case 3:
				mvwprintw(w, 2, 2, "*");
				break;
			case 4:
				mvwprintw(w, 2, 2, "/");
				break;
		}
		refresh();

		key = wgetch(w);
		
		if(key == 27 || key == 96)
		{
			exit = true;
		} else if(!opf)
		{
			switch(key)
			{
				case 272:
					suma = -suma;
					break;
				case 48 ... 57:
					a = key - 48;
					suma = (suma * 10) + a;
					break;
				case 43:
				case 273:
					op = 1;
					opf = true;
					break;
				case 45:
				case 274:
					op = 2;
					opf = true;
					break;
				case 42:
				case 275:
					op = 3;
					opf = true;
					break;
				case 47:
				case 276:
					op = 4;
					opf = true;
					break;
			}
			if(opf && suma == 0)
			{
				suma = res;
				res = 0;
			}
		} else if(opf)
		{
			switch(key)
			{
				case 45:
					sumb = -sumb;
					break;
				case 48 ... 57:
					b = key - 48;
					sumb = (sumb * 10) + b;
					break;
				case 10:
				case 61:
					if(op == 1)
						res = suma + sumb;
					if(op == 2)
						res = suma - sumb;
					if(op == 3)
						res = suma * sumb;
					if(op == 4)
						res = suma / sumb;
					suma = 0;
					sumb = 0;
					op = 0;
					opf = false;
					break;
			}
		}
	}

	endwin();
	return 0;
}
