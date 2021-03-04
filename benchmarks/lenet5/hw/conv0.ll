; ModuleID = 'conv0.bc'
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: norecurse nounwind uwtable
define void @conv0() #0 {
entry:
  br label %for.cond4.preheader

for.cond4.preheader:                              ; preds = %entry, %for.inc63
  %indvars.iv35 = phi i64 [ 0, %entry ], [ %indvars.iv.next36, %for.inc63 ]
  %0 = mul nuw nsw i64 %indvars.iv35, 25
  %1 = add nuw nsw i64 %0, 12
  %2 = mul nuw nsw i64 %indvars.iv35, 784
  %3 = add nsw i64 %2, -58
  %4 = add nsw i64 %0, 10
  %5 = add nsw i64 %0, 11
  %6 = add nsw i64 %0, 13
  %7 = add nsw i64 %0, 17
  %arrayidx36.3.2 = getelementptr inbounds i8, i8* inttoptr (i64 268567583 to i8*), i64 %7
  %arrayidx36.2.2 = getelementptr inbounds i8, i8* inttoptr (i64 268567583 to i8*), i64 %1
  %8 = add nsw i64 %0, 7
  %arrayidx36.1.2 = getelementptr inbounds i8, i8* inttoptr (i64 268567583 to i8*), i64 %8
  %9 = add nsw i64 %0, 2
  %arrayidx36.250 = getelementptr inbounds i8, i8* inttoptr (i64 268567583 to i8*), i64 %9
  %10 = add nsw i64 %0, 16
  %arrayidx36.3.1 = getelementptr inbounds i8, i8* inttoptr (i64 268567583 to i8*), i64 %10
  %arrayidx36.2.1 = getelementptr inbounds i8, i8* inttoptr (i64 268567583 to i8*), i64 %5
  %11 = add nsw i64 %0, 6
  %arrayidx36.1.1 = getelementptr inbounds i8, i8* inttoptr (i64 268567583 to i8*), i64 %11
  %12 = add nsw i64 %0, 1
  %arrayidx36.144 = getelementptr inbounds i8, i8* inttoptr (i64 268567583 to i8*), i64 %12
  %13 = add nsw i64 %0, 15
  %arrayidx36.3 = getelementptr inbounds i8, i8* inttoptr (i64 268567583 to i8*), i64 %13
  %arrayidx36.2 = getelementptr inbounds i8, i8* inttoptr (i64 268567583 to i8*), i64 %4
  %14 = add nsw i64 %0, 5
  %arrayidx36.1 = getelementptr inbounds i8, i8* inttoptr (i64 268567583 to i8*), i64 %14
  %arrayidx36 = getelementptr inbounds i8, i8* inttoptr (i64 268567583 to i8*), i64 %0
  %15 = add nsw i64 %0, 3
  %arrayidx36.356 = getelementptr inbounds i8, i8* inttoptr (i64 268567583 to i8*), i64 %15
  %16 = add nsw i64 %0, 8
  %arrayidx36.1.3 = getelementptr inbounds i8, i8* inttoptr (i64 268567583 to i8*), i64 %16
  %arrayidx36.2.3 = getelementptr inbounds i8, i8* inttoptr (i64 268567583 to i8*), i64 %6
  %17 = add nsw i64 %0, 18
  %arrayidx36.3.3 = getelementptr inbounds i8, i8* inttoptr (i64 268567583 to i8*), i64 %17
  br label %for.cond7.preheader

for.cond7.preheader:                              ; preds = %for.inc57, %for.cond4.preheader
  %indvars.iv27 = phi i64 [ 0, %for.cond4.preheader ], [ %indvars.iv.next28, %for.inc57 ]
  %cmp12 = icmp slt i64 %indvars.iv27, 2
  %18 = add nuw nsw i64 %indvars.iv27, 2
  %cmp17 = icmp sgt i64 %18, 31
  %19 = mul nuw nsw i64 %indvars.iv27, 28
  %20 = add nsw i64 %3, %19
  %21 = shl i64 %indvars.iv27, 5
  %22 = add i64 %21, -64
  %23 = shl i64 %indvars.iv27, 5
  %24 = add i64 %23, -32
  %25 = shl i64 %indvars.iv27, 5
  %26 = shl i64 %indvars.iv27, 5
  %27 = add i64 %26, 32
  br label %for.body9

