# Lifting

Activate the environment and go into this folder:

```sh
source llvm-env
cd example/2_lifting
```

## Simple example

To get familiar with remill's output we will use a very simple example:

```c
uint64_t lift0(uint64_t rdi)
{
    return rdi;
}
```

In assembly:

```x86
mov rax, rdi
ret
```

Now lift it to bitcode:

```sh
remill-lift-15 -bytes "4889f8c3" -address 0x1000 -arch amd64 -ir_out lift0.ll
```

Presentation notes:

- Look at external functions (like in the introduction)
- `__remill_xxxx` helpers
- We will use `llvm-link` to link this together
- TODO: pretty much copy life of an instruction

TODO: flag to prevent optimizations?
TODO: use the slice_inputs and slice_outputs!
TODO: copy the relevant helpers

## More complex example

Compile `lift1.cpp` to `x86_64` and obtain the hexdump of the function:

```sh
clang-15 -c -target x86_64-linux-gnu -O1 lift1.cpp -o lift1.o
llvm-objcopy -O binary --only-section=.text lift1.o lift1.bin
echo $(hexdump -ve '1/1 "%02x"' lift1.bin)
```

Now lift the function:

```sh
./build/remill-lift -bytes "48c1ef03488d047f480539050000c3" -arch amd64 -ir_out lift1.bc
llvm-dis lift1.bc
```
