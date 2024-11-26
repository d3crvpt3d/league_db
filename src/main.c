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

	struct Bools {
		char string: 1;
		char key_value: 1;
	};

	struct Bools bools = {0};

	//parse json
	char in_string = 0;
	char Comma_Colon = 0;
	
	char *currKey = 0;
	void *currVal = 0;
	char *strStart = 0;
	char is_obj = 0;
	
	struct JObject *currObj = createJObject();
	struct JObject *currParent = 0;

	//root obj
	struct JObject *root = createJObject();

	currObj = root;

	//skip first '{'
	for(u_int32_t i = 1; text[i] != 0; i++){

		//dont interpret if in string
		if(in_string){

			//end of string
			if(text[i] == '"'){
				in_string = 0;
				text[i] = '\0';
			}
			continue;
		}

		switch (text[i]){
		
			//start of string
			case '"':
				in_string=1;
				strStart = &text[i]+1;//first character of literal
				break;

			//string in front was a key
			case ':':
				Comma_Colon = 1;//colon last
				currKey = strStart;
				break;

			//obj to parent and create new child
			case '{':
				currParent = currObj;
				currObj = createJObject();
				
				currObj->parent = currParent;
				break;

			//end of value
			case ',':
				Comma_Colon = 0;//comma last
				appendToJObject(currObj, (struct Element){currKey, strStart, is_obj});
				break;

			case '}':
				is_obj = 1;
				break;
		}

	}

	free(text);
	return 0;
}
