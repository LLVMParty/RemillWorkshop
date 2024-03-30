# LLVM IR

First, go into this folder:

```sh
cd exercises/1_llvmir
```

Then running `which clang-15` should output: `/cxx-common/install/bin/clang-15`.

Compile the file to LLVM bitcode:

```sh
clang-15 -c ir1.cpp -emit-llvm -o ir1.bc
```

This will generate `ir1.bc`, which you can disassemble with `llvm-dis` (and then view in VSCode):

```sh
llvm-dis ir1.bc -o ir1.ll
llvm-dis ir1.bc -o ir1.ll --show-annotations
```

You can also emit a human-readable bitcode file directly:

```sh
clang-15 -c ir2.cpp -emit-llvm -S -o ir2.ll
```

Try with some optimization flags as well:

```sh
clang-15 -c -O3 ir2.cpp -emit-llvm -S -o ir2-opt.ll
```

_Exercise_: Look at the generated bitcode before/after optimizations and try to understand what is going on.
