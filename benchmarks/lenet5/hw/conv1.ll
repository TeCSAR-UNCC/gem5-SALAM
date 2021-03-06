; ModuleID = 'conv1.bc'
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: norecurse nounwind uwtable
define void @conv1() #0 {
entry:
  br label %for.cond1.preheader

for.cond1.preheader:                              ; preds = %for.inc63, %entry
  %indvars.iv36 = phi i64 [ 0, %entry ], [ %indvars.iv.next37, %for.inc63 ]
  %0 = mul nuw nsw i64 %indvars.iv36, 25
  %1 = add nuw nsw i64 %0, 12
  %2 = mul nuw nsw i64 %indvars.iv36, 100
  %3 = add nsw i64 %2, -22
  br label %for.cond4.preheader

for.cond4.preheader:                              ; preds = %for.inc60, %for.cond1.preheader
  %indvars.iv32 = phi i64 [ 0, %for.cond1.preheader ], [ %indvars.iv.next33, %for.inc60 ]
  %4 = mul nuw nsw i64 %indvars.iv32, 196
  br label %for.cond7.preheader

for.cond7.preheader:                              ; preds = %for.inc57, %for.cond4.preheader
  %indvars.iv27 = phi i64 [ 0, %for.cond4.preheader ], [ %indvars.iv.next28, %for.inc57 ]
  %cmp12 = icmp slt i64 %indvars.iv27, 2
  %5 = add nuw nsw i64 %indvars.iv27, 2
  %cmp17 = icmp sgt i64 %5, 13
  %6 = mul nuw nsw i64 %indvars.iv27, 10
  %7 = add nsw i64 %3, %6
  br label %for.body9

for.body9:                                        ; preds = %for.inc54, %for.cond7.preheader
  %indvars.iv21 = phi i64 [ 0, %for.cond7.preheader ], [ %indvars.iv.next22, %for.inc54 ]
  %cmp10 = icmp slt i64 %indvars.iv21, 2
  %or.cond = or i1 %cmp12, %cmp10
  %8 = add nuw nsw i64 %indvars.iv21, 2
  %cmp14 = icmp sgt i64 %8, 13
  %or.cond1 = or i1 %or.cond, %cmp14
  %or.cond2 = or i1 %cmp17, %or.cond1
  br i1 %or.cond2, label %for.inc54, label %for.cond18.preheader

for.cond18.preheader:                             ; preds = %for.body9
  %9 = add nuw nsw i64 %indvars.iv21, %4
  br label %for.cond21.preheader

for.cond21.preheader:                             ; preds = %for.inc40, %for.cond18.preheader
  %indvars.iv14 = phi i64 [ -2, %for.cond18.preheader ], [ %indvars.iv.next15, %for.inc40 ]
  %sum.06 = phi i32 [ 0, %for.cond18.preheader ], [ %add39.lcssa, %for.inc40 ]
  %10 = add nsw i64 %indvars.iv14, %indvars.iv27
  %11 = mul nsw i64 %10, 14
  %12 = add nsw i64 %9, %11
  %13 = add nsw i64 %1, %indvars.iv14
  br label %for.inc

for.inc:                                          ; preds = %for.inc, %for.cond21.preheader
  %indvars.iv = phi i64 [ -2, %for.cond21.preheader ], [ %indvars.iv.next, %for.inc ]
  %sum.14 = phi i32 [ %sum.06, %for.cond21.preheader ], [ %add39, %for.inc ]
  %14 = add nsw i64 %12, %indvars.iv
  %arrayidx = getelementptr inbounds i8, i8* inttoptr (i64 268578319 to i8*), i64 %14
  %15 = load i8, i8* %arrayidx, align 1
  %conv = zext i8 %15 to i32
  %16 = mul nsw i64 %indvars.iv, 5
  %17 = add nsw i64 %13, %16
  %arrayidx36 = getelementptr inbounds i8, i8* inttoptr (i64 268579495 to i8*), i64 %17
  %18 = load i8, i8* %arrayidx36, align 1
  %conv37 = zext i8 %18 to i32
  %mul38 = mul nuw nsw i32 %conv37, %conv
  %add39 = add nsw i32 %mul38, %sum.14
  %indvars.iv.next = add nsw i64 %indvars.iv, 1
  %exitcond = icmp eq i64 %indvars.iv.next, 2
  br i1 %exitcond, label %for.inc40, label %for.inc, !llvm.loop !1

for.inc40:                                        ; preds = %for.inc
  %add39.lcssa = phi i32 [ %add39, %for.inc ]
  %indvars.iv.next15 = add nsw i64 %indvars.iv14, 1
  %exitcond20 = icmp eq i64 %indvars.iv.next15, 2
  br i1 %exitcond20, label %for.end42, label %for.cond21.preheader, !llvm.loop !3

for.end42:                                        ; preds = %for.inc40
  %add39.lcssa.lcssa = phi i32 [ %add39.lcssa, %for.inc40 ]
  %19 = add nsw i64 %7, %indvars.iv21
  %arrayidx50 = getelementptr inbounds i8, i8* inttoptr (i64 268579895 to i8*), i64 %19
  %20 = load i8, i8* %arrayidx50, align 1
  %conv51 = zext i8 %20 to i32
  %add52 = add nsw i32 %conv51, %add39.lcssa.lcssa
  %conv53 = trunc i32 %add52 to i8
  store i8 %conv53, i8* %arrayidx50, align 1
  br label %for.inc54

for.inc54:                                        ; preds = %for.body9, %for.end42
  %indvars.iv.next22 = add nuw nsw i64 %indvars.iv21, 1
  %exitcond26 = icmp eq i64 %indvars.iv.next22, 14
  br i1 %exitcond26, label %for.inc57, label %for.body9, !llvm.loop !4

for.inc57:                                        ; preds = %for.inc54
  %indvars.iv.next28 = add nuw nsw i64 %indvars.iv27, 1
  %exitcond31 = icmp eq i64 %indvars.iv.next28, 14
  br i1 %exitcond31, label %for.inc60, label %for.cond7.preheader, !llvm.loop !5

for.inc60:                                        ; preds = %for.inc57
  %indvars.iv.next33 = add nuw nsw i64 %indvars.iv32, 1
  %exitcond35 = icmp eq i64 %indvars.iv.next33, 6
  br i1 %exitcond35, label %for.inc63, label %for.cond4.preheader, !llvm.loop !6

for.inc63:                                        ; preds = %for.inc60
  %indvars.iv.next37 = add nuw nsw i64 %indvars.iv36, 1
  %exitcond42 = icmp eq i64 %indvars.iv.next37, 16
  br i1 %exitcond42, label %for.end65, label %for.cond1.preheader, !llvm.loop !7

for.end65:                                        ; preds = %for.inc63
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
