; ModuleID = 'conv1.bc'
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: norecurse nounwind uwtable
define void @conv1() #0 {
entry:
  br label %for.cond1.preheader

for.cond1.preheader:                              ; preds = %for.inc13, %entry
  %indvars.iv53 = phi i64 [ 0, %entry ], [ %indvars.iv.next54, %for.inc13 ]
  %0 = mul nuw nsw i64 %indvars.iv53, 84
  br label %for.cond4.preheader

for.cond4.preheader:                              ; preds = %for.inc10, %for.cond1.preheader
  %indvars.iv48 = phi i64 [ 0, %for.cond1.preheader ], [ %indvars.iv.next49, %for.inc10 ]
  %1 = mul nuw nsw i64 %indvars.iv48, 6
  %2 = add nuw nsw i64 %1, %0
  br label %for.body6

for.body6:                                        ; preds = %for.body6, %for.cond4.preheader
  %indvars.iv44 = phi i64 [ 0, %for.cond4.preheader ], [ %indvars.iv.next45, %for.body6 ]
  %3 = load i32, i32* inttoptr (i64 268568652 to i32*), align 4
  %4 = add nsw i64 %2, %indvars.iv44
  %arrayidx = getelementptr inbounds i32, i32* inttoptr (i64 268568657 to i32*), i64 %4
  store i32 %3, i32* %arrayidx, align 4
  %indvars.iv.next45 = add nuw nsw i64 %indvars.iv44, 1
  %exitcond47 = icmp eq i64 %indvars.iv.next45, 6
  br i1 %exitcond47, label %for.inc10, label %for.body6, !llvm.loop !1

for.inc10:                                        ; preds = %for.body6
  %indvars.iv.next49 = add nuw nsw i64 %indvars.iv48, 1
  %exitcond52 = icmp eq i64 %indvars.iv.next49, 14
  br i1 %exitcond52, label %for.inc13, label %for.cond4.preheader, !llvm.loop !3

for.inc13:                                        ; preds = %for.inc10
  %indvars.iv.next54 = add nuw nsw i64 %indvars.iv53, 1
  %exitcond56 = icmp eq i64 %indvars.iv.next54, 5
  br i1 %exitcond56, label %for.body18.preheader, label %for.cond1.preheader, !llvm.loop !4

for.body18.preheader:                             ; preds = %for.inc13
  br label %for.body18

for.body18:                                       ; preds = %for.body18.preheader, %for.inc98
  %h.111 = phi i32 [ %inc99, %for.inc98 ], [ 0, %for.body18.preheader ]
  %cmp19 = icmp sgt i32 %h.111, 4
  br i1 %cmp19, label %for.cond20.preheader, label %for.cond40.preheader

for.cond20.preheader:                             ; preds = %for.body18
  %rem26 = srem i32 %h.111, 5
  %mul28 = mul nsw i32 %rem26, 84
  %5 = sext i32 %mul28 to i64
  br label %for.cond23.preheader

for.cond40.preheader.loopexit:                    ; preds = %for.inc37
  br label %for.cond40.preheader

for.cond40.preheader:                             ; preds = %for.cond40.preheader.loopexit, %for.body18
  %add45 = add nuw nsw i32 %h.111, 5
  %cmp46 = icmp sgt i32 %add45, 14
  br label %for.body42

for.cond23.preheader:                             ; preds = %for.inc37, %for.cond20.preheader
  %indvars.iv16 = phi i64 [ 0, %for.cond20.preheader ], [ %indvars.iv.next17, %for.inc37 ]
  %6 = mul nuw nsw i64 %indvars.iv16, 6
  %7 = add nsw i64 %6, %5
  br label %for.body25

for.body25:                                       ; preds = %for.body25, %for.cond23.preheader
  %indvars.iv = phi i64 [ 0, %for.cond23.preheader ], [ %indvars.iv.next, %for.body25 ]
  %8 = load i32, i32* inttoptr (i64 268568652 to i32*), align 4
  %9 = add nsw i64 %7, %indvars.iv
  %arrayidx33 = getelementptr inbounds i32, i32* inttoptr (i64 268568657 to i32*), i64 %9
  store i32 %8, i32* %arrayidx33, align 4
  %indvars.iv.next = add nuw nsw i64 %indvars.iv, 1
  %exitcond = icmp eq i64 %indvars.iv.next, 6
  br i1 %exitcond, label %for.inc37, label %for.body25, !llvm.loop !5

for.inc37:                                        ; preds = %for.body25
  %indvars.iv.next17 = add nuw nsw i64 %indvars.iv16, 1
  %exitcond20 = icmp eq i64 %indvars.iv.next17, 14
  br i1 %exitcond20, label %for.cond40.preheader.loopexit, label %for.cond23.preheader, !llvm.loop !6

for.body42:                                       ; preds = %for.inc95, %for.cond40.preheader
  %w.210 = phi i32 [ 0, %for.cond40.preheader ], [ %inc96, %for.inc95 ]
  %add43 = add nuw nsw i32 %w.210, 5
  %cmp44 = icmp sgt i32 %add43, 14
  %or.cond = or i1 %cmp46, %cmp44
  br i1 %or.cond, label %for.inc95, label %for.cond51.preheader.preheader

