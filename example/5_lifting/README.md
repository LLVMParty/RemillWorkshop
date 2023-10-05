# Lifting

Activate the environment and go into this folder:

```sh
source llvm-env
cd example/5_lifting
```

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
