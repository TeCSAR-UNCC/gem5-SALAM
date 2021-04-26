; ModuleID = 'pool1.bc'
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: norecurse nounwind uwtable
define void @pool1() #0 {
entry:
  br label %for.cond1.preheader

for.cond1.preheader:                              ; preds = %entry, %for.cond19.backedge.4
  %indvars.iv38 = phi i64 [ 0, %entry ], [ %indvars.iv.next39, %for.cond19.backedge.4 ]
  br label %for.cond4.preheader

for.cond4.preheader:                              ; preds = %for.inc16, %for.cond1.preheader
  %indvars.iv17 = phi i64 [ 0, %for.cond1.preheader ], [ %indvars.iv.next18, %for.inc16 ]
  %0 = mul nuw nsw i64 %indvars.iv17, 160
  br label %for.cond7.preheader

for.cond7.preheader:                              ; preds = %for.inc13, %for.cond4.preheader
  %indvars.iv12 = phi i64 [ 0, %for.cond4.preheader ], [ %indvars.iv.next13, %for.inc13 ]
  %1 = shl i64 %indvars.iv12, 4
  %2 = add nuw nsw i64 %1, %0
  br label %for.body9

for.body9:                                        ; preds = %for.body9, %for.cond7.preheader
  %indvars.iv = phi i64 [ 0, %for.cond7.preheader ], [ %indvars.iv.next, %for.body9 ]
  %3 = load i32, i32* inttoptr (i64 268579941 to i32*), align 4
  %4 = add nuw nsw i64 %2, %indvars.iv
  %arrayidx = getelementptr inbounds i32, i32* inttoptr (i64 268579946 to i32*), i64 %4
  store i32 %3, i32* %arrayidx, align 4
  %indvars.iv.next = add nuw nsw i64 %indvars.iv, 1
  %exitcond = icmp eq i64 %indvars.iv.next, 16
  br i1 %exitcond, label %for.inc13, label %for.body9, !llvm.loop !1

for.inc13:                                        ; preds = %for.body9
  %indvars.iv.next13 = add nuw nsw i64 %indvars.iv12, 1
  %exitcond16 = icmp eq i64 %indvars.iv.next13, 10
  br i1 %exitcond16, label %for.inc16, label %for.cond7.preheader, !llvm.loop !3

for.inc16:                                        ; preds = %for.inc13
  %indvars.iv.next18 = add nuw nsw i64 %indvars.iv17, 1
  %exitcond20 = icmp eq i64 %indvars.iv.next18, 2
  br i1 %exitcond20, label %for.cond26.preheader, label %for.cond4.preheader, !llvm.loop !4

for.cond26.preheader:                             ; preds = %for.inc16
  %indvars.iv.next39 = add nuw nsw i64 %indvars.iv38, 2
  br label %for.cond29.preheader

for.cond29.preheader:                             ; preds = %for.end50, %for.cond26.preheader
  %indvars.iv31 = phi i64 [ 0, %for.cond26.preheader ], [ %indvars.iv.next32, %for.end50 ]
  %5 = add nuw nsw i64 %indvars.iv38, %indvars.iv31
  br label %for.cond32.preheader

for.cond32.preheader:                             ; preds = %for.inc48, %for.cond29.preheader
  %indvars.iv26 = phi i64 [ 0, %for.cond29.preheader ], [ %indvars.iv.next27, %for.inc48 ]
  %sum.07 = phi i32 [ 0, %for.cond29.preheader ], [ %add44.lcssa, %for.inc48 ]
  %6 = mul nuw nsw i64 %indvars.iv26, 160
  %7 = add nuw nsw i64 %5, %6
  br label %for.inc45

