#ifndef PARSER_H
#define PARSER_H

#include <fstream>

#include "node.h"
#include "tree.h"

namespace dom {

	class Parser
	{
	private:
		char* filename;
		std::ifstream fd;
	public:
		Parser(char* fname);

		Tree* parse();
	};

}   // namespace dom

#endif  // PARSER_H