; ModuleID = 'pool0.bc'
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: norecurse nounwind uwtable
define void @pool0() #0 {
entry:
  br label %for.cond1.preheader

for.cond1.preheader:                              ; preds = %entry, %for.inc57
  %indvars.iv38 = phi i64 [ 0, %entry ], [ %indvars.iv.next39, %for.inc57 ]
  br label %for.cond4.preheader

for.cond19.preheader:                             ; preds = %for.inc16
  %indvars.iv.next39 = add nuw nsw i64 %indvars.iv38, 2
  br label %for.cond29.preheader

for.cond4.preheader:                              ; preds = %for.inc16, %for.cond1.preheader
  %indvars.iv17 = phi i64 [ 0, %for.cond1.preheader ], [ %indvars.iv.next18, %for.inc16 ]
  %0 = mul nuw nsw i64 %indvars.iv17, 168
  br label %for.cond7.preheader

for.cond7.preheader:                              ; preds = %for.inc13, %for.cond4.preheader
  %indvars.iv12 = phi i64 [ 0, %for.cond4.preheader ], [ %indvars.iv.next13, %for.inc13 ]
  %1 = mul nuw nsw i64 %indvars.iv12, 6
  %2 = add nuw nsw i64 %1, %0
  br label %for.body9

for.body9:                                        ; preds = %for.body9, %for.cond7.preheader
  %indvars.iv = phi i64 [ 0, %for.cond7.preheader ], [ %indvars.iv.next, %for.body9 ]
  %3 = load i32, i32* inttoptr (i64 268567303 to i32*), align 4
  %4 = add nuw nsw i64 %2, %indvars.iv
  %arrayidx = getelementptr inbounds i32, i32* inttoptr (i64 268567308 to i32*), i64 %4
  store i32 %3, i32* %arrayidx, align 4
  %indvars.iv.next = add nuw nsw i64 %indvars.iv, 1
  %exitcond = icmp eq i64 %indvars.iv.next, 6
  br i1 %exitcond, label %for.inc13, label %for.body9, !llvm.loop !1

for.inc13:                                        ; preds = %for.body9
  %indvars.iv.next13 = add nuw nsw i64 %indvars.iv12, 1
  %exitcond16 = icmp eq i64 %indvars.iv.next13, 28
  br i1 %exitcond16, label %for.inc16, label %for.cond7.preheader, !llvm.loop !3

for.inc16:                                        ; preds = %for.inc13
  %indvars.iv.next18 = add nuw nsw i64 %indvars.iv17, 1
  %exitcond20 = icmp eq i64 %indvars.iv.next18, 2
  br i1 %exitcond20, label %for.cond19.preheader, label %for.cond4.preheader, !llvm.loop !4

for.cond19.backedge:                              ; preds = %for.inc48.5
  %add44.5.lcssa.lcssa = phi i32 [ %add44.5.lcssa, %for.inc48.5 ]
  %div.5 = sdiv i32 %add44.5.lcssa.lcssa, 4
  store i32 %div.5, i32* inttoptr (i64 268568652 to i32*), align 4
  %cmp20 = icmp slt i64 %indvars.iv.next36, 28
  br i1 %cmp20, label %for.cond29.preheader, label %for.inc57

for.cond29.preheader:                             ; preds = %for.cond19.preheader, %for.cond19.backedge
  %indvars.iv35 = phi i64 [ 0, %for.cond19.preheader ], [ %indvars.iv.next36, %for.cond19.backedge ]
  %indvars.iv.next36 = add nuw nsw i64 %indvars.iv35, 2
  %5 = add nuw nsw i64 %indvars.iv35, %indvars.iv38
  br label %for.cond32.preheader

for.cond32.preheader:                             ; preds = %for.inc48, %for.cond29.preheader
  %indvars.iv26 = phi i64 [ 0, %for.cond29.preheader ], [ %indvars.iv.next27, %for.inc48 ]
  %sum.07 = phi i32 [ 0, %for.cond29.preheader ], [ %add44.lcssa, %for.inc48 ]
  %6 = mul nuw nsw i64 %indvars.iv26, 168
  %7 = add nuw nsw i64 %5, %6
  br label %for.inc45

