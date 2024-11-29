#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

#include "../include/jobject.h"

char *readFileToBuffer(char *filename){
	
	//Read file into buffer
	FILE *fptr;
	fptr = fopen(filename, "r");
	
	if(!fptr){fprintf(stderr, "No such File %s\n", filename); return NULL;}
	
	fseek(fptr, 0L, SEEK_END);
	int size = ftell(fptr); 
	char *text = malloc(sizeof(char) * size+1);
	
	if(!text){fprintf(stderr, "Can't get Heap Memory"); return NULL;}
	
	rewind(fptr);
	fread(text, 1, size, fptr);
	fclose(fptr);
	
	return text;
}

int main(int argc, char **argv){

	//TODO connect DuckDB
	//...
	
	char *text = readFileToBuffer(argv[1]);

	//parse json
	char in_string = 0;
	char Comma_Colon = 0;
	
	char *currKey = 0;
	void *currVal = 0;
	char *strStart = 0;
	char is_obj = 0;
	
	//root obj
	JObject root;
	
	//init root
	initJObject(&root, NULL, text);

	//recursive parse raw from childs
	recursiveParseJObject(&root);

	free(text);
	return 0;
}
