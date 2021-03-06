; ModuleID = 'pool0.bc'
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: norecurse nounwind uwtable
define void @pool0() #0 {
entry:
  br label %for.cond1.preheader

for.cond1.preheader:                              ; preds = %for.inc38, %entry
  %indvars.iv20 = phi i64 [ 0, %entry ], [ %indvars.iv.next21, %for.inc38 ]
  %0 = mul nuw nsw i64 %indvars.iv20, 784
  %1 = mul nuw nsw i64 %indvars.iv20, 196
  %2 = trunc i64 %1 to i32
  br label %for.cond4.preheader

for.cond4.preheader:                              ; preds = %for.cond1.preheader, %for.inc35
  %indvars.iv18 = phi i64 [ 0, %for.cond1.preheader ], [ %indvars.iv.next19, %for.inc35 ]
  %3 = trunc i64 %indvars.iv18 to i32
  %div22 = sdiv i32 %3, 2
  %mul23 = mul nsw i32 %div22, 14
  %add24 = add nsw i32 %mul23, %2
  br label %for.cond7.preheader

for.cond7.preheader:                              ; preds = %for.cond4.preheader, %for.end20
  %indvars.iv15 = phi i64 [ 0, %for.cond4.preheader ], [ %indvars.iv.next16, %for.end20 ]
  %4 = add nuw nsw i64 %indvars.iv15, %0
  br label %for.cond10.preheader

for.cond10.preheader:                             ; preds = %for.inc18, %for.cond7.preheader
  %indvars.iv9 = phi i64 [ 0, %for.cond7.preheader ], [ %indvars.iv.next10, %for.inc18 ]
  %sum.04 = phi i32 [ 0, %for.cond7.preheader ], [ %add17.lcssa, %for.inc18 ]
  %5 = add nuw nsw i64 %indvars.iv9, %indvars.iv18
  %6 = mul nuw nsw i64 %5, 28
  %7 = add nuw nsw i64 %4, %6
  br label %for.inc

for.inc:                                          ; preds = %for.inc, %for.cond10.preheader
  %indvars.iv = phi i64 [ 0, %for.cond10.preheader ], [ %indvars.iv.next, %for.inc ]
  %sum.12 = phi i32 [ %sum.04, %for.cond10.preheader ], [ %add17, %for.inc ]
  %8 = add nuw nsw i64 %7, %indvars.iv
  %arrayidx = getelementptr inbounds i8, i8* inttoptr (i64 268572438 to i8*), i64 %8
  %9 = load i8, i8* %arrayidx, align 1
  %conv = zext i8 %9 to i32
  %add17 = add nsw i32 %conv, %sum.12
  %indvars.iv.next = add nuw nsw i64 %indvars.iv, 1
  %exitcond = icmp eq i64 %indvars.iv.next, 2
  br i1 %exitcond, label %for.inc18, label %for.inc, !llvm.loop !1

for.inc18:                                        ; preds = %for.inc
  %add17.lcssa = phi i32 [ %add17, %for.inc ]
  %indvars.iv.next10 = add nuw nsw i64 %indvars.iv9, 1
  %exitcond14 = icmp eq i64 %indvars.iv.next10, 2
  br i1 %exitcond14, label %for.end20, label %for.cond10.preheader, !llvm.loop !3

for.end20:                                        ; preds = %for.inc18
  %add17.lcssa.lcssa = phi i32 [ %add17.lcssa, %for.inc18 ]
  %div = sdiv i32 %add17.lcssa.lcssa, 4
  %10 = trunc i64 %indvars.iv15 to i32
  %div25 = sdiv i32 %10, 2
  %add26 = add nsw i32 %add24, %div25
  %idxprom27 = sext i32 %add26 to i64
  %arrayidx28 = getelementptr inbounds i8, i8* inttoptr (i64 268577142 to i8*), i64 %idxprom27
  %11 = load i8, i8* %arrayidx28, align 1
  %conv29 = zext i8 %11 to i32
  %add30 = add nsw i32 %conv29, %div
  %conv31 = trunc i32 %add30 to i8
  store i8 %conv31, i8* %arrayidx28, align 1
  %indvars.iv.next16 = add nuw nsw i64 %indvars.iv15, 2
  %cmp5 = icmp slt i64 %indvars.iv.next16, 28
  br i1 %cmp5, label %for.cond7.preheader, label %for.inc35, !llvm.loop !4

for.inc35:                                        ; preds = %for.end20
  %indvars.iv.next19 = add nuw nsw i64 %indvars.iv18, 2
  %cmp2 = icmp slt i64 %indvars.iv.next19, 28
  br i1 %cmp2, label %for.cond4.preheader, label %for.inc38, !llvm.loop !5

for.inc38:                                        ; preds = %for.inc35
  %indvars.iv.next21 = add nuw nsw i64 %indvars.iv20, 1
  %exitcond24 = icmp eq i64 %indvars.iv.next21, 6
  br i1 %exitcond24, label %for.end40, label %for.cond1.preheader, !llvm.loop !6

for.end40:                                        ; preds = %for.inc38
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
