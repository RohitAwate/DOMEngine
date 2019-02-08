#include <iostream>
#include "engine.h"

int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		std::cout << "Usage: dom-engine <filename>" << std::endl;
		return 1;
	}

	dom::Parser parser(argv[1]);
	dom::Tree* dtree = parser.parse();

	dom::Shell* shell = new dom::Shell(dtree);
	shell->start();
}