#ifndef SHELL_H
#define SHELL_H

#include <regex>

#include "node.h"
#include "tree.h"

namespace dom {

#define TEXT_BOLD 		"\e[1m"
#define TEXT_ITALIC 	"\e[3m"
#define TEXT_UNDERLINE 	"\e[4m"
#define TEXT_RESET 		"\e[0m"

class Shell
{
private:
	Tree* dtree;
	const static std::regex SELECTOR_CMD_FORMAT;
public:
	Shell(Tree* _dtree);

	void start();
private:
	void resolveCmd(std::string& cmd);
};

} // namespace dom

#endif  // SHELL_H