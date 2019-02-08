#include "tree.h"
#include "style.h"

#include <sstream>

#define INDENT_WIDTH 4

namespace dom {

Tree::Tree(Node* root_) : root(root_) {}

Node* Tree::match(std::string& identifier)
{
	if (root->children->at(0)->matches(identifier))
	{
		std::cout << TEXT_BOLD << root->children->at(0)->toString() << TEXT_RESET;
	}
}

void Tree::print()
{
	print(root, "", true);
}

bool Tree::isBuilt()
{
	return root != NULL;
}

std::string Tree::generateTreePrefix(std::string prefix, bool isTail)
{
	std::ostringstream childPrefix;
	childPrefix << prefix;
	if (isTail) childPrefix << " ";
	else childPrefix << "│";

	for (int j = 1; j < INDENT_WIDTH; j++)
			childPrefix << " ";

	return childPrefix.str();
}

/*
	Prints the Tree to the console similar to the UNIX 'tree' command.
	Inspired by this excellent answer on StackOverflow: https://stackoverflow.com/a/8948691/6948907

	It first prints the prefix for the current node. This includes the continuing branches
	of its ancestors. Next, based on whether the current node is a leaf or not, respective
	Unicode box-drawing characters are drawn.

	Unlike the StackOverflow answer, I wanted to keep the indent width mutable rather than hardcoding
	it into the prefix strings, so the horizontal line Unicode character is printed out as per the
	indentation width. Finally, the current node is printed.

	Now, we add the append the extending branches from this node to the prefix string.
	All children except the last are recursively printed with isTail set to false.
	The last child is then printed in a recursive call with isTail set to true.
*/
void Tree::print(Node* current, std::string prefix, bool isTail)
{
	std::cout << prefix;
	if (isTail) std::cout << "└";
	else std::cout << "├";

	for (int i = 0; i < INDENT_WIDTH; i++) std::cout << "─";
	std::cout << " " << current->toString() << std::endl;

	if (!current->children) return;

	auto childPrefix = generateTreePrefix(prefix, isTail);

	for (int i = 0; i < current->children->size() - 1; i++)
		print(current->children->at(i), childPrefix, false);

	print(current->children->back(), childPrefix, true);
}

} // namespace dom
