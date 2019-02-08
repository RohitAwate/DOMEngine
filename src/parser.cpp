#include "parser.h"
#include "util.h"

#include <stack>

#define Log(x) std::cout << x << std::endl

namespace dom
{

Parser::Parser(char* fname) : filename(fname) {}

Tree* Parser::parse()
{
    fd.open(filename);
    if (!fd.good())
    {
        Log("Could not open file: " << filename);
        return NULL;
    }

    std::string line;
    std::stack<std::string> stack;
    int curr, len;
    std::string windowType = "window";
    Node* root = new Node(windowType);
    Node* node = root;
    while (std::getline(fd, line))
    {
        // Initialize curr to first character
        curr = 0;

        len = line.length();
        while (curr < len)
        {
            // Traverse to first non-blank character
            while (std::isspace(line[curr])) { curr++; }

            // Identify closing tag
            if (line.substr(curr, 2) == "</")
            {
                curr += 2;
                int start = curr;

                /*
                    Get the tagname.
                    We'll get the index of '>' so that we can extract the substring
                    from start to that index, which will yield us the tagname.
                    */
                while (line[curr] != '>') curr++;

                std::string tag = line.substr(start, curr-start);

                // If identified, look for opening tag on stack.
                // Print error and exit if not found
                if (stack.empty() || stack.top() != tag)
                {
                    Log("Unclosed tag: </" << stack.top() << ">");
                    std::exit(1);
                }
                else // If found, pop the opening tag from stack and add to tree
                {
                    node = node->getParent();
                    stack.pop();
                }
            }
            
            // Skip through comments
            else if (line.substr(curr, 2) == "<!")
                while (line[curr] != '>') curr++;

            /*
                    Else, identify opening tag.
                    Checking for '<' since there may be just text of an element.
            */
            else if (line[curr] == '<')
            {
                curr++;
                int start = curr;

                /*
                    Get the tagname.
                    We'll get the index of '>' so that we can extract the substring
                    from start to that index, which will yield us the tagname.
                */
                while (line[curr] != '>' && line[curr] != ' ') curr++;

                // Push opening tag onto stack
                std::string tag = line.substr(start, curr-start);
                stack.push(tag);

                // When new opening tag is fully parsed, create a new node and append it as a child of 'node'
                Node* newNode = new Node(tag);
                node->appendChild(newNode);
                node = newNode;

                /*
                    Parse attributes.
                    If preceeding while loop was terminated by a ' ', we check for that here.
                    This is so because then the element has some attributes.
                */
                if (line[curr] != '>')
                {
                    int attrStart = ++curr;

                    // Get the index of '>'
                    while (curr < len && line[curr] != '>') curr++;

                    auto tokens = util::tokenize(line.substr(attrStart, curr-attrStart), ' ');

                    newNode->attributes = new std::map<std::string, std::string>();
                    for (auto token : tokens)
                    {
                        auto pair = util::tokenize(token, '=');
                        if (pair.size() == 2)
                        {
                            auto val = pair.at(1);
                            val = val.substr(1, val.length()-2);
                            (*newNode->attributes)[pair.at(0)] = val;
                        }
                    }
                }
            }

            // Identify closing tag (if available)
            while (curr < len && line[curr] != '<') curr++;

            if (line.substr(curr, 2) == "</")
            {
                curr += 2;
                int start = curr;

                /*
                    Get the tagname.
                    We'll get the index of '>' so that we can extract the substring
                    from start to that index, which will yield us the tagname.
                    */
                while (line[curr] != '>') { curr++; }

                std::string tag = line.substr(start, curr-start);

                // If identified, look for opening tag on stack.
                // Print error and exit if not found
                if (stack.empty() || stack.top() != tag)
                {
                    Log("Unclosed tag: </" << stack.top() << ">");
                    std::exit(1);
                }
                else // If found, pop the opening tag from stack and add to tree
                {
                    node = node->getParent();
                    stack.pop();
                }
            }
        }
    }

    fd.close();

    if (stack.empty())
        return new Tree(root);
    else
    {
        Log("[Parsing Error] Unpaired tag(s):");
        while (!stack.empty())
        {
            Log(" - " << stack.top());
            stack.pop();
        }

        std::exit(1);
    }
}

} // namespace dom
