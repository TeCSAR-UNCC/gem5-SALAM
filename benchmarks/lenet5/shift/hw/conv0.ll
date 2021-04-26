; ModuleID = 'conv0.bc'
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: norecurse nounwind uwtable
define void @conv0() #0 {
entry:
  br label %for.cond1.preheader

for.cond1.preheader:                              ; preds = %for.inc13, %entry
  %indvars.iv48 = phi i64 [ 0, %entry ], [ %indvars.iv.next49, %for.inc13 ]
  %0 = shl nsw i64 %indvars.iv48, 5
  br label %for.inc10

for.inc10:                                        ; preds = %for.inc10, %for.cond1.preheader
  %indvars.iv43 = phi i64 [ 0, %for.cond1.preheader ], [ %indvars.iv.next44, %for.inc10 ]
  %1 = load i32, i32* inttoptr (i64 268566581 to i32*), align 4
  %2 = add i64 %indvars.iv43, %0
  %arrayidx = getelementptr inbounds i32, i32* inttoptr (i64 268566599 to i32*), i64 %2
  store i32 %1, i32* %arrayidx, align 4
  %indvars.iv.next44 = add nuw nsw i64 %indvars.iv43, 1
  %exitcond47 = icmp eq i64 %indvars.iv.next44, 32
  br i1 %exitcond47, label %for.inc13, label %for.inc10, !llvm.loop !1

for.inc13:                                        ; preds = %for.inc10
  %indvars.iv.next49 = add nuw nsw i64 %indvars.iv48, 1
  %exitcond51 = icmp eq i64 %indvars.iv.next49, 5
  br i1 %exitcond51, label %for.body18.preheader, label %for.cond1.preheader, !llvm.loop !3

for.body18.preheader:                             ; preds = %for.inc13
  br label %for.body18

for.body18:                                       ; preds = %for.body18.preheader, %for.inc98
  %h.111 = phi i32 [ %inc99, %for.inc98 ], [ 0, %for.body18.preheader ]
  %cmp19 = icmp sgt i32 %h.111, 4
  br i1 %cmp19, label %for.cond20.preheader, label %for.cond40.preheader

for.cond20.preheader:                             ; preds = %for.body18
  %rem26 = srem i32 %h.111, 5
  %mul27 = shl nsw i32 %rem26, 5
  %3 = sext i32 %mul27 to i64
  br label %for.inc37

for.cond40.preheader.loopexit:                    ; preds = %for.inc37
  br label %for.cond40.preheader

for.cond40.preheader:                             ; preds = %for.cond40.preheader.loopexit, %for.body18
  %add45 = add nuw nsw i32 %h.111, 5
  %cmp46 = icmp sgt i32 %add45, 32
  br label %for.body42

for.inc37:                                        ; preds = %for.inc37, %for.cond20.preheader
  %indvars.iv16 = phi i64 [ 0, %for.cond20.preheader ], [ %indvars.iv.next17, %for.inc37 ]
  %4 = load i32, i32* inttoptr (i64 268566581 to i32*), align 4
  %5 = add i64 %indvars.iv16, %3
  %arrayidx33 = getelementptr inbounds i32, i32* inttoptr (i64 268566599 to i32*), i64 %5
  store i32 %4, i32* %arrayidx33, align 4
  %indvars.iv.next17 = add nuw nsw i64 %indvars.iv16, 1
  %exitcond = icmp eq i64 %indvars.iv.next17, 32
  br i1 %exitcond, label %for.cond40.preheader.loopexit, label %for.inc37, !llvm.loop !4

for.body42:                                       ; preds = %for.inc95, %for.cond40.preheader
  %w.210 = phi i32 [ 0, %for.cond40.preheader ], [ %inc96, %for.inc95 ]
  %add43 = add nuw nsw i32 %w.210, 5
  %cmp44 = icmp sgt i32 %add43, 32
  %or.cond = or i1 %cmp46, %cmp44
  br i1 %or.cond, label %for.inc95, label %for.cond51.preheader.preheader

