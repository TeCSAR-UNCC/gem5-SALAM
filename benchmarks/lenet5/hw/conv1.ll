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
  %4 = add nsw i64 %0, 10
  %5 = add nsw i64 %0, 11
  %6 = add nsw i64 %0, 13
  %7 = add nsw i64 %0, 17
  %arrayidx36.3.2 = getelementptr inbounds i8, i8* inttoptr (i64 268579495 to i8*), i64 %7
  %arrayidx36.2.2 = getelementptr inbounds i8, i8* inttoptr (i64 268579495 to i8*), i64 %1
  %8 = add nsw i64 %0, 7
  %arrayidx36.1.2 = getelementptr inbounds i8, i8* inttoptr (i64 268579495 to i8*), i64 %8
  %9 = add nsw i64 %0, 2
  %arrayidx36.251 = getelementptr inbounds i8, i8* inttoptr (i64 268579495 to i8*), i64 %9
  %10 = add nsw i64 %0, 16
  %arrayidx36.3.1 = getelementptr inbounds i8, i8* inttoptr (i64 268579495 to i8*), i64 %10
  %arrayidx36.2.1 = getelementptr inbounds i8, i8* inttoptr (i64 268579495 to i8*), i64 %5
  %11 = add nsw i64 %0, 6
  %arrayidx36.1.1 = getelementptr inbounds i8, i8* inttoptr (i64 268579495 to i8*), i64 %11
  %12 = add nsw i64 %0, 1
  %arrayidx36.145 = getelementptr inbounds i8, i8* inttoptr (i64 268579495 to i8*), i64 %12
  %13 = add nsw i64 %0, 15
  %arrayidx36.3 = getelementptr inbounds i8, i8* inttoptr (i64 268579495 to i8*), i64 %13
  %arrayidx36.2 = getelementptr inbounds i8, i8* inttoptr (i64 268579495 to i8*), i64 %4
  %14 = add nsw i64 %0, 5
  %arrayidx36.1 = getelementptr inbounds i8, i8* inttoptr (i64 268579495 to i8*), i64 %14
  %arrayidx36 = getelementptr inbounds i8, i8* inttoptr (i64 268579495 to i8*), i64 %0
  %15 = add nsw i64 %0, 3
  %arrayidx36.357 = getelementptr inbounds i8, i8* inttoptr (i64 268579495 to i8*), i64 %15
  %16 = add nsw i64 %0, 8
  %arrayidx36.1.3 = getelementptr inbounds i8, i8* inttoptr (i64 268579495 to i8*), i64 %16
  %arrayidx36.2.3 = getelementptr inbounds i8, i8* inttoptr (i64 268579495 to i8*), i64 %6
  %17 = add nsw i64 %0, 18
  %arrayidx36.3.3 = getelementptr inbounds i8, i8* inttoptr (i64 268579495 to i8*), i64 %17
  br label %for.cond4.preheader

for.cond4.preheader:                              ; preds = %for.inc60, %for.cond1.preheader
  %indvars.iv32 = phi i64 [ 0, %for.cond1.preheader ], [ %indvars.iv.next33, %for.inc60 ]
  %18 = mul nuw nsw i64 %indvars.iv32, 196
  br label %for.cond7.preheader

for.cond7.preheader:                              ; preds = %for.inc57, %for.cond4.preheader
  %indvars.iv27 = phi i64 [ 0, %for.cond4.preheader ], [ %indvars.iv.next28, %for.inc57 ]
  %cmp12 = icmp slt i64 %indvars.iv27, 2
  %19 = add nuw nsw i64 %indvars.iv27, 2
  %cmp17 = icmp sgt i64 %19, 13
  %20 = mul nuw nsw i64 %indvars.iv27, 10
  %21 = add nsw i64 %3, %20
  %22 = mul i64 %indvars.iv27, 14
  %23 = add i64 %22, -28
  %24 = mul i64 %indvars.iv27, 14
  %25 = add i64 %24, -14
  %26 = mul nsw i64 %indvars.iv27, 14
  %27 = mul i64 %indvars.iv27, 14
  %28 = add i64 %27, 14
  br label %for.body9

for.body9:                                        ; preds = %for.inc54, %for.cond7.preheader
  %indvars.iv21 = phi i64 [ 0, %for.cond7.preheader ], [ %indvars.iv.next22, %for.inc54 ]
  %cmp10 = icmp slt i64 %indvars.iv21, 2
  %or.cond = or i1 %cmp12, %cmp10
  %29 = add nuw nsw i64 %indvars.iv21, 2
  %cmp14 = icmp sgt i64 %29, 13
  %or.cond1 = or i1 %or.cond, %cmp14
  %or.cond2 = or i1 %cmp17, %or.cond1
  br i1 %or.cond2, label %for.inc54, label %for.inc.3

