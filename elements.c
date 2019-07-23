#include "elements.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <stropts.h>
#define POS 2
#define SIZE 2
// this is the initial position and size of snake

// this function is used to check whether buttons are pressed
int _kbhit() {
    static const int STDIN = 0;
    static bool initialized = false;

    if (! initialized) {
        // Use termios to turn off line buffering
        struct termios term;
        tcgetattr(STDIN, &term);
        term.c_lflag &= ~ICANON;
        tcsetattr(STDIN, TCSANOW, &term);
        setbuf(stdin, NULL);
        initialized = true;
    }

    int bytesWaiting;
    ioctl(STDIN, FIONREAD, &bytesWaiting);
    return bytesWaiting;
}


int pos_size[POS][SIZE] ={{4,1},{7,7}};
//POS: 1 is y, 0 is x
//SIZE: 1 is LEN(X), 0 is WIDE(Y)
//Description: this function is initialise snake's pos,size, length
//	Input: Snake Pointers
//	Output: 1 is success
//		0 is bugs
//	Effect: nothing
int snake_init(void *self)
{
	Snake* temp = self;
	//initialize position of snake
	temp->head.Obj_pos[0]= pos_size[1][0];
	temp->head.Obj_pos[1]= pos_size[1][1];

	//initializ size of snake
	temp->head.Obj_size[0]= pos_size[0][0];
	temp->head.Obj_size[1]= pos_size[0][1];

	// put head into body
	temp->body[0]= temp->head;

	int i =0;
	// initialize all snake pieces position
	for(i=0; i < temp->body[0].Obj_size[0];i++)
	{
		temp->body[i].Obj_pos[0] = temp->head.Obj_pos[0]-i;
		temp->body[i].Obj_pos[1] = temp->head.Obj_pos[1];
	}
	temp->ori = HORIZON;
	temp->dir = RIGHT;
	return 1;
}
//Description: make snake moving according to the direction
//	Input: snake pointer, current direction
//	Output: snake pointer with new pos
//	Effect: decrement or imcrement all snake pieces' position
void *snake_move(void *self, Direction direction)
{
	Snake * snake  = self;
	if(!snake) return NULL;
	else
	{
		switch(snake->ori)
		{
			case VERTICAL:
				switch (direction)
				{
					case RIGHT:
						moving(snake,1,0);
						break;
					case LEFT:
						moving(snake,-1,0);
						break;
					case UP:
						moving(snake,0,-1);
						break;
					case DOWN:
						moving(snake,0,1);
						break;
				}
				break;
			case HORIZON:
				switch (direction)
				{
					case UP:
						moving(snake,0,-1);
						break;
					case DOWN:
						moving(snake,0,1);
						break;
					case RIGHT:
						moving(snake,1,0);
						break;
					case LEFT:
						moving(snake,-1,0);
						break;
				}
				break;
			default:
				break;
		}
	}
	return  snake;
}
//Description: inplement and deplement the position of snake
//	Input: snake pointer, x and y
//	Output: nothing
//	Effect: snake will have a new coordinate
void moving(void * self, int x, int y)
{
	int i =0;
	Snake *temp= self;
	//NOTE: BUG!!!
	for (i =temp->body[0].Obj_size[0];i >= 0 ;i--)
	{
		// make snake move piece by piece => when it changes direction
		// the whole snake can change slowly	
		if(i!= 0)
		{
			temp->body[i].Obj_pos[0] = temp->body[i-1].Obj_pos[0];
			temp->body[i].Obj_pos[1] = temp->body[i-1].Obj_pos[1];
		}
		else
		{
			temp->body[i].Obj_pos[0] += x;
			temp->body[i].Obj_pos[1] += y;
		}
	}
}
//Description: snake will change size after 'eat' food
//	Input: snake pointer
//	Output: snake pointer with new size
//	Effect: a new tail will replace the old one
void* snake_ChangingSize(void *self)
{
	Snake *snake = self;
	snake->head.Obj_size[0]++;
	int newLen = snake->head.Obj_size[0];
	//add new piece into snake
	//1st, we have to check the shape of the tail
	//to do that, i will compare the tail and the nearest points to tail
	//=> we can determine snake's movement and orientation
	int tempX = snake->body[newLen-2].Obj_pos[0] - snake->body[newLen-3].Obj_pos[0];
	int tempY = snake->body[newLen-2].Obj_pos[1] - snake->body[newLen-3].Obj_pos[1];
	int px = snake->body[newLen-2].Obj_pos[0];
	int py = snake->body[newLen-2].Obj_pos[1];
	if((tempX>0)&&(tempY==0))
	{
		//snake is in Horizon and moves to the left
		px++; 
	}
	else if((tempX<0)&&(tempY==0))
	{
		//snake is in Horizon and moves to the right
		px--;
	}
	else if((tempX==0)&&(tempY >0))
	{
		//snake is in Vertical and moves to the down
		py++;
	}
	else if((tempX==0)&&(tempY<0))
	{
		// snake is in Vertical and moves to the Up
		py--;
	}
	else
	{
		//sth is wrong
		printf("!!!!!\n");
	}
	snake->body[newLen-1].Obj_pos[0] = px;
	snake->body[newLen-1].Obj_pos[1] = py;
	snake->body[0].Obj_size[0]= newLen;
	return snake;
}
// initializing Object moving and changing size !!!
Object SnakeProto= 
{
	.init = snake_init,
	.move = snake_move,
	.ChangingSize = snake_ChangingSize
};

