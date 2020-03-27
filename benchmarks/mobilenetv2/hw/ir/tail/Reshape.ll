; ModuleID = 'Reshape.bc'
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: norecurse nounwind uwtable
define void @reshape() #0 {
  br label %.preheader

.preheader:                                       ; preds = %0, %132
  %indvars.iv8 = phi i64 [ 0, %0 ], [ %indvars.iv.next9, %132 ]
  %1 = mul nuw nsw i64 %indvars.iv8, 5
  br label %2

; <label>:2                                       ; preds = %2, %.preheader
  %indvars.iv = phi i64 [ 0, %.preheader ], [ %indvars.iv.next.425, %2 ]
  %3 = load volatile i8, i8* inttoptr (i64 789258064 to i8*), align 16
  %4 = mul nuw nsw i64 %indvars.iv, 25
  %5 = add nuw nsw i64 %1, %4
  %6 = getelementptr inbounds i8, i8* inttoptr (i64 789258065 to i8*), i64 %5
  store volatile i8 %3, i8* %6, align 1
  %7 = load volatile i8, i8* inttoptr (i64 789258064 to i8*), align 16
  %8 = mul i64 %indvars.iv, 25
  %9 = add i64 %8, 25
  %10 = add nuw nsw i64 %1, %9
  %11 = getelementptr inbounds i8, i8* inttoptr (i64 789258065 to i8*), i64 %10
  store volatile i8 %7, i8* %11, align 1
  %12 = load volatile i8, i8* inttoptr (i64 789258064 to i8*), align 16
  %13 = mul i64 %indvars.iv, 25
  %14 = add i64 %13, 50
  %15 = add nuw nsw i64 %1, %14
  %16 = getelementptr inbounds i8, i8* inttoptr (i64 789258065 to i8*), i64 %15
  store volatile i8 %12, i8* %16, align 1
  %17 = load volatile i8, i8* inttoptr (i64 789258064 to i8*), align 16
  %18 = mul i64 %indvars.iv, 25
  %19 = add i64 %18, 75
  %20 = add nuw nsw i64 %1, %19
  %21 = getelementptr inbounds i8, i8* inttoptr (i64 789258065 to i8*), i64 %20
  store volatile i8 %17, i8* %21, align 1
  %22 = load volatile i8, i8* inttoptr (i64 789258064 to i8*), align 16
  %23 = mul i64 %indvars.iv, 25
  %24 = add i64 %23, 100
  %25 = add nuw nsw i64 %1, %24
  %26 = getelementptr inbounds i8, i8* inttoptr (i64 789258065 to i8*), i64 %25
  store volatile i8 %22, i8* %26, align 1
  %indvars.iv.next.425 = add nsw i64 %indvars.iv, 5
  %exitcond.426 = icmp eq i64 %indvars.iv.next.425, 1280
  br i1 %exitcond.426, label %.preheader.111, label %2

.preheader.111:                                   ; preds = %2
  %27 = add nuw nsw i64 %1, 1
  br label %29

; <label>:28                                      ; preds = %132
  ret void

; <label>:29                                      ; preds = %29, %.preheader.111
  %indvars.iv.1 = phi i64 [ 0, %.preheader.111 ], [ %indvars.iv.next.1.4, %29 ]
  %30 = load volatile i8, i8* inttoptr (i64 789258064 to i8*), align 16
  %31 = mul nuw nsw i64 %indvars.iv.1, 25
  %32 = add nuw nsw i64 %27, %31
  %33 = getelementptr inbounds i8, i8* inttoptr (i64 789258065 to i8*), i64 %32
  store volatile i8 %30, i8* %33, align 1
  %34 = load volatile i8, i8* inttoptr (i64 789258064 to i8*), align 16
  %35 = mul i64 %indvars.iv.1, 25
  %36 = add i64 %35, 25
  %37 = add nuw nsw i64 %27, %36
  %38 = getelementptr inbounds i8, i8* inttoptr (i64 789258065 to i8*), i64 %37
  store volatile i8 %34, i8* %38, align 1
  %39 = load volatile i8, i8* inttoptr (i64 789258064 to i8*), align 16
  %40 = mul i64 %indvars.iv.1, 25
  %41 = add i64 %40, 50
  %42 = add nuw nsw i64 %27, %41
  %43 = getelementptr inbounds i8, i8* inttoptr (i64 789258065 to i8*), i64 %42
  store volatile i8 %39, i8* %43, align 1
  %44 = load volatile i8, i8* inttoptr (i64 789258064 to i8*), align 16
  %45 = mul i64 %indvars.iv.1, 25
  %46 = add i64 %45, 75
  %47 = add nuw nsw i64 %27, %46
  %48 = getelementptr inbounds i8, i8* inttoptr (i64 789258065 to i8*), i64 %47
  store volatile i8 %44, i8* %48, align 1
  %49 = load volatile i8, i8* inttoptr (i64 789258064 to i8*), align 16
  %50 = mul i64 %indvars.iv.1, 25
  %51 = add i64 %50, 100
  %52 = add nuw nsw i64 %27, %51
  %53 = getelementptr inbounds i8, i8* inttoptr (i64 789258065 to i8*), i64 %52
  store volatile i8 %49, i8* %53, align 1
  %indvars.iv.next.1.4 = add nsw i64 %indvars.iv.1, 5
  %exitcond.1.4 = icmp eq i64 %indvars.iv.next.1.4, 1280
  br i1 %exitcond.1.4, label %.preheader.212, label %29

