#include "script.h"
#include "util.h"
#include "style.h"

#include <fstream>

namespace dom
{

    ScriptRunner::ScriptRunner(Tree* _dtree, char* scriptSrc) : dtree(_dtree), interpreter(_dtree)
    {
        std::ifstream fd(scriptSrc);

        std::string line;
        while (std::getline(fd, line)) this->src.push_back(line);

        fd.close();
    }
    
    int ScriptRunner::parse()
    {
        char curr;
        int len;

        commands = new std::vector<ScriptCommand*>();
        std::string selector;
        std::vector<std::string>* subCmds;
        bool bracesPaired = true;

        for (std::string line: src)
        {
            len = line.length();
            curr = 0;

            while (curr < len)
            {
                // Skip whitespace
                while (curr < len && std::isspace(line[curr])) { curr++; }

                /*
                    Selector

                    Selector is set as 'selector' and all further commands
                    are added as its subcommands until a closing brace is encountered.
                */
                if (line[curr] == '$')
                {
                    int start = curr;

                    while (curr < len && line[curr] != ')') curr++;
                    if (!selector.empty())
                    {
                        Log("Nested selectors not allowed.");
                        return -1;
                    }

                    std::string cmd = line.substr(start, curr-start+1);
                    selector = cmd;
                    subCmds = new std::vector<std::string>();
                }

                // Opening brace
                else if (line[curr] == '{')
                {
                    if (!bracesPaired || selector.empty())
                    {
                        Log("Syntax error: Stray '{'");
                        return -1;
                    }

                    bracesPaired = false;
                }

                // Closing brace
                else if (line[curr] == '}')
                {
                    if (bracesPaired || selector.empty())
                    {
                        Log("Syntax error: Stray '}'");
                        return -1;
                    }

                    commands->push_back(new ScriptCommand{selector, subCmds});
                    selector.clear();
                    bracesPaired = true;
                }

                // Command
                else
                {
                    int start = curr;
                    while (curr < len && line[curr] != ';') curr++;
                    std::string cmd = line.substr(start, curr-start);

                    if (selector.empty())
                        commands->push_back(new ScriptCommand{cmd, nullptr});
                    else
                        subCmds->push_back(cmd);
                }
                curr++;
            }
        }

        return 1;
    }

    int ScriptRunner::run()
    {
        auto res = parse();
        if (res != 1) return res;

        for (auto scriptCmd : *commands)
        {
            Log("> " << scriptCmd->cmd);
            if (scriptCmd->subCmds)
            {
                auto selected = interpreter.select(scriptCmd->cmd);
                if (selected)
                {
                    Log(TEXT_BOLD << "$ " << selected->toString() << TEXT_RESET);
                    for (auto subCmd : *scriptCmd->subCmds)
                    {
                        Log(".. > " << subCmd << TEXT_RESET);
                        interpreter.resolveSubCmd(subCmd, selected);
                    }
                }
                else Log("No match found.");
            }
            else
            {
                interpreter.resolveCmd(scriptCmd->cmd);
            }

            Log("────────────────────────");
        }

        return 1;
    }


} // namespace dom
