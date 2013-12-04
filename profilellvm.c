#include<stdio.h>
#include<string.h>
#include<malloc.h>
#include <stdlib.h>

typedef struct edge{
	char * parent;
	char * child;
	int visited;
	struct edge *next;
} edge;
static edge * list=NULL;

void profile_dump(){
	const char* filename="tracedump.log";
	FILE *file=fopen(filename,"w");
	edge *node=list;
	if(file){
		while(node){
			fprintf(file,"\n%s %s %d\n",node->parent,node->child,node->visited);
			node=node->next;
		}
	}	
}

void gen_profile(const void *s1, const void *s2)
{
	const char *from=(char *)s1;
	const char *to=(char *)s2;
	edge * node,*last;
	if(list==NULL)
	{
		list=(edge *)malloc(sizeof(edge));
		node=list;
		node->parent=(char *)malloc(sizeof(char)*strlen(from));
		strcpy(node->parent,from);
		node->child=(char *)malloc(sizeof(char)*strlen(to));
		strcpy(node->child,to);		
		node->visited=1;
		node->next=NULL;
		atexit(profile_dump);
	}
	else{
		node=list;
		int found=0;
		while(node){
			if(strcmp(node->parent,from)==0 && strcmp(node->child,to)==0){
				found=1;
				break;
			}
			last=node;
			node=node->next;
		}
		if(found==1)
			(node->visited)++;
		else{
			last->next=(edge *)malloc(sizeof(edge));
			node=last->next;
			node->parent=(char *)malloc(sizeof(char)*strlen(from));
	                strcpy(node->parent,from);
        	        node->child=(char *)malloc(sizeof(char)*strlen(to));
                	strcpy(node->child,to);
                	node->visited=1;
                	node->next=NULL;
		}
		
	}
 //       profile_dump();
}