for.inc45:                                        ; preds = %for.inc45, %for.cond32.preheader
  %indvars.iv21 = phi i64 [ 0, %for.cond32.preheader ], [ %indvars.iv.next22, %for.inc45 ]
  %sum.15 = phi i32 [ %sum.07, %for.cond32.preheader ], [ %add44, %for.inc45 ]
  %8 = mul nuw nsw i64 %indvars.iv21, 6
  %9 = add nuw nsw i64 %7, %8
  %arrayidx43 = getelementptr inbounds i32, i32* inttoptr (i64 268567308 to i32*), i64 %9
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
  store i32 %div, i32* inttoptr (i64 268568652 to i32*), align 4
  %11 = or i64 %5, 1
  br label %for.cond32.preheader.1

for.inc57:                                        ; preds = %for.cond19.backedge
  %cmp = icmp slt i64 %indvars.iv.next39, 28
  br i1 %cmp, label %for.cond1.preheader, label %for.end59, !llvm.loop !7

for.end59:                                        ; preds = %for.inc57
  ret void

for.cond32.preheader.1:                           ; preds = %for.inc48.1, %for.end50
  %indvars.iv26.1 = phi i64 [ 0, %for.end50 ], [ %indvars.iv.next27.1, %for.inc48.1 ]
  %sum.07.1 = phi i32 [ 0, %for.end50 ], [ %add44.1.lcssa, %for.inc48.1 ]
  %12 = mul nuw nsw i64 %indvars.iv26.1, 168
  %13 = add nuw nsw i64 %11, %12
  br label %for.inc45.1

for.inc45.1:                                      ; preds = %for.inc45.1, %for.cond32.preheader.1
  %indvars.iv21.1 = phi i64 [ 0, %for.cond32.preheader.1 ], [ %indvars.iv.next22.1, %for.inc45.1 ]
  %sum.15.1 = phi i32 [ %sum.07.1, %for.cond32.preheader.1 ], [ %add44.1, %for.inc45.1 ]
  %14 = mul nuw nsw i64 %indvars.iv21.1, 6
  %15 = add nuw nsw i64 %13, %14
  %arrayidx43.1 = getelementptr inbounds i32, i32* inttoptr (i64 268567308 to i32*), i64 %15
  %16 = load i32, i32* %arrayidx43.1, align 4
  %add44.1 = add i32 %16, %sum.15.1
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
  store i32 %div.1, i32* inttoptr (i64 268568652 to i32*), align 4
  %17 = add nuw nsw i64 %5, 2
  br label %for.cond32.preheader.2

for.cond32.preheader.2:                           ; preds = %for.inc48.2, %for.end50.1
  %indvars.iv26.2 = phi i64 [ 0, %for.end50.1 ], [ %indvars.iv.next27.2, %for.inc48.2 ]
  %sum.07.2 = phi i32 [ 0, %for.end50.1 ], [ %add44.2.lcssa, %for.inc48.2 ]
  %18 = mul nuw nsw i64 %indvars.iv26.2, 168
  %19 = add nuw nsw i64 %17, %18
  br label %for.inc45.2

for.inc45.2:                                      ; preds = %for.inc45.2, %for.cond32.preheader.2
  %indvars.iv21.2 = phi i64 [ 0, %for.cond32.preheader.2 ], [ %indvars.iv.next22.2, %for.inc45.2 ]
  %sum.15.2 = phi i32 [ %sum.07.2, %for.cond32.preheader.2 ], [ %add44.2, %for.inc45.2 ]
  %20 = mul nuw nsw i64 %indvars.iv21.2, 6
  %21 = add nuw nsw i64 %19, %20
  %arrayidx43.2 = getelementptr inbounds i32, i32* inttoptr (i64 268567308 to i32*), i64 %21
  %22 = load i32, i32* %arrayidx43.2, align 4
  %add44.2 = add i32 %22, %sum.15.2
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
  store i32 %div.2, i32* inttoptr (i64 268568652 to i32*), align 4
  %23 = add nuw nsw i64 %5, 3
  br label %for.cond32.preheader.3

for.cond32.preheader.3:                           ; preds = %for.inc48.3, %for.end50.2
  %indvars.iv26.3 = phi i64 [ 0, %for.end50.2 ], [ %indvars.iv.next27.3, %for.inc48.3 ]
  %sum.07.3 = phi i32 [ 0, %for.end50.2 ], [ %add44.3.lcssa, %for.inc48.3 ]
  %24 = mul nuw nsw i64 %indvars.iv26.3, 168
  %25 = add nuw nsw i64 %23, %24
  br label %for.inc45.3