for.cond51.preheader.preheader:                   ; preds = %for.body42
  br label %for.cond51.preheader

for.cond51.preheader:                             ; preds = %for.cond51.preheader.preheader, %for.end88
  %indvars.iv38 = phi i64 [ %indvars.iv.next39, %for.end88 ], [ 0, %for.cond51.preheader.preheader ]
  %10 = mul nuw nsw i64 %indvars.iv38, 150
  br label %for.cond54.preheader

for.cond54.preheader:                             ; preds = %for.inc86, %for.cond51.preheader
  %indvars.iv32 = phi i64 [ 0, %for.cond51.preheader ], [ %indvars.iv.next33, %for.inc86 ]
  %sum.08 = phi i32 [ 0, %for.cond51.preheader ], [ %add79.lcssa.lcssa, %for.inc86 ]
  %11 = mul nuw nsw i64 %indvars.iv32, 84
  %12 = mul nuw nsw i64 %indvars.iv32, 30
  %13 = add nuw nsw i64 %12, %10
  br label %for.cond57.preheader

for.cond57.preheader:                             ; preds = %for.inc83, %for.cond54.preheader
  %indvars.iv26 = phi i64 [ 0, %for.cond54.preheader ], [ %indvars.iv.next27, %for.inc83 ]
  %sum.16 = phi i32 [ %sum.08, %for.cond54.preheader ], [ %add79.lcssa, %for.inc83 ]
  %14 = mul nuw nsw i64 %indvars.iv26, 6
  %15 = add nuw nsw i64 %14, %11
  %16 = add nuw nsw i64 %13, %14
  br label %for.inc80

for.inc80:                                        ; preds = %for.inc80, %for.cond57.preheader
  %indvars.iv21 = phi i64 [ 0, %for.cond57.preheader ], [ %indvars.iv.next22, %for.inc80 ]
  %sum.24 = phi i32 [ %sum.16, %for.cond57.preheader ], [ %add79, %for.inc80 ]
  %17 = add nuw nsw i64 %15, %indvars.iv21
  %arrayidx66 = getelementptr inbounds i32, i32* inttoptr (i64 268568657 to i32*), i64 %17
  %18 = load i32, i32* %arrayidx66, align 4
  %19 = add nuw nsw i64 %16, %indvars.iv21
  %arrayidx77 = getelementptr inbounds i32, i32* inttoptr (i64 268570337 to i32*), i64 %19
  %20 = load i32, i32* %arrayidx77, align 4
  %mul78 = mul i32 %20, %18
  %add79 = add i32 %mul78, %sum.24
  %indvars.iv.next22 = add nuw nsw i64 %indvars.iv21, 1
  %exitcond25 = icmp eq i64 %indvars.iv.next22, 6
  br i1 %exitcond25, label %for.inc83, label %for.inc80, !llvm.loop !7

for.inc83:                                        ; preds = %for.inc80
  %add79.lcssa = phi i32 [ %add79, %for.inc80 ]
  %indvars.iv.next27 = add nuw nsw i64 %indvars.iv26, 1
  %exitcond31 = icmp eq i64 %indvars.iv.next27, 5
  br i1 %exitcond31, label %for.inc86, label %for.cond57.preheader, !llvm.loop !8

for.inc86:                                        ; preds = %for.inc83
  %add79.lcssa.lcssa = phi i32 [ %add79.lcssa, %for.inc83 ]
  %indvars.iv.next33 = add nuw nsw i64 %indvars.iv32, 1
  %exitcond37 = icmp eq i64 %indvars.iv.next33, 5
  br i1 %exitcond37, label %for.end88, label %for.cond54.preheader, !llvm.loop !9

for.end88:                                        ; preds = %for.inc86
  %add79.lcssa.lcssa.lcssa = phi i32 [ %add79.lcssa.lcssa, %for.inc86 ]
  %21 = load i32, i32* inttoptr (i64 268579937 to i32*), align 4
  %mul90 = mul i32 %21, %add79.lcssa.lcssa.lcssa
  store i32 %mul90, i32* inttoptr (i64 268579941 to i32*), align 4
  %indvars.iv.next39 = add nuw nsw i64 %indvars.iv38, 1
  %exitcond41 = icmp eq i64 %indvars.iv.next39, 16
  br i1 %exitcond41, label %for.inc95.loopexit, label %for.cond51.preheader, !llvm.loop !10

for.inc95.loopexit:                               ; preds = %for.end88
  br label %for.inc95

for.inc95:                                        ; preds = %for.inc95.loopexit, %for.body42
  %inc96 = add nuw nsw i32 %w.210, 1
  %exitcond42 = icmp eq i32 %inc96, 14
  br i1 %exitcond42, label %for.inc98, label %for.body42, !llvm.loop !11

for.inc98:                                        ; preds = %for.inc95
  %inc99 = add nuw nsw i32 %h.111, 1
  %exitcond43 = icmp eq i32 %inc99, 14
  br i1 %exitcond43, label %for.end100, label %for.body18, !llvm.loop !12

for.end100:                                       ; preds = %for.inc98
  ret void
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
!8 = distinct !{!8, !2}
!9 = distinct !{!9, !2}
!10 = distinct !{!10, !2}
!11 = distinct !{!11, !2}
!12 = distinct !{!12, !2}