for.body9:                                        ; preds = %for.inc54, %for.cond7.preheader
  %indvars.iv21 = phi i64 [ 0, %for.cond7.preheader ], [ %indvars.iv.next22, %for.inc54 ]
  %cmp10 = icmp slt i64 %indvars.iv21, 2
  %or.cond = or i1 %cmp12, %cmp10
  %28 = add nuw nsw i64 %indvars.iv21, 2
  %cmp14 = icmp sgt i64 %28, 31
  %or.cond1 = or i1 %or.cond, %cmp14
  %or.cond2 = or i1 %cmp17, %or.cond1
  br i1 %or.cond2, label %for.inc54, label %for.inc.3

for.inc54:                                        ; preds = %for.body9, %for.inc.3
  %indvars.iv.next22 = add nuw nsw i64 %indvars.iv21, 1
  %exitcond26 = icmp eq i64 %indvars.iv.next22, 32
  br i1 %exitcond26, label %for.inc57, label %for.body9

for.inc57:                                        ; preds = %for.inc54
  %indvars.iv.next28 = add nuw nsw i64 %indvars.iv27, 1
  %exitcond31 = icmp eq i64 %indvars.iv.next28, 32
  br i1 %exitcond31, label %for.inc63, label %for.cond7.preheader

for.inc63:                                        ; preds = %for.inc57
  %indvars.iv.next36 = add nuw nsw i64 %indvars.iv35, 1
  %exitcond41 = icmp eq i64 %indvars.iv.next36, 6
  br i1 %exitcond41, label %for.end65, label %for.cond4.preheader

for.end65:                                        ; preds = %for.inc63
  ret void

