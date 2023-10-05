# LLVM IR

Activate the environment and go into this folder:

```sh
source llvm-env
cd example/1_llvmir
```

Then doing `which clang-15` should output: `/cxx-common/install/bin/clang-15`.

Compile the file to LLVM bitcode:

```sh
clang-15 -c ir1.cpp -emit-llvm
```

This will generate `ir1.bc`, which you can disassemble with `llvm-dis`:

```sh
llvm-dis ir1.bc
llvm-dis ir1.bc --show-annotations
```
