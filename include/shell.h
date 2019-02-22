#ifndef SHELL_H
#define SHELL_H

#include "node.h"
#include "tree.h"
#include "interpreter.h"

namespace dom {

	class Shell
	{
	private:
		Tree* dtree;
		Interpreter* interpreter;
	public:
		Shell(Tree* _dtree);

		void start();
	private:
		void startSubCmdLoop(Node* selected) const;
	};

} // namespace dom

#endif  // SHELL_H