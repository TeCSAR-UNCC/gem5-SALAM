; ModuleID = 'PWConv.bc'
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: norecurse nounwind uwtable
define void @PWConv() #0 {
  br label %.preheader5

.preheader5:                                      ; preds = %86, %0
  %y.013 = phi i32 [ 0, %0 ], [ %87, %86 ]
  br label %.preheader3.preheader28

.preheader3.preheader28:                          ; preds = %.preheader5, %84
  %x.012 = phi i32 [ 0, %.preheader5 ], [ %85, %84 ]
  %1 = load volatile i8, i8* inttoptr (i64 788535461 to i8*), align 1
  store volatile i8 %1, i8* inttoptr (i64 788535990 to i8*), align 2
  %2 = load volatile i8, i8* inttoptr (i64 788535461 to i8*), align 1
  store volatile i8 %2, i8* inttoptr (i64 788535991 to i8*), align 1
  %3 = load volatile i8, i8* inttoptr (i64 788535461 to i8*), align 1
  store volatile i8 %3, i8* inttoptr (i64 788535992 to i8*), align 8
  %4 = load volatile i8, i8* inttoptr (i64 788535461 to i8*), align 1
  store volatile i8 %4, i8* inttoptr (i64 788535993 to i8*), align 1
  %5 = load volatile i8, i8* inttoptr (i64 788535461 to i8*), align 1
  store volatile i8 %5, i8* inttoptr (i64 788535994 to i8*), align 2
  %6 = load volatile i8, i8* inttoptr (i64 788535461 to i8*), align 1
  store volatile i8 %6, i8* inttoptr (i64 788535995 to i8*), align 1
  %7 = load volatile i8, i8* inttoptr (i64 788535461 to i8*), align 1
  store volatile i8 %7, i8* inttoptr (i64 788535996 to i8*), align 4
  %8 = load volatile i8, i8* inttoptr (i64 788535461 to i8*), align 1
  store volatile i8 %8, i8* inttoptr (i64 788535997 to i8*), align 1
  %9 = load volatile i8, i8* inttoptr (i64 788535461 to i8*), align 1
  store volatile i8 %9, i8* inttoptr (i64 788535998 to i8*), align 2
  %10 = load volatile i8, i8* inttoptr (i64 788535461 to i8*), align 1
  store volatile i8 %10, i8* inttoptr (i64 788535999 to i8*), align 1
  %11 = load volatile i8, i8* inttoptr (i64 788535461 to i8*), align 1
  store volatile i8 %11, i8* inttoptr (i64 788536000 to i8*), align 64
  %12 = load volatile i8, i8* inttoptr (i64 788535461 to i8*), align 1
  store volatile i8 %12, i8* inttoptr (i64 788536001 to i8*), align 1
  %13 = load volatile i8, i8* inttoptr (i64 788535461 to i8*), align 1
  store volatile i8 %13, i8* inttoptr (i64 788536002 to i8*), align 2
  %14 = load volatile i8, i8* inttoptr (i64 788535461 to i8*), align 1
  store volatile i8 %14, i8* inttoptr (i64 788536003 to i8*), align 1
  %15 = load volatile i8, i8* inttoptr (i64 788535461 to i8*), align 1
  store volatile i8 %15, i8* inttoptr (i64 788536004 to i8*), align 4
  %16 = load volatile i8, i8* inttoptr (i64 788535461 to i8*), align 1
  store volatile i8 %16, i8* inttoptr (i64 788536005 to i8*), align 1
  %17 = load volatile i8, i8* inttoptr (i64 788535461 to i8*), align 1
  store volatile i8 %17, i8* inttoptr (i64 788536006 to i8*), align 2
  %18 = load volatile i8, i8* inttoptr (i64 788535461 to i8*), align 1
  store volatile i8 %18, i8* inttoptr (i64 788536007 to i8*), align 1
  %19 = load volatile i8, i8* inttoptr (i64 788535461 to i8*), align 1
  store volatile i8 %19, i8* inttoptr (i64 788536008 to i8*), align 8
  %20 = load volatile i8, i8* inttoptr (i64 788535461 to i8*), align 1
  store volatile i8 %20, i8* inttoptr (i64 788536009 to i8*), align 1
  %21 = load volatile i8, i8* inttoptr (i64 788535461 to i8*), align 1
  store volatile i8 %21, i8* inttoptr (i64 788536010 to i8*), align 2
  %22 = load volatile i8, i8* inttoptr (i64 788535461 to i8*), align 1
  store volatile i8 %22, i8* inttoptr (i64 788536011 to i8*), align 1
  %23 = load volatile i8, i8* inttoptr (i64 788535461 to i8*), align 1
  store volatile i8 %23, i8* inttoptr (i64 788536012 to i8*), align 4
  %24 = load volatile i8, i8* inttoptr (i64 788535461 to i8*), align 1
  store volatile i8 %24, i8* inttoptr (i64 788536013 to i8*), align 1
  br label %.preheader

.preheader:                                       ; preds = %.preheader3.preheader28, %56
  %indvars.iv23 = phi i64 [ 0, %.preheader3.preheader28 ], [ %indvars.iv.next24, %56 ]
  %sext29 = shl i64 %indvars.iv23, 32
  %25 = ashr exact i64 %sext29, 32
  %26 = getelementptr inbounds i8, i8* inttoptr (i64 788535846 to i8*), i64 %25
  %27 = load volatile i8, i8* %26, align 1
  %28 = getelementptr inbounds i8, i8* inttoptr (i64 788535966 to i8*), i64 %25
  %29 = load volatile i8, i8* %28, align 1
  %30 = mul nuw nsw i64 %indvars.iv23, 24
  br label %31