for.inc54:                                        ; preds = %for.body9, %for.inc.3
  %indvars.iv.next22 = add nuw nsw i64 %indvars.iv21, 1
  %exitcond26 = icmp eq i64 %indvars.iv.next22, 14
  br i1 %exitcond26, label %for.inc57, label %for.body9

for.inc57:                                        ; preds = %for.inc54
  %indvars.iv.next28 = add nuw nsw i64 %indvars.iv27, 1
  %exitcond31 = icmp eq i64 %indvars.iv.next28, 14
  br i1 %exitcond31, label %for.inc60, label %for.cond7.preheader

for.inc60:                                        ; preds = %for.inc57
  %indvars.iv.next33 = add nuw nsw i64 %indvars.iv32, 1
  %exitcond35 = icmp eq i64 %indvars.iv.next33, 6
  br i1 %exitcond35, label %for.inc63, label %for.cond4.preheader

for.inc63:                                        ; preds = %for.inc60
  %indvars.iv.next37 = add nuw nsw i64 %indvars.iv36, 1
  %exitcond42 = icmp eq i64 %indvars.iv.next37, 16
  br i1 %exitcond42, label %for.end65, label %for.cond1.preheader

for.end65:                                        ; preds = %for.inc63
  ret void

for.inc.3:                                        ; preds = %for.body9
  %30 = add nuw nsw i64 %indvars.iv21, %18
  %31 = add nsw i64 %30, %23
  %32 = add nsw i64 %30, %25
  %33 = add nsw i64 %30, %26
  %34 = add nsw i64 %30, %28
  %35 = load i8, i8* %arrayidx36.3.2, align 1
  %36 = add nsw i64 %33, 1
  %arrayidx.3.2 = getelementptr inbounds i8, i8* inttoptr (i64 268578319 to i8*), i64 %36
  %37 = load i8, i8* %arrayidx.3.2, align 1
  %mul38.3.2 = mul i8 %35, %37
  %38 = load i8, i8* %arrayidx36.2.2, align 1
  %arrayidx.2.2 = getelementptr inbounds i8, i8* inttoptr (i64 268578319 to i8*), i64 %33
  %39 = load i8, i8* %arrayidx.2.2, align 1
  %mul38.2.2 = mul i8 %38, %39
  %40 = load i8, i8* %arrayidx36.1.2, align 1
  %41 = add nsw i64 %33, -1
  %arrayidx.1.2 = getelementptr inbounds i8, i8* inttoptr (i64 268578319 to i8*), i64 %41
  %42 = load i8, i8* %arrayidx.1.2, align 1
  %mul38.1.2 = mul i8 %40, %42
  %43 = load i8, i8* %arrayidx36.251, align 1
  %44 = add nsw i64 %33, -2
  %arrayidx.249 = getelementptr inbounds i8, i8* inttoptr (i64 268578319 to i8*), i64 %44
  %45 = load i8, i8* %arrayidx.249, align 1
  %mul38.253 = mul i8 %43, %45
  %46 = load i8, i8* %arrayidx36.3.1, align 1
  %47 = add nsw i64 %32, 1
  %arrayidx.3.1 = getelementptr inbounds i8, i8* inttoptr (i64 268578319 to i8*), i64 %47
  %48 = load i8, i8* %arrayidx.3.1, align 1
  %mul38.3.1 = mul i8 %46, %48
  %49 = load i8, i8* %arrayidx36.2.1, align 1
  %arrayidx.2.1 = getelementptr inbounds i8, i8* inttoptr (i64 268578319 to i8*), i64 %32
  %50 = load i8, i8* %arrayidx.2.1, align 1
  %mul38.2.1 = mul i8 %49, %50
  %51 = load i8, i8* %arrayidx36.1.1, align 1
  %52 = add nsw i64 %32, -1
  %arrayidx.1.1 = getelementptr inbounds i8, i8* inttoptr (i64 268578319 to i8*), i64 %52
  %53 = load i8, i8* %arrayidx.1.1, align 1
  %mul38.1.1 = mul i8 %51, %53
  %54 = load i8, i8* %arrayidx36.145, align 1
  %55 = add nsw i64 %32, -2
  %arrayidx.143 = getelementptr inbounds i8, i8* inttoptr (i64 268578319 to i8*), i64 %55
  %56 = load i8, i8* %arrayidx.143, align 1
  %mul38.147 = mul i8 %54, %56
  %57 = load i8, i8* %arrayidx36.3, align 1
  %58 = add nsw i64 %31, 1
  %arrayidx.3 = getelementptr inbounds i8, i8* inttoptr (i64 268578319 to i8*), i64 %58
  %59 = load i8, i8* %arrayidx.3, align 1
  %mul38.3 = mul i8 %57, %59
  %60 = load i8, i8* %arrayidx36.2, align 1
  %arrayidx.2 = getelementptr inbounds i8, i8* inttoptr (i64 268578319 to i8*), i64 %31
  %61 = load i8, i8* %arrayidx.2, align 1
  %mul38.2 = mul i8 %60, %61
  %62 = load i8, i8* %arrayidx36.1, align 1
  %63 = add nsw i64 %31, -1
  %arrayidx.1 = getelementptr inbounds i8, i8* inttoptr (i64 268578319 to i8*), i64 %63
  %64 = load i8, i8* %arrayidx.1, align 1
  %mul38.1 = mul i8 %62, %64
  %65 = load i8, i8* %arrayidx36, align 1
  %66 = add nsw i64 %31, -2
  %arrayidx = getelementptr inbounds i8, i8* inttoptr (i64 268578319 to i8*), i64 %66
  %67 = load i8, i8* %arrayidx, align 1
  %mul38 = mul i8 %65, %67
  %add39.1 = add i8 %mul38.1, %mul38
  %add39.2 = add i8 %mul38.2, %add39.1
  %add39.3 = add i8 %mul38.3, %add39.2
  %add39.148 = add i8 %mul38.147, %add39.3
  %add39.1.1 = add i8 %mul38.1.1, %add39.148
  %add39.2.1 = add i8 %mul38.2.1, %add39.1.1
  %add39.3.1 = add i8 %mul38.3.1, %add39.2.1
  %add39.254 = add i8 %mul38.253, %add39.3.1
  %add39.1.2 = add i8 %mul38.1.2, %add39.254
  %add39.2.2 = add i8 %mul38.2.2, %add39.1.2
  %add39.3.2 = add i8 %mul38.3.2, %add39.2.2
  %68 = add nsw i64 %34, -2
  %arrayidx.355 = getelementptr inbounds i8, i8* inttoptr (i64 268578319 to i8*), i64 %68
  %69 = load i8, i8* %arrayidx.355, align 1
  %70 = load i8, i8* %arrayidx36.357, align 1
  %mul38.359 = mul i8 %70, %69
  %add39.360 = add i8 %mul38.359, %add39.3.2
  %71 = add nsw i64 %34, -1
  %arrayidx.1.3 = getelementptr inbounds i8, i8* inttoptr (i64 268578319 to i8*), i64 %71
  %72 = load i8, i8* %arrayidx.1.3, align 1
  %73 = load i8, i8* %arrayidx36.1.3, align 1
  %mul38.1.3 = mul i8 %73, %72
  %add39.1.3 = add i8 %mul38.1.3, %add39.360
  %arrayidx.2.3 = getelementptr inbounds i8, i8* inttoptr (i64 268578319 to i8*), i64 %34
  %74 = load i8, i8* %arrayidx.2.3, align 1
  %75 = load i8, i8* %arrayidx36.2.3, align 1
  %mul38.2.3 = mul i8 %75, %74
  %add39.2.3 = add i8 %mul38.2.3, %add39.1.3
  %76 = add nsw i64 %34, 1
  %arrayidx.3.3 = getelementptr inbounds i8, i8* inttoptr (i64 268578319 to i8*), i64 %76
  %77 = load i8, i8* %arrayidx.3.3, align 1
  %78 = load i8, i8* %arrayidx36.3.3, align 1
  %mul38.3.3 = mul i8 %78, %77
  %add39.3.3 = add i8 %mul38.3.3, %add39.2.3
  %79 = add nsw i64 %21, %indvars.iv21
  %arrayidx50 = getelementptr inbounds i8, i8* inttoptr (i64 268579895 to i8*), i64 %79
  %80 = load i8, i8* %arrayidx50, align 1
  %add52 = add i8 %80, %add39.3.3
  store i8 %add52, i8* %arrayidx50, align 1
  br label %for.inc54
}

attributes #0 = { norecurse nounwind uwtable "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.ident = !{!0}

!0 = !{!"clang version 3.8.1 "}
