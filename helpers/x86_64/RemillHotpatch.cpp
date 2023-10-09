/*
  Original author: https://github.com/fvrmatteo
*/

#include <cmath>

#include "remill/Arch/X86/Runtime/State.h"
#include "remill/Arch/X86/Runtime/Types.h"

#include "remill/Arch/Runtime/Definitions.h"
#include "remill/Arch/Runtime/Intrinsics.h"
#include "remill/Arch/Runtime/Operators.h"
#include "remill/Arch/Runtime/Types.h"

// Special definitions

extern "C" uint64_t __undef;

template <typename T> __attribute__((always_inline)) T UNDEF() {
  return (T)__undef;
}

// Remill propietary defines

#define REG_AL state.gpr.rax.byte.low
#define REG_AH state.gpr.rax.byte.high
#define REG_AX state.gpr.rax.word
#define REG_EAX state.gpr.rax.dword
#define REG_RAX state.gpr.rax.qword

#define REG_DL state.gpr.rdx.bytes.low
#define REG_DH state.gpr.rdx.bytes.high
#define REG_DX state.gpr.rdx.word
#define REG_EDX state.gpr.rdx.dword
#define REG_RDX state.gpr.rdx.qword

#define REG_ESP state.gpr.rsp.dword
#define REG_RSP state.gpr.rsp.qword

#define REG_EIP state.gpr.rip.dword
#define REG_RIP state.gpr.rip.qword

#if 64 == ADDRESS_SIZE_BITS
#define REG_XSP REG_RSP
#define REG_PC REG_RIP
#define REG_XAX REG_RAX
#define REG_XDX REG_RDX
#else
#define REG_XSP REG_ESP
#define REG_PC REG_EIP
#define REG_XAX REG_EAX
#define REG_XDX REG_EDX
#endif

// Modified version of the Remill "POP REG" semantic, we write 'undef' to the popped stack
// location

template <typename D> DEF_SEM(POP, D dst) {
  auto undef = static_cast<typename BaseType<D>::BT>(UNDEF<uint64_t>());
  addr_t op_size = ZExtTo<D>(ByteSizeOf(dst));
  addr_t old_xsp = Read(REG_XSP);
  addr_t new_xsp = UAdd(old_xsp, op_size);
  Write(REG_XSP, new_xsp);
  WriteZExt(dst, Read(ReadPtr<D>(old_xsp _IF_32BIT(REG_SS_BASE))));
  // Undefine the old stack slot
  Write(WritePtr<D>(old_xsp _IF_32BIT(REG_SS_BASE)), undef);
  return memory;
}

// Export the ISEL to we'll be able to merge the modules

DEF_ISEL(POP_GPRv_8F_16) = POP<R16W>;
DEF_ISEL_R32or64W(POP_GPRv_8F, POP);

DEF_ISEL(POP_GPRv_51_16) = POP<R16W>;
DEF_ISEL(POP_GPRv_58_16) = POP<R16W>;
DEF_ISEL_R32or64W(POP_GPRv_51, POP);
DEF_ISEL_R32or64W(POP_GPRv_58, POP);

DEF_ISEL(POP_MEMv_16) = POP<M16W>;
DEF_ISEL_M32or64W(POP_MEMv, POP);

// Modified version of the Remill x86/x64 "UDIV" semantics, we assume the divisor cannot be 0

#define ClearArithFlags()                                                                          \
  do {                                                                                             \
    state.aflag.cf = __remill_undefined_8();                                                       \
    state.aflag.pf = __remill_undefined_8();                                                       \
    state.aflag.af = __remill_undefined_8();                                                       \
    state.aflag.zf = __remill_undefined_8();                                                       \
    state.aflag.sf = __remill_undefined_8();                                                       \
    state.aflag.of = __remill_undefined_8();                                                       \
  } while (false)

#define MAKE_DIVxax(name, src1, src2, dst1, dst2)                                                  \
  template <typename S3> DEF_SEM(DIV##name, S3 src3, PC next_pc) {                                 \
    auto lhs_low = ZExt(Read(src1));                                                               \
    auto lhs_high = ZExt(Read(src2));                                                              \
    auto rhs = ZExt(Read(src3));                                                                   \
    auto shift = ZExt(BitSizeOf(src3));                                                            \
    auto lhs = UOr(UShl(lhs_high, shift), lhs_low);                                                \
    WriteZExt(REG_PC, Read(next_pc));                                                              \
    auto quot = UDiv(lhs, rhs);                                                                    \
    auto rem = URem(lhs, rhs);                                                                     \
    auto quot_trunc = Trunc(quot);                                                                 \
    auto rem_trunc = Trunc(rem);                                                                   \
    WriteZExt(dst1, quot_trunc);                                                                   \
    WriteZExt(dst2, rem_trunc);                                                                    \
    ClearArithFlags();                                                                             \
    return memory;                                                                                 \
  }

