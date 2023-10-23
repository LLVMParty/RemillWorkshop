#include <stdint.h>

// clang-15 -c experiment.cpp -emit-llvm -S -o experiment.ll -O1

struct Reg final {
  union {
    alignas(1) struct {
      uint8_t low;
      uint8_t high;
    } byte;
    alignas(2) uint16_t word;
    alignas(4) uint32_t dword;
    alignas(8) uint64_t qword;
  } __attribute__((packed));
} __attribute__((packed));

extern Reg rax;
extern Reg rbx;
extern Reg rcx;

extern "C" void lifted_block() {
  rax.qword = rcx.qword * rbx.qword;
  rax.byte.low = 6;
}