.preheader.212:                                   ; preds = %29
  %54 = add nuw nsw i64 %1, 2
  br label %55

; <label>:55                                      ; preds = %55, %.preheader.212
  %indvars.iv.2 = phi i64 [ 0, %.preheader.212 ], [ %indvars.iv.next.2.4, %55 ]
  %56 = load volatile i8, i8* inttoptr (i64 789258064 to i8*), align 16
  %57 = mul nuw nsw i64 %indvars.iv.2, 25
  %58 = add nuw nsw i64 %54, %57
  %59 = getelementptr inbounds i8, i8* inttoptr (i64 789258065 to i8*), i64 %58
  store volatile i8 %56, i8* %59, align 1
  %60 = load volatile i8, i8* inttoptr (i64 789258064 to i8*), align 16
  %61 = mul i64 %indvars.iv.2, 25
  %62 = add i64 %61, 25
  %63 = add nuw nsw i64 %54, %62
  %64 = getelementptr inbounds i8, i8* inttoptr (i64 789258065 to i8*), i64 %63
  store volatile i8 %60, i8* %64, align 1
  %65 = load volatile i8, i8* inttoptr (i64 789258064 to i8*), align 16
  %66 = mul i64 %indvars.iv.2, 25
  %67 = add i64 %66, 50
  %68 = add nuw nsw i64 %54, %67
  %69 = getelementptr inbounds i8, i8* inttoptr (i64 789258065 to i8*), i64 %68
  store volatile i8 %65, i8* %69, align 1
  %70 = load volatile i8, i8* inttoptr (i64 789258064 to i8*), align 16
  %71 = mul i64 %indvars.iv.2, 25
  %72 = add i64 %71, 75
  %73 = add nuw nsw i64 %54, %72
  %74 = getelementptr inbounds i8, i8* inttoptr (i64 789258065 to i8*), i64 %73
  store volatile i8 %70, i8* %74, align 1
  %75 = load volatile i8, i8* inttoptr (i64 789258064 to i8*), align 16
  %76 = mul i64 %indvars.iv.2, 25
  %77 = add i64 %76, 100
  %78 = add nuw nsw i64 %54, %77
  %79 = getelementptr inbounds i8, i8* inttoptr (i64 789258065 to i8*), i64 %78
  store volatile i8 %75, i8* %79, align 1
  %indvars.iv.next.2.4 = add nsw i64 %indvars.iv.2, 5
  %exitcond.2.4 = icmp eq i64 %indvars.iv.next.2.4, 1280
  br i1 %exitcond.2.4, label %.preheader.313, label %55

.preheader.313:                                   ; preds = %55
  %80 = add nuw nsw i64 %1, 3
  br label %81

