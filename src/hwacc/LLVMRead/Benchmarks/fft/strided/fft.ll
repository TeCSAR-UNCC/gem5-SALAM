; ModuleID = 'fft.c'
target datalayout = "e-p:64:64:64-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:64:64-f32:32:32-f64:64:64-v64:64:64-v128:128:128-a0:0:64-s0:64:64-f80:128:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: nounwind uwtable
define void @fft(double* nocapture %real, double* nocapture %img, double* nocapture readonly %real_twid, double* nocapture readonly %img_twid) #0 {
  br label %.preheader

.preheader:                                       ; preds = %0, %._crit_edge
  %log.04 = phi i32 [ 0, %0 ], [ %39, %._crit_edge ]
  %span.02 = phi i32 [ 512, %0 ], [ %38, %._crit_edge ]
  %1 = icmp slt i32 %span.02, 1024
  br i1 %1, label %.lr.ph, label %._crit_edge

.lr.ph:                                           ; preds = %.preheader, %35
  %odd.01 = phi i32 [ %36, %35 ], [ %span.02, %.preheader ]
  %2 = or i32 %odd.01, %span.02
  %3 = xor i32 %2, %span.02
  %4 = sext i32 %3 to i64
  %5 = getelementptr inbounds double* %real, i64 %4
  %6 = load double* %5, align 8, !tbaa !1
  %7 = sext i32 %2 to i64
  %8 = getelementptr inbounds double* %real, i64 %7
  %9 = load double* %8, align 8, !tbaa !1
  %10 = fadd double %6, %9
  %11 = fsub double %6, %9
  store double %11, double* %8, align 8, !tbaa !1
  store double %10, double* %5, align 8, !tbaa !1
  %12 = getelementptr inbounds double* %img, i64 %4
  %13 = load double* %12, align 8, !tbaa !1
  %14 = getelementptr inbounds double* %img, i64 %7
  %15 = load double* %14, align 8, !tbaa !1
  %16 = fadd double %13, %15
  %17 = fsub double %13, %15
  store double %17, double* %14, align 8, !tbaa !1
  store double %16, double* %12, align 8, !tbaa !1
  %18 = shl i32 %3, %log.04
  %19 = and i32 %18, 1023
  %20 = icmp eq i32 %19, 0
  br i1 %20, label %35, label %21

; <label>:21                                      ; preds = %.lr.ph
  %22 = zext i32 %19 to i64
  %23 = getelementptr inbounds double* %real_twid, i64 %22
  %24 = load double* %23, align 8, !tbaa !1
  %25 = load double* %8, align 8, !tbaa !1
  %26 = fmul double %24, %25
  %27 = getelementptr inbounds double* %img_twid, i64 %22
  %28 = load double* %27, align 8, !tbaa !1
  %29 = load double* %14, align 8, !tbaa !1
  %30 = fmul double %28, %29
  %31 = fsub double %26, %30
  %32 = fmul double %24, %29
  %33 = fmul double %25, %28
  %34 = fadd double %33, %32
  store double %34, double* %14, align 8, !tbaa !1
  store double %31, double* %8, align 8, !tbaa !1
  br label %35

; <label>:35                                      ; preds = %.lr.ph, %21
  %36 = add nsw i32 %2, 1
  %37 = icmp slt i32 %36, 1024
  br i1 %37, label %.lr.ph, label %._crit_edge

._crit_edge:                                      ; preds = %35, %.preheader
  %38 = ashr i32 %span.02, 1
  %39 = add nsw i32 %log.04, 1
  %exitcond = icmp eq i32 %39, 10
  br i1 %exitcond, label %40, label %.preheader

; <label>:40                                      ; preds = %._crit_edge
  ret void
}

attributes #0 = { nounwind uwtable "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.ident = !{!0}

!0 = metadata !{metadata !"clang version 3.4 (tags/RELEASE_34/final)"}
!1 = metadata !{metadata !2, metadata !2, i64 0}
!2 = metadata !{metadata !"double", metadata !3, i64 0}
!3 = metadata !{metadata !"omnipotent char", metadata !4, i64 0}
!4 = metadata !{metadata !"Simple C/C++ TBAA"}
