; ModuleID = 'top.c'
source_filename = "top.c"
target datalayout = "e-m:e-p:32:32-Fi8-i64:64-v128:64:128-a:0:32-n32-S64"
target triple = "armv7-pc-none-eabi"

; Function Attrs: nofree norecurse nounwind
define dso_local void @vadd(i32* nocapture readonly %a, i32* nocapture readonly %b, i32* nocapture %c) local_unnamed_addr #0 {
entry:
  br label %for.body

for.body:                                         ; preds = %for.body, %entry
  %i.09 = phi i32 [ 0, %entry ], [ %inc, %for.body ]
  %arrayidx = getelementptr inbounds i32, i32* %a, i32 %i.09
  %0 = load i32, i32* %arrayidx, align 4, !tbaa !3
  %arrayidx1 = getelementptr inbounds i32, i32* %b, i32 %i.09
  %1 = load i32, i32* %arrayidx1, align 4, !tbaa !3
  %add = add i32 %1, %0
  %arrayidx2 = getelementptr inbounds i32, i32* %c, i32 %i.09
  store i32 %add, i32* %arrayidx2, align 4, !tbaa !3
  %inc = add nuw nsw i32 %i.09, 1
  %exitcond = icmp eq i32 %inc, 256
  br i1 %exitcond, label %for.end, label %for.body

for.end:                                          ; preds = %for.body
  ret void
}


; Function Attrs: nofree norecurse nounwind
define dso_local void @top() local_unnamed_addr #0 {
entry:
  call void @vadd(i32* nonnull inttoptr (i32 268566656 to i32*), i32* nonnull inttoptr (i32 268567744 to i32*), i32* nonnull inttoptr (i32 268568832 to i32*))
  ret void
}

attributes #0 = { nofree norecurse nounwind "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "frame-pointer"="all" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="generic" "target-features"="+armv7-a,+d32,+dsp,+fp64,+neon,+strict-align,+vfp2,+vfp2sp,+vfp3,+vfp3d16,+vfp3d16sp,+vfp3sp,-thumb-mode" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.module.flags = !{!0, !1}
!llvm.ident = !{!2}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 1, !"min_enum_size", i32 4}
!2 = !{!"clang version 10.0.1 (https://github.com/llvm/llvm-project.git ef32c611aa214dea855364efd7ba451ec5ec3f74)"}
!3 = !{!4, !4, i64 0}
!4 = !{!"int", !5, i64 0}
!5 = !{!"omnipotent char", !6, i64 0}
!6 = !{!"Simple C/C++ TBAA"}
