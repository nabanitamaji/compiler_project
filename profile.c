#include<stdio.h>

void nabanita_profile(const void *s1,const void *s2)
{
	const char *s3=(char *)s1;
	const char *s4=(char *)s2;
	fprintf(stderr,"\n%s->%s\n",s3,s4);
}