//Description: initialize food's position by using random
//	Input: food pointer
//	Output: 1 is success
//		0 is fail
//	Effect: nothing
int Food_init(void* self)
{
	Food* food = self;
	food->proto.state = LIVE;
	//using random to create food position
	srand(time(NULL));		// no similar values in position
	food->Obj_pos[0] = rand()%(LEN-1)+1;	// create a range of random from 0 to len
	food->Obj_pos[1] = rand()%(WIDE-1)+1;	// create a range of random from 0 to wide
	food->state = LIVE;
	return 1;
}
Object FoodProto =
{
	.init = Food_init
};
//Description: this function is used to initialize all points in the map.
//		At beginning, all points (except the the boundary) are Empty
//		boundary points are BOUNDARY. if SNAKE and FOOD are initialised
//		assigning theses objects
//	Input:	Map pointer
//	Output: 1 if it's success
//		0 if it's not
//	Effect:	nothing
int Map_init (void* self)
{
	Map* temp = self; 		//define the type of void
	temp->snake  = NEW(Snake,pos_size[0],pos_size[1]);
	//create food in the map
	//--------------------------------------------------------------------
	int pos[2] = {0,0};
	int size[2] = {0,0};
	temp->food = NEW(Food,pos,size);
	temp->food->state = LIVE;
	//--------------------------------------------------------------------
	if(!temp->snake->_(init)(temp->snake)) return 0;
	// NOTE: PUT AN IF STATEMENT TO CHECK WHETHER SNAKE IS INITIALISED FOR NOT!!!
	Update(temp);
	return 1;
}
//Description: this function is used to print all points in map
//				EMPTY = .
//				BOUNDARY = /
//				SNAKE = *
//				FOOD = 0	
//		Input: map pointer
//		Output: nothing
//		Effect: all points are printed. EMPTY = . ,BOUNDARY =/ ,SNAKE =* ,FOOD=0 

void print_game(void* self)
{
	int i,j;
	Map *temp = self;		//define the type of void
	for(j=0; j < WIDE; j++)
	{
		for(i =0; i < LEN;i++)		
		{
			// print all points with its name
			switch(temp->points[i][j].name)
			{
				case EMPTY:
					printf(". ");
					break;
				case BOUNDARY:
					printf("/ ");
					break;
				case SNAKE:
					printf("* ");
					break;
				case FOOD:
					printf("0 ");
					break;
				//NOTE: THIS CASE NEED TO USED ERRNO LIBRARY !!!!!!	
				default:
					printf("this is an error, should use errno");
					break;
			}
			
		}
			printf("\n");
	}
}
//Description: annoucing user that they are die
//	Input: Map pointer
//	Output: 0 is success
//		1 is fail(sth wrong)	WILL BE ADDED SOME IF CONDITION 
//	Effect: nothing 
int Die(void* self)
{
	Map *map = self;
	printf("You are die!! GAME OVER.\n");
	if(map)
	{
		if(map->snake) map->snake->_(destroy)(map->snake);
		if(map->food) map->food->_(destroy)(map->food);
		map->_(destroy)(map);
	}	
	return 0;
}
//Description: Update the coordinates of Snake and Food and put them into Map's coordiantes
//	Input: Map pointer
//	Output: 1 is success
//		0 is fail
//	Effect: nothing
int Update(void *self)
{
	Map *temp = self;
	Snake* snake = temp->snake;
	Food* food = temp->food;
	if(food->state == DIE)	food->_(init)(food);
	int i,j;
	for(i=0;i < LEN;i++)
	{
		for(j=0;j<WIDE;j++)
		{

			if(i==0||j==0||i ==LEN-1||j==WIDE-1)
			{
				temp->points[i][j].name = BOUNDARY;
			}
			else
			{
				temp->points[i][j].name = EMPTY;
			}
		}
	}
	temp->points[food->Obj_pos[0]][food->Obj_pos[1]].name = FOOD;
	for(i =0; i < snake->body[0].Obj_size[0];i++)
	{
		int px = snake->body[i].Obj_pos[0];
		int py = snake->body[i].Obj_pos[1];
		if(px >= LEN||py >= WIDE||px ==0 || py== 0||temp->points[px][py].name == SNAKE) 
		{
			snake->head.state = DIE;
			return Die(temp);
		}
		if((temp->points[px][py].name == FOOD)&&(i==snake->body[0].Obj_size[0]-1))
		{
			printf("%d",snake->body[0].Obj_size[0]);
			snake->_(ChangingSize)(snake);
			food->state = DIE;
		}
		temp->points[px][py].name = SNAKE;
	// len is x (0), wide is y (1) [x][y]	
	// becarefull there will be bugs here!!!!
	}

	return 1;
}
//Description: snake's movement will change according to the pressed button
//	Input: snake pointer, letter of buttons
//	Output: nothing
//	Effect: snake's movement will change
void reaction_on_keyboard(void*self,char k)
{
	Map* temp = self;
	Snake* snake = temp->snake;
	switch(k)
	{
		case 'w':
			if(snake->dir == DOWN) break;
			snake->dir= UP;
			snake->_(move)(snake,UP);
			snake->ori = VERTICAL;
			break;
		case 's':
			if(snake->dir== UP) break;
			snake->dir = DOWN;
			snake->_(move)(snake,DOWN);
			snake->ori = VERTICAL;
			break;
		case 'a':
			if(snake->dir ==RIGHT) break;
			snake->dir = LEFT;
			snake->_(move)(snake,LEFT);
			snake->ori = HORIZON;
			break;
		case 'd':
			if(snake->dir == LEFT) break;
			snake->dir = RIGHT;
			snake->_(move)(snake,RIGHT);
			snake->ori= HORIZON;
			break;
		default:
			break;
	}
}

