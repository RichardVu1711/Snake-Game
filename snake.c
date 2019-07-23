#include <stdio.h>
#include <stdlib.h>
#include "elements.h"
#include <stdbool.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <stropts.h>
#include <time.h>
#include <unistd.h>
Object MapProto= 
{
	.init = Map_init
};

void sleepcp(int millisecons)
{
	clock_t time_end;
	time_end = clock() + millisecons *CLOCKS_PER_SEC/1000;
	while(clock()< time_end);
}
void clear_background(void)
{
	system("clear");
}
int main(int argc, char *argv[])
{
	int size[2] = {10,10};
	int pos [2] = {0,0};
	char k= ' ';
	Map* game = NEW(Map,size,pos);
	if(game->_(init)(game))// print_game(game);
	
	while(Update(game))
	{
		clear_background();
		print_game(game);
		if(_kbhit())
		{
			k = getchar();
			if(k=='p')
			{
			// adding some codes to make snake move smoothly
			}
			reaction_on_keyboard(game,k);
		}
		game->snake->_(move)(game->snake,game->snake->dir);
		if(!game) break;
		if(k == 'q')
			break;
		sleepcp(500);
	}
	/*while(1)
	{
		if(_kbhit())
		{	
			k = getchar();
			switch (k)
			{
				case 'a':		
					printf("!");
					break;		
				default:
					return 1;
					break;
			}
		}
	}*/
	return 0;
}
