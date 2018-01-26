; ModuleID = 'spmv.c'
target datalayout = "e-p:64:64:64-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:64:64-f32:32:32-f64:64:64-v64:64:64-v128:128:128-a0:0:64-s0:64:64-f80:128:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: nounwind uwtable
define void @ellpack(double* nocapture readonly %nzval, i32* nocapture readonly %cols, double* nocapture readonly %vec, double* nocapture %out) #0 {
  br label %1

; <label>:1                                       ; preds = %16, %0
  %indvars.iv4 = phi i64 [ 0, %0 ], [ %indvars.iv.next5, %16 ]
  %2 = getelementptr inbounds double* %out, i64 %indvars.iv4
  %3 = load double* %2, align 8, !tbaa !1
  %4 = mul nsw i64 %indvars.iv4, 10
  br label %5

; <label>:5                                       ; preds = %5, %1
  %indvars.iv = phi i64 [ 0, %1 ], [ %indvars.iv.next, %5 ]
  %sum.02 = phi double [ %3, %1 ], [ %15, %5 ]
  %6 = add nsw i64 %indvars.iv, %4
  %7 = getelementptr inbounds double* %nzval, i64 %6
  %8 = load double* %7, align 8, !tbaa !1
  %9 = getelementptr inbounds i32* %cols, i64 %6
  %10 = load i32* %9, align 4, !tbaa !5
  %11 = sext i32 %10 to i64
  %12 = getelementptr inbounds double* %vec, i64 %11
  %13 = load double* %12, align 8, !tbaa !1
  %14 = fmul double %8, %13
  %15 = fadd double %sum.02, %14
  %indvars.iv.next = add nuw nsw i64 %indvars.iv, 1
  %exitcond = icmp eq i64 %indvars.iv.next, 10
  br i1 %exitcond, label %16, label %5

; <label>:16                                      ; preds = %5
  store double %15, double* %2, align 8, !tbaa !1
  %indvars.iv.next5 = add nuw nsw i64 %indvars.iv4, 1
  %exitcond6 = icmp eq i64 %indvars.iv.next5, 494
  br i1 %exitcond6, label %17, label %1

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
!5 = metadata !{metadata !6, metadata !6, i64 0}
!6 = metadata !{metadata !"int", metadata !3, i64 0}
