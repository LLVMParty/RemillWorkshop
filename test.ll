; ModuleID = 'lifted_code'
source_filename = "lifted_code"
target datalayout = "e-m:e-i8:8:32-i16:16:32-i64:64-i128:128-n32:64-S128"
target triple = "aarch64-pc-linux-gnu-elf"

%struct.State = type { %struct.AArch64State }
%struct.AArch64State = type { %struct.ArchState, %struct.SIMD, i64, %struct.GPR, i64, %union.anon, %union.anon, %union.anon, i64, %struct.SR, i64 }
%struct.ArchState = type { i32, i32, %union.anon }
%struct.SIMD = type { [32 x %union.vec128_t] }
%union.vec128_t = type { %struct.uint128v1_t }
%struct.uint128v1_t = type { [1 x i128] }
%struct.GPR = type { i64, %struct.Reg, i64, %struct.Reg, i64, %struct.Reg, i64, %struct.Reg, i64, %struct.Reg, i64, %struct.Reg, i64, %struct.Reg, i64, %struct.Reg, i64, %struct.Reg, i64, %struct.Reg, i64, %struct.Reg, i64, %struct.Reg, i64, %struct.Reg, i64, %struct.Reg, i64, %struct.Reg, i64, %struct.Reg, i64, %struct.Reg, i64, %struct.Reg, i64, %struct.Reg, i64, %struct.Reg, i64, %struct.Reg, i64, %struct.Reg, i64, %struct.Reg, i64, %struct.Reg, i64, %struct.Reg, i64, %struct.Reg, i64, %struct.Reg, i64, %struct.Reg, i64, %struct.Reg, i64, %struct.Reg, i64, %struct.Reg, i64, %struct.Reg, i64, %struct.Reg }
%struct.Reg = type { %union.anon }
%union.anon = type { i64 }
%struct.SR = type { i64, %struct.Reg, i64, %struct.Reg, i8, i8, i8, i8, i8, i8, i8, i8, i8, i8, i8, i8, i8, i8, i8, i8, i8, i8, [6 x i8] }