for.inc45:                                        ; preds = %for.inc45, %for.cond32.preheader
  %indvars.iv21 = phi i64 [ 0, %for.cond32.preheader ], [ %indvars.iv.next22, %for.inc45 ]
  %sum.15 = phi i32 [ %sum.07, %for.cond32.preheader ], [ %add44, %for.inc45 ]
  %8 = shl i64 %indvars.iv21, 4
  %9 = add nuw nsw i64 %7, %8
  %arrayidx43 = getelementptr inbounds i32, i32* inttoptr (i64 268579946 to i32*), i64 %9
  %10 = load i32, i32* %arrayidx43, align 4
  %add44 = add i32 %10, %sum.15
  %indvars.iv.next22 = add nuw nsw i64 %indvars.iv21, 1
  %exitcond25 = icmp eq i64 %indvars.iv.next22, 2
  br i1 %exitcond25, label %for.inc48, label %for.inc45, !llvm.loop !5

for.inc48:                                        ; preds = %for.inc45
  %add44.lcssa = phi i32 [ %add44, %for.inc45 ]
  %indvars.iv.next27 = add nuw nsw i64 %indvars.iv26, 1
  %exitcond30 = icmp eq i64 %indvars.iv.next27, 2
  br i1 %exitcond30, label %for.end50, label %for.cond32.preheader, !llvm.loop !6

for.end50:                                        ; preds = %for.inc48
  %add44.lcssa.lcssa = phi i32 [ %add44.lcssa, %for.inc48 ]
  %div = sdiv i32 %add44.lcssa.lcssa, 4
  store i32 %div, i32* inttoptr (i64 268583150 to i32*), align 4
  %indvars.iv.next32 = add nuw nsw i64 %indvars.iv31, 1
  %exitcond34 = icmp eq i64 %indvars.iv.next32, 16
  br i1 %exitcond34, label %for.cond26.preheader.1, label %for.cond29.preheader

for.end59:                                        ; preds = %for.cond19.backedge.4
  ret void

for.cond26.preheader.1:                           ; preds = %for.end50
  %11 = add nuw nsw i64 %indvars.iv38, 2
  br label %for.cond29.preheader.1

for.cond29.preheader.1:                           ; preds = %for.end50.1, %for.cond26.preheader.1
  %indvars.iv31.1 = phi i64 [ 0, %for.cond26.preheader.1 ], [ %indvars.iv.next32.1, %for.end50.1 ]
  %12 = add nuw nsw i64 %11, %indvars.iv31.1
  br label %for.cond32.preheader.1

for.cond32.preheader.1:                           ; preds = %for.inc48.1, %for.cond29.preheader.1
  %indvars.iv26.1 = phi i64 [ 0, %for.cond29.preheader.1 ], [ %indvars.iv.next27.1, %for.inc48.1 ]
  %sum.07.1 = phi i32 [ 0, %for.cond29.preheader.1 ], [ %add44.1.lcssa, %for.inc48.1 ]
  %13 = mul nuw nsw i64 %indvars.iv26.1, 160
  %14 = add nuw nsw i64 %12, %13
  br label %for.inc45.1

for.inc45.1:                                      ; preds = %for.inc45.1, %for.cond32.preheader.1
  %indvars.iv21.1 = phi i64 [ 0, %for.cond32.preheader.1 ], [ %indvars.iv.next22.1, %for.inc45.1 ]
  %sum.15.1 = phi i32 [ %sum.07.1, %for.cond32.preheader.1 ], [ %add44.1, %for.inc45.1 ]
  %15 = shl i64 %indvars.iv21.1, 4
  %16 = add nuw nsw i64 %14, %15
  %arrayidx43.1 = getelementptr inbounds i32, i32* inttoptr (i64 268579946 to i32*), i64 %16
  %17 = load i32, i32* %arrayidx43.1, align 4
  %add44.1 = add i32 %17, %sum.15.1
  %indvars.iv.next22.1 = add nuw nsw i64 %indvars.iv21.1, 1
  %exitcond25.1 = icmp eq i64 %indvars.iv.next22.1, 2
  br i1 %exitcond25.1, label %for.inc48.1, label %for.inc45.1, !llvm.loop !5

