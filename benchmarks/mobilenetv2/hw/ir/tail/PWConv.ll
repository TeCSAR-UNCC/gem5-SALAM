; ModuleID = 'PWConv.bc'
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: norecurse nounwind uwtable
define void @PWConv() #0 {
  br label %.preheader5

.preheader5:                                      ; preds = %51, %0
  %y.013 = phi i32 [ 0, %0 ], [ %52, %51 ]
  br label %.preheader4

.preheader4:                                      ; preds = %49, %.preheader5
  %x.012 = phi i32 [ 0, %.preheader5 ], [ %50, %49 ]
  br label %1

; <label>:1                                       ; preds = %1, %.preheader4
  %indvars.iv = phi i64 [ 0, %.preheader4 ], [ %indvars.iv.next.7, %1 ]
  %2 = load volatile i8, i8* inttoptr (i64 788942912 to i8*), align 64
  %3 = getelementptr inbounds i8, i8* inttoptr (i64 789257824 to i8*), i64 %indvars.iv
  store volatile i8 %2, i8* %3, align 8
  %indvars.iv.next = or i64 %indvars.iv, 1
  %4 = load volatile i8, i8* inttoptr (i64 788942912 to i8*), align 64
  %5 = getelementptr inbounds i8, i8* inttoptr (i64 789257824 to i8*), i64 %indvars.iv.next
  store volatile i8 %4, i8* %5, align 1
  %indvars.iv.next.1 = or i64 %indvars.iv, 2
  %6 = load volatile i8, i8* inttoptr (i64 788942912 to i8*), align 64
  %7 = getelementptr inbounds i8, i8* inttoptr (i64 789257824 to i8*), i64 %indvars.iv.next.1
  store volatile i8 %6, i8* %7, align 2
  %indvars.iv.next.2 = or i64 %indvars.iv, 3
  %8 = load volatile i8, i8* inttoptr (i64 788942912 to i8*), align 64
  %9 = getelementptr inbounds i8, i8* inttoptr (i64 789257824 to i8*), i64 %indvars.iv.next.2
  store volatile i8 %8, i8* %9, align 1
  %indvars.iv.next.3 = or i64 %indvars.iv, 4
  %10 = load volatile i8, i8* inttoptr (i64 788942912 to i8*), align 64
  %11 = getelementptr inbounds i8, i8* inttoptr (i64 789257824 to i8*), i64 %indvars.iv.next.3
  store volatile i8 %10, i8* %11, align 4
  %indvars.iv.next.4 = or i64 %indvars.iv, 5
  %12 = load volatile i8, i8* inttoptr (i64 788942912 to i8*), align 64
  %13 = getelementptr inbounds i8, i8* inttoptr (i64 789257824 to i8*), i64 %indvars.iv.next.4
  store volatile i8 %12, i8* %13, align 1
  %indvars.iv.next.5 = or i64 %indvars.iv, 6
  %14 = load volatile i8, i8* inttoptr (i64 788942912 to i8*), align 64
  %15 = getelementptr inbounds i8, i8* inttoptr (i64 789257824 to i8*), i64 %indvars.iv.next.5
  store volatile i8 %14, i8* %15, align 2
  %indvars.iv.next.6 = or i64 %indvars.iv, 7
  %16 = load volatile i8, i8* inttoptr (i64 788942912 to i8*), align 64
  %17 = getelementptr inbounds i8, i8* inttoptr (i64 789257824 to i8*), i64 %indvars.iv.next.6
  store volatile i8 %16, i8* %17, align 1
  %indvars.iv.next.7 = add nsw i64 %indvars.iv, 8
  %exitcond.7 = icmp eq i64 %indvars.iv.next.7, 240
  br i1 %exitcond.7, label %.preheader.preheader, label %1

.preheader.preheader:                             ; preds = %1
  br label %.preheader

.preheader:                                       ; preds = %.preheader.preheader, %80
  %indvars.iv24 = phi i64 [ %indvars.iv.next25, %80 ], [ 0, %.preheader.preheader ]
  %sext31 = shl i64 %indvars.iv24, 32
  %18 = ashr exact i64 %sext31, 32
  %19 = getelementptr inbounds i8, i8* inttoptr (i64 789250144 to i8*), i64 %18
  %20 = load volatile i8, i8* %19, align 1
  %21 = getelementptr inbounds i8, i8* inttoptr (i64 789256544 to i8*), i64 %18
  %22 = load volatile i8, i8* %21, align 1
  %23 = mul nuw nsw i64 %indvars.iv24, 240
  br label %24

