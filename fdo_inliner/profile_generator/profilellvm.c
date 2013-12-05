#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>

typedef struct Edge{
	char * parent;
	char * child;
	int visited;
	struct Edge *next;
} Edge;
static Edge * list=NULL;

void profile_dump(){
	const char* filename="tracedump.log";
	FILE *file=fopen(filename,"w");
	Edge *node=list,*temp;
	if(file){
		while(node){
			fprintf(file,"\n%s %s %d\n",node->parent,node->child,node->visited);
			node=node->next;
		}
		fclose(file);
	}	
	node=list;
	while(node){
		temp=node->next;
		free(node);
		node=temp;
	}
}

void gen_profile(const void *s1, const void *s2) {
	const char *from=(char *)s1;
	const char *to=(char *)s2;
	Edge *node,*last;
	if(from==NULL || to==NULL)
		return;
	if(list==NULL)
	{
		node = (Edge *)malloc(sizeof(Edge));
		node->parent=(char *)malloc(sizeof(char)*strlen(from)+1);
		strcpy(node->parent,from);
		node->child=(char *)malloc(sizeof(char)*strlen(to)+1);
		strcpy(node->child,to);		
		node->visited=1;
		node->next=NULL;
		list = node;
		atexit(profile_dump);
	}else{
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
                if(found ==1){
			node->visited++;
		}else {
			node = (Edge*) malloc(sizeof(Edge));
			node->next = NULL;
			node->parent = (char *)malloc(sizeof(char)*(strlen(from)+1));
			strcpy(node->parent,from);
			node->child = (char *) malloc(sizeof(char)*(strlen(to)+1));
			strcpy(node->child,to);
			node->visited = 1;
			last->next = node;
		}
	}
}
