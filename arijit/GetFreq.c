#include<stdio.h>
#include<string.h>
#include<malloc.h>
#define FUNCNAMESZ 500

typedef struct edge{
	char * parent;
	char * child;
	int visited;
	struct edge *next;
} edge;
edge * list=NULL;

void load_profile(const char*);

int getcount(const char*filename,const char* from,const char *to){
	if(list==NULL)
		load_profile(filename);
	edge *node=list;
	int ret=0;	
	while(list){
		if(strcmp(node->parent,from)==0 && strcmp(node->child,to)==0){
			ret=node->visited;
			break;
		}else
			node=node->next;
	}
	
	return ret;
}
void load_profile(const char* filename)
{
	FILE *file=fopen(filename,"r");
	const char *from=(char *)malloc(FUNCNAMESZ*sizeof(char));
	const char *to=(char *)malloc(FUNCNAMESZ*sizeof(char));
	int cnt;
	edge *node, *last; 

	while(fscanf(file,"%s %s %d",from,to,&cnt)==1){

		node=(edge *)malloc(sizeof(edge));
		node->parent=(char *)malloc(sizeof(char)*strlen(from));
		strcpy(node->parent,from);
		node->child=(char *)malloc(sizeof(char)*strlen(to));
		strcpy(node->child,to);
		node->visited=cnt;
		node->next=NULL;

		if(list==NULL){
			list=node;
			last=list;
		}
		else{
			last->next=node;
			last=last->next;
		}
	}
}