; <label>:24                                      ; preds = %24, %.preheader
  %indvars.iv16 = phi i64 [ 0, %.preheader ], [ %indvars.iv.next17.144, %24 ]
  %sum.17 = phi i32 [ 0, %.preheader ], [ %48, %24 ]
  %25 = getelementptr inbounds i8, i8* inttoptr (i64 789257824 to i8*), i64 %indvars.iv16
  %26 = load volatile i8, i8* %25, align 2
  %27 = add nuw nsw i64 %indvars.iv16, %23
  %sext32 = shl i64 %27, 32
  %28 = ashr exact i64 %sext32, 32
  %29 = getelementptr inbounds i8, i8* inttoptr (i64 788942944 to i8*), i64 %28
  %30 = load volatile i8, i8* %29, align 2
  %31 = zext i8 %26 to i32
  %32 = sub i8 %30, %22
  %33 = sext i8 %32 to i32
  %34 = shl nuw nsw i32 %31, 16
  %sext = mul nsw i32 %34, %33
  %35 = ashr exact i32 %sext, 16
  %36 = add nsw i32 %35, %sum.17
  %indvars.iv.next17 = or i64 %indvars.iv16, 1
  %37 = getelementptr inbounds i8, i8* inttoptr (i64 789257824 to i8*), i64 %indvars.iv.next17
  %38 = load volatile i8, i8* %37, align 1
  %39 = add nuw nsw i64 %indvars.iv.next17, %23
  %sext32.1 = shl i64 %39, 32
  %40 = ashr exact i64 %sext32.1, 32
  %41 = getelementptr inbounds i8, i8* inttoptr (i64 788942944 to i8*), i64 %40
  %42 = load volatile i8, i8* %41, align 1
  %43 = zext i8 %38 to i32
  %44 = sub i8 %42, %22
  %45 = sext i8 %44 to i32
  %46 = shl nuw nsw i32 %43, 16
  %sext.143 = mul nsw i32 %46, %45
  %47 = ashr exact i32 %sext.143, 16
  %48 = add nsw i32 %47, %36
  %indvars.iv.next17.144 = add nsw i64 %indvars.iv16, 2
  %exitcond18.145 = icmp eq i64 %indvars.iv.next17.144, 80
  br i1 %exitcond18.145, label %.preheader.129.preheader, label %24

.preheader.129.preheader:                         ; preds = %24
  %.lcssa = phi i32 [ %48, %24 ]
  br label %.preheader.129

; <label>:49                                      ; preds = %80
  %50 = add nuw nsw i32 %x.012, 1
  %exitcond27 = icmp eq i32 %50, 5
  br i1 %exitcond27, label %51, label %.preheader4

; <label>:51                                      ; preds = %49
  %52 = add nuw nsw i32 %y.013, 1
  %exitcond28 = icmp eq i32 %52, 5
  br i1 %exitcond28, label %53, label %.preheader5

; <label>:53                                      ; preds = %51
  ret void

.preheader.129:                                   ; preds = %.preheader.129.preheader, %.preheader.129
  %indvars.iv16.1 = phi i64 [ %indvars.iv.next17.1, %.preheader.129 ], [ 0, %.preheader.129.preheader ]
  %sum.17.1 = phi i32 [ %66, %.preheader.129 ], [ %.lcssa, %.preheader.129.preheader ]
  %54 = add nuw nsw i64 %indvars.iv16.1, 80
  %55 = getelementptr inbounds i8, i8* inttoptr (i64 789257824 to i8*), i64 %54
  %56 = load volatile i8, i8* %55, align 1
  %57 = add nuw nsw i64 %54, %23
  %sext33 = shl i64 %57, 32
  %58 = ashr exact i64 %sext33, 32
  %59 = getelementptr inbounds i8, i8* inttoptr (i64 788942944 to i8*), i64 %58
  %60 = load volatile i8, i8* %59, align 1
  %61 = zext i8 %56 to i32
  %62 = sub i8 %60, %22
  %63 = sext i8 %62 to i32
  %64 = shl nuw nsw i32 %61, 16
  %sext.1 = mul nsw i32 %64, %63
  %65 = ashr exact i32 %sext.1, 16
  %66 = add nsw i32 %65, %sum.17.1
  %indvars.iv.next17.1 = add nuw nsw i64 %indvars.iv16.1, 1
  %exitcond18.1 = icmp eq i64 %indvars.iv.next17.1, 80
  br i1 %exitcond18.1, label %.preheader.230.preheader, label %.preheader.129

