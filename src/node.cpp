#include "node.h"
#include "util.h"
#include "style.h"

#include <sstream>

namespace dom {

Node::Node(std::string& stype) : type(stype), parent(NULL), attributes(NULL) {}

std::string& Node::getInnerHTML() { return innerHTML; }

void Node::setInnerHTML(std::string& innerHTML) { this->innerHTML = innerHTML; }

Node* Node::getParent(){ return parent; }

void Node::setParent(Node* parent) { this->parent = parent; }

void Node::appendChild(Node* child)
{
	// Lazy initializing the vector to prevent memory allocation for leaf nodes
	if (children == NULL)
		children = new std::vector<Node*>();

	child->setParent(this);
	children->push_back(child);
}

std::string Node::toString()
{
	std::ostringstream out;
	out << type;

	if (attributes != NULL)
	{
		auto id = util::mapGet<std::string, std::string>(*attributes, "id");
		if (id != "") out << COLOR_RED << "#" << id << COLOR_RESET;

		auto className = util::mapGet<std::string, std::string>(*attributes, "class");
		if (className != "") out << COLOR_BLUE << "." << className << COLOR_RESET;
	}

	return out.str();
}

/*
	*	Parses the identifier and compares it with the ID and class of the node.
	* 	Returns:
	* 	 1 - if match found
	* 	 0 - if node has no attributes or doesn't match
	* 	-1 - if invalid identifier is supplied, such as one containing more than one IDs
	*/
int Node::matches(std::string& identifier)
{
	if (!attributes) return 0;
	if (identifier == "") return -1;

	auto id = util::mapGet<std::string, std::string>(*attributes, "id");
	auto classNamesStr = util::mapGet<std::string, std::string>(*attributes, "class");
	if (id == "" && classNamesStr == "") return 0;

	int len = identifier.length();
	int curr = 0;

	std::string argId;
	std::vector<std::string> argClassNames;
	char ch;
	while (curr < len)
	{
		ch = identifier[curr];
		if (ch == '.')
		{
			curr++;
			int start = curr;

			while (identifier[curr] != '.' && identifier[curr] != '#' && curr < len) curr++;

			argClassNames.push_back(identifier.substr(start, curr - start));
		}
		else if (ch == '#')
		{
			curr++;
			int start = curr;

			while (identifier[curr] != '.' && identifier[curr] != '#' && curr < len) curr++;

			if (argId != "") return -1;
			argId = identifier.substr(start, curr - start);
		}
		else curr++;
	}

	if (argId != id) return 0;
	return argClassNames == util::tokenize(classNamesStr, ' ');
}

} // namespace dom
