#include <stdlib.h>
#include <sys/types.h>

#include "../include/jobject.h"

//trim whitespaces
char *trim(char *in){

	u_int64_t front = 0;
	u_int64_t back = 0;

	//put back at last place
	while(in[back] != '\0'){
		back++;
	}
	
	//trim front
	while (in[front++] == ' ' || in[front] == '\n' || in[front] == '\r' || in[front] == '\t'){
		front++;
	}

	//trim back
	do {
		back--;	
	} while (in[back] == ' ' || in[back] == '\n' || in[back] == '\r' || in[back] == '\t');

	in[back+1] = '\0';

	return in+front;//return trimmed string
}

void initJObject(JObject *address, char *key, char *raw_value){
	
	//trimmes raw to make shure '{' of object is at raw[0]
	*address = (JObject) {trim(raw_value), raw_value[0] == '{', 1, 0, key, NULL};
}

//free childs, then yourself (recursive)
int deleteJObject(JObject *obj){

	//recursive call for childs
	for(int i = 0; i < obj->length; i++){

		//
		if( ((JObject *) obj->value[i])->is_obj ){
		
			if(!deleteJObject(obj->value[i])){
				return -3;
			}
		
		}

	}

	//free array memory
	free(obj->value);
	obj->value = 0;

	//free self
	free(obj);
	obj = 0;
	
	return 0;
}

int appendToJObject(JObject *self, JObject *obj){

	JObject *tmp;

	//realloc if full
	if(self->space == self->length){

		//double space
		self->space = self->space * 2;
		tmp = realloc(self->value, self->space);
	
		//check if realloc was successful
		if(!tmp){
			self->space = self->space / 2;
			return -1;
		}
	}

	//insert value and increment length
	self->value[self->length++] = obj;

	return 0;
}

//turn raw into Elements aka. key/value pairs
int parseObject(JObject *object){

	//if escaped by '"' or not
	int8_t in_string = 0;
	char *strStart = 0;
	
	//read value as string into raw of child object
	int8_t in_value = 0;
	char *valStart = 0;

	char *currKey = 0;

	u_int64_t pos = 0;

	//iterate raw data
	while(object->raw[pos] != '\0'){

		//check if end of value then append object with their raw data to parent
		if(object->raw[pos] == ',' && !in_string){

			JObject tmp;

			object->raw[pos] = '\0';//set end of string

			initJObject(&tmp, currKey, valStart);

			in_value = 0;

			continue;
		}else if(in_value){//skip if in value
			continue;
		}

		//skip if escaped or change state
		if(object->raw[pos] == '"'){
		
			//save pos of start/end of string
			if(in_string){
				object->raw[pos] = '\0';
				currKey = strStart;
			}else{
				strStart = &object->raw[pos+1]; 
			}

			in_string = !in_string;
		}else if(in_string){
			continue;
		}

		//switch on non escaped spectial characters
		switch(object->raw[pos]){
		
			//colon = value start
			case ':':
				in_value = 1;
				valStart = &object->raw[pos+1];
				break;
		}
	}

	return 0;
}

int recursiveParseJObject(JObject *obj){

	if(!obj->is_obj){
		return 0;
	}

	parseObject(obj);

	for(u_int64_t i = 0; i < obj->length; i++){
		recursiveParseObject(obj->value[i]);
	}

	return 0;
}
