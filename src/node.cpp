#include "node.h"
#include "util.h"
#include "style.h"

#include <sstream>

namespace dom {

Node::Node(std::string& stype) : type(stype), parent(nullptr), attributes(nullptr) {}

std::string& Node::getInnerHTML() { return innerHTML; }

void Node::setInnerHTML(std::string& innerHTML) { this->innerHTML = innerHTML; }

Node* Node::getParent(){ return parent; }

void Node::setParent(Node* parent) { this->parent = parent; }

void Node::appendChild(Node* child)
{
	// Lazy initializing the vector to prevent memory allocation for leaf nodes
	if (children == nullptr)
		children = new std::vector<Node*>();

	child->setParent(this);
	children->push_back(child);
}

std::string Node::toString()
{
	std::ostringstream out;
	out << type;

	if (attributes != nullptr)
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
int Node::matches(SelectorPair& selPair)
{
	if (!attributes) return 0;

	auto id = util::mapGet<std::string, std::string>(*attributes, "id");
	auto classNamesStr = util::mapGet<std::string, std::string>(*attributes, "class");

	if (selPair.first.empty())
		// No ID in identifier, compare only on class
		return selPair.second == util::tokenize(classNamesStr, ' ');
	else if (selPair.second.empty())
		// No classes in identifier, compare only ID
		return selPair.first == id;
	else
		// Compare both
		return selPair.first == id && selPair.second == util::tokenize(classNamesStr, ' ');
}

void Node::forEachChild(void(*lambda)(Node*))
{
	if (children != nullptr)
		for (auto child : *children) lambda(child);
}

} // namespace dom
