#include <stdio.h>

void f1(int i ,int j) {
	if(i < j) {
		i = 100+i;
	}else if(i == j) {
		j = i+j;
		f2(j);
	}else {
		j= i - j;
		f2(j);
	}
}

void f2(int p ) {

	if(p >100) {
		printf("%d\n",p);
	}else if( p < 89) {
		f3();
	}

}
void f3() {
	printf("This is %s",__func__);
}


int main() {
	int i  = 200 ;
	int j = 234;
	for(int p = 0 ; p< j ;++p) {
		if( i < j) {
			p = i;
			f1(p,j);
		}		
	}
	return 0;
}