MAKE_DIVxax(ax, REG_AL, REG_AH, REG_AL, REG_AH) MAKE_DIVxax(dxax, REG_AX, REG_DX, REG_AX, REG_DX);
MAKE_DIVxax(edxeax, REG_EAX, REG_EDX, REG_XAX, REG_XDX);
IF_64BIT(MAKE_DIVxax(rdxrax, REG_RAX, REG_RDX, REG_RAX, REG_RDX));

#undef MAKE_DIVxax

#define MAKE_IDIVxax(name, src1, src2, dst1, dst2)                                                 \
  template <typename S3> DEF_SEM(IDIV##name, S3 src3, PC next_pc) {                                \
    auto lhs_low = ZExt(Read(src1));                                                               \
    auto lhs_high = ZExt(Read(src2));                                                              \
    auto rhs = SExt(Read(src3));                                                                   \
    auto shift = ZExt(BitSizeOf(src3));                                                            \
    auto lhs = Signed(UOr(UShl(lhs_high, shift), lhs_low));                                        \
    WriteZExt(REG_PC, Read(next_pc));                                                              \
    auto quot = SDiv(lhs, rhs);                                                                    \
    auto rem = SRem(lhs, rhs);                                                                     \
    auto quot_trunc = Trunc(quot);                                                                 \
    auto rem_trunc = Trunc(rem);                                                                   \
    WriteZExt(dst1, Unsigned(quot_trunc));                                                         \
    WriteZExt(dst2, Unsigned(rem_trunc));                                                          \
    ClearArithFlags();                                                                             \
    return memory;                                                                                 \
  }

MAKE_IDIVxax(ax, REG_AL, REG_AH, REG_AL, REG_AH) MAKE_IDIVxax(dxax, REG_AX, REG_DX, REG_AX, REG_DX);
MAKE_IDIVxax(edxeax, REG_EAX, REG_EDX, REG_XAX, REG_XDX);
IF_64BIT(MAKE_IDIVxax(rdxrax, REG_RAX, REG_RDX, REG_RAX, REG_RDX));

#undef MAKE_IDIVxax

DEF_ISEL(IDIV_MEMb) = IDIVax<M8>;
DEF_ISEL(IDIV_GPR8) = IDIVax<R8>;
DEF_ISEL(IDIV_MEMv_8) = IDIVax<M8>;
DEF_ISEL(IDIV_MEMv_16) = IDIVdxax<M16>;
DEF_ISEL(IDIV_MEMv_32) = IDIVedxeax<M32>;
IF_64BIT(DEF_ISEL(IDIV_MEMv_64) = IDIVrdxrax<M64>;)
DEF_ISEL(IDIV_GPRv_8) = IDIVax<R8>;
DEF_ISEL(IDIV_GPRv_16) = IDIVdxax<R16>;
DEF_ISEL(IDIV_GPRv_32) = IDIVedxeax<R32>;
IF_64BIT(DEF_ISEL(IDIV_GPRv_64) = IDIVrdxrax<R64>;)

DEF_ISEL(DIV_MEMb) = DIVax<M8>;
DEF_ISEL(DIV_GPR8) = DIVax<R8>;
DEF_ISEL(DIV_MEMv_8) = DIVax<M8>;
DEF_ISEL(DIV_MEMv_16) = DIVdxax<M16>;
DEF_ISEL(DIV_MEMv_32) = DIVedxeax<M32>;
IF_64BIT(DEF_ISEL(DIV_MEMv_64) = DIVrdxrax<M64>;)
DEF_ISEL(DIV_GPRv_8) = DIVax<R8>;
DEF_ISEL(DIV_GPRv_16) = DIVdxax<R16>;
DEF_ISEL(DIV_GPRv_32) = DIVedxeax<R32>;
IF_64BIT(DEF_ISEL(DIV_GPRv_64) = DIVrdxrax<R64>;)
