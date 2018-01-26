; ModuleID = 'stencil.c'
target datalayout = "e-p:64:64:64-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:64:64-f32:32:32-f64:64:64-v64:64:64-v128:128:128-a0:0:64-s0:64:64-f80:128:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: nounwind uwtable
define void @stencil(i32* nocapture readonly %orig, i32* nocapture %sol, i32* nocapture readonly %filter) #0 {
  br label %.preheader6

.preheader6:                                      ; preds = %19, %0
  %indvars.iv15 = phi i64 [ 0, %0 ], [ %indvars.iv.next16, %19 ]
  %1 = shl i64 %indvars.iv15, 6
  br label %.preheader3

.preheader3:                                      ; preds = %16, %.preheader6
  %indvars.iv12 = phi i64 [ 0, %.preheader6 ], [ %indvars.iv.next13, %16 ]
  br label %.preheader

.preheader:                                       ; preds = %15, %.preheader3
  %indvars.iv9 = phi i64 [ 0, %.preheader3 ], [ %indvars.iv.next10, %15 ]
  %temp.05 = phi i32 [ 0, %.preheader3 ], [ %14, %15 ]
  %2 = mul nsw i64 %indvars.iv9, 3
  %3 = add nsw i64 %indvars.iv9, %indvars.iv15
  %4 = shl i64 %3, 6
  %5 = add nsw i64 %4, %indvars.iv12
  br label %6

; <label>:6                                       ; preds = %6, %.preheader
  %indvars.iv = phi i64 [ 0, %.preheader ], [ %indvars.iv.next, %6 ]
  %temp.12 = phi i32 [ %temp.05, %.preheader ], [ %14, %6 ]
  %7 = add nsw i64 %indvars.iv, %2
  %8 = getelementptr inbounds i32* %filter, i64 %7
  %9 = load i32* %8, align 4, !tbaa !1
  %10 = add nsw i64 %5, %indvars.iv
  %11 = getelementptr inbounds i32* %orig, i64 %10
  %12 = load i32* %11, align 4, !tbaa !1
  %13 = mul nsw i32 %12, %9
  %14 = add nsw i32 %13, %temp.12
  %indvars.iv.next = add nuw nsw i64 %indvars.iv, 1
  %exitcond = icmp eq i64 %indvars.iv.next, 3
  br i1 %exitcond, label %15, label %6

; <label>:15                                      ; preds = %6
  %indvars.iv.next10 = add nuw nsw i64 %indvars.iv9, 1
  %exitcond11 = icmp eq i64 %indvars.iv.next10, 3
  br i1 %exitcond11, label %16, label %.preheader

; <label>:16                                      ; preds = %15
  %17 = add nsw i64 %indvars.iv12, %1
  %18 = getelementptr inbounds i32* %sol, i64 %17
  store i32 %14, i32* %18, align 4, !tbaa !1
  %indvars.iv.next13 = add nuw nsw i64 %indvars.iv12, 1
  %exitcond14 = icmp eq i64 %indvars.iv.next13, 62
  br i1 %exitcond14, label %19, label %.preheader3

; <label>:19                                      ; preds = %16
  %indvars.iv.next16 = add nuw nsw i64 %indvars.iv15, 1
  %exitcond17 = icmp eq i64 %indvars.iv.next16, 126
  br i1 %exitcond17, label %20, label %.preheader6

; <label>:20                                      ; preds = %19
  ret void
}

attributes #0 = { nounwind uwtable "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.ident = !{!0}

!0 = metadata !{metadata !"clang version 3.4 (tags/RELEASE_34/final)"}
!1 = metadata !{metadata !2, metadata !2, i64 0}
!2 = metadata !{metadata !"int", metadata !3, i64 0}
!3 = metadata !{metadata !"omnipotent char", metadata !4, i64 0}
!4 = metadata !{metadata !"Simple C/C++ TBAA"}
