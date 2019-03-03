#include "tree.h"
#include "style.h"
#include "util.h"

#include <sstream>

#define INDENT_WIDTH 4

namespace dom {

	Tree::Tree(Node* root) : root(root) {}

	/*
		Private delegate of dom::Tree::match(std::string) which
		performs a recursive depth-first traversal of the tree
		and returns the first node that matches the selector.
	*/
	Node* Tree::match(Node* node, Selector& selector)
	{
		if (node->matches(selector)) return node;

		if (node->children)
		{
			for (auto child : *(node)->children)
			{
				if (child->matches(selector)) return child;
				else
				{
					auto matchedChild = match(child, selector);
					if (matchedChild != nullptr) return matchedChild;
				}
			}
		}

		return nullptr;
	}

	// Returns the first node in the tree that matches the given identifier
	Node* Tree::match(std::string& selector)
	{
		Selector sel = tokenizeSelector(selector);
		return match(this->root, sel);
	}

	void Tree::print()
	{
		print(root, "", true);
	}

	bool Tree::isBuilt()
	{
		return root != nullptr;
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
		Prints the tree to the console similar to the UNIX 'tree' command.
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

	std::string Tree::toHTML()
	{
		if (root == nullptr) return "";

		return root->children->at(0)->toHTML();
	}

	/*
		Tokenizes the selector and returns a Selector object which
		contains the type, ID and the classes.

		If a selector contains multiple IDs or types, only the first ones are considered.
	*/
	Selector Tree::tokenizeSelector(std::string& selector)
	{
		int len = selector.length();
		int curr = 0;

		std::string argType;
		std::string argId;
		std::vector<std::string> argClassNames;
		char ch;
		while (curr < len)
		{
			ch = selector[curr];
			if (ch == '.')
			{
				curr++;
				int start = curr;

				while (selector[curr] != '.' && selector[curr] != '#' && selector[curr] != '@' && curr < len) curr++;

				argClassNames.push_back(selector.substr(start, curr - start));
			}
			else if (ch == '#')
			{
				curr++;
				int start = curr;

				while (selector[curr] != '.' && selector[curr] != '#' && selector[curr] != '@' && curr < len) curr++;

				if (argId != "") continue;	// Only matching the first ID
				argId = selector.substr(start, curr - start);
			}
			else if (ch == '@')
			{
				curr++;
				int start = curr;

				while (selector[curr] != '.' && selector[curr] != '#' && selector[curr] != '@' && curr < len) curr++;

				if (argType != "") continue;	// Only matching the first type
				argType = selector.substr(start, curr - start);
			}
			else curr++;
		}

		Selector sel { argType, argId, argClassNames };
		return sel;
	}

} // namespace dom
