#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define height	50
#define width	100

struct p {
	int x;
	int y;
};

struct s {
	struct p p;
	int dir;
	int len;
};


struct p rP() {
	struct p p;
	p.x = (rand() % (width - 2)) + 1;
	if ( p.x%2 == 0 ){
	} else {
		p.x++;
	}
	p.y = (rand() % (height - 2)) + 1;
	return p;
}

bool endGame(WINDOW * win) {
	nodelay(win, 0);
	mvwprintw(win, height / 2 , width / 2, "GAME OVER");
	wrefresh(win);
	wgetch(win);
}

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
			default:
				break;
                }
                wrefresh(menu);
        }
        delwin(menu);
        if(o == 1)
                return c;
        else
                return 1;
}

int main()
{
	srand(time(0));
	struct p candy = rP();
	struct p o = {width / 2, height / 2};
	struct s s = { o, 0, 1}; //dir is 0 = down, 1 = up, 2 = left, 3 = right.
	struct items sit = {2, {"Play Game", "Exit"}};
	struct p pray[60];
	int key, n, i, a = 0, time = 200000, score = 0, state = 0;
	char title[12] = "nsnake";
	bool exit = false;
	
	initscr();
	int sx = (COLS - width) / 2, sy = (LINES - height) / 2;
	curs_set(0);
	noecho();
	keypad(stdscr, TRUE);
	WINDOW * win = newwin(height, width, sy, sx);
	refresh();

	while(!exit)
	{
		switch(state)
		{
			case 0:	//Start menu state
				nodelay(win, 0);
				state = menu((LINES - 15) / 2, (COLS - 10) / 2, 10, 15, sit.len, sit.items, title);
				if(state == 0) {
					state = 1;
				} else if(state == 1) {
					exit = true;
				}
				break;
			case 1: //Play state
				nodelay(win, 1);
				switch(key = wgetch(win))
				{
					case 115: //W go up
						if( s.dir == 2 || s.dir == 3) { s.dir = 0; }
						break;
					case 119: //S go down
						if( s.dir == 2 || s.dir == 3) { s.dir = 1; }
						break;
					case 97: //A go left
						if(s.dir == 0 || s.dir == 1) { s.dir = 2; }
						break;
					case 100: //D go right
						if(s.dir == 0 || s.dir == 1) { s.dir = 3; }
						break;
					case 27: //ESC
						state = 0;
					default:
						break;
				}
				switch(s.dir)
				{
					case 0:
						s.p.y++;
						break;
					case 1:
						s.p.y--;
						break;
					case 2:
						s.p.x-=2;
						break;
					case 3:
						s.p.x+=2;
						break;
				}
				if (s.p.y >= height-1 || s.p.y <= 0 || s.p.x >= width-1 || s.p.x <= 0) { endGame(win); exit = true; }
				for (i = 2; i < s.len + 1; i++) {
					if(s.p.x == pray[i].x && s.p.y == pray[i].y) {
						endGame(win); exit = true;
						break;
					}
				}
			//	if (s.p.y == height-1) { s.p.y = 1; }
			//	if (s.p.y == 0) { s.p.y = height-1;}
			//	if (s.p.x >= width-1) { s.p.x = 2; }
			//	if (s.p.x <= 0) { s.p.x = width-2; }
				if (s.p.x == candy.x && s.p.y == candy.y) {
					candy = rP();
					score += 100;
					a += 1000;
					time = time - score * 5;
					s.len++;
				}
				wclear(win);
				for ( n = s.len; n > 0; n--) {
					pray[0] = s.p;
					pray[n] = pray[n-1];
					mvwprintw(win, pray[n+1].y, pray[n+1].x, "+");
				}
				mvwprintw(win, s.p.y, s.p.x, "@");
				mvwprintw(win, candy.y, candy.x, "#");
				box(win, 0, 0);
				mvwprintw(win, 0, 45, " Score: %i ", score);
				wrefresh(win);
				usleep(time);
				break;
		}
	}
	endwin();
	return 0;
}