define ptr @sub_13f5980.2(ptr noalias %state, i64 %program_counter, ptr noalias %memory) {
  %X17 = getelementptr inbounds %struct.State, ptr %state, i32 0, i32 0, i32 3, i32 35, i32 0, i32 0
  %X16 = getelementptr inbounds %struct.State, ptr %state, i32 0, i32 0, i32 3, i32 33, i32 0, i32 0
  %BRANCH_TAKEN = alloca i8, align 4
  %RETURN_PC = alloca i64, align 8
  %MONITOR = alloca i64, align 8
  %STATE = alloca ptr, align 8
  store ptr %state, ptr %STATE, align 8
  %MEMORY = alloca ptr, align 8
  store ptr %memory, ptr %MEMORY, align 8
  %NEXT_PC = alloca i64, align 8
  store i64 %program_counter, ptr %NEXT_PC, align 8
  %WZR = alloca i32, align 4
  store i32 0, ptr %WZR, align 4
  %XZR = alloca i64, align 8
  store i64 0, ptr %XZR, align 8
  %IGNORE_WRITE_TO_WZR = alloca i32, align 4
  %IGNORE_WRITE_TO_XZR = alloca i64, align 8
  %SUPPRESS_WRITEBACK = alloca i64, align 8
  %PC = getelementptr inbounds %struct.State, ptr %state, i32 0, i32 0, i32 3, i32 65, i32 0, i32 0
  store i64 %program_counter, ptr %NEXT_PC, align 8
  br label %1

1:                                                ; preds = %0
  %2 = load i64, ptr %NEXT_PC, align 8
  store i64 %2, ptr %PC, align 8
  %3 = add i64 %2, 4
  store i64 %3, ptr %NEXT_PC, align 8
  %X161 = ptrtoint ptr %X16 to i64
  %4 = load i64, ptr %PC, align 8
  %5 = add i64 %4, 150376448
  %6 = load ptr, ptr %MEMORY, align 8
  %and.i.i = and i64 %5, -4096
  store i64 %and.i.i, ptr %X16, align 8
  store ptr %6, ptr %MEMORY, align 8
  br label %7

7:                                                ; preds = %1
  %8 = load i64, ptr %NEXT_PC, align 8
  store i64 %8, ptr %PC, align 8
  %9 = add i64 %8, 4
  store i64 %9, ptr %NEXT_PC, align 8
  %X172 = ptrtoint ptr %X17 to i64
  %10 = load i64, ptr %X16, align 8
  %11 = add i64 %10, 1872
  %12 = load ptr, ptr %MEMORY, align 8
  %call.i.i = call i64 @__remill_read_memory_64(ptr noundef %12, i64 noundef %11) #2
  store i64 %call.i.i, ptr %X17, align 8
  store ptr %12, ptr %MEMORY, align 8
  br label %13

13:                                               ; preds = %7
  %14 = load i64, ptr %NEXT_PC, align 8
  store i64 %14, ptr %PC, align 8
  %15 = add i64 %14, 4
  store i64 %15, ptr %NEXT_PC, align 8
  %X163 = ptrtoint ptr %X16 to i64
  %16 = load i64, ptr %X16, align 8
  %17 = load ptr, ptr %MEMORY, align 8
  %add.i.i = add i64 1872, %16
  store i64 %add.i.i, ptr %X16, align 8
  store ptr %17, ptr %MEMORY, align 8
  br label %18

18:                                               ; preds = %13
  %19 = load i64, ptr %NEXT_PC, align 8
  store i64 %19, ptr %PC, align 8
  %20 = add i64 %19, 4
  store i64 %20, ptr %NEXT_PC, align 8
  %21 = load i64, ptr %X17, align 8
  %NEXT_PC4 = ptrtoint ptr %NEXT_PC to i64
  %22 = load ptr, ptr %MEMORY, align 8
  %pc.i = getelementptr inbounds %struct.AArch64State, ptr %state, i64 0, i32 3, i32 65
  store i64 %21, ptr %pc.i, align 16
  store i64 %21, ptr %NEXT_PC, align 8
  store ptr %22, ptr %MEMORY, align 8
  %23 = load i64, ptr %NEXT_PC, align 8
  store i64 %23, ptr %PC, align 8
  %24 = load ptr, ptr %MEMORY, align 8
  %25 = load i64, ptr %PC, align 8
  %26 = tail call ptr @__remill_jump(ptr %state, i64 %25, ptr %24)
  ret ptr %26
}

; Function Attrs: mustprogress noduplicate nofree noinline nosync nounwind optnone readnone willreturn
declare i64 @__remill_read_memory_64(ptr noundef readnone, i64 noundef) #0

; Function Attrs: noduplicate noinline nounwind optnone
declare ptr @__remill_jump(ptr noundef nonnull align 1, i64 noundef, ptr noundef) #1