; <label>:31                                      ; preds = %31, %.preheader
  %indvars.iv16 = phi i64 [ 0, %.preheader ], [ %indvars.iv.next17.1, %31 ]
  %sum.17 = phi i32 [ 0, %.preheader ], [ %55, %31 ]
  %32 = getelementptr inbounds i8, i8* inttoptr (i64 788535990 to i8*), i64 %indvars.iv16
  %33 = load volatile i8, i8* %32, align 2
  %34 = add nuw nsw i64 %indvars.iv16, %30
  %sext30 = shl i64 %34, 32
  %35 = ashr exact i64 %sext30, 32
  %36 = getelementptr inbounds i8, i8* inttoptr (i64 788535462 to i8*), i64 %35
  %37 = load volatile i8, i8* %36, align 2
  %38 = zext i8 %33 to i32
  %39 = sub i8 %37, %29
  %40 = sext i8 %39 to i32
  %41 = shl nuw nsw i32 %38, 16
  %sext = mul nsw i32 %41, %40
  %42 = ashr exact i32 %sext, 16
  %43 = add nsw i32 %42, %sum.17
  %indvars.iv.next17 = or i64 %indvars.iv16, 1
  %44 = getelementptr inbounds i8, i8* inttoptr (i64 788535990 to i8*), i64 %indvars.iv.next17
  %45 = load volatile i8, i8* %44, align 1
  %46 = add nuw nsw i64 %indvars.iv.next17, %30
  %sext30.1 = shl i64 %46, 32
  %47 = ashr exact i64 %sext30.1, 32
  %48 = getelementptr inbounds i8, i8* inttoptr (i64 788535462 to i8*), i64 %47
  %49 = load volatile i8, i8* %48, align 1
  %50 = zext i8 %45 to i32
  %51 = sub i8 %49, %29
  %52 = sext i8 %51 to i32
  %53 = shl nuw nsw i32 %50, 16
  %sext.1 = mul nsw i32 %53, %52
  %54 = ashr exact i32 %sext.1, 16
  %55 = add nsw i32 %54, %43
  %indvars.iv.next17.1 = add nsw i64 %indvars.iv16, 2
  %exitcond18.1 = icmp eq i64 %indvars.iv.next17.1, 24
  br i1 %exitcond18.1, label %56, label %31

; <label>:56                                      ; preds = %31
  %.lcssa = phi i32 [ %55, %31 ]
  %57 = sext i8 %27 to i32
  %58 = getelementptr inbounds i8, i8* inttoptr (i64 788535870 to i8*), i64 %25
  %59 = load volatile i8, i8* %58, align 1
  %60 = zext i8 %59 to i32
  %61 = getelementptr inbounds i8, i8* inttoptr (i64 788535894 to i8*), i64 %25
  %62 = load volatile i8, i8* %61, align 1
  %63 = icmp sgt i8 %62, -1
  %64 = shl nsw i32 %57, 16
  %sext2 = mul nsw i32 %64, %60
  %65 = ashr exact i32 %sext2, 16
  %66 = load volatile i8, i8* %61, align 1
  %67 = sext i8 %66 to i32
  %68 = ashr i32 %65, %67
  %69 = sub nsw i32 0, %67
  %70 = shl i32 %65, %69
  %scaled_bias.0 = select i1 %63, i32 %68, i32 %70
  %71 = add nsw i32 %scaled_bias.0, %.lcssa
  %72 = getelementptr inbounds i8, i8* inttoptr (i64 788535918 to i8*), i64 %25
  %73 = load volatile i8, i8* %72, align 1
  %74 = zext i8 %73 to i32
  %75 = mul nsw i32 %71, %74
  %76 = getelementptr inbounds i8, i8* inttoptr (i64 788535942 to i8*), i64 %25
  %77 = load volatile i8, i8* %76, align 1
  %78 = zext i8 %77 to i32
  %79 = ashr i32 %75, %78
  %80 = icmp slt i32 %79, 255
  %. = select i1 %80, i32 %79, i32 255
  %81 = icmp slt i32 %., 0
  %82 = select i1 %81, i32 0, i32 %.
  %83 = trunc i32 %82 to i8
  store volatile i8 %83, i8* inttoptr (i64 788529185 to i8*), align 1
  %indvars.iv.next24 = add nuw nsw i64 %indvars.iv23, 1
  %exitcond25 = icmp eq i64 %indvars.iv.next24, 16
  br i1 %exitcond25, label %84, label %.preheader

; <label>:84                                      ; preds = %56
  %85 = add nuw nsw i32 %x.012, 1
  %exitcond26 = icmp eq i32 %85, 80
  br i1 %exitcond26, label %86, label %.preheader3.preheader28

; <label>:86                                      ; preds = %84
  %87 = add nuw nsw i32 %y.013, 1
  %exitcond27 = icmp eq i32 %87, 80
  br i1 %exitcond27, label %88, label %.preheader5

; <label>:88                                      ; preds = %86
  ret void
}

attributes #0 = { norecurse nounwind uwtable "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.ident = !{!0}

!0 = !{!"clang version 3.8.1 "}