for.cond51.preheader.preheader:                   ; preds = %for.body42
  br label %for.cond51.preheader

for.cond51.preheader:                             ; preds = %for.cond51.preheader.preheader, %for.end88
  %indvars.iv34 = phi i64 [ %indvars.iv.next35, %for.end88 ], [ 0, %for.cond51.preheader.preheader ]
  %6 = mul nuw nsw i64 %indvars.iv34, 25
  br label %for.cond54.preheader

for.cond54.preheader:                             ; preds = %for.inc86, %for.cond51.preheader
  %indvars.iv28 = phi i64 [ 0, %for.cond51.preheader ], [ %indvars.iv.next29, %for.inc86 ]
  %sum.08 = phi i32 [ 0, %for.cond51.preheader ], [ %add79.lcssa, %for.inc86 ]
  %7 = shl i64 %indvars.iv28, 5
  %8 = mul nuw nsw i64 %indvars.iv28, 5
  %9 = add nuw nsw i64 %8, %6
  br label %for.inc83

for.inc83:                                        ; preds = %for.inc83, %for.cond54.preheader
  %indvars.iv23 = phi i64 [ 0, %for.cond54.preheader ], [ %indvars.iv.next24, %for.inc83 ]
  %sum.16 = phi i32 [ %sum.08, %for.cond54.preheader ], [ %add79, %for.inc83 ]
  %10 = add i64 %indvars.iv23, %7
  %arrayidx66 = getelementptr inbounds i32, i32* inttoptr (i64 268566599 to i32*), i64 %10
  %11 = load i32, i32* %arrayidx66, align 4
  %12 = add i64 %9, %indvars.iv23
  %arrayidx77 = getelementptr inbounds i32, i32* inttoptr (i64 268566699 to i32*), i64 %12
  %13 = load i32, i32* %arrayidx77, align 4
  %mul78 = mul i32 %13, %11
  %add79 = add i32 %mul78, %sum.16
  %indvars.iv.next24 = add nuw nsw i64 %indvars.iv23, 1
  %exitcond27 = icmp eq i64 %indvars.iv.next24, 5
  br i1 %exitcond27, label %for.inc86, label %for.inc83, !llvm.loop !5

for.inc86:                                        ; preds = %for.inc83
  %add79.lcssa = phi i32 [ %add79, %for.inc83 ]
  %indvars.iv.next29 = add nuw nsw i64 %indvars.iv28, 1
  %exitcond33 = icmp eq i64 %indvars.iv.next29, 5
  br i1 %exitcond33, label %for.end88, label %for.cond54.preheader, !llvm.loop !6

for.end88:                                        ; preds = %for.inc86
  %add79.lcssa.lcssa = phi i32 [ %add79.lcssa, %for.inc86 ]
  %14 = load i32, i32* inttoptr (i64 268567299 to i32*), align 4
  %mul90 = mul i32 %14, %add79.lcssa.lcssa
  store i32 %mul90, i32* inttoptr (i64 268567303 to i32*), align 4
  %indvars.iv.next35 = add nuw nsw i64 %indvars.iv34, 1
  %exitcond37 = icmp eq i64 %indvars.iv.next35, 6
  br i1 %exitcond37, label %for.inc95.loopexit, label %for.cond51.preheader, !llvm.loop !7

for.inc95.loopexit:                               ; preds = %for.end88
  br label %for.inc95

for.inc95:                                        ; preds = %for.inc95.loopexit, %for.body42
  %inc96 = add nuw nsw i32 %w.210, 1
  %exitcond38 = icmp eq i32 %inc96, 32
  br i1 %exitcond38, label %for.inc98, label %for.body42, !llvm.loop !8

for.inc98:                                        ; preds = %for.inc95
  %inc99 = add nuw nsw i32 %h.111, 1
  %exitcond39 = icmp eq i32 %inc99, 32
  br i1 %exitcond39, label %for.end100, label %for.body18, !llvm.loop !9

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
