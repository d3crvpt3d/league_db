#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

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
	fptr = fopen("test_items.json", "r");
	fseek(fptr, 0L, SEEK_END);
	int size = ftell(fptr); 
	char *text = malloc(sizeof(char) * size+1);

	rewind(fptr);

	fread(text, 1, size, fptr);
	fclose(fptr);

	//pre things for token loop
	char *token = strtok(text, "\"");
	u_int32_t jdepth = 0;

	//item values
	//malloc to free
	char *itemId = malloc(1);
	char *itemName = malloc(1);
	char *itemDepth = malloc(1);
	int wasitem = 0;

	//go through each token
	while(token != NULL){
	
		//keep track of json-depth
		if (strcmp(token, "{")){
			jdepth++;

			//first entry
			if(jdepth == 2){
				itemId = strtok(NULL, "\"");
			}

		}else if (strcmp(token, "}")){

			//if item ended
			if(jdepth == 2 && wasitem){
				printf("INSERT INTO item VALUES (%s,$$%s$$,%s);\n", itemId, itemName, itemDepth);//never do this!!!
				//reset itemdepth
				itemDepth = "1";
				wasitem = 0;
			}

			jdepth--;
		}

		switch (jdepth){
			case 2: getId(itemId, token); break;
			case 3: getValues(itemName, itemDepth, token, wasitem); break;
		}

		token = strtok(NULL, "\"");
	}

	free(text);
	return 0;
}
