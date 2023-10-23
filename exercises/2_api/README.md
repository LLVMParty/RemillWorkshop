# Lifting

During these exercises you will write simple command line utilities to inspect and manipulate bitcode.

Activate the environment and go into this folder:

```sh
source llvm-env
cd exercises/2_api
```

## Creating a new tool

In the exercises below you'll use the template `bc-tool.cpp` to create tools of your own. As an example you can create `bc-stats` with the following steps:

1. Copy/paste `src/bc-tool.cpp` and rename it to `bc-stats.cpp`
2. Add a new executable target to the bottom of the `cmake.toml` file:
```toml
[target.bc-stats]
type = "llvm-tool"
sources = ["src/bc-stats.cpp"]
```
3. Run the `CMake: Build` command from the command palette in VSCode to build the new tool.
4. Make sure your newly-created `bc-stats` executable is accessible from your `PATH`.

## bc-stats

_Exercise_: write a simple tool to count the number of load/store/call instructions.

**Note**: For this exercise you have to remove remove the logic related to saving the output bitcode and just output the results to the command line as follows:

```
myFunction:
  load: X
  store: Y
  call: Z
```

## bc-demangle

_Exercise_: Create a tool that demangles the function names to make it easier to read the LLVM IR.

An example file with name mangling is in `mangled.cpp`. You can get the bitcode by compiling it:

```sh
clang-15 -c mangled.cpp -emit-llvm -S -o mangled.ll
```

## bc-graphviz

_Exercise_: Create a tool to visualize the control flow graph of a function using GraphViz:

```
digraph G {
    A -> B;
    A -> C;
    B -> D;
    C -> D;
}
```

You can preview `output.dot` in VSCode, or run `dot -Tpng output.dot` (after `apt install graphviz`).

Bonus: add more features and try replicating `opt -dot-cfg my.bc`
