#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

#include "../include/jobject.h"


int main(int argc, char **argv){

	//TODO connect DuckDB
	//...

	//Read file into buffer
	FILE *fptr;
	fptr = fopen(argv[1], "r");
	
	if(!fptr){fprintf(stderr, "No such File %s\n", argv[1]); return -1;}
	
	fseek(fptr, 0L, SEEK_END);
	int size = ftell(fptr); 
	char *text = malloc(sizeof(char) * size+1);
	
	if(!text){fprintf(stderr, "Can't get Heap Memory"); return -2;}
	
	rewind(fptr);
	fread(text, 1, size, fptr);
	fclose(fptr);

	//parse json
	char in_string = 0;
	char *currKey = 0;
	void *currData = 0;
	struct JObject *currObj = 0;

	//root obj
	struct JObject *root = createJObject();

	currObj = root;

	//skip first '{'
	for(int i = 1; text[i] != 0; i++){

		switch(c){
		
			//start of object
			case '{':
				//create new object as child of current
				appendToJObject(&currObj, {});
				break;
		
			//end of object
			case '}':
				//do smth
				break;
		
			//save key/value string	
			case '"':
				//toggle bool
				in_string ^= in_string;

				break;
		
			case ',':
				//next child
				
				break;
		}

	}

	free(text);
	return 0;
}
