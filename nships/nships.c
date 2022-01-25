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

struct p { int x;
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

struct p getPlace(WINDOW * w)
{
	struct p p = {2, 1};
	int e = 0, wh, ww;
	curs_set(1);
	getmaxyx(w, wh, ww);
	wmove(w, p.y, p.x);
	wrefresh(w);
	for(;;)
	{
		switch(getch())
		{
			case 119: // up
				if(p.y > 1)
					--p.y;
				break;
			case 115: // down
				if(p.y < wh - 2)
					++p.y;
				break;
			case 97: // left
				if(p.x > 2)
					p.x-=2;
				break;
			case 100: // right
				if(p.x < ww - 3)
					p.x+=2;
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

struct ship setPlace(WINDOW * w, int len)
{
	struct ship s = {{2, 1}, 0, len, 1};
	int e = 0, wh, ww;
	char character[64];
	curs_set(0);
	getmaxyx(w, wh, ww);
	for(;;)
	{
		switch(getch())
		{
			case 119:
				if(s.p.y > 1)
					s.p.y--;
				break;
			case 115:
				if(s.dir == 0 && s.p.y < wh - 2)
					s.p.y++;
				else if(s.dir == 1 && s.p.y + 1 < wh - s.len)
					s.p.y++;
				break;
			case 97:
				if(s.p.x > 2)
					s.p.x-=2;
				break;
			case 100:
				if(s.dir == 0 && s.p.x < ww - s.len - (s.len + 1))
					s.p.x+=2;
				else if(s.dir == 1 && s.p.x < ww - 3)
					s.p.x+=2;
				break;
			case 114:
				if(s.dir == 0) {
					s.dir = 1;
					if(s.p.y + s.len > wh - s.len)
						s.p.y = s.p.y - (s.p.y + s.len - wh) - 1;
				}
				else {
					s.dir = 0;
					if(s.p.x + s.len > ww - s.len)
						s.p.x = s.p.x - (s.p.x + s.len - ww) - (s.len + 1);
				}
				break;
			case 10:
				e = 1;
				break;
		}
		wclear(w);
		box(w, 0, 0);
		for(int l=0;l<s.len;++l)
		{
			if(s.dir == 0) {
				if(l == 0)
					strcpy(character, "<");
				else if(l == s.len - 1)
					strcpy(character, ">");
				else
					strcpy(character, "-");
				mvwprintw(w, s.p.y, s.p.x + (l * 2), "%s", character);
			}
			else {
				if(l == 0)
					strcpy(character, "^");
				else if(l == s.len - 1)
					strcpy(character, "v");
				else
					strcpy(character, "|");
				mvwprintw(w, s.p.y + l, s.p.x, "%s", character);
			}
		}
		wrefresh(w);
		if(e == 1)
			break;
	}
	return s;
}

int main() 
{
	struct player p1, p2;
	struct items start = {3, {"Host Game", "Join Game", "Exit"}};
	struct ship p1s[5], p2s[5];
	struct p p1p[248], p2p[248];
	int state = 0, e = 0, sfd, round = 0, host, p1i = 0, p2i = 0, phase = 0, l, buffer;
	char ip[128];

	initscr();
	curs_set(0);
	noecho();
	WINDOW * p1w = newwin(12, 23, 1, 0);
	WINDOW * p2w = newwin(12, 23, 1, 23);
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
				// Draw ships depending on host or not
				// switch(host)
				wrefresh(p1w);
				wrefresh(p2w);
				switch(phase)
				{
					case 0: // Choose ship locations
						for(int s=0;s<5;++s)
						{
							if(s < 3)
								l = 3;
							else if(s < 4)
								l = 4;
							else
								l = 5;
							switch(host)
							{
								case 0:
									p2s[s] = setPlace(p2w, l);
									break;
								case 1:
									p1s[s] = setPlace(p1w, l);
									break;
							}
						}
						for(int s=0;s<5;++s)
						{
							switch(host)
							{
								case 0:
									//send to p1
									//recieve p1
									break;
								case 1:
									//recieve p2
									//send to p2
									break;
							}
						}
						phase = 1;
						break;
					case 1: // Play the game
						switch(round)
						{
							case 0:
								if(host == 0) {
									p2p[p2i] = getPlace(p1w);
									send(sfd, &p2p[p2i].x, sizeof(p2p[p2i].x), 0);
									send(sfd, &p2p[p2i].y, sizeof(p2p[p2i].y), 0);
								}
								else {
									printw("%s's turn", p2.name);
									recv(sfd, &p2p[p2i].x, 128, 0);
									recv(sfd, &p2p[p2i].y, 128, 0);
									mvwprintw(p1w, 0, 0, "%i %i", p2p[p2i].x, p2p[p2i].y);
									getch();
								}
								++p2i;
								round = 1;
								break;
							case 1:
								if(host == 1) {
									p1p[p1i] = getPlace(p2w);
									//send(sfd, p1p[p1i], sizeof(p1p[p1i]), 0);
								}
								else {
									printw("%s's turn", p1.name);
									//recv(sfd, p1p[p1i], sizeof(p1p[p1i]), 0);
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
