#include <ncurses.h>

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
