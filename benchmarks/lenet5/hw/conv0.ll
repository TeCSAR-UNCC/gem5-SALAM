; ModuleID = 'conv0.bc'
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: norecurse nounwind uwtable
define void @conv0() #0 {
entry:
  br label %for.cond1.preheader

for.cond1.preheader:                              ; preds = %for.inc46, %entry
  %indvars.iv28 = phi i64 [ 1, %entry ], [ %indvars.iv.next29, %for.inc46 ]
  %0 = add nsw i64 %indvars.iv28, -2
  %1 = shl i64 %indvars.iv28, 5
  br label %for.cond4.preheader

for.cond4.preheader:                              ; preds = %for.end38, %for.cond1.preheader
  %indvars.iv23 = phi i64 [ 1, %for.cond1.preheader ], [ %indvars.iv.next24, %for.end38 ]
  %2 = add nsw i64 %indvars.iv23, -2
  %3 = add nsw i64 %indvars.iv23, -1
  %4 = add nsw i64 %indvars.iv23, 1
  %5 = add nsw i64 %indvars.iv23, 2
  br label %for.cond7.preheader

for.cond7.preheader:                              ; preds = %for.inc36, %for.cond4.preheader
  %sum.08 = phi i8 [ 0, %for.cond4.preheader ], [ %sum.3.4.lcssa, %for.inc36 ]
  %c.07 = phi i32 [ 0, %for.cond4.preheader ], [ %inc37, %for.inc36 ]
  br label %for.body12

for.body12:                                       ; preds = %for.cond7.preheader, %for.inc.4
  %indvars.iv14 = phi i64 [ 0, %for.cond7.preheader ], [ %indvars.iv.next15, %for.inc.4 ]
  %sum.16 = phi i8 [ %sum.08, %for.cond7.preheader ], [ %sum.3.4, %for.inc.4 ]
  %6 = sub nuw nsw i64 4, %indvars.iv14
  %7 = add nsw i64 %0, %indvars.iv14
  %8 = shl i64 %7, 5
  %9 = mul nuw nsw i64 %6, 5
  %10 = or i64 %2, %7
  %11 = trunc i64 %10 to i32
  %12 = icmp ult i32 %11, 32
  br i1 %12, label %if.then, label %for.inc

if.then:                                          ; preds = %for.body12
  %13 = add nuw nsw i64 %9, 4
  %14 = add nsw i64 %2, %8
  %arrayidx = getelementptr inbounds i8, i8* inttoptr (i64 268566559 to i8*), i64 %14
  %15 = load i8, i8* %arrayidx, align 1
  %arrayidx27 = getelementptr inbounds i8, i8* inttoptr (i64 268574751 to i8*), i64 %13
  %16 = load i8, i8* %arrayidx27, align 1
  %mul29 = mul i8 %16, %15
  %add31 = add i8 %mul29, %sum.16
  br label %for.inc

for.inc:                                          ; preds = %for.body12, %if.then
  %sum.3 = phi i8 [ %add31, %if.then ], [ %sum.16, %for.body12 ]
  %17 = or i64 %3, %7
  %18 = trunc i64 %17 to i32
  %19 = icmp ult i32 %18, 32
  br i1 %19, label %if.then.1, label %for.inc.1

for.inc36:                                        ; preds = %for.inc.4
  %sum.3.4.lcssa = phi i8 [ %sum.3.4, %for.inc.4 ]
  %inc37 = add nuw nsw i32 %c.07, 1
  %exitcond22 = icmp eq i32 %inc37, 6
  br i1 %exitcond22, label %for.end38, label %for.cond7.preheader

for.end38:                                        ; preds = %for.inc36
  %sum.3.4.lcssa.lcssa = phi i8 [ %sum.3.4.lcssa, %for.inc36 ]
  %20 = add nuw nsw i64 %indvars.iv23, %1
  %arrayidx42 = getelementptr inbounds i8, i8* inttoptr (i64 268575951 to i8*), i64 %20
  store i8 %sum.3.4.lcssa.lcssa, i8* %arrayidx42, align 1
  %indvars.iv.next24 = add nuw nsw i64 %indvars.iv23, 1
  %exitcond27 = icmp eq i64 %indvars.iv.next24, 31
  br i1 %exitcond27, label %for.inc46, label %for.cond4.preheader

for.inc46:                                        ; preds = %for.end38
  %indvars.iv.next29 = add nuw nsw i64 %indvars.iv28, 1
  %exitcond32 = icmp eq i64 %indvars.iv.next29, 31
  br i1 %exitcond32, label %for.end48, label %for.cond1.preheader