; <label>:81                                      ; preds = %81, %.preheader.313
  %indvars.iv.3 = phi i64 [ 0, %.preheader.313 ], [ %indvars.iv.next.3.4, %81 ]
  %82 = load volatile i8, i8* inttoptr (i64 789258064 to i8*), align 16
  %83 = mul nuw nsw i64 %indvars.iv.3, 25
  %84 = add nuw nsw i64 %80, %83
  %85 = getelementptr inbounds i8, i8* inttoptr (i64 789258065 to i8*), i64 %84
  store volatile i8 %82, i8* %85, align 1
  %86 = load volatile i8, i8* inttoptr (i64 789258064 to i8*), align 16
  %87 = mul i64 %indvars.iv.3, 25
  %88 = add i64 %87, 25
  %89 = add nuw nsw i64 %80, %88
  %90 = getelementptr inbounds i8, i8* inttoptr (i64 789258065 to i8*), i64 %89
  store volatile i8 %86, i8* %90, align 1
  %91 = load volatile i8, i8* inttoptr (i64 789258064 to i8*), align 16
  %92 = mul i64 %indvars.iv.3, 25
  %93 = add i64 %92, 50
  %94 = add nuw nsw i64 %80, %93
  %95 = getelementptr inbounds i8, i8* inttoptr (i64 789258065 to i8*), i64 %94
  store volatile i8 %91, i8* %95, align 1
  %96 = load volatile i8, i8* inttoptr (i64 789258064 to i8*), align 16
  %97 = mul i64 %indvars.iv.3, 25
  %98 = add i64 %97, 75
  %99 = add nuw nsw i64 %80, %98
  %100 = getelementptr inbounds i8, i8* inttoptr (i64 789258065 to i8*), i64 %99
  store volatile i8 %96, i8* %100, align 1
  %101 = load volatile i8, i8* inttoptr (i64 789258064 to i8*), align 16
  %102 = mul i64 %indvars.iv.3, 25
  %103 = add i64 %102, 100
  %104 = add nuw nsw i64 %80, %103
  %105 = getelementptr inbounds i8, i8* inttoptr (i64 789258065 to i8*), i64 %104
  store volatile i8 %101, i8* %105, align 1
  %indvars.iv.next.3.4 = add nsw i64 %indvars.iv.3, 5
  %exitcond.3.4 = icmp eq i64 %indvars.iv.next.3.4, 1280
  br i1 %exitcond.3.4, label %.preheader.414, label %81

.preheader.414:                                   ; preds = %81
  %106 = add nuw nsw i64 %1, 4
  br label %107

; <label>:107                                     ; preds = %107, %.preheader.414
  %indvars.iv.4 = phi i64 [ 0, %.preheader.414 ], [ %indvars.iv.next.4.4, %107 ]
  %108 = load volatile i8, i8* inttoptr (i64 789258064 to i8*), align 16
  %109 = mul nuw nsw i64 %indvars.iv.4, 25
  %110 = add nuw nsw i64 %106, %109
  %111 = getelementptr inbounds i8, i8* inttoptr (i64 789258065 to i8*), i64 %110
  store volatile i8 %108, i8* %111, align 1
  %112 = load volatile i8, i8* inttoptr (i64 789258064 to i8*), align 16
  %113 = mul i64 %indvars.iv.4, 25
  %114 = add i64 %113, 25
  %115 = add nuw nsw i64 %106, %114
  %116 = getelementptr inbounds i8, i8* inttoptr (i64 789258065 to i8*), i64 %115
  store volatile i8 %112, i8* %116, align 1
  %117 = load volatile i8, i8* inttoptr (i64 789258064 to i8*), align 16
  %118 = mul i64 %indvars.iv.4, 25
  %119 = add i64 %118, 50
  %120 = add nuw nsw i64 %106, %119
  %121 = getelementptr inbounds i8, i8* inttoptr (i64 789258065 to i8*), i64 %120
  store volatile i8 %117, i8* %121, align 1
  %122 = load volatile i8, i8* inttoptr (i64 789258064 to i8*), align 16
  %123 = mul i64 %indvars.iv.4, 25
  %124 = add i64 %123, 75
  %125 = add nuw nsw i64 %106, %124
  %126 = getelementptr inbounds i8, i8* inttoptr (i64 789258065 to i8*), i64 %125
  store volatile i8 %122, i8* %126, align 1
  %127 = load volatile i8, i8* inttoptr (i64 789258064 to i8*), align 16
  %128 = mul i64 %indvars.iv.4, 25
  %129 = add i64 %128, 100
  %130 = add nuw nsw i64 %106, %129
  %131 = getelementptr inbounds i8, i8* inttoptr (i64 789258065 to i8*), i64 %130
  store volatile i8 %127, i8* %131, align 1
  %indvars.iv.next.4.4 = add nsw i64 %indvars.iv.4, 5
  %exitcond.4.4 = icmp eq i64 %indvars.iv.next.4.4, 1280
  br i1 %exitcond.4.4, label %132, label %107

; <label>:132                                     ; preds = %107
  %indvars.iv.next9 = add nuw nsw i64 %indvars.iv8, 1
  %exitcond10 = icmp eq i64 %indvars.iv.next9, 5
  br i1 %exitcond10, label %28, label %.preheader
}

attributes #0 = { norecurse nounwind uwtable "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.ident = !{!0}

!0 = !{!"clang version 3.8.1 "}
