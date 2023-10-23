; ModuleID = 'experiment.cpp'
source_filename = "experiment.cpp"
target datalayout = "e-m:e-i8:8:32-i16:16:32-i64:64-i128:128-n32:64-S128"
target triple = "aarch64-unknown-linux-gnu"

%struct.Reg = type { %union.anon }
%union.anon = type { i64 }

@rcx = external local_unnamed_addr global %struct.Reg, align 1
@rbx = external local_unnamed_addr global %struct.Reg, align 1
@rax = external local_unnamed_addr global %struct.Reg, align 1

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn uwtable
define dso_local void @lifted_block() local_unnamed_addr #0 {
entry:
  %0 = load i64, ptr @rcx, align 1, !tbaa !6
  %1 = load i64, ptr @rbx, align 1, !tbaa !6
  %mul = mul i64 %1, %0
  store i64 %mul, ptr @rax, align 1, !tbaa !6
  store i8 6, ptr @rax, align 1, !tbaa !6
  ret void
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn uwtable "frame-pointer"="non-leaf" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="generic" "target-features"="+neon,+outline-atomics,+v8a" }

!llvm.module.flags = !{!0, !1, !2, !3, !4}
!llvm.ident = !{!5}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 7, !"PIC Level", i32 2}
!2 = !{i32 7, !"PIE Level", i32 2}
!3 = !{i32 7, !"uwtable", i32 2}
!4 = !{i32 7, !"frame-pointer", i32 1}
!5 = !{!"clang version 15.0.4"}
!6 = !{!7, !7, i64 0}
!7 = !{!"omnipotent char", !8, i64 0}
!8 = !{!"Simple C++ TBAA"}
