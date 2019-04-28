#ifndef REPL_H
#define REPL_H

#include "node.h"
#include "tree.h"
#include "vm.h"

namespace dom {

	class REPL
	{
	private:
		Tree* tree;
		VirtualMachine* vm;
	public:
		REPL(Tree* tree);

		void start();
	private:
		void startSubCmdLoop(Node* selected) const;
	};

} // namespace dom

#endif  // REPL_H