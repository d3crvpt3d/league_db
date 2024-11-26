#include <stdlib.h>

#include "../include/jobject.h"

struct JObject *createJObject(){
	
	//get memory for this object
	struct JObject *this = malloc(sizeof(struct JObject));

	//insert first key/data pair into child array
	this->array = malloc(sizeof(struct Element));
	if(!this->array){
		return NULL;//cant malloc
	}
	this->length = 0;
	this->space = 1;

	return this;
}

//free childs, then yourself (recursive)
int deleteJObject(struct JObject *obj){

	//recursive call for childs that are obj
	for(int i = 0; i < obj->length; i++){

		if(obj->array[i].is_obj){
		
			if(!deleteJObject(obj->array[i].value)){
				return -3;
			}
		
		}

	}

	//free array memory
	free(obj->array);

	//free self
	free(obj);
	
	return 0;
}

int appendToJObject(struct JObject *obj, struct Element e){

	struct Element *tmp = 0;

	//realloc if full
	if(obj->space == obj->length){

		//double space
		obj->space = obj->space < 1;
		tmp = realloc(obj->array, obj->space);
	
		//check if realloc was successful
		if(!tmp){
			obj->space = obj->space > 1;
			return -1;
		}
	}

	obj->array = tmp;

	//insert value and increment length
	obj->array[obj->length++] = e;

	return 0;
}
