#ifndef NODE_H
#define NODE_H

#include <iostream>
#include <vector>
#include <map>

namespace dom {

typedef std::pair<std::string, std::vector<std::string>> SelectorPair;

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
	Node(std::string& stype);

	std::string& getInnerHTML();

	void setInnerHTML(std::string& innerHTML);

	Node* getParent();

	void setParent(Node* parent);

	void appendChild(Node* child);

	std::string toString();

	/*
	 *	Parses the identifier and compares it with the ID and class of the node.
	 * 	Returns:
	 * 	 1 - if match found
	 * 	 0 - if node has no attributes or doesn't match
	 * 	-1 - if invalid identifier is supplied, such as one containing more than one IDs
	 */
	int matches(SelectorPair& selPair);
};

} // namespace dom

#endif  // NODE_H