for.inc.3:                                        ; preds = %for.body9
  %29 = add nsw i64 %indvars.iv21, %22
  %30 = add nsw i64 %indvars.iv21, %24
  %31 = add nsw i64 %indvars.iv21, %25
  %32 = add nsw i64 %indvars.iv21, %27
  %33 = load i8, i8* %arrayidx36.3.2, align 1
  %34 = add nsw i64 %31, 1
  %arrayidx.3.2 = getelementptr inbounds i8, i8* inttoptr (i64 268566559 to i8*), i64 %34
  %35 = load i8, i8* %arrayidx.3.2, align 1
  %mul38.3.2 = mul i8 %33, %35
  %36 = load i8, i8* %arrayidx36.2.2, align 1
  %arrayidx.2.2 = getelementptr inbounds i8, i8* inttoptr (i64 268566559 to i8*), i64 %31
  %37 = load i8, i8* %arrayidx.2.2, align 1
  %mul38.2.2 = mul i8 %36, %37
  %38 = load i8, i8* %arrayidx36.1.2, align 1
  %39 = add nsw i64 %31, -1
  %arrayidx.1.2 = getelementptr inbounds i8, i8* inttoptr (i64 268566559 to i8*), i64 %39
  %40 = load i8, i8* %arrayidx.1.2, align 1
  %mul38.1.2 = mul i8 %38, %40
  %41 = load i8, i8* %arrayidx36.250, align 1
  %42 = add nsw i64 %31, -2
  %arrayidx.248 = getelementptr inbounds i8, i8* inttoptr (i64 268566559 to i8*), i64 %42
  %43 = load i8, i8* %arrayidx.248, align 1
  %mul38.252 = mul i8 %41, %43
  %44 = load i8, i8* %arrayidx36.3.1, align 1
  %45 = add nsw i64 %30, 1
  %arrayidx.3.1 = getelementptr inbounds i8, i8* inttoptr (i64 268566559 to i8*), i64 %45
  %46 = load i8, i8* %arrayidx.3.1, align 1
  %mul38.3.1 = mul i8 %44, %46
  %47 = load i8, i8* %arrayidx36.2.1, align 1
  %arrayidx.2.1 = getelementptr inbounds i8, i8* inttoptr (i64 268566559 to i8*), i64 %30
  %48 = load i8, i8* %arrayidx.2.1, align 1
  %mul38.2.1 = mul i8 %47, %48
  %49 = load i8, i8* %arrayidx36.1.1, align 1
  %50 = add nsw i64 %30, -1
  %arrayidx.1.1 = getelementptr inbounds i8, i8* inttoptr (i64 268566559 to i8*), i64 %50
  %51 = load i8, i8* %arrayidx.1.1, align 1
  %mul38.1.1 = mul i8 %49, %51
  %52 = load i8, i8* %arrayidx36.144, align 1
  %53 = add nsw i64 %30, -2
  %arrayidx.142 = getelementptr inbounds i8, i8* inttoptr (i64 268566559 to i8*), i64 %53
  %54 = load i8, i8* %arrayidx.142, align 1
  %mul38.146 = mul i8 %52, %54
  %55 = load i8, i8* %arrayidx36.3, align 1
  %56 = add nsw i64 %29, 1
  %arrayidx.3 = getelementptr inbounds i8, i8* inttoptr (i64 268566559 to i8*), i64 %56
  %57 = load i8, i8* %arrayidx.3, align 1
  %mul38.3 = mul i8 %55, %57
  %58 = load i8, i8* %arrayidx36.2, align 1
  %arrayidx.2 = getelementptr inbounds i8, i8* inttoptr (i64 268566559 to i8*), i64 %29
  %59 = load i8, i8* %arrayidx.2, align 1
  %mul38.2 = mul i8 %58, %59
  %60 = load i8, i8* %arrayidx36.1, align 1
  %61 = add nsw i64 %29, -1
  %arrayidx.1 = getelementptr inbounds i8, i8* inttoptr (i64 268566559 to i8*), i64 %61
  %62 = load i8, i8* %arrayidx.1, align 1
  %mul38.1 = mul i8 %60, %62
  %63 = load i8, i8* %arrayidx36, align 1
  %64 = add nsw i64 %29, -2
  %arrayidx = getelementptr inbounds i8, i8* inttoptr (i64 268566559 to i8*), i64 %64
  %65 = load i8, i8* %arrayidx, align 1
  %mul38 = mul i8 %63, %65
  %add39.1 = add i8 %mul38.1, %mul38
  %add39.2 = add i8 %mul38.2, %add39.1
  %add39.3 = add i8 %mul38.3, %add39.2
  %add39.147 = add i8 %mul38.146, %add39.3
  %add39.1.1 = add i8 %mul38.1.1, %add39.147
  %add39.2.1 = add i8 %mul38.2.1, %add39.1.1
  %add39.3.1 = add i8 %mul38.3.1, %add39.2.1
  %add39.253 = add i8 %mul38.252, %add39.3.1
  %add39.1.2 = add i8 %mul38.1.2, %add39.253
  %add39.2.2 = add i8 %mul38.2.2, %add39.1.2
  %add39.3.2 = add i8 %mul38.3.2, %add39.2.2
  %66 = add nsw i64 %32, -2
  %arrayidx.354 = getelementptr inbounds i8, i8* inttoptr (i64 268566559 to i8*), i64 %66
  %67 = load i8, i8* %arrayidx.354, align 1
  %68 = load i8, i8* %arrayidx36.356, align 1
  %mul38.358 = mul i8 %68, %67
  %add39.359 = add i8 %mul38.358, %add39.3.2
  %69 = add nsw i64 %32, -1
  %arrayidx.1.3 = getelementptr inbounds i8, i8* inttoptr (i64 268566559 to i8*), i64 %69
  %70 = load i8, i8* %arrayidx.1.3, align 1
  %71 = load i8, i8* %arrayidx36.1.3, align 1
  %mul38.1.3 = mul i8 %71, %70
  %add39.1.3 = add i8 %mul38.1.3, %add39.359
  %arrayidx.2.3 = getelementptr inbounds i8, i8* inttoptr (i64 268566559 to i8*), i64 %32
  %72 = load i8, i8* %arrayidx.2.3, align 1
  %73 = load i8, i8* %arrayidx36.2.3, align 1
  %mul38.2.3 = mul i8 %73, %72
  %add39.2.3 = add i8 %mul38.2.3, %add39.1.3
  %74 = add nsw i64 %32, 1
  %arrayidx.3.3 = getelementptr inbounds i8, i8* inttoptr (i64 268566559 to i8*), i64 %74
  %75 = load i8, i8* %arrayidx.3.3, align 1
  %76 = load i8, i8* %arrayidx36.3.3, align 1
  %mul38.3.3 = mul i8 %76, %75
  %add39.3.3 = add i8 %mul38.3.3, %add39.2.3
  %77 = add nsw i64 %20, %indvars.iv21
  %arrayidx50 = getelementptr inbounds i8, i8* inttoptr (i64 268567733 to i8*), i64 %77
  %78 = load i8, i8* %arrayidx50, align 1
  %add52 = add i8 %78, %add39.3.3
  store i8 %add52, i8* %arrayidx50, align 1
  br label %for.inc54
}

attributes #0 = { norecurse nounwind uwtable "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.ident = !{!0}

!0 = !{!"clang version 3.8.1 "}
