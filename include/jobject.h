#pragma once

#include <sys/types.h>

struct Data {
	char *string;
	struct JObject *obj;
};

struct Value {
	char *key;
	struct Data data;
};

struct JObject {
	long space;
	u_int size;
	struct Value *array;
};

//malloc's itself with key
struct JObject *createJObject(struct Value v);

//free's recusively childs and itself
int deleteJObject(struct JObject *obj);

//insert
int appendToJObject(struct JObject *, struct Value);