define ptr @sub_13f53e0.1(ptr noalias %state, i64 %program_counter, ptr noalias %memory) {
  %X17 = getelementptr inbounds %struct.State, ptr %state, i32 0, i32 0, i32 3, i32 35, i32 0, i32 0
  %X16 = getelementptr inbounds %struct.State, ptr %state, i32 0, i32 0, i32 3, i32 33, i32 0, i32 0
  %BRANCH_TAKEN = alloca i8, align 4
  %RETURN_PC = alloca i64, align 8
  %MONITOR = alloca i64, align 8
  %STATE = alloca ptr, align 8
  store ptr %state, ptr %STATE, align 8
  %MEMORY = alloca ptr, align 8
  store ptr %memory, ptr %MEMORY, align 8
  %NEXT_PC = alloca i64, align 8
  store i64 %program_counter, ptr %NEXT_PC, align 8
  %WZR = alloca i32, align 4
  store i32 0, ptr %WZR, align 4
  %XZR = alloca i64, align 8
  store i64 0, ptr %XZR, align 8
  %IGNORE_WRITE_TO_WZR = alloca i32, align 4
  %IGNORE_WRITE_TO_XZR = alloca i64, align 8
  %SUPPRESS_WRITEBACK = alloca i64, align 8
  %PC = getelementptr inbounds %struct.State, ptr %state, i32 0, i32 0, i32 3, i32 65, i32 0, i32 0
  store i64 %program_counter, ptr %NEXT_PC, align 8
  br label %1

1:                                                ; preds = %0
  %2 = load i64, ptr %NEXT_PC, align 8
  store i64 %2, ptr %PC, align 8
  %3 = add i64 %2, 4
  store i64 %3, ptr %NEXT_PC, align 8
  %X161 = ptrtoint ptr %X16 to i64
  %4 = load i64, ptr %PC, align 8
  %5 = add i64 %4, 150376448
  %6 = load ptr, ptr %MEMORY, align 8
  %and.i.i = and i64 %5, -4096
  store i64 %and.i.i, ptr %X16, align 8
  store ptr %6, ptr %MEMORY, align 8
  br label %7

7:                                                ; preds = %1
  %8 = load i64, ptr %NEXT_PC, align 8
  store i64 %8, ptr %PC, align 8
  %9 = add i64 %8, 4
  store i64 %9, ptr %NEXT_PC, align 8
  %X172 = ptrtoint ptr %X17 to i64
  %10 = load i64, ptr %X16, align 8
  %11 = add i64 %10, 1152
  %12 = load ptr, ptr %MEMORY, align 8
  %call.i.i = call i64 @__remill_read_memory_64(ptr noundef %12, i64 noundef %11) #2
  store i64 %call.i.i, ptr %X17, align 8
  store ptr %12, ptr %MEMORY, align 8
  br label %13

13:                                               ; preds = %7
  %14 = load i64, ptr %NEXT_PC, align 8
  store i64 %14, ptr %PC, align 8
  %15 = add i64 %14, 4
  store i64 %15, ptr %NEXT_PC, align 8
  %X163 = ptrtoint ptr %X16 to i64
  %16 = load i64, ptr %X16, align 8
  %17 = load ptr, ptr %MEMORY, align 8
  %add.i.i = add i64 1152, %16
  store i64 %add.i.i, ptr %X16, align 8
  store ptr %17, ptr %MEMORY, align 8
  br label %18

18:                                               ; preds = %13
  %19 = load i64, ptr %NEXT_PC, align 8
  store i64 %19, ptr %PC, align 8
  %20 = add i64 %19, 4
  store i64 %20, ptr %NEXT_PC, align 8
  %21 = load i64, ptr %X17, align 8
  %NEXT_PC4 = ptrtoint ptr %NEXT_PC to i64
  %22 = load ptr, ptr %MEMORY, align 8
  %pc.i = getelementptr inbounds %struct.AArch64State, ptr %state, i64 0, i32 3, i32 65
  store i64 %21, ptr %pc.i, align 16
  store i64 %21, ptr %NEXT_PC, align 8
  store ptr %22, ptr %MEMORY, align 8
  %23 = load i64, ptr %NEXT_PC, align 8
  store i64 %23, ptr %PC, align 8
  %24 = load ptr, ptr %MEMORY, align 8
  %25 = load i64, ptr %PC, align 8
  %26 = tail call ptr @__remill_jump(ptr %state, i64 %25, ptr %24)
  ret ptr %26
}