for.inc48.1:                                      ; preds = %for.inc45.1
  %add44.1.lcssa = phi i32 [ %add44.1, %for.inc45.1 ]
  %indvars.iv.next27.1 = add nuw nsw i64 %indvars.iv26.1, 1
  %exitcond30.1 = icmp eq i64 %indvars.iv.next27.1, 2
  br i1 %exitcond30.1, label %for.end50.1, label %for.cond32.preheader.1, !llvm.loop !6

for.end50.1:                                      ; preds = %for.inc48.1
  %add44.1.lcssa.lcssa = phi i32 [ %add44.1.lcssa, %for.inc48.1 ]
  %div.1 = sdiv i32 %add44.1.lcssa.lcssa, 4
  store i32 %div.1, i32* inttoptr (i64 268583150 to i32*), align 4
  %indvars.iv.next32.1 = add nuw nsw i64 %indvars.iv31.1, 1
  %exitcond34.1 = icmp eq i64 %indvars.iv.next32.1, 16
  br i1 %exitcond34.1, label %for.cond26.preheader.2, label %for.cond29.preheader.1

for.cond26.preheader.2:                           ; preds = %for.end50.1
  %18 = add nuw nsw i64 %indvars.iv38, 4
  br label %for.cond29.preheader.2

for.cond29.preheader.2:                           ; preds = %for.end50.2, %for.cond26.preheader.2
  %indvars.iv31.2 = phi i64 [ 0, %for.cond26.preheader.2 ], [ %indvars.iv.next32.2, %for.end50.2 ]
  %19 = add nuw nsw i64 %18, %indvars.iv31.2
  br label %for.cond32.preheader.2

for.cond32.preheader.2:                           ; preds = %for.inc48.2, %for.cond29.preheader.2
  %indvars.iv26.2 = phi i64 [ 0, %for.cond29.preheader.2 ], [ %indvars.iv.next27.2, %for.inc48.2 ]
  %sum.07.2 = phi i32 [ 0, %for.cond29.preheader.2 ], [ %add44.2.lcssa, %for.inc48.2 ]
  %20 = mul nuw nsw i64 %indvars.iv26.2, 160
  %21 = add nuw nsw i64 %19, %20
  br label %for.inc45.2

for.inc45.2:                                      ; preds = %for.inc45.2, %for.cond32.preheader.2
  %indvars.iv21.2 = phi i64 [ 0, %for.cond32.preheader.2 ], [ %indvars.iv.next22.2, %for.inc45.2 ]
  %sum.15.2 = phi i32 [ %sum.07.2, %for.cond32.preheader.2 ], [ %add44.2, %for.inc45.2 ]
  %22 = shl i64 %indvars.iv21.2, 4
  %23 = add nuw nsw i64 %21, %22
  %arrayidx43.2 = getelementptr inbounds i32, i32* inttoptr (i64 268579946 to i32*), i64 %23
  %24 = load i32, i32* %arrayidx43.2, align 4
  %add44.2 = add i32 %24, %sum.15.2
  %indvars.iv.next22.2 = add nuw nsw i64 %indvars.iv21.2, 1
  %exitcond25.2 = icmp eq i64 %indvars.iv.next22.2, 2
  br i1 %exitcond25.2, label %for.inc48.2, label %for.inc45.2, !llvm.loop !5

for.inc48.2:                                      ; preds = %for.inc45.2
  %add44.2.lcssa = phi i32 [ %add44.2, %for.inc45.2 ]
  %indvars.iv.next27.2 = add nuw nsw i64 %indvars.iv26.2, 1
  %exitcond30.2 = icmp eq i64 %indvars.iv.next27.2, 2
  br i1 %exitcond30.2, label %for.end50.2, label %for.cond32.preheader.2, !llvm.loop !6

