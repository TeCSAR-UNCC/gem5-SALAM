; ModuleID = 'stencil.c'
target datalayout = "e-p:64:64:64-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:64:64-f32:32:32-f64:64:64-v64:64:64-v128:128:128-a0:0:64-s0:64:64-f80:128:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: nounwind uwtable
define void @stencil3d(i32* nocapture readonly %C, i32* nocapture readonly %orig, i32* nocapture %sol) #0 {
  br label %.preheader14

.preheader14:                                     ; preds = %13, %0
  %indvars.iv38 = phi i64 [ 0, %0 ], [ %indvars.iv.next39, %13 ]
  %1 = shl i64 %indvars.iv38, 4
  %2 = shl i64 %indvars.iv38, 4
  %3 = add i64 %2, 15872
  br label %4

; <label>:4                                       ; preds = %4, %.preheader14
  %indvars.iv35 = phi i64 [ 0, %.preheader14 ], [ %indvars.iv.next36, %4 ]
  %5 = add nsw i64 %indvars.iv35, %1
  %6 = getelementptr inbounds i32* %orig, i64 %5
  %7 = load i32* %6, align 4, !tbaa !1
  %8 = getelementptr inbounds i32* %sol, i64 %5
  store i32 %7, i32* %8, align 4, !tbaa !1
  %9 = add nsw i64 %3, %indvars.iv35
  %10 = getelementptr inbounds i32* %orig, i64 %9
  %11 = load i32* %10, align 4, !tbaa !1
  %12 = getelementptr inbounds i32* %sol, i64 %9
  store i32 %11, i32* %12, align 4, !tbaa !1
  %indvars.iv.next36 = add nuw nsw i64 %indvars.iv35, 1
  %exitcond37 = icmp eq i64 %indvars.iv.next36, 16
  br i1 %exitcond37, label %13, label %4

; <label>:13                                      ; preds = %4
  %indvars.iv.next39 = add nuw nsw i64 %indvars.iv38, 1
  %exitcond40 = icmp eq i64 %indvars.iv.next39, 32
  br i1 %exitcond40, label %.preheader10, label %.preheader14

.preheader10:                                     ; preds = %13, %25
  %indvars.iv32 = phi i64 [ %indvars.iv.next33, %25 ], [ 1, %13 ]
  %14 = shl i64 %indvars.iv32, 9
  %15 = or i64 %14, 496
  br label %16

; <label>:16                                      ; preds = %16, %.preheader10
  %indvars.iv29 = phi i64 [ 0, %.preheader10 ], [ %indvars.iv.next30, %16 ]
  %17 = add nsw i64 %indvars.iv29, %14
  %18 = getelementptr inbounds i32* %orig, i64 %17
  %19 = load i32* %18, align 4, !tbaa !1
  %20 = getelementptr inbounds i32* %sol, i64 %17
  store i32 %19, i32* %20, align 4, !tbaa !1
  %21 = add nsw i64 %indvars.iv29, %15
  %22 = getelementptr inbounds i32* %orig, i64 %21
  %23 = load i32* %22, align 4, !tbaa !1
  %24 = getelementptr inbounds i32* %sol, i64 %21
  store i32 %23, i32* %24, align 4, !tbaa !1
  %indvars.iv.next30 = add nuw nsw i64 %indvars.iv29, 1
  %exitcond31 = icmp eq i64 %indvars.iv.next30, 16
  br i1 %exitcond31, label %25, label %16

; <label>:25                                      ; preds = %16
  %indvars.iv.next33 = add nuw nsw i64 %indvars.iv32, 1
  %exitcond34 = icmp eq i64 %indvars.iv.next33, 31
  br i1 %exitcond34, label %.preheader6, label %.preheader10

.preheader6:                                      ; preds = %25, %38
  %indvars.iv26 = phi i64 [ %indvars.iv.next27, %38 ], [ 1, %25 ]
  %26 = shl i64 %indvars.iv26, 5
  br label %28

.preheader4:                                      ; preds = %38
  %27 = getelementptr inbounds i32* %C, i64 1
  br label %.preheader2

; <label>:28                                      ; preds = %28, %.preheader6
  %indvars.iv23 = phi i64 [ 1, %.preheader6 ], [ %indvars.iv.next24, %28 ]
  %29 = add nsw i64 %indvars.iv23, %26
  %30 = shl nsw i64 %29, 4
  %31 = getelementptr inbounds i32* %orig, i64 %30
  %32 = load i32* %31, align 4, !tbaa !1
  %33 = getelementptr inbounds i32* %sol, i64 %30
  store i32 %32, i32* %33, align 4, !tbaa !1
  %34 = or i64 %30, 15
  %35 = getelementptr inbounds i32* %orig, i64 %34
  %36 = load i32* %35, align 4, !tbaa !1
  %37 = getelementptr inbounds i32* %sol, i64 %34
  store i32 %36, i32* %37, align 4, !tbaa !1
  %indvars.iv.next24 = add nuw nsw i64 %indvars.iv23, 1
  %exitcond25 = icmp eq i64 %indvars.iv.next24, 31
  br i1 %exitcond25, label %38, label %28

