#include "node.h"
#include "util.h"
#include "style.h"

#include <sstream>

namespace dom {

	std::string Selector::toString() const
	{
		std::ostringstream out;
		
		if (!type.empty())
			out << "@" << type;
		if (!id.empty())
			out << "#" << id;
		if (!classNames.empty())
			for (auto className: classNames)
				out << "." << className;

		return out.str();
	}

	Node::Node(const std::string& type) : type(type), parent(nullptr), attributes(nullptr), children(nullptr) {}

	std::string& Node::getInnerHTML() { return innerHTML; }

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

	std::string Node::toString() const
	{
		std::ostringstream out;
		out << type;

		if (attributes != nullptr)
		{
			auto id = util::mapGet<std::string, std::string>(*attributes, "id");
			if (!id.empty()) out << COLOR_RED << "#" << id << COLOR_RESET;

			auto className = util::mapGet<std::string, std::string>(*attributes, "class");
			if (!className.empty()) out << COLOR_BLUE << "." << className << COLOR_RESET;
		}

		return out.str();
	}

	// Parses the identifier and compares it with the type, ID and classes of the node.
	bool Node::matches(const Selector& selector)
	{
		std::string id, classNamesStr;
		if (attributes)
		{
			id = util::mapGet<std::string, std::string>(*attributes, "id");
			classNamesStr = util::mapGet<std::string, std::string>(*attributes, "class");
		}

		if (selector.id.empty() && selector.classNames.empty() && selector.type.empty())
			return false;

		bool result = true;
		if (!selector.id.empty())
			result &= selector.id == id;

		if (!selector.type.empty())
			result &= selector.type == type;

		if (!selector.classNames.empty())
			result &= selector.classNames == util::tokenize(classNamesStr, ' ');

		return result;
	}

	void Node::forEachChild(std::function<void(const Node* child)>& lambda) const
	{
		if (children != nullptr)
			for (auto child : *children)
				lambda(child);
	}

	void Node::forEachAttribute(std::function<void(const std::string&, const std::string&)>& lambda) const
	{
		if (attributes != nullptr)
			for (auto itr : *attributes)
				lambda(itr.first, itr.second);
	}

} // namespace dom
