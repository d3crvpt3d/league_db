#include "../include/jobject.h"
#include <stdlib.h>

struct JObject *createJObject(char *key){
	
	struct JObject *this = malloc(sizeof(struct JObject));

	//TODO

	return 0;
}

//free childs, then yourself (recursive)
int deleteJObject(struct JObject *jobject){
	
	while(1){
		//TODO
	}

	free(jobject);
	return 0;
}
