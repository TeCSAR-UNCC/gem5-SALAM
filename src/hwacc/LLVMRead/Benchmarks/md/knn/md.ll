; ModuleID = 'md.c'
target datalayout = "e-p:64:64:64-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:64:64-f32:32:32-f64:64:64-v64:64:64-v128:128:128-a0:0:64-s0:64:64-f80:128:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: nounwind uwtable
define void @md_kernel(double* nocapture %force_x, double* nocapture %force_y, double* nocapture %force_z, double* nocapture readonly %position_x, double* nocapture readonly %position_y, double* nocapture readonly %position_z, i32* nocapture readonly %NL) #0 {
  br label %1

; <label>:1                                       ; preds = %41, %0
  %indvars.iv8 = phi i64 [ 0, %0 ], [ %indvars.iv.next9, %41 ]
  %2 = getelementptr inbounds double* %position_x, i64 %indvars.iv8
  %3 = load double* %2, align 8, !tbaa !1
  %4 = getelementptr inbounds double* %position_y, i64 %indvars.iv8
  %5 = load double* %4, align 8, !tbaa !1
  %6 = getelementptr inbounds double* %position_z, i64 %indvars.iv8
  %7 = load double* %6, align 8, !tbaa !1
  %8 = shl i64 %indvars.iv8, 4
  br label %9

; <label>:9                                       ; preds = %9, %1
  %indvars.iv = phi i64 [ 0, %1 ], [ %indvars.iv.next, %9 ]
  %fx.04 = phi double [ 0.000000e+00, %1 ], [ %36, %9 ]
  %fz.02 = phi double [ 0.000000e+00, %1 ], [ %40, %9 ]
  %fy.01 = phi double [ 0.000000e+00, %1 ], [ %38, %9 ]
  %10 = add nsw i64 %indvars.iv, %8
  %11 = getelementptr inbounds i32* %NL, i64 %10
  %12 = load i32* %11, align 4, !tbaa !5
  %13 = sext i32 %12 to i64
  %14 = getelementptr inbounds double* %position_x, i64 %13
  %15 = load double* %14, align 8, !tbaa !1
  %16 = getelementptr inbounds double* %position_y, i64 %13
  %17 = load double* %16, align 8, !tbaa !1
  %18 = getelementptr inbounds double* %position_z, i64 %13
  %19 = load double* %18, align 8, !tbaa !1
  %20 = fsub double %3, %15
  %21 = fsub double %5, %17
  %22 = fsub double %7, %19
  %23 = fmul double %20, %20
  %24 = fmul double %21, %21
  %25 = fadd double %23, %24
  %26 = fmul double %22, %22
  %27 = fadd double %25, %26
  %28 = fdiv double 1.000000e+00, %27
  %29 = fmul double %28, %28
  %30 = fmul double %28, %29
  %31 = fmul double %30, 1.500000e+00
  %32 = fadd double %31, -2.000000e+00
  %33 = fmul double %30, %32
  %34 = fmul double %28, %33
  %35 = fmul double %20, %34
  %36 = fadd double %fx.04, %35
  %37 = fmul double %21, %34
  %38 = fadd double %fy.01, %37
  %39 = fmul double %22, %34
  %40 = fadd double %fz.02, %39
  %indvars.iv.next = add nuw nsw i64 %indvars.iv, 1
  %exitcond = icmp eq i64 %indvars.iv.next, 16
  br i1 %exitcond, label %41, label %9

; <label>:41                                      ; preds = %9
  %42 = getelementptr inbounds double* %force_x, i64 %indvars.iv8
  store double %36, double* %42, align 8, !tbaa !1
  %43 = getelementptr inbounds double* %force_y, i64 %indvars.iv8
  store double %38, double* %43, align 8, !tbaa !1
  %44 = getelementptr inbounds double* %force_z, i64 %indvars.iv8
  store double %40, double* %44, align 8, !tbaa !1
  %indvars.iv.next9 = add nuw nsw i64 %indvars.iv8, 1
  %exitcond10 = icmp eq i64 %indvars.iv.next9, 256
  br i1 %exitcond10, label %45, label %1

; <label>:45                                      ; preds = %41
  ret void
}

attributes #0 = { nounwind uwtable "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.ident = !{!0}

!0 = metadata !{metadata !"clang version 3.4 (tags/RELEASE_34/final)"}
!1 = metadata !{metadata !2, metadata !2, i64 0}
!2 = metadata !{metadata !"double", metadata !3, i64 0}
!3 = metadata !{metadata !"omnipotent char", metadata !4, i64 0}
!4 = metadata !{metadata !"Simple C/C++ TBAA"}
!5 = metadata !{metadata !6, metadata !6, i64 0}
!6 = metadata !{metadata !"int", metadata !3, i64 0}
