#pragma once

#include <sys/types.h>

typedef struct JObject {
	char *raw;//raw string to be parsed

	u_int8_t is_obj: 1;//true if raw[0] == '{'
	long space;
	long length;
	
	char *key;
	void **value;//dyn array of object *, char *, int or double
} JObject;

void initJObject(JObject *address, char *key, char *raw);

//free's recusively childs and itself
int deleteJObject(JObject *obj);

//insert
int appendToJObject(JObject *, JObject *);

int parseObject(JObject *object);
	
int recursiveParseJObject(JObject *obj);
