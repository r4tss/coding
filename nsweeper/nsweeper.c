// Minesweeper game

#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define len(array) sizeof(array)/sizeof(*array)

struct tile
{
	int y;
	int x;
	int n;	// Number of mines around tile
	bool d;	// Display or not
	bool m;	// Mine or not
};

void cac(struct tile *t, int tS, int w, int y, int x, bool init, bool *o)
{
	int mines = 0;
	bool no = false;
	for(int i = 0;i<tS;i++)
	{
		//mvprintw(0, 30, "hi");
		//getch();
		if(t[i].y == y && t[i].x == x)
		{
			if(init && t[i].m)
			{
				*o = true;
			} else
			{
				for(int cy = 0;cy<3;cy++)
				{
					for(int cx = 0;cx<3;cx++)
					{
						//mvprintw(t[i - 1 - w + (w * cy) + cx].y + 1, t[i - 1 - w + (w * cy) + cx].x + 1, "H");
						//refresh();
						//getch();
						if(t[i - w - 1 + cx + (cy * w)].m && (i - w - 1 + cx + (cy * w) > 0) && (i - w - 1 + cx + (cy * w)) < tS)
							mines++;
						else
							cac(t, tS, w, cy, cx, false, &no);
						clear();
						mvprintw(0, 0, "%i | %i, %i | %i, %i", t[i - 1 - w + (w * cy) + cx].m, cy, cx, i, i - 1 - w + cx + (cy * w));
						refresh();
						getch();
					}
				}
				if(init || (!init && mines == 0))
					t[i].d = false;
				t[i].n = mines;
			}
		}
	}
}

int main()
{
	srand(time(NULL));

	// Most of these shouldn't be initialized, rather given a value before gameplay starts
	// wh is offset by +2 and ww is offset by +1 when it comes to logic but actual size wise ww is offset by wh * 2 + 1
	int phase = 1, wh = 8, ww = wh * 2, cy = 0, cx = 0, ty, tx, tc, ti;
	bool gO = false, t;

	initscr();
	noecho();
	refresh();

	switch(phase)
	{
		case 0:
			//WINDOW * m = newwin(5, 10, );
			break;
		case 1:
			// Move window init to after phase 0 later
			WINDOW * w = newwin(wh + 2, ww + 1, 0, 0);

			struct tile map[wh * wh];
			for(int i = 0, x = 0, y = 0;i<len(map);i++)
			{
				map[i].y = y;
				map[i].x = x;
				map[i].n = 0;
				map[i].d = true;
				map[i].m = false;
				x+=2;
				if(x >= ww)
				{
					x = 0;
					y++;
				}
			}

			// Loop to get locations of mines in array mL, this will be moved into phase 0 later
			tc = 0;
			while(tc < wh * wh / 4)
			{
				t = false;
				// (upper - lower + 1) + lower
				ty = (rand() % (wh - 1)) + 1;
				tx = (rand() % (ww - 1)) + 1;

				if(tx % 2 != 0)
					tx++;

				for(int i = 0;i<len(map);i++)
				{
					if(ty == map[i].y && tx == map[i].x)
					{
						ti = i;
						t = true;
					}
				}

				if(t)
				{
					map[ti].m = true;
					tc++;
				}
			}

			curs_set(1);
			while(!gO)
			{
				wclear(w);
				box(w, 0, 0);

				for(int i = 0;i<len(map);i++)
				{
					if(map[i].y == cy && map[i].x == cx)
						mvwprintw(w, 0, 1, "%i", map[i].m);
					if(map[i].d)
						mvwprintw(w, map[i].y + 1, map[i].x + 1, "=");
					else if(!map[i].d)
						mvwprintw(w, map[i].y + 1, map[i].x + 1, "%i", map[i].n);
					else if(map[i].m)
					{
						mvwprintw(w, map[i].y + 1, map[i].x + 1, "*");
						//mvprintw(l, ww + 3, "%i. %i, %i", l, map[i].y, map[i].x);
					}
				}


				wmove(w, cy + 1, cx + 1);
				wrefresh(w);

				switch(getch())
				{
					case 97:
						if(cx > 1)
							cx-=2;
						break;
					case 100:
						if(cx < ww - 3)
							cx+=2;
						break;
					case 119:
						if(cy >= 1)
							cy--;
						break;
					case 115:
						if(cy < wh - 1)
							cy++;
						break;
					case 10:
						cac(map, len(map), ww / 2, cy, cx, true, &gO);
						break;
				}
			}
			mvwprintw(w, wh / 2, ww / 3, "Game Over");
			curs_set(0);
			wrefresh(w);
			getch();
			break;
	}

	endwin();
	return 0;
}
