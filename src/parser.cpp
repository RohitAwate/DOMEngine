#include "parser.h"
#include "util.h"

#include <stack>

namespace dom {

    Parser::Parser(char* htmlSrc)
    {
        std::ifstream fd(htmlSrc);

        std::string line;
        while (std::getline(fd, line))
        {
            this->src.push_back(line);
        }

        fd.close();
    }

    // Tokenizes the source line by line and builds a DOM tree.
    Tree* Parser::parse()
    {
        std::stack<std::string> stack;
        Node* root = new Node("document");
        Node* node = root;
        for (std::string line: src)
        {
            auto tokens = util::tokenizeTag(line);

            for (std::string token : tokens)
            {
                // Tag
                if (token[0] == '<')
                {
                    // Closing tag
                    if (token[1] == '/')
                    {
                        auto tagName = getTagName(token);

                        if (stack.empty() || stack.top() != tagName)
                        {
                            util::logSyntaxError("Stray tag: </" + tagName + ">");
                            std::exit(1);
                        }
                        else
                        {
                            // Return to parent and pop from stack
                            node = node->getParent();
                            stack.pop();
                        }
                    }

                    // Comment; ignore
                    else if (token[1] == '!')
                        continue;

                    // Opening tag
                    else
                    {
                        auto tagName = getTagName(token);

                        // Push onto stack
                        stack.push(tagName);
                        
                        // Create new node and set it as a child of 'node'
                        Node* newNode = new Node(tagName);
                        node->appendChild(newNode);

                        // Make newNode as 'node' to add children
                        node = newNode;

                        // Parse attributes
                        node->attributes = getAttributes(token);
                    }
                }
            }
        }

        if (stack.empty())
            return new Tree(root);
        else
        {
            util::logSyntaxError("Unpaired tag(s): ");
            while (!stack.empty())
            {
                Log(" - " << stack.top());
                stack.pop();
            }

            std::exit(1);
        }
    }

    std::string Parser::getTagName(std::string& tag)
    {
        int start;
        if (tag.length() > 2 && tag.substr(0, 2) == "</")
            start = 2;
        else
            start = 1;

        int curr = start;
        while (curr < tag.length() && !std::isspace(tag[curr]) && tag[curr] != '>') curr++;

        return tag.substr(start, curr-start);
    }

    std::map<std::string, std::string>* Parser::getAttributes(std::string& tag)
    {
        std::map<std::string, std::string>* pairs = new std::map<std::string, std::string>();
        auto truncTag = tag.substr(1, tag.length() - 2);
        auto tokens = util::tokenize(truncTag, 32, true);

        int equalsLoc;
        std::string token;
        // Skipping the tagname, hence i starts from 1
        for (int i = 1; i < tokens.size(); i++)
        {
            token = tokens.at(i);
            equalsLoc = token.find_first_of('=');
            (*pairs)[token.substr(0, equalsLoc)] = token.substr(equalsLoc + 2, token.length() - equalsLoc - 3);
        }

        return pairs;
    }

} // namespace dom
