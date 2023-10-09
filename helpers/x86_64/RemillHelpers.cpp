/*
  Original author: https://github.com/fvrmatteo
*/

#include <cstdint>
#include <cstring>
#include <remill/Arch/Runtime/Types.h>

#include "remill/Arch/X86/Runtime/State.h"

// Memory layout (0 length arrays treated as a simple pointer to unknown memory)

extern "C" uint8_t RAM[0];

// Implementation of the Remill memory access (read/write) intrinsics

extern "C" __attribute__((optnone)) uint8_t __remill_read_memory_8(Memory *m, addr_t a) {
  uint8_t v = 0;
  std::memcpy(&v, &RAM[a], sizeof(v));
  return v;
}

extern "C" __attribute__((optnone)) uint16_t __remill_read_memory_16(Memory *m, addr_t a) {
  uint16_t v = 0;
  std::memcpy(&v, &RAM[a], sizeof(v));
  return v;
}

extern "C" __attribute__((optnone)) uint32_t __remill_read_memory_32(Memory *m, addr_t a) {
  uint32_t v = 0;
  std::memcpy(&v, &RAM[a], sizeof(v));
  return v;
}

extern "C" __attribute__((optnone)) uint64_t __remill_read_memory_64(Memory *m, addr_t a) {
  uint64_t v = 0;
  std::memcpy(&v, &RAM[a], sizeof(v));
  return v;
}

extern "C" __attribute__((optnone)) Memory *__remill_write_memory_8(Memory *m,
  addr_t a,
  uint8_t v) {
  std::memcpy(&RAM[a], &v, sizeof(v));
  return m;
}

extern "C" __attribute__((optnone)) Memory *__remill_write_memory_16(Memory *m,
  addr_t a,
  uint16_t v) {
  std::memcpy(&RAM[a], &v, sizeof(v));
  return m;
}

extern "C" __attribute__((optnone)) Memory *__remill_write_memory_32(Memory *m,
  addr_t a,
  uint32_t v) {
  std::memcpy(&RAM[a], &v, sizeof(v));
  return m;
}

extern "C" __attribute__((optnone)) Memory *__remill_write_memory_64(Memory *m,
  addr_t a,
  uint64_t v) {
  std::memcpy(&RAM[a], &v, sizeof(v));
  return m;
}

// Implementation of the Remill flag and comparison computation intrinsics

extern "C" bool __remill_flag_computation_zero(bool result, ...) {
  return result;
}

extern "C" bool __remill_flag_computation_sign(bool result, ...) {
  return result;
}

extern "C" bool __remill_flag_computation_overflow(bool result, ...) {
  return result;
}

extern "C" bool __remill_flag_computation_carry(bool result, ...) {
  return result;
}

extern "C" bool __remill_compare_sle(bool result) {
  return result;
}

extern "C" bool __remill_compare_slt(bool result) {
  return result;
}

extern "C" bool __remill_compare_sge(bool result) {
  return result;
}

extern "C" bool __remill_compare_sgt(bool result) {
  return result;
}

extern "C" bool __remill_compare_ule(bool result) {
  return result;
}

extern "C" bool __remill_compare_ult(bool result) {
  return result;
}

extern "C" bool __remill_compare_ugt(bool result) {
  return result;
}

extern "C" bool __remill_compare_uge(bool result) {
  return result;
}

extern "C" bool __remill_compare_eq(bool result) {
  return result;
}

extern "C" bool __remill_compare_neq(bool result) {
  return result;
}

// Implementation of the Remill undefined values

extern "C" uint8_t __remill_undefined_8() {
  return 0;
}

extern "C" uint16_t __remill_undefined_16() {
  return 0;
}

extern "C" uint32_t __remill_undefined_32() {
  return 0;
}

extern "C" uint64_t __remill_undefined_64() {
  return 0;
}

// Implementation of the Remill atomic load and store semantics

extern "C" Memory *__remill_atomic_begin(Memory *memory) {
  return memory;
}

extern "C" Memory *__remill_atomic_end(Memory *memory) {
  return memory;
}

// Implementation of the Remill atomic compare-exchange semantics

// Would be awesome to use '__sync_val_compare_and_swap' in the following intrinsics, but LLVM is
// still being too conservative and we cannot proceed with it. The function call will generate a
// 'cmpxchg' LLVM instruction which is atomic and has 'seq_cst' success/failure ordering. Here
// https://github.com/llvm/llvm-project/blob/main/llvm/lib/Analysis/AliasAnalysis.cpp#L470 we can
// see they default to llvm::ModRefInfo::ModRef unless the access is unordered, even though the
// accessed memory could be not aliasing. Relevant information:
// 1. https://reviews.llvm.org/D31726
// 2. https://github.com/llvm/llvm-project/blob/main/llvm/lib/Analysis/MemorySSA.cpp#L1772
// TODO: start a conversation on Discourse to see if we can find a solution to this.

extern "C" __attribute__((noinline)) uint8_t __lock_cmpxchg_8(uint8_t *a, uint8_t &o, uint8_t n) {
  uint8_t v = *a;
  if (v == o) {
    *a = n;
  }
  return v;
}

extern "C" __attribute__((noinline)) Memory *__remill_compare_exchange_memory_8(Memory *m,
  addr_t a,
  uint8_t &o,
  uint8_t n) {
  // o = __sync_val_compare_and_swap((uint8_t *)(&RAM[a]), o, n);
  o = __lock_cmpxchg_8((uint8_t *)&RAM[a], o, n);
  return m;
}

extern "C" __attribute__((noinline)) uint16_t __lock_cmpxchg_16(uint16_t *a,
  uint16_t &o,
  uint16_t n) {
  uint16_t v = *a;
  if (v == o) {
    *a = n;
  }
  return v;
}

extern "C" __attribute__((noinline)) Memory *__remill_compare_exchange_memory_16(Memory *m,
  addr_t a,
  uint16_t &o,
  uint16_t n) {
  // o = __sync_val_compare_and_swap((uint16_t *)(&RAM[a]), o, n);
  o = __lock_cmpxchg_16((uint16_t *)&RAM[a], o, n);
  return m;
}

extern "C" __attribute__((noinline)) uint32_t __lock_cmpxchg_32(uint32_t *a,
  uint32_t o,
  uint32_t n) {
  uint32_t v = *a;
  if (v == o) {
    *a = n;
  }
  return v;
}

extern "C" __attribute__((noinline)) Memory *__remill_compare_exchange_memory_32(Memory *m,
  addr_t a,
  uint32_t &o,
  uint32_t n) {
  // o = __sync_val_compare_and_swap((uint32_t *)(&RAM[a]), o, n);
  o = __lock_cmpxchg_32((uint32_t *)&RAM[a], o, n);
  return m;
}

extern "C" __attribute__((noinline)) uint64_t __lock_cmpxchg_64(uint64_t *a,
  uint64_t &o,
  uint64_t n) {
  uint64_t v = *a;
  if (v == o) {
    *a = n;
  }
  return v;
}

extern "C" __attribute__((noinline)) Memory *__remill_compare_exchange_memory_64(Memory *m,
  addr_t a,
  uint64_t &o,
  uint64_t n) {
  // o = __sync_val_compare_and_swap((uint64_t *)(&RAM[a]), o, n);
  o = __lock_cmpxchg_64((uint64_t *)&RAM[a], o, n);
  return m;
}
