#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

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
	int ships;
	char name[1024];
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
	clear();
        if(o == 1)
                return c;
        else
                return -1;
}

void getstrwin(char * ip, char *title)
{
	char str[80];
	echo();
	WINDOW * getip = newwin(3, 20, (LINES - 3) / 2, (COLS - 20) / 2);
	box(getip, 0, 0);
	refresh();
	mvwprintw(getip, 0, 1, title);
	wmove(getip, 1, 1);
	wgetnstr(getip, str, 18);
	strncpy(ip, str, 128);
	delwin(getip);
	noecho();
}

void gameWin(int x, struct player p) 
{
	WINDOW * gWin = newwin(30, 45, 0, x);
	refresh();
	box(gWin, 0, 0);
	mvwprintw(gWin, 0, 1, p.name);
	wrefresh(gWin);
}

int main() 
{
	struct player p1, p2;
	struct items start = {3, {"Host Game", "Join Game", "Exit"}};
	int state = 0, e = 0, sfd, host;
	char title[15] = "nships", nameTitle[20] = "Enter Name", ipTitle[20] = "Enter IP", ip[128];

	initscr();
	curs_set(0);
	noecho();
	WINDOW * g = newwin(100, 300, (LINES - 100) / 2, (COLS - 200) / 2);
	refresh();
	while(e == 0)
	{
		switch(state)
		{
			case 0:	//Start menu
				state = menu((LINES - 5) / 2, (COLS - 15) / 2, 5, 15, start.len, start.items, title) + 1;
				if(state == 3)
					state = 4;
				break;
			case 1:	//Host menu
				clear();
				printw("Waiting for client to connect");
				refresh();
				sfd = hostGame();
				clear();
				if(sfd == 0) {
					printw("Failed to host");
					state = 0;
				}
				else {
					host = 1;
					getstrwin(p1.name, "Enter your name");
					clear();
					printw("Waiting for response from client");
					refresh();
					send(sfd, p1.name, sizeof(p1.name), 0);
					recv(sfd, p2.name, sizeof(p2.name), 0);
					state = 3;
				}
				break;
			case 2:	//Client menu
				getstrwin(ip, ipTitle);
				sfd = joinGame(ip);
				clear();
				if(sfd == 0) {
					printw("Failed to connect");
					state = 0;
				}
				else {
					host = 0;
					getstrwin(p2.name, "Enter your name");
					clear();
					printw("Waiting for response from host");
					refresh();
					recv(sfd, p1.name, sizeof(p1.name), 0);
					send(sfd, p2.name, sizeof(p2.name), 0);
					state = 3;
				}
				break;
			case 3: //Game loop
				clear();
				gameWin(45, p2);
				gameWin(0, p1);
				refresh();
				getch();
				state = 4;
				break;
			case 4:	//Exit state
				e = 1;
				break;
		}
		refresh();
	}
	close(sfd);

	endwin();
	return 0;
}
