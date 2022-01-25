#include <ncurses.h>
#include <string.h>
#define MAPH = 9
#define MAPW = 9

struct point
{
	int x;
	int y;
};

struct person
{
	struct point p;
	char name[32];
	char model;
};

int main()
{
	struct person player;
	player.model = '@';
	strcpy(player.name, "ooo");
	printf("%s\n", player.name);
}