for.end50.2:                                      ; preds = %for.inc48.2
  %add44.2.lcssa.lcssa = phi i32 [ %add44.2.lcssa, %for.inc48.2 ]
  %div.2 = sdiv i32 %add44.2.lcssa.lcssa, 4
  store i32 %div.2, i32* inttoptr (i64 268583150 to i32*), align 4
  %indvars.iv.next32.2 = add nuw nsw i64 %indvars.iv31.2, 1
  %exitcond34.2 = icmp eq i64 %indvars.iv.next32.2, 16
  br i1 %exitcond34.2, label %for.cond26.preheader.3, label %for.cond29.preheader.2

for.cond26.preheader.3:                           ; preds = %for.end50.2
  %25 = add nuw nsw i64 %indvars.iv38, 6
  br label %for.cond29.preheader.3

for.cond29.preheader.3:                           ; preds = %for.end50.3, %for.cond26.preheader.3
  %indvars.iv31.3 = phi i64 [ 0, %for.cond26.preheader.3 ], [ %indvars.iv.next32.3, %for.end50.3 ]
  %26 = add nuw nsw i64 %25, %indvars.iv31.3
  br label %for.cond32.preheader.3

for.cond32.preheader.3:                           ; preds = %for.inc48.3, %for.cond29.preheader.3
  %indvars.iv26.3 = phi i64 [ 0, %for.cond29.preheader.3 ], [ %indvars.iv.next27.3, %for.inc48.3 ]
  %sum.07.3 = phi i32 [ 0, %for.cond29.preheader.3 ], [ %add44.3.lcssa, %for.inc48.3 ]
  %27 = mul nuw nsw i64 %indvars.iv26.3, 160
  %28 = add nuw nsw i64 %26, %27
  br label %for.inc45.3

for.inc45.3:                                      ; preds = %for.inc45.3, %for.cond32.preheader.3
  %indvars.iv21.3 = phi i64 [ 0, %for.cond32.preheader.3 ], [ %indvars.iv.next22.3, %for.inc45.3 ]
  %sum.15.3 = phi i32 [ %sum.07.3, %for.cond32.preheader.3 ], [ %add44.3, %for.inc45.3 ]
  %29 = shl i64 %indvars.iv21.3, 4
  %30 = add nuw nsw i64 %28, %29
  %arrayidx43.3 = getelementptr inbounds i32, i32* inttoptr (i64 268579946 to i32*), i64 %30
  %31 = load i32, i32* %arrayidx43.3, align 4
  %add44.3 = add i32 %31, %sum.15.3
  %indvars.iv.next22.3 = add nuw nsw i64 %indvars.iv21.3, 1
  %exitcond25.3 = icmp eq i64 %indvars.iv.next22.3, 2
  br i1 %exitcond25.3, label %for.inc48.3, label %for.inc45.3, !llvm.loop !5

for.inc48.3:                                      ; preds = %for.inc45.3
  %add44.3.lcssa = phi i32 [ %add44.3, %for.inc45.3 ]
  %indvars.iv.next27.3 = add nuw nsw i64 %indvars.iv26.3, 1
  %exitcond30.3 = icmp eq i64 %indvars.iv.next27.3, 2
  br i1 %exitcond30.3, label %for.end50.3, label %for.cond32.preheader.3, !llvm.loop !6

for.end50.3:                                      ; preds = %for.inc48.3
  %add44.3.lcssa.lcssa = phi i32 [ %add44.3.lcssa, %for.inc48.3 ]
  %div.3 = sdiv i32 %add44.3.lcssa.lcssa, 4
  store i32 %div.3, i32* inttoptr (i64 268583150 to i32*), align 4
  %indvars.iv.next32.3 = add nuw nsw i64 %indvars.iv31.3, 1
  %exitcond34.3 = icmp eq i64 %indvars.iv.next32.3, 16
  br i1 %exitcond34.3, label %for.cond26.preheader.4, label %for.cond29.preheader.3

