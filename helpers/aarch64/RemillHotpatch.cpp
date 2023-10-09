/*
  Original author: https://github.com/fvrmatteo
*/

#include "remill/Arch/AArch64/Runtime/State.h"
#include "remill/Arch/AArch64/Runtime/Types.h"
#include "remill/Arch/Runtime/Intrinsics.h"
#include "remill/Arch/Runtime/Operators.h"

#include <cstdint>
#include <cstring>

// Special definitions

extern "C" uint8_t RAM[0];
extern "C" uint64_t __undef;

// Special attempt at identifying "ADD SP, IMM" and undefine the virtually popped stack slots

void __attribute__((always_inline)) __undefine_stack_slot(addr_t a) {
  std::memcpy(&RAM[a], &__undef, sizeof(__undef));
}

template <typename D, typename S1, typename S2> DEF_SEM(ADD, D dst, S1 src1, S2 src2) {
  if ((&(state.gpr.sp.qword) == dst.val_ref) && (state.gpr.sp.qword == Read(src1))) {
    for (uint64_t i = 0; i < Read(src2); i += 8)
      __undefine_stack_slot(UAdd(Read(src1), i));
  }
  WriteZExt(dst, UAdd(Read(src1), Read(src2)));
  return memory;
}

DEF_ISEL(ADD_64_ADDSUB_IMM) = ADD<R64W, R64, I64>;

// Modified version of the Remill AArch64 "UDIV" semantics, we assume the divisor cannot be 0

template <typename D, typename S> DEF_SEM(UDIV, D dst, S src1, S src2) {
  auto lhs = Read(src1);
  auto rhs = Read(src2);
  WriteZExt(dst, UDiv(lhs, rhs));
  return memory;
}

DEF_ISEL(UDIV_32_DP_2SRC) = UDIV<R32W, R32>;
DEF_ISEL(UDIV_64_DP_2SRC) = UDIV<R64W, R64>;

// Modified version of the Remill AArch64 "SDIV" semantics, we assume the divisor cannot be 0

template <typename D, typename S> DEF_SEM(SDIV, D dst, S src1, S src2) {
  auto lhs = Signed(Read(src1));
  auto rhs = Signed(Read(src2));
  WriteZExt(dst, Unsigned(SDiv(lhs, rhs)));
  return memory;
}

DEF_ISEL(SDIV_32_DP_2SRC) = SDIV<R32W, R32>;
DEF_ISEL(SDIV_64_DP_2SRC) = SDIV<R64W, R64>;
