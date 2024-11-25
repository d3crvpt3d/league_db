#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include "../include/objects.h"
#include "../include/jstack.h"

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

	//stack for json
	struct JStack jstack = {0};

	//root obj
	struct Json root = malloc(sizeof(struct Json));

	for(int i = 0; text[i] != 0; i++){

		switch(c){
		
			//start of object
			case '{':
				//create new object as child of current
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
