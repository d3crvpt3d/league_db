#pragma once

//int, double, char*
union Value {
	int 	as_integer;
	float 	as_decimal;
	char 	*as_string;
};

struct JObject {
	char *key;
	union Value value;

	//dyn alloc
	struct JObject *childs;
};

//malloc's itself with key
struct JObject *createJObject(char *key);

//free's recusively childs and itself
int deleteJObject(struct JObject *jobject);
