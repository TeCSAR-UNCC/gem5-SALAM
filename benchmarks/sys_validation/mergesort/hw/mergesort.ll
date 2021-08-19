; ModuleID = 'mergesort.c'
source_filename = "mergesort.c"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

; Function Attrs: nofree noinline norecurse nounwind uwtable
define dso_local void @merge(i32, i32, i32) local_unnamed_addr #0 {
  br label %5

4:                                                ; preds = %5
  ret void

5:                                                ; preds = %5, %3
  %6 = phi i64 [ 0, %3 ], [ %10, %5 ]
  %7 = getelementptr inbounds i32, i32* inttoptr (i64 789577728 to i32*), i64 %6
  %8 = load i32, i32* %7, align 4, !tbaa !2
  %9 = getelementptr inbounds i32, i32* inttoptr (i64 789579776 to i32*), i64 %6
  store i32 %8, i32* %9, align 4, !tbaa !2
  %10 = add nuw nsw i64 %6, 1
  %11 = icmp eq i64 %10, 5
  br i1 %11, label %4, label %5
}

; Function Attrs: nofree noinline norecurse nounwind uwtable
define dso_local void @mergesort() local_unnamed_addr #0 {
  br label %1

1:                                                ; preds = %0, %8
  %2 = phi i32 [ 1, %0 ], [ %9, %8 ]
  %3 = shl i32 %2, 1
  br label %4

4:                                                ; preds = %1, %4
  %5 = phi i32 [ 0, %1 ], [ %6, %4 ]
  tail call void @merge(i32 undef, i32 undef, i32 undef) #1
  %6 = add nsw i32 %5, %3
  %7 = icmp slt i32 %6, 2048
  br i1 %7, label %4, label %8

8:                                                ; preds = %4
  %9 = shl nsw i32 %2, 1
  %10 = icmp slt i32 %2, 1024
  br i1 %10, label %1, label %11

11:                                               ; preds = %8
  ret void
}

attributes #0 = { nofree noinline norecurse nounwind uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nobuiltin }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{!"clang version 9.0.0-2~ubuntu18.04.2 (tags/RELEASE_900/final)"}
!2 = !{!3, !3, i64 0}
!3 = !{!"int", !4, i64 0}
!4 = !{!"omnipotent char", !5, i64 0}
!5 = !{!"Simple C/C++ TBAA"}
