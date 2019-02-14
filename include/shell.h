#ifndef SHELL_H
#define SHELL_H

#include <regex>

#include "node.h"
#include "tree.h"

namespace dom {

class Shell
{
private:
	Tree* dtree;
	const static std::regex SELECTOR_CMD_FORMAT;
public:
	Shell(Tree* _dtree);

	void start();
private:
	void resolveCmd(std::string& cmd) const;

	void resolveSubCmd(std::string& subCmd, Node* selected) const;

	void startSubCmdLoop(Node* selected) const;
};

} // namespace dom

#endif  // SHELL_H