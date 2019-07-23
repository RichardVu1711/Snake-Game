#ifndef	_elements_h
#define	_elements_h

#include "object.h"
#include <time.h>
#define LEN 20
#define	WIDE 15
#define MAX_LEN 300
typedef enum 
{
	EMPTY, SNAKE, FOOD, BOUNDARY
} Space;

typedef enum
{
	HORIZON, VERTICAL
} Orientation;


typedef struct
{
	Object proto;
	Object head;
	Object body[MAX_LEN];
	Space name;
	Orientation ori;
	Direction dir;
	
} Snake;
void *snake_move(void *self,Direction direction);
void moving(void *self,int x, int y);
int snake_init (void *self); 
void *snake_ChangingSize(void *self);
typedef struct
{
    Object proto;
    State state;
	int Obj_pos[2];
    Space name;
} Food;
int Food_init (void *self);
typedef struct 
{
	Object proto;
	void *self;
	Space name;
} Points;
typedef struct
{
	Object proto;
	Points points[LEN][WIDE];	// all points in the map
	Snake *snake;
	Food *food;
	
} Map;

int Map_init(void* self);
void print_game(void* self);

void* Map_move(void *self, Direction direction);
int Update(void *self);
int Die(void *self);
void reaction_on_keyboard(void *self,char k);
int _kbhit();
#endif
