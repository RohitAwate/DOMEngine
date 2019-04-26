# Virtual Machine and Bytecode Specification

This document describes the VM's architecture and the bytecode format.

## Virtual Machine
The VM is responsible for manipulating the DOM tree. Its input may come from a script or the REPL.
### Statements and Routines

The VM accepts input in the form of **statements**, which encapsulate two things:
- the instruction itself
- one argument

Based on the instruction to be executed, the corresponding **routine** is determined from a lookup table. The VM then executes this routine by passing it the DOM tree and the argument.

These routines have the following naming scheme: `routine` + instruction name.
For example, the SEL instruction would have a routine named `routineSEL`.

### Registers
The VM offers certain data structures which can be used by the instructions. These are:
- `tree` - The DOM tree itself
- `selection` - A single DOM node
- `mselection` - A vector of DOM nodes
- `buffer` - A DOM node which can be used by instructions 
- `flag` - A boolean value which defaults to false

## Bytecode
### Independent Instructions
These instructions **do not require** any nodes to be selected. Any selections made will be ignored.
- `SEL <string:selector>` - Sets `selection` to the first node that matches the selector string
- `MSEL <string:selector>` - Adds all the nodes that match the selector string to `mselection`
- `SAVE <string:filepath>` - Saves the HTML representation of the tree to the specified file. This instruction uses the value of `flag` to determine whether or not to format the HTML.
- `PRINT` - Prints the DOM tree

### Dependent Instructions
These instructions **require** one or more nodes to be selected before execution. For instructions requiring a single node, the `selection` register is referred, whereas for those requiring multiple nodes, `mselection` is referred. If `selection` or `mselection` do not contain nodes required by their respective instructions, the VM will throw an error.

- `ATTRS` - Displays the node's attributes
- `CHILDREN` - Displays the node's children
- `INNERHTML` - Displays the node's innerHTML
- `PARENT` - Displays the node's parent

**IMPORTANT**: All of the above instructions have corresponding _multiplied_ versions which perform the same operations but for all nodes in `mselection`. These instructions have the prefix `M`. For example, the multiplied version of `CHILDREN` is `MCHILDREN`.

### Meta Instructions
These instructions are intrinsic to the VM itself and are used to manipulate register values. The naming convention is as follows: register mnemonic + action.
- `SELCLR` - Sets `selection` to `nullptr`
- `MSELCLR` - Removes all nodes from `mselection`
- `ASELCLR` - Performs both `SELCLR` and `MSELCLR`
- `FLAGSET` - Sets `flag` to `true`
- `FLAGCLR` - Sets `flag` to `false`
- `BUFSET <dom::Node*:addr>` - Sets `buffer` to `addr`
- `BUFCLR` - Sets `buffer` to `nullptr`