define ptr @sub_142eb80(ptr noalias %state, i64 %program_counter, ptr noalias %memory) {
  %X17 = getelementptr inbounds %struct.State, ptr %state, i32 0, i32 0, i32 3, i32 35, i32 0, i32 0
  %X16 = getelementptr inbounds %struct.State, ptr %state, i32 0, i32 0, i32 3, i32 33, i32 0, i32 0
  %X4 = getelementptr inbounds %struct.State, ptr %state, i32 0, i32 0, i32 3, i32 9, i32 0, i32 0
  %X3 = getelementptr inbounds %struct.State, ptr %state, i32 0, i32 0, i32 3, i32 7, i32 0, i32 0
  %X6 = getelementptr inbounds %struct.State, ptr %state, i32 0, i32 0, i32 3, i32 13, i32 0, i32 0
  %X2 = getelementptr inbounds %struct.State, ptr %state, i32 0, i32 0, i32 3, i32 5, i32 0, i32 0
  %SP = getelementptr inbounds %struct.State, ptr %state, i32 0, i32 0, i32 3, i32 63, i32 0, i32 0
  %X1 = getelementptr inbounds %struct.State, ptr %state, i32 0, i32 0, i32 3, i32 3, i32 0, i32 0
  %X0 = getelementptr inbounds %struct.State, ptr %state, i32 0, i32 0, i32 3, i32 1, i32 0, i32 0
  %X5 = getelementptr inbounds %struct.State, ptr %state, i32 0, i32 0, i32 3, i32 11, i32 0, i32 0
  %X30 = getelementptr inbounds %struct.State, ptr %state, i32 0, i32 0, i32 3, i32 61, i32 0, i32 0
  %X29 = getelementptr inbounds %struct.State, ptr %state, i32 0, i32 0, i32 3, i32 59, i32 0, i32 0
  %BRANCH_TAKEN = alloca i8, align 4
  %RETURN_PC = alloca i64, align 8
  %MONITOR = alloca i64, align 8
  %STATE = alloca ptr, align 8
  store ptr %state, ptr %STATE, align 8
  %MEMORY = alloca ptr, align 8
  store ptr %memory, ptr %MEMORY, align 8
  %NEXT_PC = alloca i64, align 8
  store i64 %program_counter, ptr %NEXT_PC, align 8
  %WZR = alloca i32, align 4
  store i32 0, ptr %WZR, align 4
  %XZR = alloca i64, align 8
  store i64 0, ptr %XZR, align 8
  %IGNORE_WRITE_TO_WZR = alloca i32, align 4
  %IGNORE_WRITE_TO_XZR = alloca i64, align 8
  %SUPPRESS_WRITEBACK = alloca i64, align 8
  %PC = getelementptr inbounds %struct.State, ptr %state, i32 0, i32 0, i32 3, i32 65, i32 0, i32 0
  store i64 %program_counter, ptr %NEXT_PC, align 8
  br label %1

1:                                                ; preds = %0
  %2 = load i64, ptr %NEXT_PC, align 8
  store i64 %2, ptr %PC, align 8
  %3 = add i64 %2, 4
  store i64 %3, ptr %NEXT_PC, align 8
  %4 = load ptr, ptr %MEMORY, align 8
  store ptr %4, ptr %MEMORY, align 8
  br label %5

5:                                                ; preds = %1
  %6 = load i64, ptr %NEXT_PC, align 8
  store i64 %6, ptr %PC, align 8
  %7 = add i64 %6, 4
  store i64 %7, ptr %NEXT_PC, align 8
  %X291 = ptrtoint ptr %X29 to i64
  %8 = load ptr, ptr %MEMORY, align 8
  store i64 0, ptr %X29, align 8
  store ptr %8, ptr %MEMORY, align 8
  br label %9

9:                                                ; preds = %5
  %10 = load i64, ptr %NEXT_PC, align 8
  store i64 %10, ptr %PC, align 8
  %11 = add i64 %10, 4
  store i64 %11, ptr %NEXT_PC, align 8
  %X302 = ptrtoint ptr %X30 to i64
  %12 = load ptr, ptr %MEMORY, align 8
  store i64 0, ptr %X30, align 8
  store ptr %12, ptr %MEMORY, align 8
  br label %13

13:                                               ; preds = %9
  %14 = load i64, ptr %NEXT_PC, align 8
  store i64 %14, ptr %PC, align 8
  %15 = add i64 %14, 4
  store i64 %15, ptr %NEXT_PC, align 8
  %X53 = ptrtoint ptr %X5 to i64
  %16 = load i64, ptr %XZR, align 8
  %17 = load i64, ptr %X0, align 8
  %18 = load ptr, ptr %MEMORY, align 8
  %or.i.i = or i64 %17, %16
  store i64 %or.i.i, ptr %X5, align 8
  store ptr %18, ptr %MEMORY, align 8
  br label %19

19:                                               ; preds = %13
  %20 = load i64, ptr %NEXT_PC, align 8
  store i64 %20, ptr %PC, align 8
  %21 = add i64 %20, 4
  store i64 %21, ptr %NEXT_PC, align 8
  %X14 = ptrtoint ptr %X1 to i64
  %22 = load i64, ptr %SP, align 8
  %23 = load ptr, ptr %MEMORY, align 8
  %call.i.i36 = call i64 @__remill_read_memory_64(ptr noundef %23, i64 noundef %22) #2
  store i64 %call.i.i36, ptr %X1, align 8
  store ptr %23, ptr %MEMORY, align 8
  br label %24

24:                                               ; preds = %19
  %25 = load i64, ptr %NEXT_PC, align 8
  store i64 %25, ptr %PC, align 8
  %26 = add i64 %25, 4
  store i64 %26, ptr %NEXT_PC, align 8
  %X25 = ptrtoint ptr %X2 to i64
  %27 = load i64, ptr %SP, align 8
  %28 = load ptr, ptr %MEMORY, align 8
  %add.i.i35 = add i64 8, %27
  store i64 %add.i.i35, ptr %X2, align 8
  store ptr %28, ptr %MEMORY, align 8
  br label %29

29:                                               ; preds = %24
  %30 = load i64, ptr %NEXT_PC, align 8
  store i64 %30, ptr %PC, align 8
  %31 = add i64 %30, 4
  store i64 %31, ptr %NEXT_PC, align 8
  %X66 = ptrtoint ptr %X6 to i64
  %32 = load i64, ptr %SP, align 8
  %33 = load ptr, ptr %MEMORY, align 8
  store i64 %32, ptr %X6, align 8
  store ptr %33, ptr %MEMORY, align 8
  br label %34

34:                                               ; preds = %29
  %35 = load i64, ptr %NEXT_PC, align 8
  store i64 %35, ptr %PC, align 8
  %36 = add i64 %35, 4
  store i64 %36, ptr %NEXT_PC, align 8
  %X07 = ptrtoint ptr %X0 to i64
  %37 = load i64, ptr %PC, align 8
  %38 = add i64 %37, 150102016
  %39 = load ptr, ptr %MEMORY, align 8
  %and.i.i34 = and i64 %38, -4096
  store i64 %and.i.i34, ptr %X0, align 8
  store ptr %39, ptr %MEMORY, align 8
  br label %40

40:                                               ; preds = %34
  %41 = load i64, ptr %NEXT_PC, align 8
  store i64 %41, ptr %PC, align 8
  %42 = add i64 %41, 4
  store i64 %42, ptr %NEXT_PC, align 8
  %X08 = ptrtoint ptr %X0 to i64
  %43 = load i64, ptr %X0, align 8
  %44 = add i64 %43, 3088
  %45 = load ptr, ptr %MEMORY, align 8
  %call.i.i33 = call i64 @__remill_read_memory_64(ptr noundef %45, i64 noundef %44) #2
  store i64 %call.i.i33, ptr %X0, align 8
  store ptr %45, ptr %MEMORY, align 8
  br label %46

46:                                               ; preds = %40
  %47 = load i64, ptr %NEXT_PC, align 8
  store i64 %47, ptr %PC, align 8
  %48 = add i64 %47, 4
  store i64 %48, ptr %NEXT_PC, align 8
  %X39 = ptrtoint ptr %X3 to i64
  %49 = load ptr, ptr %MEMORY, align 8
  store i64 0, ptr %X3, align 8
  store ptr %49, ptr %MEMORY, align 8
  br label %50

50:                                               ; preds = %46
  %51 = load i64, ptr %NEXT_PC, align 8
  store i64 %51, ptr %PC, align 8
  %52 = add i64 %51, 4
  store i64 %52, ptr %NEXT_PC, align 8
  %X410 = ptrtoint ptr %X4 to i64
  %53 = load ptr, ptr %MEMORY, align 8
  store i64 0, ptr %X4, align 8
  store ptr %53, ptr %MEMORY, align 8
  br label %54

54:                                               ; preds = %50
  %55 = load i64, ptr %NEXT_PC, align 8
  store i64 %55, ptr %PC, align 8
  %56 = add i64 %55, 4
  store i64 %56, ptr %NEXT_PC, align 8
  %57 = load i64, ptr %PC, align 8
  %58 = sub i64 %57, 235468
  %59 = load i64, ptr %PC, align 8
  %60 = add i64 %59, 4
  %NEXT_PC11 = ptrtoint ptr %NEXT_PC to i64
  %RETURN_PC12 = ptrtoint ptr %RETURN_PC to i64
  %61 = load ptr, ptr %MEMORY, align 8
  %x30.i31 = getelementptr inbounds %struct.AArch64State, ptr %state, i64 0, i32 3, i32 61
  store i64 %60, ptr %x30.i31, align 16
  %pc.i32 = getelementptr inbounds %struct.AArch64State, ptr %state, i64 0, i32 3, i32 65
  store i64 %58, ptr %pc.i32, align 16
  store i64 %58, ptr %NEXT_PC, align 8
  store i64 %60, ptr %RETURN_PC, align 8
  store ptr %61, ptr %MEMORY, align 8
  %62 = load ptr, ptr %MEMORY, align 8
  %63 = load i64, ptr %PC, align 8
  %64 = call ptr @sub_13f53e0(ptr %state, i64 %63, ptr %62)
  %65 = load i64, ptr %RETURN_PC, align 8
  store i64 %65, ptr %NEXT_PC, align 8
  br label %66

66:                                               ; preds = %54
  %67 = load i64, ptr %NEXT_PC, align 8
  store i64 %67, ptr %PC, align 8
  %68 = add i64 %67, 4
  store i64 %68, ptr %NEXT_PC, align 8
  %69 = load i64, ptr %PC, align 8
  %70 = sub i64 %69, 234032
  %71 = load i64, ptr %PC, align 8
  %72 = add i64 %71, 4
  %NEXT_PC13 = ptrtoint ptr %NEXT_PC to i64
  %RETURN_PC14 = ptrtoint ptr %RETURN_PC to i64
  %73 = load ptr, ptr %MEMORY, align 8
  %x30.i = getelementptr inbounds %struct.AArch64State, ptr %state, i64 0, i32 3, i32 61
  store i64 %72, ptr %x30.i, align 16
  %pc.i30 = getelementptr inbounds %struct.AArch64State, ptr %state, i64 0, i32 3, i32 65
  store i64 %70, ptr %pc.i30, align 16
  store i64 %70, ptr %NEXT_PC, align 8
  store i64 %72, ptr %RETURN_PC, align 8
  store ptr %73, ptr %MEMORY, align 8
  %74 = load ptr, ptr %MEMORY, align 8
  %75 = load i64, ptr %PC, align 8
  %76 = call ptr @sub_13f5980(ptr %state, i64 %75, ptr %74)
  %77 = load i64, ptr %RETURN_PC, align 8
  store i64 %77, ptr %NEXT_PC, align 8
  br label %78

78:                                               ; preds = %66
  %79 = load i64, ptr %NEXT_PC, align 8
  store i64 %79, ptr %PC, align 8
  %80 = add i64 %79, 4
  store i64 %80, ptr %NEXT_PC, align 8
  %X015 = ptrtoint ptr %X0 to i64
  %81 = load i64, ptr %PC, align 8
  %82 = add i64 %81, 150126592
  %83 = load ptr, ptr %MEMORY, align 8
  %and.i.i29 = and i64 %82, -4096
  store i64 %and.i.i29, ptr %X0, align 8
  store ptr %83, ptr %MEMORY, align 8
  br label %84

84:                                               ; preds = %78
  %85 = load i64, ptr %NEXT_PC, align 8
  store i64 %85, ptr %PC, align 8
  %86 = add i64 %85, 4
  store i64 %86, ptr %NEXT_PC, align 8
  %X016 = ptrtoint ptr %X0 to i64
  %87 = load i64, ptr %X0, align 8
  %88 = add i64 %87, 2632
  %89 = load ptr, ptr %MEMORY, align 8
  %call.i.i28 = call i64 @__remill_read_memory_64(ptr noundef %89, i64 noundef %88) #2
  store i64 %call.i.i28, ptr %X0, align 8
  store ptr %89, ptr %MEMORY, align 8
  br label %90

90:                                               ; preds = %84
  %91 = load i64, ptr %NEXT_PC, align 8
  store i64 %91, ptr %PC, align 8
  %92 = add i64 %91, 4
  store i64 %92, ptr %NEXT_PC, align 8
  %BRANCH_TAKEN17 = ptrtoint ptr %BRANCH_TAKEN to i64
  %93 = load i64, ptr %PC, align 8
  %94 = add i64 %93, 8
  %95 = load i64, ptr %PC, align 8
  %96 = add i64 %95, 4
  %97 = load i64, ptr %X0, align 8
  %NEXT_PC18 = ptrtoint ptr %NEXT_PC to i64
  %98 = load ptr, ptr %MEMORY, align 8
  %cmp.i.i = icmp eq i64 %97, 0
  %conv.i = zext i1 %cmp.i.i to i8
  store i8 %conv.i, ptr %BRANCH_TAKEN, align 1
  %cond1.i.i = select i1 %cmp.i.i, i64 %94, i64 %96
  %pc.i27 = getelementptr inbounds %struct.AArch64State, ptr %state, i64 0, i32 3, i32 65
  store i64 %cond1.i.i, ptr %pc.i27, align 16
  store i64 %cond1.i.i, ptr %NEXT_PC, align 8
  store ptr %98, ptr %MEMORY, align 8
  %99 = load i8, ptr %BRANCH_TAKEN, align 1
  %100 = icmp eq i8 %99, 1
  br i1 %100, label %101, label %110

101:                                              ; preds = %90
  %102 = load i64, ptr %NEXT_PC, align 8
  store i64 %102, ptr %PC, align 8
  %103 = add i64 %102, 4
  store i64 %103, ptr %NEXT_PC, align 8
  %104 = load i64, ptr %X30, align 8
  %NEXT_PC24 = ptrtoint ptr %NEXT_PC to i64
  %105 = load ptr, ptr %MEMORY, align 8
  %pc.i26 = getelementptr inbounds %struct.AArch64State, ptr %state, i64 0, i32 3, i32 65
  store i64 %104, ptr %pc.i26, align 16
  store i64 %104, ptr %NEXT_PC, align 8
  store ptr %105, ptr %MEMORY, align 8
  %106 = load i64, ptr %NEXT_PC, align 8
  store i64 %106, ptr %PC, align 8
  %107 = load ptr, ptr %MEMORY, align 8
  %108 = load i64, ptr %PC, align 8
  %109 = tail call ptr @__remill_function_return(ptr %state, i64 %108, ptr %107)
  ret ptr %109

110:                                              ; preds = %90
  %111 = load i64, ptr %NEXT_PC, align 8
  store i64 %111, ptr %PC, align 8
  %112 = add i64 %111, 4
  store i64 %112, ptr %NEXT_PC, align 8
  %113 = load i64, ptr %PC, align 8
  %114 = sub i64 %113, 234688
  %NEXT_PC19 = ptrtoint ptr %NEXT_PC to i64
  %115 = load ptr, ptr %MEMORY, align 8
  %pc.i25 = getelementptr inbounds %struct.AArch64State, ptr %state, i64 0, i32 3, i32 65
  store i64 %114, ptr %pc.i25, align 16
  store i64 %114, ptr %NEXT_PC, align 8
  store ptr %115, ptr %MEMORY, align 8
  br label %116

116:                                              ; preds = %110
  %117 = load i64, ptr %NEXT_PC, align 8
  store i64 %117, ptr %PC, align 8
  %118 = add i64 %117, 4
  store i64 %118, ptr %NEXT_PC, align 8
  %X1620 = ptrtoint ptr %X16 to i64
  %119 = load i64, ptr %PC, align 8
  %120 = add i64 %119, 150376448
  %121 = load ptr, ptr %MEMORY, align 8
  %and.i.i = and i64 %120, -4096
  store i64 %and.i.i, ptr %X16, align 8
  store ptr %121, ptr %MEMORY, align 8
  br label %122

122:                                              ; preds = %116
  %123 = load i64, ptr %NEXT_PC, align 8
  store i64 %123, ptr %PC, align 8
  %124 = add i64 %123, 4
  store i64 %124, ptr %NEXT_PC, align 8
  %X1721 = ptrtoint ptr %X17 to i64
  %125 = load i64, ptr %X16, align 8
  %126 = add i64 %125, 1552
  %127 = load ptr, ptr %MEMORY, align 8
  %call.i.i = call i64 @__remill_read_memory_64(ptr noundef %127, i64 noundef %126) #2
  store i64 %call.i.i, ptr %X17, align 8
  store ptr %127, ptr %MEMORY, align 8
  br label %128

128:                                              ; preds = %122
  %129 = load i64, ptr %NEXT_PC, align 8
  store i64 %129, ptr %PC, align 8
  %130 = add i64 %129, 4
  store i64 %130, ptr %NEXT_PC, align 8
  %X1622 = ptrtoint ptr %X16 to i64
  %131 = load i64, ptr %X16, align 8
  %132 = load ptr, ptr %MEMORY, align 8
  %add.i.i = add i64 1552, %131
  store i64 %add.i.i, ptr %X16, align 8
  store ptr %132, ptr %MEMORY, align 8
  br label %133

133:                                              ; preds = %128
  %134 = load i64, ptr %NEXT_PC, align 8
  store i64 %134, ptr %PC, align 8
  %135 = add i64 %134, 4
  store i64 %135, ptr %NEXT_PC, align 8
  %136 = load i64, ptr %X17, align 8
  %NEXT_PC23 = ptrtoint ptr %NEXT_PC to i64
  %137 = load ptr, ptr %MEMORY, align 8
  %pc.i = getelementptr inbounds %struct.AArch64State, ptr %state, i64 0, i32 3, i32 65
  store i64 %136, ptr %pc.i, align 16
  store i64 %136, ptr %NEXT_PC, align 8
  store ptr %137, ptr %MEMORY, align 8
  %138 = load i64, ptr %NEXT_PC, align 8
  store i64 %138, ptr %PC, align 8
  %139 = load ptr, ptr %MEMORY, align 8
  %140 = load i64, ptr %PC, align 8
  %141 = tail call ptr @__remill_jump(ptr %state, i64 %140, ptr %139)
  ret ptr %141
}

declare ptr @sub_13f53e0(ptr noalias, i64, ptr noalias)

declare ptr @sub_13f5980(ptr noalias, i64, ptr noalias)

; Function Attrs: noduplicate noinline nounwind optnone
declare ptr @__remill_function_return(ptr noundef nonnull align 1, i64 noundef, ptr noundef) #1

attributes #0 = { mustprogress noduplicate nofree noinline nosync nounwind optnone readnone willreturn "frame-pointer"="non-leaf" "no-builtins" "no-trapping-math"="true" "stack-protector-buffer-size"="8" }
attributes #1 = { noduplicate noinline nounwind optnone "frame-pointer"="non-leaf" "no-builtins" "no-trapping-math"="true" "stack-protector-buffer-size"="8" }
attributes #2 = { nobuiltin nounwind readnone willreturn "no-builtins" }
