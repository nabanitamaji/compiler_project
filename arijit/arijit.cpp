#include <cstdio>
#include <iostream>
using namespace std;

class Base {
	public:
		inline void test();
};

void Base::test() {
	cout<<"Name of the function "<<__func__<<endl;
}

inline void testFunction() {
	int i = 0 ; 
	for(; i < 100;++i) {
		i = i+10;
		printf("This is a dummy loop\n");
	}
}

int main() {
	const char *name = "Arijit";
	testFunction();
	return 0;
}
