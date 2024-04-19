# Lifting

First, go into this folder:

```sh
cd exercises/3_lifting
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
remill-lift -bytes "4889f8c3" -address 0x1000 -arch amd64 -ir_pre_out lift0-pre.ll -ir_out lift0.ll
```

First open `lift0-pre.ll` and do the following exercises:
- Take a look at the generated `sub_1000` function.
- Find the semantics of the `ret` instruction (use the `bc-demangle` tool to make it easier to read).

Now look at the optimized `lift0.ll` and do these exercises:
- What IR instruction stores the result in `rax`?
- What IR instruction reads the return address (`[rsp]`)?

## Recovering the calling convention

First we need to copy the helpers for our architecture to the current directory:

```sh
cp ../../build/helpers/x86_64/RemillHelpers.ll ./helpers.ll
```

Then link the helpers and optimize:

```sh
llvm-link lift0.ll helpers.ll -o lift0-linked.ll -S
opt "-passes=default<O3>" lift0-linked.ll -o lift0-recovered.ll -S
```

_Note_: You can completely customize the passes here (try the `strip` pass to clean things up). See [this Godbolt](https://godbolt.org/z/q9hPPehz3) for an example.

## More complex example

Compile `lift1.cpp` to `x86_64` and obtain the hexdump of the function:

```sh
clang-15 -c -target x86_64-linux-gnu -O1 lift1.cpp -o lift1.o
llvm-objcopy -O binary --only-section=.text lift1.o lift1.bin
echo $(hexdump -ve '1/1 "%02x"' lift1.bin)
```

Also output the clean bitcode for comparison purposes:

```sh
clang-15 -c lift1.cpp -emit-llvm -S -o lift1-clean.ll -O3
```

```llvm
define dso_local i64 @test1_linear_flow(i64 noundef %n) local_unnamed_addr #0 {
entry:
  %shr = lshr i64 %n, 3
  %mul = mul nuw nsw i64 %shr, 3
  %add = add nuw nsw i64 %mul, 1337
  ret i64 %add
}
```

Now lift the function:

```sh
remill-lift -bytes "48c1ef03488d047f480539050000c3" -address 0x1000 -arch amd64 -ir_out lift1.ll
```

Exercise: clean up the function and reconstruct the LLVM IR

## Recovering the calling convention

The custom implementation of `remill-lift` shows how you might get a similar result using the C++ API:

```sh
remill-lift -bytes "48c1ef03488d047f480539050000c3" -address 0x1000 -arch amd64 -ir_out lift1.ll -call_inputs RDI -call_output RAX
llvm-link lift1.ll helpers.ll -o lift1-linked.ll -S
opt "-passes=default<O3>,strip" lift1-linked.ll -o lift1-recovered.ll -S
```

**Note**: the function with the recovered calling convention is named `@call_sub_<address>`.

Exercises:
- Compile `lift3.cpp` and reconstruct the `test3_complex_cfg` function.
- What happens if any stack slots are used for local variables?

## Recovering a function from a binary

Compile `lift3.cpp` to an ELF file:

```sh
clang-15 -O3 lift3.cpp -o lift3.elf
llvm-objdump --disassemble-symbols=test3_complex_cfg lift3.elf
```

_Exercise_: look at the `remill-lift.cpp` to figure out how to use the `-binary` argument and lift the function from `lift3.elf` directly.
