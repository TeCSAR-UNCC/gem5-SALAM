; ModuleID = 'sort.c'
target datalayout = "e-p:64:64:64-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:64:64-f32:32:32-f64:64:64-v64:64:64-v128:128:128-a0:0:64-s0:64:64-f80:128:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: nounwind uwtable
define void @local_scan(i32* nocapture %bucket) #0 {
  br label %.preheader

.preheader:                                       ; preds = %10, %0
  %indvars.iv3 = phi i64 [ 0, %0 ], [ %indvars.iv.next4, %10 ]
  %1 = shl nsw i64 %indvars.iv3, 4
  br label %2

; <label>:2                                       ; preds = %2, %.preheader
  %indvars.iv = phi i64 [ 1, %.preheader ], [ %indvars.iv.next, %2 ]
  %3 = add nsw i64 %indvars.iv, %1
  %4 = add nsw i64 %3, -1
  %5 = getelementptr inbounds i32* %bucket, i64 %4
  %6 = load i32* %5, align 4, !tbaa !1
  %7 = getelementptr inbounds i32* %bucket, i64 %3
  %8 = load i32* %7, align 4, !tbaa !1
  %9 = add nsw i32 %8, %6
  store i32 %9, i32* %7, align 4, !tbaa !1
  %indvars.iv.next = add nuw nsw i64 %indvars.iv, 1
  %exitcond = icmp eq i64 %indvars.iv.next, 16
  br i1 %exitcond, label %10, label %2

; <label>:10                                      ; preds = %2
  %indvars.iv.next4 = add nuw nsw i64 %indvars.iv3, 1
  %exitcond5 = icmp eq i64 %indvars.iv.next4, 128
  br i1 %exitcond5, label %11, label %.preheader

; <label>:11                                      ; preds = %10
  ret void
}

; Function Attrs: nounwind uwtable
define void @sum_scan(i32* nocapture %sum, i32* nocapture readonly %bucket) #0 {
  store i32 0, i32* %sum, align 4, !tbaa !1
  br label %1

; <label>:1                                       ; preds = %1, %0
  %indvars.iv = phi i64 [ 1, %0 ], [ %indvars.iv.next, %1 ]
  %2 = shl i64 %indvars.iv, 4
  %3 = add nsw i64 %2, -1
  %4 = add nsw i64 %indvars.iv, -1
  %5 = getelementptr inbounds i32* %sum, i64 %4
  %6 = load i32* %5, align 4, !tbaa !1
  %7 = getelementptr inbounds i32* %bucket, i64 %3
  %8 = load i32* %7, align 4, !tbaa !1
  %9 = add nsw i32 %8, %6
  %10 = getelementptr inbounds i32* %sum, i64 %indvars.iv
  store i32 %9, i32* %10, align 4, !tbaa !1
  %indvars.iv.next = add nuw nsw i64 %indvars.iv, 1
  %exitcond = icmp eq i64 %indvars.iv.next, 128
  br i1 %exitcond, label %11, label %1

; <label>:11                                      ; preds = %1
  ret void
}

; Function Attrs: nounwind uwtable
define void @last_step_scan(i32* nocapture %bucket, i32* nocapture readonly %sum) #0 {
  br label %.preheader

.preheader:                                       ; preds = %9, %0
  %indvars.iv3 = phi i64 [ 0, %0 ], [ %indvars.iv.next4, %9 ]
  %1 = shl i64 %indvars.iv3, 4
  %2 = getelementptr inbounds i32* %sum, i64 %indvars.iv3
  br label %3

; <label>:3                                       ; preds = %3, %.preheader
  %indvars.iv = phi i64 [ 0, %.preheader ], [ %indvars.iv.next, %3 ]
  %4 = add nsw i64 %indvars.iv, %1
  %5 = getelementptr inbounds i32* %bucket, i64 %4
  %6 = load i32* %5, align 4, !tbaa !1
  %7 = load i32* %2, align 4, !tbaa !1
  %8 = add nsw i32 %7, %6
  store i32 %8, i32* %5, align 4, !tbaa !1
  %indvars.iv.next = add nuw nsw i64 %indvars.iv, 1
  %exitcond = icmp eq i64 %indvars.iv.next, 16
  br i1 %exitcond, label %9, label %3

; <label>:9                                       ; preds = %3
  %indvars.iv.next4 = add nuw nsw i64 %indvars.iv3, 1
  %exitcond5 = icmp eq i64 %indvars.iv.next4, 128
  br i1 %exitcond5, label %10, label %.preheader

; <label>:10                                      ; preds = %9
  ret void
}

; Function Attrs: nounwind uwtable
define void @init(i32* nocapture %bucket) #0 {
  br label %1

; <label>:1                                       ; preds = %1, %0
  %indvars.iv = phi i64 [ 0, %0 ], [ %indvars.iv.next, %1 ]
  %2 = getelementptr inbounds i32* %bucket, i64 %indvars.iv
  store i32 0, i32* %2, align 4, !tbaa !1
  %indvars.iv.next = add nuw nsw i64 %indvars.iv, 1
  %exitcond = icmp eq i64 %indvars.iv.next, 2048
  br i1 %exitcond, label %3, label %1

; <label>:3                                       ; preds = %1
  ret void
}

; Function Attrs: nounwind uwtable
define void @hist(i32* nocapture %bucket, i32* nocapture readonly %a, i32 %exp) #0 {
  br label %.preheader