; <label>:38                                      ; preds = %28
  %indvars.iv.next27 = add nuw nsw i64 %indvars.iv26, 1
  %exitcond28 = icmp eq i64 %indvars.iv.next27, 31
  br i1 %exitcond28, label %.preheader4, label %.preheader6

.preheader2:                                      ; preds = %95, %.preheader4
  %indvars.iv20 = phi i64 [ 1, %.preheader4 ], [ %indvars.iv.next21, %95 ]
  %39 = shl nsw i64 %indvars.iv20, 5
  %40 = shl i64 %indvars.iv20, 5
  %41 = add i64 %40, 32
  %42 = add i64 %40, 268435424
  %43 = or i64 %39, 1
  %44 = add i64 %39, 268435455
  br label %.preheader

.preheader:                                       ; preds = %93, %.preheader2
  %indvars.iv17 = phi i64 [ 1, %.preheader2 ], [ %indvars.iv.next18, %93 ]
  %j.23 = phi i32 [ 1, %.preheader2 ], [ %94, %93 ]
  %45 = add nsw i64 %indvars.iv17, %39
  %46 = shl nsw i64 %45, 4
  %47 = add nsw i64 %41, %indvars.iv17
  %48 = shl i64 %47, 4
  %49 = trunc i64 %42 to i32
  %50 = add nsw i32 %49, %j.23
  %51 = shl i32 %50, 4
  %52 = add i64 %43, %indvars.iv17
  %53 = shl i64 %52, 4
  %54 = trunc i64 %44 to i32
  %55 = add i32 %54, %j.23
  %56 = shl i32 %55, 4
  %57 = add i64 %46, -1
  %58 = sext i32 %56 to i64
  %59 = sext i32 %51 to i64
  br label %60

; <label>:60                                      ; preds = %60, %.preheader
  %indvars.iv = phi i64 [ 1, %.preheader ], [ %indvars.iv.next, %60 ]
  %61 = add nsw i64 %indvars.iv, %46
  %62 = getelementptr inbounds i32* %orig, i64 %61
  %63 = load i32* %62, align 4, !tbaa !1
  %64 = add nsw i64 %indvars.iv, %48
  %65 = getelementptr inbounds i32* %orig, i64 %64
  %66 = load i32* %65, align 4, !tbaa !1
  %67 = add nsw i64 %indvars.iv, %59
  %68 = getelementptr inbounds i32* %orig, i64 %67
  %69 = load i32* %68, align 4, !tbaa !1
  %70 = add nsw i32 %69, %66
  %71 = add nsw i64 %indvars.iv, %53
  %72 = getelementptr inbounds i32* %orig, i64 %71
  %73 = load i32* %72, align 4, !tbaa !1
  %74 = add nsw i32 %70, %73
  %75 = add nsw i64 %indvars.iv, %58
  %76 = getelementptr inbounds i32* %orig, i64 %75
  %77 = load i32* %76, align 4, !tbaa !1
  %78 = add nsw i32 %74, %77
  %indvars.iv.next = add nuw nsw i64 %indvars.iv, 1
  %79 = add nsw i64 %indvars.iv.next, %46
  %80 = getelementptr inbounds i32* %orig, i64 %79
  %81 = load i32* %80, align 4, !tbaa !1
  %82 = add nsw i32 %78, %81
  %83 = add i64 %57, %indvars.iv
  %84 = getelementptr inbounds i32* %orig, i64 %83
  %85 = load i32* %84, align 4, !tbaa !1
  %86 = add nsw i32 %82, %85
  %87 = load i32* %C, align 4, !tbaa !1
  %88 = mul nsw i32 %87, %63
  %89 = load i32* %27, align 4, !tbaa !1
  %90 = mul nsw i32 %89, %86
  %91 = add nsw i32 %90, %88
  %92 = getelementptr inbounds i32* %sol, i64 %61
  store i32 %91, i32* %92, align 4, !tbaa !1
  %exitcond = icmp eq i64 %indvars.iv.next, 15
  br i1 %exitcond, label %93, label %60

; <label>:93                                      ; preds = %60
  %indvars.iv.next18 = add nuw nsw i64 %indvars.iv17, 1
  %94 = add nsw i32 %j.23, 1
  %exitcond19 = icmp eq i64 %indvars.iv.next18, 31
  br i1 %exitcond19, label %95, label %.preheader

; <label>:95                                      ; preds = %93
  %indvars.iv.next21 = add nuw nsw i64 %indvars.iv20, 1
  %exitcond22 = icmp eq i64 %indvars.iv.next21, 31
  br i1 %exitcond22, label %96, label %.preheader2

; <label>:96                                      ; preds = %95
  ret void
}

attributes #0 = { nounwind uwtable "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.ident = !{!0}

!0 = metadata !{metadata !"clang version 3.4 (tags/RELEASE_34/final)"}
!1 = metadata !{metadata !2, metadata !2, i64 0}
!2 = metadata !{metadata !"int", metadata !3, i64 0}
!3 = metadata !{metadata !"omnipotent char", metadata !4, i64 0}
!4 = metadata !{metadata !"Simple C/C++ TBAA"}
