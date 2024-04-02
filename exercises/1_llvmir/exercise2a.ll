%struct.Player = type { i32, [256 x i8], %struct.Vec3, %struct.Vec3, %struct.Bullet }
%struct.Vec3 = type { float, float, float }
%struct.Bullet = type { i32, %struct.Vec3 }

define dso_local i32 @exercise0(ptr nocapture noundef readonly %array, i64 noundef %n) local_unnamed_addr #0 {
entry:
  %arrayidx = getelementptr inbounds i64, ptr %array, i64 %n
  %0 = load i64, ptr %arrayidx, align 8
  %conv = trunc i64 %0 to i32
  ret i32 %conv
}

define dso_local i32 @exercise1(ptr nocapture noundef readonly %player) local_unnamed_addr #0 {
entry:
  %0 = load i32, ptr %player, align 4
  ret i32 %0
}

define dso_local nonnull ptr @exercise2(ptr noundef readnone %player) local_unnamed_addr #1 {
entry:
  %name = getelementptr inbounds %struct.Player, ptr %player, i64 0, i32 1
  ret ptr %name
}

define dso_local nonnull ptr @exercise3(ptr noundef readnone %player) local_unnamed_addr #2 {
entry:
  %position = getelementptr inbounds %struct.Player, ptr %player, i64 0, i32 2
  ret ptr %position
}

define dso_local float @exercise4(ptr noundef readonly %player) local_unnamed_addr #3 {
entry:
  %call = tail call ptr @exercise3(ptr noundef %player)
  %0 = load float, ptr %call, align 4
  %direction = getelementptr inbounds %struct.Player, ptr %player, i64 0, i32 3
  %1 = load float, ptr %direction, align 4
  %add = fadd float %0, %1
  ret float %add
}

define dso_local float @exercise5(ptr nocapture noundef readonly %player) local_unnamed_addr #0 {
entry:
  %z = getelementptr inbounds %struct.Player, ptr %player, i64 0, i32 4, i32 1, i32 2
  %0 = load float, ptr %z, align 4
  ret float %0
}

attributes #0 = { argmemonly mustprogress nofree norecurse nosync nounwind readonly willreturn uwtable "frame-pointer"="none" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { mustprogress nofree norecurse nosync nounwind readnone willreturn uwtable "frame-pointer"="none" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #2 = { mustprogress nofree noinline norecurse nosync nounwind readnone willreturn uwtable "frame-pointer"="none" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #3 = { mustprogress nofree norecurse nosync nounwind readonly willreturn uwtable "frame-pointer"="none" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
