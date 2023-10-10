/*
  Original author: https://github.com/fvrmatteo
*/

#include <cstdint>
#include <cstring>
#include <remill/Arch/Runtime/Types.h>

#include "remill/Arch/X86/Runtime/State.h"

#define HELPER extern "C" __attribute__((always_inline))

// Memory layout (0 length arrays treated as a simple pointer to unknown memory)

extern "C" uint8_t RAM[0];

// Implementation of the remill hint calls

HELPER Memory *__remill_function_return(State *, addr_t, Memory *memory) {
  return memory;
}

// Implementation of the Remill memory access (read/write) intrinsics

HELPER uint8_t __remill_read_memory_8(Memory *m, addr_t a) {
  uint8_t v = 0;
  std::memcpy(&v, &RAM[a], sizeof(v));
  return v;
}

HELPER uint16_t __remill_read_memory_16(Memory *m, addr_t a) {
  uint16_t v = 0;
  std::memcpy(&v, &RAM[a], sizeof(v));
  return v;
}

HELPER uint32_t __remill_read_memory_32(Memory *m, addr_t a) {
  uint32_t v = 0;
  std::memcpy(&v, &RAM[a], sizeof(v));
  return v;
}

HELPER uint64_t __remill_read_memory_64(Memory *m, addr_t a) {
  uint64_t v = 0;
  std::memcpy(&v, &RAM[a], sizeof(v));
  return v;
}

HELPER Memory *__remill_write_memory_8(Memory *m, addr_t a, uint8_t v) {
  std::memcpy(&RAM[a], &v, sizeof(v));
  return m;
}

HELPER Memory *__remill_write_memory_16(Memory *m, addr_t a, uint16_t v) {
  std::memcpy(&RAM[a], &v, sizeof(v));
  return m;
}

HELPER Memory *__remill_write_memory_32(Memory *m, addr_t a, uint32_t v) {
  std::memcpy(&RAM[a], &v, sizeof(v));
  return m;
}

HELPER Memory *__remill_write_memory_64(Memory *m, addr_t a, uint64_t v) {
  std::memcpy(&RAM[a], &v, sizeof(v));
  return m;
}

// Implementation of the Remill flag and comparison computation intrinsics

HELPER bool __remill_flag_computation_zero(bool result, ...) {
  return result;
}

HELPER bool __remill_flag_computation_sign(bool result, ...) {
  return result;
}

HELPER bool __remill_flag_computation_overflow(bool result, ...) {
  return result;
}

HELPER bool __remill_flag_computation_carry(bool result, ...) {
  return result;
}

HELPER bool __remill_compare_sle(bool result) {
  return result;
}

HELPER bool __remill_compare_slt(bool result) {
  return result;
}

HELPER bool __remill_compare_sge(bool result) {
  return result;
}

HELPER bool __remill_compare_sgt(bool result) {
  return result;
}

HELPER bool __remill_compare_ule(bool result) {
  return result;
}

HELPER bool __remill_compare_ult(bool result) {
  return result;
}

HELPER bool __remill_compare_ugt(bool result) {
  return result;
}

HELPER bool __remill_compare_uge(bool result) {
  return result;
}

HELPER bool __remill_compare_eq(bool result) {
  return result;
}

HELPER bool __remill_compare_neq(bool result) {
  return result;
}

// Implementation of the Remill undefined values

HELPER uint8_t __remill_undefined_8() {
  return 0;
}

HELPER uint16_t __remill_undefined_16() {
  return 0;
}

HELPER uint32_t __remill_undefined_32() {
  return 0;
}

HELPER uint64_t __remill_undefined_64() {
  return 0;
}

// Calling conventions (workshop-specific)

extern "C" Memory *sub_1000(State &state, Reg curr_pc, Memory *memory);

HELPER uint64_t call1(uint64_t rdi) {
  State state = {};
  state.gpr.rdi.qword = rdi;
  sub_1000(state, state.gpr.rip, nullptr);
  return state.gpr.rax.qword;
}
