#pragma once

//int, double, char*
union Value {
	int 	as_integer;
	float 	as_decimal;
	char 	*as_string;
}

struct JStack {
	char *key;
	union Value value;

	//dyn alloc
	struct JStack *childs;
}

//malloc's itself with key
struct JStack *createJStack(char *key);

//free's recusively childs and itself
deleteJStack(struct JStack *jstack);
