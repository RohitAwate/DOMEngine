#include <iostream>
#include "engine.h"

#define Log(x) std::cout << (x) << std::endl

int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		Log("Usage: dom-engine <filename>");
		return 1;
	}

	Log(argv[1]);
}