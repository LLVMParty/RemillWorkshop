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

Lift the assembly to bitcode:

```sh
remill-lift -bytes "4889f8c3" -address 0x1000 -arch amd64 -ir_pre_out lift0-pre.ll -ir_out lift0.ll
```

_Note_: the `-address` argument you specify will determine the name of the lifted function. For `0x1000` the function name will be `sub_1000`.

Run the Global Dead Code Elimination (`GlobalDCE`) pass to get rid of unused functions and global variables:

```sh
opt lift0-pre.ll -passes=globaldce -o lift0-opt.ll -S
```

Demangle the function names to make the IR easier to read:

```sh
bc-demangle lift0-opt.ll lift0-demangle.ll
```

Open `lift0-demangle.ll` and do the following exercises:
- Find and read the lifted function.
- Find the function that contains the semantics of the `ret` instruction.

Look at the optimized `lift0.ll` and answer the following questions:
- What LLVM IR instruction stores the result in `rax`?
- What LLVM IR instruction reads the return address (`[rsp]`)?

## Recovering the calling convention

First we need to copy the helpers for our architecture to the current directory:

```sh
cp ../../build/helpers/x86_64/RemillHelpers.ll ./helpers.ll
```

Then link the helpers and optimize:

```sh
llvm-link lift0.ll helpers.ll -o lift0-linked.ll -S
opt "-passes=inline" lift0-linked.ll -o lift0-inline.ll -S
opt "-passes=default<O3>" lift0-inline.ll -o lift0-recovered.ll -S
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

Exercise: clean up the function and reconstruct the LLVM IR using the steps from the previous exercises.

## Recovering the calling convention

The custom implementation of `remill-lift` shows how you might get a similar result using the C++ API:

```sh
remill-lift -bytes "48c1ef03488d047f480539050000c3" -address 0x1000 -arch amd64 -ir_out lift1.ll -call_inputs RDI -call_output RAX
llvm-link lift1.ll helpers.ll -o lift1-linked.ll -S
opt "-passes=inline" lift1-linked.ll -o lift1-inline.ll -S
opt "-passes=default<O3>,strip" lift1-inline.ll -o lift1-recovered.ll -S
```

**Note**: the function with the recovered calling convention is named `@call_sub_<address>`.

Exercises:
- Compile `lift3.cpp` and reconstruct the `test3_complex_cfg` function.
- What happens if any stack slots are used for local variables?

## Lifting a function from a binary

_Exercise_: Use `remill-lift` with `-binary lift3.elf` to lift the `test3_complex_cfg` function from the binary. You can find the address with the following command:

```sh
llvm-objdump --disassemble-symbols=test3_complex_cfg -M intel lift3.elf
```

Compare the cleaned output to `lift3-clean.ll` for reference.

## Cleaning up the `RAM` pointer

The helpers use a special `RAM` pointer, which we need to clean up. Compile `lift4.cpp` to x86_64 (like `lift1.cpp`) and perform the recovery steps.

Once you get `lift4-recovered.ll` you can clean up the `RAM` with this command:

```sh
ram-cleaner lift4-recovered.ll lift4-ramcleaned.ll
```
