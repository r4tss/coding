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

struct ship {
	struct p p;
	int dir;
	int len;
	int live;
}

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

struct p getPlace(struct p p, WINDOW * w)
{
	p.x = 1;
	p.y = 1;
	int e = 0;
	curs_set(1);
	for(;;)
	{
		switch(getch())
		{
			case 119: // up
				if(p.y > 1)
					--p.y;
				break;
			case 115: // down
				if(p.y < 10)
					++p.y;
				break;
			case 97: // left
				if(p.x > 1)
					--p.x;
				break;
			case 100: // right
				if(p.x < 10)
					++p.x;
				break;
			case 10:
				e = 1;
				break;
		}
		wmove(w, p.y, p.x);
		wrefresh(w);
		if(e == 1)
			break;
	}
	curs_set(0);
	return p;
}

struct ship setPlace(struct ship s, WINDOW * w)
{
	s.p.x = 1;
	s.p.y = 1;
	int e = 0;
	curs_set(1);
	for(;;)
	{
		switch(getch())
		{
			case 119:
				if(s.p.y > 1)
					s.p.y--;
				break;
			case 115:
				if(s.p.y < 10)
					s.p.y++;
				break;
			case 97:
				if(s.p.x > 1)
					s.p.x--;
				break;
			case 100:
				if(s.p.x < 10)
					s.p.x++;
				break;
			case 114:
				if(s.dir == 3)
					s.dir = 0;
				else
					s.dir++;
				break;
			case 82:
				if(s.dir == 0)
					s.dir = 3;
				else
					s.dir--;
			case 10:
				e = 1;
				break;
		}
		wmove(w, s.p.y, s.p.x);
		wrefresh(w);
		if(e == 1)
			break;
	}
}

int main() 
{
	struct player p1, p2;
	struct p p1p[256], p2p[256];
	struct items start = {3, {"Host Game", "Join Game", "Exit"}};
	int state = 0, e = 0, sfd, round = 0, host, p0i = 0, p2i = 0, phase = 0;
	char ip[128];

	initscr();
	curs_set(0);
	noecho();
	WINDOW * p1w = newwin(30, 45, 1, 0);
	WINDOW * p2w = newwin(30, 45, 1, 45);
	refresh();
	while(e == 0)
	{
		switch(state)
		{
			case 0:	//Start menu
				state = menu((LINES - 5) / 2, (COLS - 15) / 2, 5, 15, start.len, start.items, "nships") + 1;
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
				getstrwin(ip, "Enter IP");
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
				refresh();
				box(p1w, 0, 0);
				box(p2w, 0, 0);
				mvwprintw(p1w, 0, 1, p1.name);
				mvwprintw(p2w, 0, 1, p2.name);
				if(host == 0)
					wmove(p1w, 0, 0);
				else
					wmove(p2w, 0, 0);
				wrefresh(p1w);
				wrefresh(p2w);
				switch(phase)
				{
					case 0: // Choose ship locations
						switch(host)
						{
							case 0:
								break;
							case 1:
								break;
						}
						break;
					case 1: // Play the game
						switch(round)
						{
							case 0:
								if(host == 0) {
									p2p[p2i] = getPlace(p2.p, p2w);
									send(sfd, p2p[p2i], sizeof(p2p[p2i]), 0);
								}
								else {
									printw("%s's turn", p2.name);
									recv(sfd, p2p[p2i], sizeof(p2p[p2i]), 0);
								}
								++p2i;
								round = 1;
								break;
							case 1:
								if(host == 1) {
									p1p[p1i] = getPlace(p1.p, p1w);
									send(sfd, p1p[p1i], sizeof(p1p[p1i]), 0);
								}
								else {
									printw("%s's turn", p1.name);
									recv(sfd, p1p[p1i], sizeof(p1p[p1i]), 0);
								}
								++p1i;
								round = 0;
								break;
						}
						break;
				}
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