for.inc45.3:                                      ; preds = %for.inc45.3, %for.cond32.preheader.3
  %indvars.iv21.3 = phi i64 [ 0, %for.cond32.preheader.3 ], [ %indvars.iv.next22.3, %for.inc45.3 ]
  %sum.15.3 = phi i32 [ %sum.07.3, %for.cond32.preheader.3 ], [ %add44.3, %for.inc45.3 ]
  %26 = mul nuw nsw i64 %indvars.iv21.3, 6
  %27 = add nuw nsw i64 %25, %26
  %arrayidx43.3 = getelementptr inbounds i32, i32* inttoptr (i64 268567308 to i32*), i64 %27
  %28 = load i32, i32* %arrayidx43.3, align 4
  %add44.3 = add i32 %28, %sum.15.3
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
  store i32 %div.3, i32* inttoptr (i64 268568652 to i32*), align 4
  %29 = add nuw nsw i64 %5, 4
  br label %for.cond32.preheader.4

for.cond32.preheader.4:                           ; preds = %for.inc48.4, %for.end50.3
  %indvars.iv26.4 = phi i64 [ 0, %for.end50.3 ], [ %indvars.iv.next27.4, %for.inc48.4 ]
  %sum.07.4 = phi i32 [ 0, %for.end50.3 ], [ %add44.4.lcssa, %for.inc48.4 ]
  %30 = mul nuw nsw i64 %indvars.iv26.4, 168
  %31 = add nuw nsw i64 %29, %30
  br label %for.inc45.4

for.inc45.4:                                      ; preds = %for.inc45.4, %for.cond32.preheader.4
  %indvars.iv21.4 = phi i64 [ 0, %for.cond32.preheader.4 ], [ %indvars.iv.next22.4, %for.inc45.4 ]
  %sum.15.4 = phi i32 [ %sum.07.4, %for.cond32.preheader.4 ], [ %add44.4, %for.inc45.4 ]
  %32 = mul nuw nsw i64 %indvars.iv21.4, 6
  %33 = add nuw nsw i64 %31, %32
  %arrayidx43.4 = getelementptr inbounds i32, i32* inttoptr (i64 268567308 to i32*), i64 %33
  %34 = load i32, i32* %arrayidx43.4, align 4
  %add44.4 = add i32 %34, %sum.15.4
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
  store i32 %div.4, i32* inttoptr (i64 268568652 to i32*), align 4
  %35 = add nuw nsw i64 %5, 5
  br label %for.cond32.preheader.5

for.cond32.preheader.5:                           ; preds = %for.inc48.5, %for.end50.4
  %indvars.iv26.5 = phi i64 [ 0, %for.end50.4 ], [ %indvars.iv.next27.5, %for.inc48.5 ]
  %sum.07.5 = phi i32 [ 0, %for.end50.4 ], [ %add44.5.lcssa, %for.inc48.5 ]
  %36 = mul nuw nsw i64 %indvars.iv26.5, 168
  %37 = add nuw nsw i64 %35, %36
  br label %for.inc45.5

for.inc45.5:                                      ; preds = %for.inc45.5, %for.cond32.preheader.5
  %indvars.iv21.5 = phi i64 [ 0, %for.cond32.preheader.5 ], [ %indvars.iv.next22.5, %for.inc45.5 ]
  %sum.15.5 = phi i32 [ %sum.07.5, %for.cond32.preheader.5 ], [ %add44.5, %for.inc45.5 ]
  %38 = mul nuw nsw i64 %indvars.iv21.5, 6
  %39 = add nuw nsw i64 %37, %38
  %arrayidx43.5 = getelementptr inbounds i32, i32* inttoptr (i64 268567308 to i32*), i64 %39
  %40 = load i32, i32* %arrayidx43.5, align 4
  %add44.5 = add i32 %40, %sum.15.5
  %indvars.iv.next22.5 = add nuw nsw i64 %indvars.iv21.5, 1
  %exitcond25.5 = icmp eq i64 %indvars.iv.next22.5, 2
  br i1 %exitcond25.5, label %for.inc48.5, label %for.inc45.5, !llvm.loop !5

for.inc48.5:                                      ; preds = %for.inc45.5
  %add44.5.lcssa = phi i32 [ %add44.5, %for.inc45.5 ]
  %indvars.iv.next27.5 = add nuw nsw i64 %indvars.iv26.5, 1
  %exitcond30.5 = icmp eq i64 %indvars.iv.next27.5, 2
  br i1 %exitcond30.5, label %for.cond19.backedge, label %for.cond32.preheader.5, !llvm.loop !6
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
