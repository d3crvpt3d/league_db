#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include "../include/objects.h"
#include "../include/char_switch.h"

//info:
//we can do var = strtok(NULL, text, delimiter),
//where var will be &text[x]


//if token is "name" then copy next token to *name
void getValues(char *name, char *depth, char *token, int wasitem){
	
	//get name
	if(strcmp(token, "name")){
		free(name);//free old string
		name = strdup(strtok(NULL, "\""));
	
		//has name so is item
		wasitem = 1;

	}

	//get depth
	if(strcmp(token, "depth")){
		free(depth);//free old string
		depth = strdup(strtok(NULL, "\""));
	}
}

//if depth = 2 and ',' then a new item starts
void getId(char *id, char *token){
	if(strcmp(token, ",")){
		free(id);//free old string
		id = strdup(strtok(NULL, "\""));
	}
}

int main(int argc, char **argv){

	//TODO connect DuckDB
	//...

	//Read file into buffer
	FILE *fptr;
	fptr = fopen(argv[1], "r");
	if(!fptr){
		printf("No such File %s\n", argv[1]);
		return -1;
	}
	fseek(fptr, 0L, SEEK_END);
	int size = ftell(fptr); 
	char *text = malloc(sizeof(char) * size+1);

	rewind(fptr);

	fread(text, 1, size, fptr);
	fclose(fptr);

	//pre things for token loop
	char token = text[0];
	u_int32_t jdepth = 0;

	//go through each token
	while(token != NULL){
		char_switch();
	}

	free(text);
	return 0;
}
