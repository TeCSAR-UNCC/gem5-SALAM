; ModuleID = 'top.c'
source_filename = "top.c"
target datalayout = "e-m:e-p:32:32-Fi8-i64:64-v128:64:128-a:0:32-n32-S64"
target triple = "armv7-pc-none-eabi"

%struct.BTree = type { [7 x i32], [8 x %struct.BTree*], i32, i32 }

; Function Attrs: nofree norecurse nounwind
define dso_local void @top(i32* nocapture readonly %a, i32* nocapture %b, i32* nocapture readonly %c) local_unnamed_addr #0 {
entry:
  %0 = bitcast i32* %c to %struct.BTree**
  %1 = load %struct.BTree*, %struct.BTree** %0, align 4, !tbaa !3
  %2 = load i32, i32* %a, align 4, !tbaa !3
  %n = getelementptr inbounds %struct.BTree, %struct.BTree* %1, i32 0, i32 3
  %3 = load i32, i32* %n, align 4, !tbaa !7
  %cmp23 = icmp sgt i32 %3, 0
  br i1 %cmp23, label %for.body, label %cleanup

for.cond:                                         ; preds = %if.end
  %4 = load i32, i32* %n, align 4, !tbaa !7
  %cmp = icmp slt i32 %inc, %4
  br i1 %cmp, label %for.body, label %cleanup

for.body:                                         ; preds = %entry, %for.cond
  %i.024 = phi i32 [ %inc, %for.cond ], [ 0, %entry ]
  %arrayidx = getelementptr inbounds %struct.BTree, %struct.BTree* %1, i32 0, i32 0, i32 %i.024
  %5 = load i32, i32* %arrayidx, align 4, !tbaa !3
  %cmp1 = icmp ult i32 %2, %5
  br i1 %cmp1, label %if.then, label %if.end

if.then:                                          ; preds = %for.body
  %arrayidx2 = getelementptr inbounds %struct.BTree, %struct.BTree* %1, i32 0, i32 1, i32 0
  %6 = load %struct.BTree*, %struct.BTree** %arrayidx2, align 4, !tbaa !9
  br label %cleanup

if.end:                                           ; preds = %for.body
  %cmp5 = icmp eq i32 %2, %5
  %inc = add nuw nsw i32 %i.024, 1
  br i1 %cmp5, label %if.then6, label %for.cond

if.then6:                                         ; preds = %if.end
  store i32 1, i32* %b, align 4, !tbaa !3
  br label %cleanup

cleanup:                                          ; preds = %for.cond, %entry, %if.then6, %if.then
  %cursor.0 = phi %struct.BTree* [ %6, %if.then ], [ %1, %if.then6 ], [ %1, %entry ], [ %1, %for.cond ]
  %7 = ptrtoint %struct.BTree* %cursor.0 to i32
  store i32 %7, i32* %b, align 4, !tbaa !3
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
!7 = !{!8, !4, i64 64}
!8 = !{!"BTree", !5, i64 0, !5, i64 28, !4, i64 60, !4, i64 64}
!9 = !{!10, !10, i64 0}
!10 = !{!"any pointer", !5, i64 0}
