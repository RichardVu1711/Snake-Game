#ifndef _object_h
#define _object_h

#include <stdio.h>
#include <stdlib.h>
typedef enum
{
	LEFT,RIGHT,UP,DOWN
} Direction;

typedef enum
{
	DIE, LIVE
} State;
typedef struct 
{
	//determine the size of object
	// 0 is length, 1 is wide
	int Obj_size[2];
	//determine the position of object
	// 0 is x, 1 is y
	int Obj_pos[2];
	State state;
	void *(*move)(void *self, Direction direction );
	int (*init)(void *self);
	void (*destroy)(void *self);
	void *(*ChangingSize)(void *self);
} Object;

int Object_init(void *self);
void *Object_move(void *self,Direction direction);
void Object_destroy(void *self);
void *Object_ChangingSize(void *self);
void *Object_new(size_t size, Object proto,int Obj_size[2], int Obj_pos[2]);

//NEW is used as a short name of Object_new
//T#Proto is a protocol that any input T(object)'s functions will follow
//if they aren't set
//T is size_t size, N is int size[2],M is int pos[2]
#define NEW(T,N,M) Object_new(sizeof(T), T##Proto, N,M)
#define _(N) proto.N

#endif

