/*
  Original author: https://github.com/fvrmatteo
*/

#include <cstdint>
#include <cstring>

#include "remill/Arch/AArch64/Runtime/State.h"

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
