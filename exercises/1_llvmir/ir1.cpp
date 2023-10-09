#include <stdio.h>

// Command: clang-15 -c -emit-llvm ir1.cpp -o ir1.bc
// Godbolt: https://godbolt.org/z/EscKns6s6

int main(int argc, char **argv) {
  puts("Hello from LLVM!");
}
