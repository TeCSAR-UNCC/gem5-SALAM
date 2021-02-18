; ModuleID = 'pool1.bc'
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: norecurse nounwind uwtable
define void @pool1() #0 {
entry:
  br label %for.cond1.preheader

for.cond1.preheader:                              ; preds = %for.inc53, %entry
  %k.011 = phi i32 [ 0, %entry ], [ %inc54, %for.inc53 ]
  br label %for.cond4.preheader

for.cond4.preheader:                              ; preds = %for.cond1.preheader, %for.inc50
  %indvars.iv27 = phi i64 [ 1, %for.cond1.preheader ], [ %indvars.iv.next28, %for.inc50 ]
  %0 = trunc i64 %indvars.iv27 to i32
  %div = sdiv i32 %0, 2
  %mul42 = mul nsw i32 %div, 10
  %1 = mul i64 %indvars.iv27, 10
  %2 = add i64 %1, -10
  %3 = mul nuw nsw i64 %indvars.iv27, 10
  br label %for.cond7.preheader

for.cond7.preheader:                              ; preds = %for.cond4.preheader, %for.cond7.preheader
  %indvars.iv24 = phi i64 [ 1, %for.cond4.preheader ], [ %indvars.iv.next25, %for.cond7.preheader ]
  %4 = add nsw i64 %indvars.iv24, -1
  %5 = add nuw nsw i64 %4, %2
  %arrayidx = getelementptr inbounds i8, i8* inttoptr (i64 268686226 to i8*), i64 %5
  %6 = load i8, i8* %arrayidx, align 1
  %7 = load i8, i8* inttoptr (i64 268699029 to i8*), align 1
  %mul32 = mul i8 %7, %6
  %8 = add nuw nsw i64 %indvars.iv24, %2
  %arrayidx.1 = getelementptr inbounds i8, i8* inttoptr (i64 268686226 to i8*), i64 %8
  %9 = load i8, i8* %arrayidx.1, align 1
  %10 = load i8, i8* inttoptr (i64 268699028 to i8*), align 4
  %mul32.1 = mul i8 %10, %9
  %11 = add nuw nsw i64 %4, %3
  %arrayidx.131 = getelementptr inbounds i8, i8* inttoptr (i64 268686226 to i8*), i64 %11
  %12 = load i8, i8* %arrayidx.131, align 1
  %13 = load i8, i8* inttoptr (i64 268699027 to i8*), align 1
  %mul32.133 = mul i8 %13, %12
  %14 = add nuw nsw i64 %indvars.iv24, %3
  %arrayidx.1.1 = getelementptr inbounds i8, i8* inttoptr (i64 268686226 to i8*), i64 %14
  %15 = load i8, i8* %arrayidx.1.1, align 1
  %16 = load i8, i8* inttoptr (i64 268699026 to i8*), align 2
  %mul32.1.1 = mul i8 %16, %15
  %add34.1 = add i8 %mul32.1, %mul32
  %add34.134 = add i8 %mul32.133, %add34.1
  %add34.1.1 = add i8 %mul32.1.1, %add34.134
  %add34.138 = add i8 %mul32, %add34.1.1
  %add34.1.139 = add i8 %mul32.1, %add34.138
  %add34.134.1 = add i8 %mul32.133, %add34.1.139
  %add34.1.1.1 = add i8 %mul32.1.1, %add34.134.1
  %add34.2 = add i8 %mul32, %add34.1.1.1
  %add34.1.2 = add i8 %mul32.1, %add34.2
  %add34.134.2 = add i8 %mul32.133, %add34.1.2
  %add34.1.1.2 = add i8 %mul32.1.1, %add34.134.2
  %add34.3 = add i8 %mul32, %add34.1.1.2
  %add34.1.3 = add i8 %mul32.1, %add34.3
  %add34.134.3 = add i8 %mul32.133, %add34.1.3
  %add34.1.1.3 = add i8 %mul32.1.1, %add34.134.3
  %add34.4 = add i8 %mul32, %add34.1.1.3
  %add34.1.4 = add i8 %mul32.1, %add34.4
  %add34.134.4 = add i8 %mul32.133, %add34.1.4
  %add34.1.1.4 = add i8 %mul32.1.1, %add34.134.4
  %add34.5 = add i8 %mul32, %add34.1.1.4
  %add34.1.5 = add i8 %mul32.1, %add34.5
  %add34.134.5 = add i8 %mul32.133, %add34.1.5
  %add34.1.1.5 = add i8 %mul32.1.1, %add34.134.5
  %add34.6 = add i8 %mul32, %add34.1.1.5
  %add34.1.6 = add i8 %mul32.1, %add34.6
  %add34.134.6 = add i8 %mul32.133, %add34.1.6
  %add34.1.1.6 = add i8 %mul32.1.1, %add34.134.6
  %add34.7 = add i8 %mul32, %add34.1.1.6
  %add34.1.7 = add i8 %mul32.1, %add34.7
  %add34.134.7 = add i8 %mul32.133, %add34.1.7
  %add34.1.1.7 = add i8 %mul32.1.1, %add34.134.7
  %add34.8 = add i8 %mul32, %add34.1.1.7
  %add34.1.8 = add i8 %mul32.1, %add34.8
  %add34.134.8 = add i8 %mul32.133, %add34.1.8
  %add34.1.1.8 = add i8 %mul32.1.1, %add34.134.8
  %add34.9 = add i8 %mul32, %add34.1.1.8
  %add34.1.9 = add i8 %mul32.1, %add34.9
  %add34.134.9 = add i8 %mul32.133, %add34.1.9
  %add34.1.1.9 = add i8 %mul32.1.1, %add34.134.9
  %add34.10 = add i8 %mul32, %add34.1.1.9
  %add34.1.10 = add i8 %mul32.1, %add34.10
  %add34.134.10 = add i8 %mul32.133, %add34.1.10
  %add34.1.1.10 = add i8 %mul32.1.1, %add34.134.10
  %add34.11 = add i8 %mul32, %add34.1.1.10
  %add34.1.11 = add i8 %mul32.1, %add34.11
  %add34.134.11 = add i8 %mul32.133, %add34.1.11
  %add34.1.1.11 = add i8 %mul32.1.1, %add34.134.11
  %add34.12 = add i8 %mul32, %add34.1.1.11
  %add34.1.12 = add i8 %mul32.1, %add34.12
  %add34.134.12 = add i8 %mul32.133, %add34.1.12
  %add34.1.1.12 = add i8 %mul32.1.1, %add34.134.12
  %add34.13 = add i8 %mul32, %add34.1.1.12
  %add34.1.13 = add i8 %mul32.1, %add34.13
  %add34.134.13 = add i8 %mul32.133, %add34.1.13
  %add34.1.1.13 = add i8 %mul32.1.1, %add34.134.13
  %add34.14 = add i8 %mul32, %add34.1.1.13
  %add34.1.14 = add i8 %mul32.1, %add34.14
  %add34.134.14 = add i8 %mul32.133, %add34.1.14
  %add34.1.1.14 = add i8 %mul32.1.1, %add34.134.14
  %add34.15 = add i8 %mul32, %add34.1.1.14
  %add34.1.15 = add i8 %mul32.1, %add34.15
  %add34.134.15 = add i8 %mul32.133, %add34.1.15
  %add34.1.1.15 = add i8 %mul32.1.1, %add34.134.15
  %17 = trunc i64 %indvars.iv24 to i32
  %div43 = sdiv i32 %17, 2
  %add44 = add nsw i32 %div43, %mul42
  %idxprom45 = sext i32 %add44 to i64
  %arrayidx46 = getelementptr inbounds i8, i8* inttoptr (i64 268699538 to i8*), i64 %idxprom45
  store i8 %add34.1.1.15, i8* %arrayidx46, align 1
  %indvars.iv.next25 = add nuw nsw i64 %indvars.iv24, 2
  %cmp5 = icmp slt i64 %indvars.iv.next25, 9
  br i1 %cmp5, label %for.cond7.preheader, label %for.inc50

for.inc50:                                        ; preds = %for.cond7.preheader
  %indvars.iv.next28 = add nuw nsw i64 %indvars.iv27, 2
  %cmp2 = icmp slt i64 %indvars.iv.next28, 9
  br i1 %cmp2, label %for.cond4.preheader, label %for.inc53

for.inc53:                                        ; preds = %for.inc50
  %inc54 = add nuw nsw i32 %k.011, 1
  %exitcond30 = icmp eq i32 %inc54, 16
  br i1 %exitcond30, label %for.end55, label %for.cond1.preheader

for.end55:                                        ; preds = %for.inc53
  ret void
}

attributes #0 = { norecurse nounwind uwtable "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.ident = !{!0}

!0 = !{!"clang version 3.8.1 "}
