#pragma once

struct Element {
	char *key;
	void *value;	
	int is_obj;
};

struct JObject {
	
	//array
	long space;
	long length;
	struct Element *array;//dyn array of key/value pairs

	//parent back prop.
	struct JObject *parent;
};

//malloc's itself with key
struct JObject *createJObject(void);

//free's recusively childs and itself
int deleteJObject(struct JObject *obj);

//insert
int appendToJObject(struct JObject *, struct Element);