for.end48:                                        ; preds = %for.inc46
  ret void

if.then.1:                                        ; preds = %for.inc
  %21 = add nsw i64 %3, %8
  %arrayidx.1 = getelementptr inbounds i8, i8* inttoptr (i64 268566559 to i8*), i64 %21
  %22 = load i8, i8* %arrayidx.1, align 1
  %23 = add nsw i64 %9, 3
  %arrayidx27.1 = getelementptr inbounds i8, i8* inttoptr (i64 268574751 to i8*), i64 %23
  %24 = load i8, i8* %arrayidx27.1, align 1
  %mul29.1 = mul i8 %24, %22
  %add31.1 = add i8 %mul29.1, %sum.3
  br label %for.inc.1

for.inc.1:                                        ; preds = %if.then.1, %for.inc
  %sum.3.1 = phi i8 [ %add31.1, %if.then.1 ], [ %sum.3, %for.inc ]
  %25 = or i64 %indvars.iv23, %7
  %26 = trunc i64 %25 to i32
  %27 = icmp ult i32 %26, 32
  br i1 %27, label %if.then.2, label %for.inc.2

if.then.2:                                        ; preds = %for.inc.1
  %28 = add nsw i64 %indvars.iv23, %8
  %arrayidx.2 = getelementptr inbounds i8, i8* inttoptr (i64 268566559 to i8*), i64 %28
  %29 = load i8, i8* %arrayidx.2, align 1
  %30 = add nsw i64 %9, 2
  %arrayidx27.2 = getelementptr inbounds i8, i8* inttoptr (i64 268574751 to i8*), i64 %30
  %31 = load i8, i8* %arrayidx27.2, align 1
  %mul29.2 = mul i8 %31, %29
  %add31.2 = add i8 %mul29.2, %sum.3.1
  br label %for.inc.2

for.inc.2:                                        ; preds = %if.then.2, %for.inc.1
  %sum.3.2 = phi i8 [ %add31.2, %if.then.2 ], [ %sum.3.1, %for.inc.1 ]
  %32 = or i64 %4, %7
  %33 = trunc i64 %32 to i32
  %34 = icmp ult i32 %33, 32
  br i1 %34, label %if.then.3, label %for.inc.3

if.then.3:                                        ; preds = %for.inc.2
  %35 = add nsw i64 %4, %8
  %arrayidx.3 = getelementptr inbounds i8, i8* inttoptr (i64 268566559 to i8*), i64 %35
  %36 = load i8, i8* %arrayidx.3, align 1
  %37 = add nsw i64 %9, 1
  %arrayidx27.3 = getelementptr inbounds i8, i8* inttoptr (i64 268574751 to i8*), i64 %37
  %38 = load i8, i8* %arrayidx27.3, align 1
  %mul29.3 = mul i8 %38, %36
  %add31.3 = add i8 %mul29.3, %sum.3.2
  br label %for.inc.3

for.inc.3:                                        ; preds = %if.then.3, %for.inc.2
  %sum.3.3 = phi i8 [ %add31.3, %if.then.3 ], [ %sum.3.2, %for.inc.2 ]
  %39 = or i64 %5, %7
  %40 = trunc i64 %39 to i32
  %41 = icmp ult i32 %40, 32
  br i1 %41, label %if.then.4, label %for.inc.4

if.then.4:                                        ; preds = %for.inc.3
  %42 = add nsw i64 %5, %8
  %arrayidx.4 = getelementptr inbounds i8, i8* inttoptr (i64 268566559 to i8*), i64 %42
  %43 = load i8, i8* %arrayidx.4, align 1
  %arrayidx27.4 = getelementptr inbounds i8, i8* inttoptr (i64 268574751 to i8*), i64 %9
  %44 = load i8, i8* %arrayidx27.4, align 1
  %mul29.4 = mul i8 %44, %43
  %add31.4 = add i8 %mul29.4, %sum.3.3
  br label %for.inc.4

for.inc.4:                                        ; preds = %if.then.4, %for.inc.3
  %sum.3.4 = phi i8 [ %add31.4, %if.then.4 ], [ %sum.3.3, %for.inc.3 ]
  %indvars.iv.next15 = add nuw nsw i64 %indvars.iv14, 1
  %exitcond21 = icmp eq i64 %indvars.iv.next15, 5
  br i1 %exitcond21, label %for.inc36, label %for.body12
}

attributes #0 = { norecurse nounwind uwtable "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.ident = !{!0}

!0 = !{!"clang version 3.8.1 "}
