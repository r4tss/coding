#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <memory.h>
#include <math.h>

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

struct portal {
	struct p p1;
	struct p p2;
};

int endGame(WINDOW * win) {
	nodelay(win, 0);
	mvwprintw(win, height / 2, (width / 2) - 5, "GAME OVER");
	mvwprintw(win, (height / 2) + 1, (width / 2) - 9, "Press r to restart");
	mvwprintw(win, (height / 2) + 2, (width / 2) - 13, "or enter to go to the main menu");
	wrefresh(win);
	for(;;)
	{
		switch(wgetch(win))
		{
			case 114:
				return 2;
				break;
			case 10:
				return 0;
				break;
		}
	}
}

struct items {
        int len;
        char items[3][15];
};

int menu(int y, int x, int h, int w, int aLen, char it[][15], char *title)
{
        int key, i, c = 0, o = 0;
        WINDOW * menu = newwin(h, w, y, x);
	keypad(menu, true);
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
			case 259:
                        case 119:
                                if(c > 0)
                                        --c;
                                break;
			case 258:
                        case 115:
                                if(c < aLen - 1)
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
	struct items sit = {3, {"Play Game", "Guide", "Exit"}};
	struct p pray[60];
	struct portal port;
	int key, n, i, a = 0, time = 200000, score = 0, state = 0, effect = 0, effect_timer;
	char title[12] = "nsnake";
	bool exit = false;
	
	initscr();
	int sx = (COLS - width) / 2, sy = (LINES - height) / 2;
	curs_set(0);
	noecho();
	WINDOW * win = newwin(height, width, sy, sx);
	keypad(win, true);
	refresh();

	WINDOW * hw = newwin(7, 16, sy + (height/2) - 5, sx + width + 3);
	box(hw, 0, 0);
	mvwprintw(hw, 0, 2, " Highscores ");
	//for(i=0;i<5;++i)
	//{
	//	mvwprintw(hw, i+1, 1, "%s", )
	//}
	wrefresh(hw);
	while(!exit)
	{
		switch(state)
		{
			case 0:	//Start menu state
				nodelay(win, 0);
				wclear(win);
				wrefresh(win);
				//mvprintw(LINES - 1, 0, "W, A, S, D to navigate     Press enter to select     Esc to quit");
				state = menu((LINES - 15) / 2, (COLS - 10) / 2, 10, 15, sit.len, sit.items, title);
				if(state == 0) {
					state = 1;
					break;
				} else if(state == 1) {
					state = 3;
					break;
				} else if(state == 2) {
					exit = true;
					break;
				}
			case 1: //Play state
				nodelay(win, 1);
				switch(key = wgetch(win))
				{
					case 258:
					case 115: //W go up
						if( s.dir == 2 || s.dir == 3) { s.dir = 0; }
						break;
					case 259:
					case 119: //S go down
						if( s.dir == 2 || s.dir == 3) { s.dir = 1; }
						break;
					case 260:
					case 97: //A go left
						if(s.dir == 0 || s.dir == 1) { s.dir = 2; }
						break;
					case 261:
					case 100: //D go right
						if(s.dir == 0 || s.dir == 1) { s.dir = 3; }
						break;
					case 27: //ESC
						state = 0;
						break;
					default:
						break;
				}
				if(s.p.x == port.p1.x && s.p.y == port.p1.y)
					s.p = port.p2;
				else if(s.p.x == port.p2.x && s.p.y == port.p2.y)
					s.p = port.p1;
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
				if ((s.p.y >= height-1 || s.p.y <= 0 || s.p.x >= width-1 || s.p.x <= 0) && effect != 3) { state = endGame(win);/* exit = true; */ break; }
				if(effect != 3)
				{
					for (i = 2; i < s.len + 1; i++) {
						if(s.p.x == pray[i].x && s.p.y == pray[i].y) {
							state = endGame(win);// exit = true;
							break;
						}
					}
				} else
				{
					if (s.p.y == height-1) { s.p.y = 1; }
					if (s.p.y == 0) { s.p.y = height-1;}
					if (s.p.x >= width-1) { s.p.x = 2; }
					if (s.p.x <= 0) { s.p.x = width-2; }
				}
				if (s.p.x == candy.x && s.p.y == candy.y) {
					candy = rP();
					if(effect == 2)
						score += 200;
					else
						score += 100;
					a += 1000;
					time = time - pow(0.5,(0.0015*(score-7000)));
					s.len++;
					if(effect == 0) {
						effect = (rand() % 5) + 1;
						effect_timer = 80;
					}
					if(effect == 5 && effect_timer == 80) {
						port.p1 = rP();
						port.p2 = rP();
					}
				}
				wclear(win);
				for ( n = s.len; n > 0; n--) {
					pray[0] = s.p;
					pray[n] = pray[n-1];
					mvwprintw(win, pray[n+1].y, pray[n+1].x, "+");
				}
				if(effect == 5) {
					mvwprintw(win, port.p1.y, port.p1.x, "O");
					mvwprintw(win, port.p2.y, port.p2.x, "O");
				}
				mvwprintw(win, s.p.y, s.p.x, "@");
				mvwprintw(win, candy.y, candy.x, "#");
				box(win, 0, 0);
				mvwprintw(win, 0, 45, " Score: %i ", score);
				mvwprintw(win, 0, 0, "effect : %i", effect);
				wrefresh(win);
				if(effect_timer > 0)
					--effect_timer;
				else
					effect = 0;
				if(effect == 1)
					usleep(time/2);
				else
					usleep(time);
				break;
			case 2: //Reset game
				s.dir = 0; s.p = o; s.len = 1; a = 0; time = 200000; score = 0; state = 1; effect = 0; memset(pray, 0, sizeof pray);
				break;
			case 3: //Guide screen
				wclear(win);
				mvwprintw(win, 5, (width / 2) - 3, "Guide\nWelcome to nsnake!\nThis is an ncurses implementation of the popular game snake.\n\nTo move in nsnake use the WASD or arrow keys.\nYou can pause the game by pressing escape and select things by pressing enter.\n\nThe goal of the game is to keep the snake alive and get as many points as possible.\nYou achieve this by picking up the candies which are repersented by hashtags(#).\nThe candies will also increase the length of your snake.\nTo keep the snake alive do not run into walls or yourself as this will kill the snake.\n\nWhen you pick up a candy you will recieve one of five random effects, these effects include:\n   double speed\n   double points\n   ghost\n   big head\n   portal\n\nPress any key to go back.");
				wrefresh(win);
				wgetch(win);
				state = 0;
				break;
		}
	}
	endwin();
	return 0;
}
