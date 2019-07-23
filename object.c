#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include "object.h"



//Description: a function to mark that all function is initialized
//	Input: object pointer
//	Output: 1=> already initialized
//	Effect: annouce that all function is ready
int Object_init(void *self)
{
	// whenver this function is called
	// it means that object is initialized
	return 1;
}

//Description: a default function when object's movement is not set
//	Input: object pointer
//	Output: NULL=> dealth
//	Effect: Nothing
void *Object_move(void *self,Direction direction)
{
	printf("you are die!. GAMEOVER.\n");
	return NULL;
}

//Description: a default function to destroy an unnecessary object
//	Input: object pointer
//	Output: NULL=>dealth
//	Effect: nothing
void Object_destroy(void* self)
{
	Object *obj = self;
	if(obj)
	{
		free(obj);
	}
	else;
	printf("already destroyed. \n");
}
//Description: a default function to destroy an unnecessary object
//	Input: object pointer
//	Output: NULL=> no change
//	Effect: nothing
void *Object_ChangingSize(void* self)
{
	return NULL;
}

//Description: create a object with initialized functions and variable
//		Input:size of Object pointer, proto of object, object's size and position
//		Ouput: a new object that ready for the game
//		Effect: nothing
void *Object_new(size_t size, Object proto,int Obj_size[2], int Obj_pos[2])
{
	// setup defaults functions of Object
	if(!proto.init) proto.init = Object_init;
	else
	{
	}
	if(!proto.destroy) proto.destroy = Object_destroy;
	else
	{
	}
	if(!proto.ChangingSize)	proto.ChangingSize = Object_ChangingSize;
	else
	{
	}
	if(!proto.move) proto.move = Object_move;
	else
	{
	}
	
	//Now, start to create an object with size, protocol proto, position, state as 
	//parameters	
	
	Object *el = calloc(1,size);
	*el = proto;
	//=> proto is an object without size,position,.. it's just a pointer to function 
	//pointers
	
	//initialize position and size
	int i;
	for(i =0; i < 2;i++)
	{
		el->Obj_pos[i]=Obj_pos[i];
		el->Obj_size[i]=Obj_size[i];			
	}


	// initialize state
	if(!el->state)
	{
		el->state = DIE;		
	}
	else
	{
	}
	if(!el->init(el))
	{
		//sth happened that makes el not to be initialized
		el ->destroy(el);
		
		return NULL;
	}
	else
	{
		return el;
	}	
}

