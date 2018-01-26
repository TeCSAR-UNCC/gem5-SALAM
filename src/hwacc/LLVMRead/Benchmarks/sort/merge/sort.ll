; ModuleID = 'sort.c'
target datalayout = "e-p:64:64:64-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:64:64-f32:32:32-f64:64:64-v64:64:64-v128:128:128-a0:0:64-s0:64:64-f80:128:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: nounwind uwtable
define void @merge(i32* nocapture %a, i32* nocapture %temp, i32 %start, i32 %m, i32 %stop) #0 {
  %1 = icmp sgt i32 %start, %m
  br i1 %1, label %._crit_edge10, label %.lr.ph9

.lr.ph9:                                          ; preds = %0
  %2 = sext i32 %start to i64
  br label %3

; <label>:3                                       ; preds = %3, %.lr.ph9
  %indvars.iv13 = phi i64 [ %indvars.iv.next14, %3 ], [ %2, %.lr.ph9 ]
  %4 = getelementptr inbounds i32* %a, i64 %indvars.iv13
  %5 = load i32* %4, align 4, !tbaa !1
  %6 = getelementptr inbounds i32* %temp, i64 %indvars.iv13
  store i32 %5, i32* %6, align 4, !tbaa !1
  %indvars.iv.next14 = add nsw i64 %indvars.iv13, 1
  %7 = trunc i64 %indvars.iv13 to i32
  %8 = icmp slt i32 %7, %m
  br i1 %8, label %3, label %._crit_edge10

._crit_edge10:                                    ; preds = %3, %0
  %9 = icmp slt i32 %m, %stop
  br i1 %9, label %.lr.ph6, label %.preheader

.lr.ph6:                                          ; preds = %._crit_edge10
  %10 = add nsw i32 %m, 1
  %11 = add nsw i32 %10, %stop
  %12 = sext i32 %m to i64
  br label %15

.preheader:                                       ; preds = %15, %._crit_edge10
  %13 = icmp sgt i32 %start, %stop
  br i1 %13, label %._crit_edge, label %.lr.ph

.lr.ph:                                           ; preds = %.preheader
  %14 = sext i32 %start to i64
  br label %24

; <label>:15                                      ; preds = %15, %.lr.ph6
  %indvars.iv11.in = phi i64 [ %indvars.iv11, %15 ], [ %12, %.lr.ph6 ]
  %indvars.iv11 = add i64 %indvars.iv11.in, 1
  %16 = getelementptr inbounds i32* %a, i64 %indvars.iv11
  %17 = load i32* %16, align 4, !tbaa !1
  %18 = trunc i64 %indvars.iv11 to i32
  %19 = sub i32 %11, %18
  %20 = sext i32 %19 to i64
  %21 = getelementptr inbounds i32* %temp, i64 %20
  store i32 %17, i32* %21, align 4, !tbaa !1
  %22 = trunc i64 %indvars.iv11 to i32
  %23 = icmp slt i32 %22, %stop
  br i1 %23, label %15, label %.preheader

; <label>:24                                      ; preds = %37, %.lr.ph
  %indvars.iv = phi i64 [ %indvars.iv.next, %37 ], [ %14, %.lr.ph ]
  %i.13 = phi i32 [ %i.2, %37 ], [ %start, %.lr.ph ]
  %j.12 = phi i32 [ %j.2, %37 ], [ %stop, %.lr.ph ]
  %25 = sext i32 %j.12 to i64
  %26 = getelementptr inbounds i32* %temp, i64 %25
  %27 = load i32* %26, align 4, !tbaa !1
  %28 = sext i32 %i.13 to i64
  %29 = getelementptr inbounds i32* %temp, i64 %28
  %30 = load i32* %29, align 4, !tbaa !1
  %31 = icmp slt i32 %27, %30
  %32 = getelementptr inbounds i32* %a, i64 %indvars.iv
  br i1 %31, label %33, label %35

; <label>:33                                      ; preds = %24
  store i32 %27, i32* %32, align 4, !tbaa !1
  %34 = add nsw i32 %j.12, -1
  br label %37

; <label>:35                                      ; preds = %24
  store i32 %30, i32* %32, align 4, !tbaa !1
  %36 = add nsw i32 %i.13, 1
  br label %37

; <label>:37                                      ; preds = %33, %35
  %j.2 = phi i32 [ %34, %33 ], [ %j.12, %35 ]
  %i.2 = phi i32 [ %i.13, %33 ], [ %36, %35 ]
  %indvars.iv.next = add nsw i64 %indvars.iv, 1
  %38 = trunc i64 %indvars.iv to i32
  %39 = icmp slt i32 %38, %stop
  br i1 %39, label %24, label %._crit_edge

._crit_edge:                                      ; preds = %37, %.preheader
  ret void
}

; Function Attrs: nounwind uwtable
define void @ms_mergesort(i32* nocapture %a, i32* nocapture %temp) #0 {
  br label %.preheader

.preheader:                                       ; preds = %0, %13
  %m.02 = phi i32 [ 1, %0 ], [ %14, %13 ]
  %1 = shl i32 %m.02, 1
  br label %2

; <label>:2                                       ; preds = %.preheader, %10
  %i.01 = phi i32 [ 0, %.preheader ], [ %11, %10 ]
  %3 = add nsw i32 %i.01, %m.02
  %4 = add nsw i32 %3, -1
  %5 = add nsw i32 %3, %m.02
  %6 = icmp slt i32 %5, 2049
  br i1 %6, label %7, label %9

; <label>:7                                       ; preds = %2
  %8 = add nsw i32 %5, -1
  tail call void @merge(i32* %a, i32* %temp, i32 %i.01, i32 %4, i32 %8) #1
  br label %10

; <label>:9                                       ; preds = %2
  tail call void @merge(i32* %a, i32* %temp, i32 %i.01, i32 %4, i32 2048) #1
  br label %10

; <label>:10                                      ; preds = %7, %9
  %11 = add nsw i32 %i.01, %1
  %12 = icmp slt i32 %11, 2048
  br i1 %12, label %2, label %13

; <label>:13                                      ; preds = %10
  %14 = shl nsw i32 %m.02, 1
  %15 = icmp slt i32 %14, 2048
  br i1 %15, label %.preheader, label %16

; <label>:16                                      ; preds = %13
  ret void
}

attributes #0 = { nounwind uwtable "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nobuiltin }

!llvm.ident = !{!0}

!0 = metadata !{metadata !"clang version 3.4 (tags/RELEASE_34/final)"}
!1 = metadata !{metadata !2, metadata !2, i64 0}
!2 = metadata !{metadata !"int", metadata !3, i64 0}
!3 = metadata !{metadata !"omnipotent char", metadata !4, i64 0}
!4 = metadata !{metadata !"Simple C/C++ TBAA"}
