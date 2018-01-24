; ModuleID = 'gemm.c'
target datalayout = "e-p:64:64:64-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:64:64-f32:32:32-f64:64:64-v64:64:64-v128:128:128-a0:0:64-s0:64:64-f80:128:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: nounwind uwtable
define void @bbgemm(double* nocapture readonly %m1, double* nocapture readonly %m2, double* nocapture %prod) #0 {
  br label %.preheader5

.preheader5:                                      ; preds = %0, %25
  %indvars.iv16 = phi i64 [ 0, %0 ], [ %indvars.iv.next17, %25 ]
  br label %.preheader3

.preheader3:                                      ; preds = %.preheader5, %22
  %indvars.iv14 = phi i64 [ 0, %.preheader5 ], [ %indvars.iv.next15, %22 ]
  br label %.preheader

.preheader:                                       ; preds = %21, %.preheader3
  %indvars.iv11 = phi i64 [ 0, %.preheader3 ], [ %indvars.iv.next12, %21 ]
  %1 = shl nsw i64 %indvars.iv11, 6
  %2 = add i64 %1, %indvars.iv14
  %3 = add i64 %1, %indvars.iv16
  br label %4

; <label>:4                                       ; preds = %20, %.preheader
  %indvars.iv8 = phi i64 [ 0, %.preheader ], [ %indvars.iv.next9, %20 ]
  %5 = add nsw i64 %indvars.iv8, %indvars.iv14
  %6 = shl i64 %5, 6
  %7 = add i64 %2, %indvars.iv8
  %8 = getelementptr inbounds double* %m1, i64 %7
  %9 = load double* %8, align 8, !tbaa !1
  %10 = add i64 %6, %indvars.iv16
  br label %11

; <label>:11                                      ; preds = %11, %4
  %indvars.iv = phi i64 [ 0, %4 ], [ %indvars.iv.next, %11 ]
  %12 = add i64 %10, %indvars.iv
  %13 = getelementptr inbounds double* %m2, i64 %12
  %14 = load double* %13, align 8, !tbaa !1
  %15 = fmul double %9, %14
  %16 = add i64 %3, %indvars.iv
  %17 = getelementptr inbounds double* %prod, i64 %16
  %18 = load double* %17, align 8, !tbaa !1
  %19 = fadd double %18, %15
  store double %19, double* %17, align 8, !tbaa !1
  %indvars.iv.next = add nuw nsw i64 %indvars.iv, 1
  %exitcond = icmp eq i64 %indvars.iv.next, 8
  br i1 %exitcond, label %20, label %11

; <label>:20                                      ; preds = %11
  %indvars.iv.next9 = add nuw nsw i64 %indvars.iv8, 1
  %exitcond10 = icmp eq i64 %indvars.iv.next9, 8
  br i1 %exitcond10, label %21, label %4

; <label>:21                                      ; preds = %20
  %indvars.iv.next12 = add nuw nsw i64 %indvars.iv11, 1
  %exitcond13 = icmp eq i64 %indvars.iv.next12, 64
  br i1 %exitcond13, label %22, label %.preheader

; <label>:22                                      ; preds = %21
  %indvars.iv.next15 = add nuw nsw i64 %indvars.iv14, 8
  %23 = trunc i64 %indvars.iv.next15 to i32
  %24 = icmp slt i32 %23, 64
  br i1 %24, label %.preheader3, label %25

; <label>:25                                      ; preds = %22
  %indvars.iv.next17 = add nuw nsw i64 %indvars.iv16, 8
  %26 = trunc i64 %indvars.iv.next17 to i32
  %27 = icmp slt i32 %26, 64
  br i1 %27, label %.preheader5, label %28

; <label>:28                                      ; preds = %25
  ret void
}

attributes #0 = { nounwind uwtable "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.ident = !{!0}

!0 = metadata !{metadata !"clang version 3.4 (tags/RELEASE_34/final)"}
!1 = metadata !{metadata !2, metadata !2, i64 0}
!2 = metadata !{metadata !"double", metadata !3, i64 0}
!3 = metadata !{metadata !"omnipotent char", metadata !4, i64 0}
!4 = metadata !{metadata !"Simple C/C++ TBAA"}
