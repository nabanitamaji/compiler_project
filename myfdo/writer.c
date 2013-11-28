#include <stdio.h>


void fdo_write(void *from, void *to) {
	char *caller = (char*) from;
	char *callee = (char*) to;
	printf("Callee is %s and caller is %s \n" , callee, caller);
}
