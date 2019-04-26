#ifndef SHELL_H
#define SHELL_H

#include "node.h"
#include "tree.h"
#include "vm.h"

namespace dom {

	class Shell
	{
	private:
		Tree* tree;
		VirtualMachine* vm;
	public:
		Shell(Tree* tree);

		void start();
	private:
		void startSubCmdLoop(Node* selected) const;
	};

} // namespace dom

#endif  // SHELL_H