#ifndef PARSER_H
#define PARSER_H

#include <fstream>

#include "node.h"
#include "tree.h"

namespace dom {

	class Parser
	{
	private:
		std::vector<std::string> src;

		static std::string getTagName(std::string& tag);

		std::map<std::string, std::string>* getAttributes(std::string& tag);
	public:
		Parser(char* htmlSrc);

		Tree* parse();
	};

}   // namespace dom

#endif  // PARSER_H