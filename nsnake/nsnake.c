#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

struct p {
	int x;
	int y;
};

struct s {
	struct p p;
	int dir;
	int len;
};

int height = 50;
int width = 100;

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

int main()
{
	srand(time(0));
	struct p candy = rP();
	struct p o = {width / 2, height / 2};
	struct s s = { o, 0, 1}; //dir is 0 = down, 1 = up, 2 = left, 3 = right.
	int key, n, i, a = 0, time = 200000, score = 0;
	struct p pray[60];
	bool exit = false;
	
	initscr();
	int sx = (COLS - width) / 2, sy = (LINES - height) / 2;
	curs_set(0);
	noecho();
	WINDOW * win = newwin(height, width, sy, sx);
	nodelay(win, 1);
	refresh();

	while(!exit)
	{
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
					exit = true;
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
	}
	endwin();
	return 0;
}