.preheader:                                       ; preds = %15, %0
  %indvars.iv3 = phi i64 [ 0, %0 ], [ %indvars.iv.next4, %15 ]
  %1 = shl i64 %indvars.iv3, 2
  %indvars.iv.next4 = add nuw nsw i64 %indvars.iv3, 1
  br label %2

; <label>:2                                       ; preds = %2, %.preheader
  %indvars.iv = phi i64 [ 0, %.preheader ], [ %indvars.iv.next, %2 ]
  %3 = add nsw i64 %indvars.iv, %1
  %4 = getelementptr inbounds i32* %a, i64 %3
  %5 = load i32* %4, align 4, !tbaa !1
  %6 = ashr i32 %5, %exp
  %7 = shl i32 %6, 9
  %8 = and i32 %7, 1536
  %9 = trunc i64 %indvars.iv.next4 to i32
  %10 = add i32 %9, %8
  %11 = sext i32 %10 to i64
  %12 = getelementptr inbounds i32* %bucket, i64 %11
  %13 = load i32* %12, align 4, !tbaa !1
  %14 = add nsw i32 %13, 1
  store i32 %14, i32* %12, align 4, !tbaa !1
  %indvars.iv.next = add nuw nsw i64 %indvars.iv, 1
  %exitcond = icmp eq i64 %indvars.iv.next, 4
  br i1 %exitcond, label %15, label %2

; <label>:15                                      ; preds = %2
  %exitcond5 = icmp eq i64 %indvars.iv.next4, 512
  br i1 %exitcond5, label %16, label %.preheader

; <label>:16                                      ; preds = %15
  ret void
}

; Function Attrs: nounwind uwtable
define void @update(i32* nocapture %b, i32* nocapture %bucket, i32* nocapture readonly %a, i32 %exp) #0 {
  br label %.preheader

.preheader:                                       ; preds = %18, %0
  %indvars.iv3 = phi i64 [ 0, %0 ], [ %indvars.iv.next4, %18 ]
  %1 = shl i64 %indvars.iv3, 2
  br label %2

; <label>:2                                       ; preds = %2, %.preheader
  %indvars.iv = phi i64 [ 0, %.preheader ], [ %indvars.iv.next, %2 ]
  %3 = add nsw i64 %indvars.iv, %1
  %4 = getelementptr inbounds i32* %a, i64 %3
  %5 = load i32* %4, align 4, !tbaa !1
  %6 = ashr i32 %5, %exp
  %7 = shl i32 %6, 9
  %8 = and i32 %7, 1536
  %9 = trunc i64 %indvars.iv3 to i32
  %10 = add nsw i32 %8, %9
  %11 = sext i32 %10 to i64
  %12 = getelementptr inbounds i32* %bucket, i64 %11
  %13 = load i32* %12, align 4, !tbaa !1
  %14 = sext i32 %13 to i64
  %15 = getelementptr inbounds i32* %b, i64 %14
  store i32 %5, i32* %15, align 4, !tbaa !1
  %16 = load i32* %12, align 4, !tbaa !1
  %17 = add nsw i32 %16, 1
  store i32 %17, i32* %12, align 4, !tbaa !1
  %indvars.iv.next = add nuw nsw i64 %indvars.iv, 1
  %exitcond = icmp eq i64 %indvars.iv.next, 4
  br i1 %exitcond, label %18, label %2

; <label>:18                                      ; preds = %2
  %indvars.iv.next4 = add nuw nsw i64 %indvars.iv3, 1
  %exitcond5 = icmp eq i64 %indvars.iv.next4, 512
  br i1 %exitcond5, label %19, label %.preheader

; <label>:19                                      ; preds = %18
  ret void
}

; Function Attrs: nounwind uwtable
define void @ss_sort(i32* nocapture %a, i32* nocapture %b, i32* nocapture %bucket, i32* nocapture %sum) #0 {
  br label %1

; <label>:1                                       ; preds = %0, %8
  %valid_buffer.02 = phi i32 [ 0, %0 ], [ %valid_buffer.1, %8 ]
  %exp.01 = phi i32 [ 0, %0 ], [ %9, %8 ]
  tail call void @init(i32* %bucket) #1
  %2 = icmp eq i32 %valid_buffer.02, 0
  br i1 %2, label %3, label %4

; <label>:3                                       ; preds = %1
  tail call void @hist(i32* %bucket, i32* %a, i32 %exp.01) #1
  br label %5

; <label>:4                                       ; preds = %1
  tail call void @hist(i32* %bucket, i32* %b, i32 %exp.01) #1
  br label %5

; <label>:5                                       ; preds = %4, %3
  tail call void @local_scan(i32* %bucket) #1
  tail call void @sum_scan(i32* %sum, i32* %bucket) #1
  tail call void @last_step_scan(i32* %bucket, i32* %sum) #1
  br i1 %2, label %6, label %7

; <label>:6                                       ; preds = %5
  tail call void @update(i32* %b, i32* %bucket, i32* %a, i32 %exp.01) #1
  br label %8

; <label>:7                                       ; preds = %5
  tail call void @update(i32* %a, i32* %bucket, i32* %b, i32 %exp.01) #1
  br label %8

; <label>:8                                       ; preds = %6, %7
  %valid_buffer.1 = phi i32 [ 1, %6 ], [ 0, %7 ]
  %9 = add nsw i32 %exp.01, 2
  %10 = icmp slt i32 %9, 32
  br i1 %10, label %1, label %11

; <label>:11                                      ; preds = %8
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
