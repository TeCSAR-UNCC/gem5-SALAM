; ModuleID = 'spmv.c'
target datalayout = "e-p:64:64:64-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:64:64-f32:32:32-f64:64:64-v64:64:64-v128:128:128-a0:0:64-s0:64:64-f80:128:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: nounwind uwtable
define void @spmv(double* nocapture readonly %val, i32* nocapture readonly %cols, i32* nocapture readonly %rowDelimiters, double* nocapture readonly %vec, double* nocapture %out) #0 {
  br label %1

; <label>:1                                       ; preds = %._crit_edge, %0
  %indvars.iv4 = phi i64 [ 0, %0 ], [ %indvars.iv.next5, %._crit_edge ]
  %2 = getelementptr inbounds i32* %rowDelimiters, i64 %indvars.iv4
  %3 = load i32* %2, align 4, !tbaa !1
  %indvars.iv.next5 = add nuw nsw i64 %indvars.iv4, 1
  %4 = getelementptr inbounds i32* %rowDelimiters, i64 %indvars.iv.next5
  %5 = load i32* %4, align 4, !tbaa !1
  %6 = icmp slt i32 %3, %5
  br i1 %6, label %.lr.ph, label %._crit_edge

.lr.ph:                                           ; preds = %1
  %7 = sext i32 %3 to i64
  br label %8

; <label>:8                                       ; preds = %.lr.ph, %8
  %indvars.iv = phi i64 [ %7, %.lr.ph ], [ %indvars.iv.next, %8 ]
  %sum.01 = phi double [ 0.000000e+00, %.lr.ph ], [ %17, %8 ]
  %9 = getelementptr inbounds double* %val, i64 %indvars.iv
  %10 = load double* %9, align 8, !tbaa !5
  %11 = getelementptr inbounds i32* %cols, i64 %indvars.iv
  %12 = load i32* %11, align 4, !tbaa !1
  %13 = sext i32 %12 to i64
  %14 = getelementptr inbounds double* %vec, i64 %13
  %15 = load double* %14, align 8, !tbaa !5
  %16 = fmul double %10, %15
  %17 = fadd double %sum.01, %16
  %indvars.iv.next = add nsw i64 %indvars.iv, 1
  %18 = trunc i64 %indvars.iv.next to i32
  %19 = icmp slt i32 %18, %5
  br i1 %19, label %8, label %._crit_edge

._crit_edge:                                      ; preds = %8, %1
  %sum.0.lcssa = phi double [ 0.000000e+00, %1 ], [ %17, %8 ]
  %20 = getelementptr inbounds double* %out, i64 %indvars.iv4
  store double %sum.0.lcssa, double* %20, align 8, !tbaa !5
  %exitcond = icmp eq i64 %indvars.iv.next5, 494
  br i1 %exitcond, label %21, label %1

; <label>:21                                      ; preds = %._crit_edge
  ret void
}

attributes #0 = { nounwind uwtable "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.ident = !{!0}

!0 = metadata !{metadata !"clang version 3.4 (tags/RELEASE_34/final)"}
!1 = metadata !{metadata !2, metadata !2, i64 0}
!2 = metadata !{metadata !"int", metadata !3, i64 0}
!3 = metadata !{metadata !"omnipotent char", metadata !4, i64 0}
!4 = metadata !{metadata !"Simple C/C++ TBAA"}
!5 = metadata !{metadata !6, metadata !6, i64 0}
!6 = metadata !{metadata !"double", metadata !3, i64 0}