for.cond26.preheader.4:                           ; preds = %for.end50.3
  %32 = add nuw nsw i64 %indvars.iv38, 8
  br label %for.cond29.preheader.4

for.cond29.preheader.4:                           ; preds = %for.end50.4, %for.cond26.preheader.4
  %indvars.iv31.4 = phi i64 [ 0, %for.cond26.preheader.4 ], [ %indvars.iv.next32.4, %for.end50.4 ]
  %33 = add nuw nsw i64 %32, %indvars.iv31.4
  br label %for.cond32.preheader.4

for.cond32.preheader.4:                           ; preds = %for.inc48.4, %for.cond29.preheader.4
  %indvars.iv26.4 = phi i64 [ 0, %for.cond29.preheader.4 ], [ %indvars.iv.next27.4, %for.inc48.4 ]
  %sum.07.4 = phi i32 [ 0, %for.cond29.preheader.4 ], [ %add44.4.lcssa, %for.inc48.4 ]
  %34 = mul nuw nsw i64 %indvars.iv26.4, 160
  %35 = add nuw nsw i64 %33, %34
  br label %for.inc45.4

for.inc45.4:                                      ; preds = %for.inc45.4, %for.cond32.preheader.4
  %indvars.iv21.4 = phi i64 [ 0, %for.cond32.preheader.4 ], [ %indvars.iv.next22.4, %for.inc45.4 ]
  %sum.15.4 = phi i32 [ %sum.07.4, %for.cond32.preheader.4 ], [ %add44.4, %for.inc45.4 ]
  %36 = shl i64 %indvars.iv21.4, 4
  %37 = add nuw nsw i64 %35, %36
  %arrayidx43.4 = getelementptr inbounds i32, i32* inttoptr (i64 268579946 to i32*), i64 %37
  %38 = load i32, i32* %arrayidx43.4, align 4
  %add44.4 = add i32 %38, %sum.15.4
  %indvars.iv.next22.4 = add nuw nsw i64 %indvars.iv21.4, 1
  %exitcond25.4 = icmp eq i64 %indvars.iv.next22.4, 2
  br i1 %exitcond25.4, label %for.inc48.4, label %for.inc45.4, !llvm.loop !5

for.inc48.4:                                      ; preds = %for.inc45.4
  %add44.4.lcssa = phi i32 [ %add44.4, %for.inc45.4 ]
  %indvars.iv.next27.4 = add nuw nsw i64 %indvars.iv26.4, 1
  %exitcond30.4 = icmp eq i64 %indvars.iv.next27.4, 2
  br i1 %exitcond30.4, label %for.end50.4, label %for.cond32.preheader.4, !llvm.loop !6

for.end50.4:                                      ; preds = %for.inc48.4
  %add44.4.lcssa.lcssa = phi i32 [ %add44.4.lcssa, %for.inc48.4 ]
  %div.4 = sdiv i32 %add44.4.lcssa.lcssa, 4
  store i32 %div.4, i32* inttoptr (i64 268583150 to i32*), align 4
  %indvars.iv.next32.4 = add nuw nsw i64 %indvars.iv31.4, 1
  %exitcond34.4 = icmp eq i64 %indvars.iv.next32.4, 16
  br i1 %exitcond34.4, label %for.cond19.backedge.4, label %for.cond29.preheader.4

for.cond19.backedge.4:                            ; preds = %for.end50.4
  %cmp = icmp slt i64 %indvars.iv.next39, 10
  br i1 %cmp, label %for.cond1.preheader, label %for.end59, !llvm.loop !7
}

attributes #0 = { norecurse nounwind uwtable "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.ident = !{!0}

!0 = !{!"clang version 3.8.1 "}
!1 = distinct !{!1, !2}
!2 = !{!"llvm.loop.unroll.disable"}
!3 = distinct !{!3, !2}
!4 = distinct !{!4, !2}
!5 = distinct !{!5, !2}
!6 = distinct !{!6, !2}
!7 = distinct !{!7, !2}
