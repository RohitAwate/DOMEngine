#ifndef NODE_H
#define NODE_H

#include <iostream>
#include <vector>
#include <map>
#include <functional>

namespace dom {

	struct Selector
	{
		std::string type;
		std::string id;
		std::vector<std::string> classNames;

		std::string toString() const;
	};

	class Node
	{
	private:
		std::string type;
		std::string innerHTML;
		Node *parent;
		std::vector<Node*> *children;
		std::map<std::string, std::string> *attributes;

		friend class Tree;
		friend class Parser;
	public:
		Node(const std::string& stype);

		std::string& getInnerHTML();

		void setInnerHTML(const std::string& innerHTML);

		Node* getParent();

		void setParent(Node* parent);

		void appendChild(Node* child);

		std::string toString() const;
		
		bool matches(const Selector& selector);

		void forEachChild(std::function<void(const Node* child)>& lambda) const;

		void forEachAttribute(std::function<void(const std::string&, const std::string&)>& lambda) const;
	};

} // namespace dom

#endif  // NODE_H