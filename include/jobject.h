#pragma once

typedef struct JObject {
	char *raw;//raw string to be parsed

	int is_obj: 1;//true if raw[0] == '{'
	long space;
	long length;
	
	char *key;
	void **value;//dyn array of object *, char *, int or double
} JObject;

//malloc's itself with key and value
void createJObject(JObject *address, char *key, char *raw);

//free's recusively childs and itself
int deleteJObject(JObject *obj);

//insert
int appendToJObject(JObject *, JObject *);

int parseObject(JObject *object);
	
int recursiveParseObject(JObject *obj);
