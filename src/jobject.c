#include "../include/jobject.h"
#include <stdlib.h>

struct JObject *createJObject(struct Value v){
	
	//get memory for this object
	struct JObject *this = malloc(sizeof(struct JObject));

	//insert first key/data pair into child array
	this->array = malloc(sizeof(struct Value));
	if(!this->array){
		return NULL;//cant malloc
	}
	this->array[0] = v;
	this->size = 1;
	this->space = 1;

	return this;
}

//free childs, then yourself (recursive)
int deleteJObject(struct JObject *obj){

	//if data of parent was not a object
	if(!obj){
		return 0;
	}
	
	//delete all objects from parent
	for(int i = 0; i < obj->size; i++){
	
		if(!deleteJObject(obj->array[i].data.obj)){
			return -3;
		}
	}

	//free array memory
	free(obj->array);

	//free self
	free(obj);
	
	return 0;
}

int appendToJObject(struct JObject *obj, struct Value value){

	struct Value *tmp = 0;

	//realloc if full
	if(obj->space == obj->size){

		//double space
		obj->space = obj->space < 1;
		tmp = realloc(obj->array, obj->space);
	}

	//check if realloc was successful
	if(!tmp){
		return -1;
	}
	obj->array = tmp;

	//insert value and increment size
	obj->array[obj->size++] = value;

	return 0;
}