.preheader.230.preheader:                         ; preds = %.preheader.129
  %.lcssa48 = phi i32 [ %66, %.preheader.129 ]
  br label %.preheader.230

.preheader.230:                                   ; preds = %.preheader.230.preheader, %.preheader.230
  %indvars.iv16.2 = phi i64 [ %indvars.iv.next17.2, %.preheader.230 ], [ 0, %.preheader.230.preheader ]
  %sum.17.2 = phi i32 [ %79, %.preheader.230 ], [ %.lcssa48, %.preheader.230.preheader ]
  %67 = add nuw nsw i64 %indvars.iv16.2, 160
  %68 = getelementptr inbounds i8, i8* inttoptr (i64 789257824 to i8*), i64 %67
  %69 = load volatile i8, i8* %68, align 1
  %70 = add nuw nsw i64 %67, %23
  %sext34 = shl i64 %70, 32
  %71 = ashr exact i64 %sext34, 32
  %72 = getelementptr inbounds i8, i8* inttoptr (i64 788942944 to i8*), i64 %71
  %73 = load volatile i8, i8* %72, align 1
  %74 = zext i8 %69 to i32
  %75 = sub i8 %73, %22
  %76 = sext i8 %75 to i32
  %77 = shl nuw nsw i32 %74, 16
  %sext.2 = mul nsw i32 %77, %76
  %78 = ashr exact i32 %sext.2, 16
  %79 = add nsw i32 %78, %sum.17.2
  %indvars.iv.next17.2 = add nuw nsw i64 %indvars.iv16.2, 1
  %exitcond18.2 = icmp eq i64 %indvars.iv.next17.2, 80
  br i1 %exitcond18.2, label %80, label %.preheader.230

; <label>:80                                      ; preds = %.preheader.230
  %.lcssa47 = phi i32 [ %79, %.preheader.230 ]
  %81 = sext i8 %20 to i32
  %82 = getelementptr inbounds i8, i8* inttoptr (i64 789251424 to i8*), i64 %18
  %83 = load volatile i8, i8* %82, align 1
  %84 = zext i8 %83 to i32
  %85 = getelementptr inbounds i8, i8* inttoptr (i64 789252704 to i8*), i64 %18
  %86 = load volatile i8, i8* %85, align 1
  %87 = icmp sgt i8 %86, -1
  %88 = shl nsw i32 %81, 16
  %sext2 = mul nsw i32 %88, %84
  %89 = ashr exact i32 %sext2, 16
  %90 = load volatile i8, i8* %85, align 1
  %91 = sext i8 %90 to i32
  %92 = ashr i32 %89, %91
  %93 = sub nsw i32 0, %91
  %94 = shl i32 %89, %93
  %scaled_bias.0 = select i1 %87, i32 %92, i32 %94
  %95 = add nsw i32 %scaled_bias.0, %.lcssa47
  %96 = getelementptr inbounds i8, i8* inttoptr (i64 789253984 to i8*), i64 %18
  %97 = load volatile i8, i8* %96, align 1
  %98 = zext i8 %97 to i32
  %99 = mul nsw i32 %95, %98
  %100 = getelementptr inbounds i8, i8* inttoptr (i64 789255264 to i8*), i64 %18
  %101 = load volatile i8, i8* %100, align 1
  %102 = zext i8 %101 to i32
  %103 = ashr i32 %99, %102
  %104 = icmp slt i32 %103, 255
  %. = select i1 %104, i32 %103, i32 255
  %105 = icmp slt i32 %., 0
  %106 = select i1 %105, i32 0, i32 %.
  %107 = trunc i32 %106 to i8
  store volatile i8 %107, i8* inttoptr (i64 789258064 to i8*), align 16
  %indvars.iv.next25 = add nuw nsw i64 %indvars.iv24, 1
  %exitcond26 = icmp eq i64 %indvars.iv.next25, 1280
  br i1 %exitcond26, label %49, label %.preheader
}

attributes #0 = { norecurse nounwind uwtable "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.ident = !{!0}

!0 = !{!"clang version 3.8.1 "}
