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
	char *currKey = 0;
	void *currVal = 0;
	struct JObject *currObj = 0;
	struct JObject *currParent = 0;

	//root obj
	struct JObject *root = createJObject();

	currObj = root;

	//skip first '{'
	for(u_int32_t i = 1; text[i] != 0; i++){

		switch(text[i]){
		
			//start of object
			case '{':
				//declare object / start of child
				
				break;
			//next obj/string/etc is value for currKey
			case ':':
				bools.
				
				//

				break;
			//end of object and child
			case '}':

				//append object or string value
				if(currObj){
					appendToJObject(currParent, (struct Element){currKey, currVal, 1});
				}else{
					;
				}

				//pop back to parent


				break;
		
			//save key/value string	
			case '"':
				//toggle bool
				in_string ^= in_string;

				break;
		
			case ',':
				//child done
				appendToJObject(currObj, (struct Element){currKey,currVal,0});
				break;
			
			default:
				continue;
		}

	}

	free(text);
	return 0;
}
