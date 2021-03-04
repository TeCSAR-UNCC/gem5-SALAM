; ModuleID = 'pool1.bc'
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: norecurse nounwind uwtable
define void @pool1() #0 {
entry:
  br label %for.cond1.preheader

for.cond1.preheader:                              ; preds = %for.inc38, %entry
  %indvars.iv20 = phi i64 [ 0, %entry ], [ %indvars.iv.next21, %for.inc38 ]
  %0 = mul nuw nsw i64 %indvars.iv20, 100
  %1 = mul nuw nsw i64 %indvars.iv20, 25
  %2 = trunc i64 %1 to i32
  %3 = or i64 %0, 2
  %4 = add nuw nsw i64 %0, 4
  %5 = add nuw nsw i64 %0, 6
  %6 = add nuw nsw i64 %0, 8
  br label %for.cond4.preheader

for.cond4.preheader:                              ; preds = %for.cond1.preheader, %for.cond4.preheader
  %indvars.iv18 = phi i64 [ 0, %for.cond1.preheader ], [ %indvars.iv.next19, %for.cond4.preheader ]
  %7 = trunc i64 %indvars.iv18 to i32
  %div22 = sdiv i32 %7, 2
  %mul23 = mul nsw i32 %div22, 5
  %add24 = add nsw i32 %mul23, %2
  %8 = mul nuw nsw i64 %indvars.iv18, 10
  %9 = or i64 %indvars.iv18, 1
  %10 = mul nuw nsw i64 %9, 10
  %11 = add nuw nsw i64 %0, %8
  %12 = add nuw nsw i64 %0, %10
  %13 = or i64 %11, 1
  %arrayidx.1 = getelementptr inbounds i8, i8* inttoptr (i64 268581496 to i8*), i64 %13
  %14 = load i8, i8* %arrayidx.1, align 1
  %conv.1 = zext i8 %14 to i32
  %arrayidx = getelementptr inbounds i8, i8* inttoptr (i64 268581496 to i8*), i64 %11
  %15 = load i8, i8* %arrayidx, align 4
  %conv = zext i8 %15 to i32
  %add17.1 = add nuw nsw i32 %conv.1, %conv
  %arrayidx.125 = getelementptr inbounds i8, i8* inttoptr (i64 268581496 to i8*), i64 %12
  %16 = load i8, i8* %arrayidx.125, align 2
  %conv.126 = zext i8 %16 to i32
  %add17.127 = add nuw nsw i32 %conv.126, %add17.1
  %17 = or i64 %12, 1
  %arrayidx.1.1 = getelementptr inbounds i8, i8* inttoptr (i64 268581496 to i8*), i64 %17
  %18 = load i8, i8* %arrayidx.1.1, align 1
  %conv.1.1 = zext i8 %18 to i32
  %add17.1.1 = add nuw nsw i32 %conv.1.1, %add17.127
  %div = lshr i32 %add17.1.1, 2
  %idxprom27 = sext i32 %add24 to i64
  %arrayidx28 = getelementptr inbounds i8, i8* inttoptr (i64 268583096 to i8*), i64 %idxprom27
  %19 = load i8, i8* %arrayidx28, align 1
  %conv29 = zext i8 %19 to i32
  %add30 = add nuw nsw i32 %conv29, %div
  %conv31 = trunc i32 %add30 to i8
  store i8 %conv31, i8* %arrayidx28, align 1
  %20 = add nuw nsw i64 %3, %8
  %21 = add nuw nsw i64 %3, %10
  %22 = or i64 %20, 1
  %arrayidx.1.128 = getelementptr inbounds i8, i8* inttoptr (i64 268581496 to i8*), i64 %22
  %23 = load i8, i8* %arrayidx.1.128, align 1
  %conv.1.129 = zext i8 %23 to i32
  %arrayidx.130 = getelementptr inbounds i8, i8* inttoptr (i64 268581496 to i8*), i64 %20
  %24 = load i8, i8* %arrayidx.130, align 2
  %conv.131 = zext i8 %24 to i32
  %add17.1.132 = add nuw nsw i32 %conv.1.129, %conv.131
  %arrayidx.125.1 = getelementptr inbounds i8, i8* inttoptr (i64 268581496 to i8*), i64 %21
  %25 = load i8, i8* %arrayidx.125.1, align 2
  %conv.126.1 = zext i8 %25 to i32
  %add17.127.1 = add nuw nsw i32 %conv.126.1, %add17.1.132
  %26 = or i64 %21, 1
  %arrayidx.1.1.1 = getelementptr inbounds i8, i8* inttoptr (i64 268581496 to i8*), i64 %26
  %27 = load i8, i8* %arrayidx.1.1.1, align 1
  %conv.1.1.1 = zext i8 %27 to i32
  %add17.1.1.1 = add nuw nsw i32 %conv.1.1.1, %add17.127.1
  %div.1 = lshr i32 %add17.1.1.1, 2
  %add26.1 = add nsw i32 %add24, 1
  %idxprom27.1 = sext i32 %add26.1 to i64
  %arrayidx28.1 = getelementptr inbounds i8, i8* inttoptr (i64 268583096 to i8*), i64 %idxprom27.1
  %28 = load i8, i8* %arrayidx28.1, align 1
  %conv29.1 = zext i8 %28 to i32
  %add30.1 = add nuw nsw i32 %conv29.1, %div.1
  %conv31.1 = trunc i32 %add30.1 to i8
  store i8 %conv31.1, i8* %arrayidx28.1, align 1
  %29 = add nuw nsw i64 %4, %8
  %30 = add nuw nsw i64 %4, %10
  %31 = or i64 %29, 1
  %arrayidx.1.2 = getelementptr inbounds i8, i8* inttoptr (i64 268581496 to i8*), i64 %31
  %32 = load i8, i8* %arrayidx.1.2, align 1
  %conv.1.2 = zext i8 %32 to i32
  %arrayidx.2 = getelementptr inbounds i8, i8* inttoptr (i64 268581496 to i8*), i64 %29
  %33 = load i8, i8* %arrayidx.2, align 4
  %conv.2 = zext i8 %33 to i32
  %add17.1.2 = add nuw nsw i32 %conv.1.2, %conv.2
  %arrayidx.125.2 = getelementptr inbounds i8, i8* inttoptr (i64 268581496 to i8*), i64 %30
  %34 = load i8, i8* %arrayidx.125.2, align 2
  %conv.126.2 = zext i8 %34 to i32
  %add17.127.2 = add nuw nsw i32 %conv.126.2, %add17.1.2
  %35 = or i64 %30, 1
  %arrayidx.1.1.2 = getelementptr inbounds i8, i8* inttoptr (i64 268581496 to i8*), i64 %35
  %36 = load i8, i8* %arrayidx.1.1.2, align 1
  %conv.1.1.2 = zext i8 %36 to i32
  %add17.1.1.2 = add nuw nsw i32 %conv.1.1.2, %add17.127.2
  %div.2 = lshr i32 %add17.1.1.2, 2
  %add26.2 = add nsw i32 %add24, 2
  %idxprom27.2 = sext i32 %add26.2 to i64
  %arrayidx28.2 = getelementptr inbounds i8, i8* inttoptr (i64 268583096 to i8*), i64 %idxprom27.2
  %37 = load i8, i8* %arrayidx28.2, align 1
  %conv29.2 = zext i8 %37 to i32
  %add30.2 = add nuw nsw i32 %conv29.2, %div.2
  %conv31.2 = trunc i32 %add30.2 to i8
  store i8 %conv31.2, i8* %arrayidx28.2, align 1
  %38 = add nuw nsw i64 %5, %8
  %39 = add nuw nsw i64 %5, %10
  %40 = or i64 %38, 1
  %arrayidx.1.3 = getelementptr inbounds i8, i8* inttoptr (i64 268581496 to i8*), i64 %40
  %41 = load i8, i8* %arrayidx.1.3, align 1
  %conv.1.3 = zext i8 %41 to i32
  %arrayidx.3 = getelementptr inbounds i8, i8* inttoptr (i64 268581496 to i8*), i64 %38
  %42 = load i8, i8* %arrayidx.3, align 2
  %conv.3 = zext i8 %42 to i32
  %add17.1.3 = add nuw nsw i32 %conv.1.3, %conv.3
  %arrayidx.125.3 = getelementptr inbounds i8, i8* inttoptr (i64 268581496 to i8*), i64 %39
  %43 = load i8, i8* %arrayidx.125.3, align 2
  %conv.126.3 = zext i8 %43 to i32
  %add17.127.3 = add nuw nsw i32 %conv.126.3, %add17.1.3
  %44 = or i64 %39, 1
  %arrayidx.1.1.3 = getelementptr inbounds i8, i8* inttoptr (i64 268581496 to i8*), i64 %44
  %45 = load i8, i8* %arrayidx.1.1.3, align 1
  %conv.1.1.3 = zext i8 %45 to i32
  %add17.1.1.3 = add nuw nsw i32 %conv.1.1.3, %add17.127.3
  %div.3 = lshr i32 %add17.1.1.3, 2
  %add26.3 = add nsw i32 %add24, 3
  %idxprom27.3 = sext i32 %add26.3 to i64
  %arrayidx28.3 = getelementptr inbounds i8, i8* inttoptr (i64 268583096 to i8*), i64 %idxprom27.3
  %46 = load i8, i8* %arrayidx28.3, align 1
  %conv29.3 = zext i8 %46 to i32
  %add30.3 = add nuw nsw i32 %conv29.3, %div.3
  %conv31.3 = trunc i32 %add30.3 to i8
  store i8 %conv31.3, i8* %arrayidx28.3, align 1
  %47 = add nuw nsw i64 %6, %8
  %48 = add nuw nsw i64 %6, %10
  %49 = or i64 %47, 1
  %arrayidx.1.4 = getelementptr inbounds i8, i8* inttoptr (i64 268581496 to i8*), i64 %49
  %50 = load i8, i8* %arrayidx.1.4, align 1
  %conv.1.4 = zext i8 %50 to i32
  %arrayidx.4 = getelementptr inbounds i8, i8* inttoptr (i64 268581496 to i8*), i64 %47
  %51 = load i8, i8* %arrayidx.4, align 4
  %conv.4 = zext i8 %51 to i32
  %add17.1.4 = add nuw nsw i32 %conv.1.4, %conv.4
  %arrayidx.125.4 = getelementptr inbounds i8, i8* inttoptr (i64 268581496 to i8*), i64 %48
  %52 = load i8, i8* %arrayidx.125.4, align 2
  %conv.126.4 = zext i8 %52 to i32
  %add17.127.4 = add nuw nsw i32 %conv.126.4, %add17.1.4
  %53 = or i64 %48, 1
  %arrayidx.1.1.4 = getelementptr inbounds i8, i8* inttoptr (i64 268581496 to i8*), i64 %53
  %54 = load i8, i8* %arrayidx.1.1.4, align 1
  %conv.1.1.4 = zext i8 %54 to i32
  %add17.1.1.4 = add nuw nsw i32 %conv.1.1.4, %add17.127.4
  %div.4 = lshr i32 %add17.1.1.4, 2
  %add26.4 = add nsw i32 %add24, 4
  %idxprom27.4 = sext i32 %add26.4 to i64
  %arrayidx28.4 = getelementptr inbounds i8, i8* inttoptr (i64 268583096 to i8*), i64 %idxprom27.4
  %55 = load i8, i8* %arrayidx28.4, align 1
  %conv29.4 = zext i8 %55 to i32
  %add30.4 = add nuw nsw i32 %conv29.4, %div.4
  %conv31.4 = trunc i32 %add30.4 to i8
  store i8 %conv31.4, i8* %arrayidx28.4, align 1
  %indvars.iv.next19 = add nuw nsw i64 %indvars.iv18, 2
  %cmp2 = icmp slt i64 %indvars.iv.next19, 10
  br i1 %cmp2, label %for.cond4.preheader, label %for.inc38

for.inc38:                                        ; preds = %for.cond4.preheader
  %indvars.iv.next21 = add nuw nsw i64 %indvars.iv20, 1
  %exitcond24 = icmp eq i64 %indvars.iv.next21, 16
  br i1 %exitcond24, label %for.end40, label %for.cond1.preheader

for.end40:                                        ; preds = %for.inc38
  ret void
}

attributes #0 = { norecurse nounwind uwtable "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.ident = !{!0}

!0 = !{!"clang version 3.8.1 "}
