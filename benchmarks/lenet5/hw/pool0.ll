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
  %4 = mul nuw nsw i64 %indvars.iv18, 28
  %5 = or i64 %indvars.iv18, 1
  %6 = mul nuw nsw i64 %5, 28
  br label %for.inc.1

for.inc35:                                        ; preds = %for.inc.1
  %indvars.iv.next19 = add nuw nsw i64 %indvars.iv18, 2
  %cmp2 = icmp slt i64 %indvars.iv.next19, 28
  br i1 %cmp2, label %for.cond4.preheader, label %for.inc38

for.inc38:                                        ; preds = %for.inc35
  %indvars.iv.next21 = add nuw nsw i64 %indvars.iv20, 1
  %exitcond24 = icmp eq i64 %indvars.iv.next21, 6
  br i1 %exitcond24, label %for.end40, label %for.cond1.preheader

for.end40:                                        ; preds = %for.inc38
  ret void

for.inc.1:                                        ; preds = %for.inc.1, %for.cond4.preheader
  %indvars.iv15 = phi i64 [ 0, %for.cond4.preheader ], [ %indvars.iv.next16, %for.inc.1 ]
  %7 = add nuw nsw i64 %indvars.iv15, %0
  %8 = add nuw nsw i64 %7, %4
  %9 = add nuw nsw i64 %7, %6
  %10 = or i64 %8, 1
  %arrayidx.1 = getelementptr inbounds i8, i8* inttoptr (i64 268572438 to i8*), i64 %10
  %11 = load i8, i8* %arrayidx.1, align 1
  %conv.1 = zext i8 %11 to i32
  %arrayidx = getelementptr inbounds i8, i8* inttoptr (i64 268572438 to i8*), i64 %8
  %12 = load i8, i8* %arrayidx, align 2
  %conv = zext i8 %12 to i32
  %add17.1 = add nuw nsw i32 %conv.1, %conv
  %arrayidx.125 = getelementptr inbounds i8, i8* inttoptr (i64 268572438 to i8*), i64 %9
  %13 = load i8, i8* %arrayidx.125, align 2
  %conv.126 = zext i8 %13 to i32
  %add17.127 = add nuw nsw i32 %conv.126, %add17.1
  %14 = or i64 %9, 1
  %arrayidx.1.1 = getelementptr inbounds i8, i8* inttoptr (i64 268572438 to i8*), i64 %14
  %15 = load i8, i8* %arrayidx.1.1, align 1
  %conv.1.1 = zext i8 %15 to i32
  %add17.1.1 = add nuw nsw i32 %conv.1.1, %add17.127
  %div = lshr i32 %add17.1.1, 2
  %16 = trunc i64 %indvars.iv15 to i32
  %div25 = sdiv i32 %16, 2
  %add26 = add nsw i32 %add24, %div25
  %idxprom27 = sext i32 %add26 to i64
  %arrayidx28 = getelementptr inbounds i8, i8* inttoptr (i64 268577142 to i8*), i64 %idxprom27
  %17 = load i8, i8* %arrayidx28, align 1
  %conv29 = zext i8 %17 to i32
  %add30 = add nuw nsw i32 %conv29, %div
  %conv31 = trunc i32 %add30 to i8
  store i8 %conv31, i8* %arrayidx28, align 1
  %indvars.iv.next16 = add nuw nsw i64 %indvars.iv15, 2
  %cmp5 = icmp slt i64 %indvars.iv.next16, 28
  br i1 %cmp5, label %for.inc.1, label %for.inc35
}

attributes #0 = { norecurse nounwind uwtable "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.ident = !{!0}

!0 = !{!"clang version 3.8.1 "}
