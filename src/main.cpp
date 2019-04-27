#include <iostream>
#include "engine.h"

int main(int argc, char *argv[])
{
	if (argc == 2)
	{
		dom::Parser parser(argv[1]);
		dom::Tree* tree = parser.parse();

		dom::Shell shell(tree);
		shell.start();
	}
	else if (argc == 3)
	{
		dom::Parser parser(argv[1]);
		dom::Tree* tree = parser.parse();

		dom::ScriptRunner runner(tree, argv[2]);
		runner.run();
	}
	else
	{
		std::cout << "Invalid or no arguments supplied." << std::endl;
		std::cout << "\nUsage:" << std::endl;
		std::cout << "For interactive shell:\ndom-engine <html-file>" << std::endl;
		std::cout << "\nFor executing scripts:\ndom-engine <html-file> <script-file>" << std::endl;
		return 1;
	}
}