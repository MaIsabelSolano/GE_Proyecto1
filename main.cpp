#include "Demo.h"

int main(int arg, char* sdf[]) {
	DemoGame demo = DemoGame();
	demo.setup();
	demo.run();
	return 0;
}