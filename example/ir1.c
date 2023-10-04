#include <stdio.h>

// Command: clang -c -emit-llvm -S ir1.c
// Godbolt: https://godbolt.org/z/EscKns6s6

int main(int argc, char **argv) {
  puts("Hello from LLVM!");
}
