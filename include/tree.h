#ifndef TREE_H
#define TREE_H

#include "node.h"

namespace dom {

	class Tree
	{
	private:
		Node* root;
	public:
		Tree(Node* root_);

		Node* match(std::string& identifier);

		void print();

		bool isBuilt();
	private:
		static std::string generateTreePrefix(std::string prefix, bool isTail);

		static void print(Node* current, std::string prefix, bool isTail);

		static SelectorPair tokenizeSelector(std::string& selector);

		Node* match(Node* node, SelectorPair& selPair);
	};

} // namespace dom

#endif // TREE_H