# DOM Manipulation Engine
`THIS IS A WORK IN PROGRESS`

A simple command-line program for parsing HTML files and building a DOM tree which can then be accessed and manipulated from a REPL or with scripts using jQuery-like selectors and commands.

This is nothing more than an effort at learning modern, idiomatic C++ and understanding the working of web browsers. Its amusing at best and useless at worst. Have fun with it!

_Check out the engine in action on ASCII Cinema:_

[![asciicast](https://asciinema.org/a/227230.svg)](https://asciinema.org/a/227230)

## Building from source
You'll need the following installed:
- A compiler which supports at least C++11: `g++`, `clang++` or `Visual C++`
- `CMake`
- `make`
- `git`

#### Steps
```bash
# Clone the repository
git clone https://github.com/RohitAwate/DOMEngine

# Open the directory
cd DOMEngine/

# Create a new directory and open it
mkdir build/ && cd build/

# Generate the build files
cmake .. -DCMAKE_BUILD_TYPE=Release

# Build the project
make

# Open the bin/ folder
cd ../bin/

# Run REPL
./dom-engine <html-file>

# Run script
./dom-engine <html-file> <script-file>
```

## Commands

- `print`\
    Prints out the DOM tree.
    <br/>
- `$("@tagname#someId.firstClass.secondClass")`\
    Selects a DOM element of type `tagname` with ID `someId` and classes `firstClass` and `secondClass`.
    ### Sub-commands for selected nodes:
    - `parent` Prints the parent
    - `children` Prints all the children
    - `attrs` Prints all the attributes
    - `innerhtml` Prints the innerHTML
    - `return` Deselects the node and returns to REPL
    <br/>
- `save file.html`\
    Generates formatted HTML for the DOM tree and saves it to the specified file.
- `exit`\
    Exits the REPL.

## Writing Scripts
DOM Engine uses a simple scripting language which utilizes the commands mentioned above along with some syntactic elements such as semicolons and braces. Here is an example script.

```
$("#alertBox.alertDiv@div")
{
    parent;
    children;
    attrs;
}

$("@div.text-field") {
    parent;
    attrs;
}

$("@div.text-field") { attrs; }

$("@body"){children;}

print;
```
### Selector Blocks
The first five lines demonstrate the usage of DOM Engine's selectors and running sub-commands on selected nodes. The selector is the same as you'd use in the REPL with the sub-commands grouped together inside the curly braces, separated by semi-colons.

The next three selector blocks show alternative formatting.

Finally, the last line shows the `print` command terminated with a semi-colon.

Save the script in a file with the `.des` extension _(DOM Engine Script)_. <br/>
Run using: `dom-engine <html-file> script.des`

## Goals and Progress
See the Kanban board [here](https://github.com/RohitAwate/DOMEngine/projects/1?fullscreen=true).

## License
MIT
    
