; ModuleID = 'gemm.c'
target datalayout = "e-p:64:64:64-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:64:64-f32:32:32-f64:64:64-v64:64:64-v128:128:128-a0:0:64-s0:64:64-f80:128:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: nounwind uwtable
define void @gemm(double* nocapture readonly %m1, double* nocapture readonly %m2, double* nocapture %prod) #0 {
  br label %.preheader

.preheader:                                       ; preds = %16, %0
  %indvars.iv8 = phi i64 [ 0, %0 ], [ %indvars.iv.next9, %16 ]
  %1 = shl nsw i64 %indvars.iv8, 6
  br label %2

; <label>:2                                       ; preds = %13, %.preheader
  %indvars.iv5 = phi i64 [ 0, %.preheader ], [ %indvars.iv.next6, %13 ]
  br label %3

; <label>:3                                       ; preds = %3, %2
  %indvars.iv = phi i64 [ 0, %2 ], [ %indvars.iv.next, %3 ]
  %sum.02 = phi double [ 0.000000e+00, %2 ], [ %12, %3 ]
  %4 = shl i64 %indvars.iv, 6
  %5 = add nsw i64 %indvars.iv, %1
  %6 = getelementptr inbounds double* %m1, i64 %5
  %7 = load double* %6, align 8, !tbaa !1
  %8 = add nsw i64 %4, %indvars.iv5
  %9 = getelementptr inbounds double* %m2, i64 %8
  %10 = load double* %9, align 8, !tbaa !1
  %11 = fmul double %7, %10
  %12 = fadd double %sum.02, %11
  %indvars.iv.next = add nuw nsw i64 %indvars.iv, 1
  %exitcond = icmp eq i64 %indvars.iv.next, 64
  br i1 %exitcond, label %13, label %3

; <label>:13                                      ; preds = %3
  %14 = add nsw i64 %indvars.iv5, %1
  %15 = getelementptr inbounds double* %prod, i64 %14
  store double %12, double* %15, align 8, !tbaa !1
  %indvars.iv.next6 = add nuw nsw i64 %indvars.iv5, 1
  %exitcond7 = icmp eq i64 %indvars.iv.next6, 64
  br i1 %exitcond7, label %16, label %2

; <label>:16                                      ; preds = %13
  %indvars.iv.next9 = add nuw nsw i64 %indvars.iv8, 1
  %exitcond10 = icmp eq i64 %indvars.iv.next9, 64
  br i1 %exitcond10, label %17, label %.preheader

; <label>:17                                      ; preds = %16
  ret void
}

attributes #0 = { nounwind uwtable "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.ident = !{!0}

!0 = metadata !{metadata !"clang version 3.4 (tags/RELEASE_34/final)"}
!1 = metadata !{metadata !2, metadata !2, i64 0}
!2 = metadata !{metadata !"double", metadata !3, i64 0}
!3 = metadata !{metadata !"omnipotent char", metadata !4, i64 0}
!4 = metadata !{metadata !"Simple C/C++ TBAA"}
