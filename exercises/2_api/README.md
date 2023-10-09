# Lifting

Activate the environment and go into this folder:

```sh
source llvm-env
cd exercises/2_api
```

## bc-stats

Exercise: write a simple tool to count the number of load/store/call instructions.

First, create a new tool from the template:

```sh
cp src/bc-tool.cpp src/bc-stats.cpp
```

Then modify `cmake.toml` and add the target:

```toml
[target.bc-stats]
type = "llvm-tool"
sources = ["src/bc-stats.cpp"]
```

You can use `CMake: Build` to build the tool and you can use it from the command line.

## bc-demangle

Exercise: Create a tool that demangles the function names to make it easier to read the LLVM IR.

_Note_: An example with name mangling is in `mangled.cpp`

## bc-graphviz

Exercise: Create a tool to visualize the control flow graph of a function using GraphViz:

```
digraph G {
    A -> B;
    A -> C;
    B -> D;
    C -> D;
}
```

You can preview `output.dot` in VSCode, or run `dot -Tpng output.dot` (after `apt install graphviz`)
