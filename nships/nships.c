#include <ncurses.h>
#include <stdlib.h>
#include <time.h>

#include "server.c"

#define height 	80
#define width	200

struct p {
	int x;
	int y;
};

struct p rP() {
	struct p p;
	p.x = (rand() % (width - 2)) + 1;
	if (p.x%2 == 0) {
	} else {
		p.x++;
	}
	p.y = (rand() % (height - 2)) + 1;
	return p;
}

struct player {
	struct p p;
	int id;
	int ships;
	char name[15];
};

struct items {
        int len;
        char items[3][15];
};

int menu(int y, int x, int h, int w, int aLen, char it[][15], char *title)
{
        int key, i, c = 0, o = 0;
        WINDOW * menu = newwin(h, w, y, x);
        refresh();
        box(menu, 0, 0);
	if(title != 0)
		mvwprintw(menu, 0, 1, "%s", title);
        while(o == 0)
        {
                for(i = 0; i < aLen; ++i) {
                        if(i == c) {
                                wattron(menu, A_STANDOUT);
                                mvwprintw(menu, i + 1, 1, "%s", it[i]);
                                wattroff(menu, A_STANDOUT);
                        } else {
                                mvwprintw(menu, i + 1, 1, "%s", it[i]);
                        }
                }
                switch(key = wgetch(menu))
                {
                        case 119:
                                if(c > 0)
                                        --c;
                                break;
                        case 115:
                                if(c < aLen)
                                        ++c;
                                break;
                        case 10:
                                o = 1;
                                break;
                        case 27:
                                o = 2;
                                break;
                }
                wrefresh(menu);
        }
        delwin(menu);
        if(o == 1)
                return c;
        else
                return -1;
}

int main() 
{
	struct player p1, p2, hostID;
	struct items start = {3, {"Host Game", "Join Game", "Exit"}};
	int state = 0;
	char title[15] = "nships";
	bool exit = false;

	initscr();
	noecho();
	while(!exit)
	{
		switch(state)
		{
			case 0:
				state = menu((LINES - 15) / 2, (COLS - 10) / 2, 5, 15, start.len, start.items, title);
				if(state == 0)
					state = 1;
					break;
				if(state == 1)
					state = 2;
					break;
				if(state == 2)
					exit = true;
				break;
			case 1:
				hostID = fork()
				break;
			case 2:
				break;
		}
	}
}
