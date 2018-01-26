; ModuleID = 'kmp.c'
target datalayout = "e-p:64:64:64-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:64:64-f32:32:32-f64:64:64-v64:64:64-v128:128:128-a0:0:64-s0:64:64-f80:128:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: nounwind uwtable
define void @CPF(i8* nocapture readonly %pattern, i32* nocapture %kmpNext) #0 {
  store i32 0, i32* %kmpNext, align 4, !tbaa !1
  br label %.preheader

.preheader:                                       ; preds = %.critedge, %0
  %indvars.iv = phi i64 [ 1, %0 ], [ %indvars.iv.next, %.critedge ]
  %k.02 = phi i32 [ 0, %0 ], [ %.k.1, %.critedge ]
  %1 = icmp sgt i32 %k.02, 0
  br i1 %1, label %.lr.ph, label %.critedge

.lr.ph:                                           ; preds = %.preheader
  %2 = getelementptr inbounds i8* %pattern, i64 %indvars.iv
  %3 = load i8* %2, align 1, !tbaa !5
  %4 = getelementptr inbounds i32* %kmpNext, i64 %indvars.iv
  br label %5

; <label>:5                                       ; preds = %.lr.ph, %10
  %k.11 = phi i32 [ %k.02, %.lr.ph ], [ %11, %10 ]
  %6 = sext i32 %k.11 to i64
  %7 = getelementptr inbounds i8* %pattern, i64 %6
  %8 = load i8* %7, align 1, !tbaa !5
  %9 = icmp eq i8 %8, %3
  br i1 %9, label %.critedge, label %10

; <label>:10                                      ; preds = %5
  %11 = load i32* %4, align 4, !tbaa !1
  %12 = icmp sgt i32 %11, 0
  br i1 %12, label %5, label %.critedge

.critedge:                                        ; preds = %10, %5, %.preheader
  %k.1.lcssa = phi i32 [ %k.02, %.preheader ], [ %k.11, %5 ], [ %11, %10 ]
  %13 = sext i32 %k.1.lcssa to i64
  %14 = getelementptr inbounds i8* %pattern, i64 %13
  %15 = load i8* %14, align 1, !tbaa !5
  %16 = getelementptr inbounds i8* %pattern, i64 %indvars.iv
  %17 = load i8* %16, align 1, !tbaa !5
  %18 = icmp eq i8 %15, %17
  %19 = zext i1 %18 to i32
  %.k.1 = add nsw i32 %19, %k.1.lcssa
  %20 = getelementptr inbounds i32* %kmpNext, i64 %indvars.iv
  store i32 %.k.1, i32* %20, align 4, !tbaa !1
  %indvars.iv.next = add nuw nsw i64 %indvars.iv, 1
  %exitcond = icmp eq i64 %indvars.iv.next, 4
  br i1 %exitcond, label %21, label %.preheader

; <label>:21                                      ; preds = %.critedge
  ret void
}

; Function Attrs: nounwind uwtable
define i32 @kmp(i8* nocapture readonly %pattern, i8* nocapture readonly %input, i32* nocapture %kmpNext, i32* nocapture %n_matches) #0 {
  store i32 0, i32* %n_matches, align 4, !tbaa !1
  tail call void @CPF(i8* %pattern, i32* %kmpNext) #1
  br label %.preheader

.preheader:                                       ; preds = %31, %0
  %indvars.iv = phi i64 [ 0, %0 ], [ %indvars.iv.next, %31 ]
  %q.04 = phi i32 [ 0, %0 ], [ %q.3, %31 ]
  %1 = icmp sgt i32 %q.04, 0
  %2 = sext i32 %q.04 to i64
  br i1 %1, label %.lr.ph, label %.thread

.lr.ph:                                           ; preds = %.preheader
  %3 = getelementptr inbounds i8* %input, i64 %indvars.iv
  %4 = load i8* %3, align 1, !tbaa !5
  br label %5

; <label>:5                                       ; preds = %.lr.ph, %11
  %6 = phi i64 [ %2, %.lr.ph ], [ %15, %11 ]
  %q.11 = phi i32 [ %q.04, %.lr.ph ], [ %13, %11 ]
  %7 = getelementptr inbounds i8* %pattern, i64 %6
  %8 = load i8* %7, align 1, !tbaa !5
  %9 = icmp eq i8 %8, %4
  %10 = sext i32 %q.11 to i64
  br i1 %9, label %.thread, label %11

; <label>:11                                      ; preds = %5
  %12 = getelementptr inbounds i32* %kmpNext, i64 %10
  %13 = load i32* %12, align 4, !tbaa !1
  %14 = icmp sgt i32 %13, 0
  %15 = sext i32 %13 to i64
  br i1 %14, label %5, label %.thread

.thread:                                          ; preds = %11, %5, %.preheader
  %q.1.lcssa = phi i32 [ %q.04, %.preheader ], [ %q.11, %5 ], [ %13, %11 ]
  %16 = phi i64 [ %2, %.preheader ], [ %10, %5 ], [ %15, %11 ]
  %17 = getelementptr inbounds i8* %pattern, i64 %16
  %18 = load i8* %17, align 1, !tbaa !5
  %19 = getelementptr inbounds i8* %input, i64 %indvars.iv
  %20 = load i8* %19, align 1, !tbaa !5
  %21 = icmp eq i8 %18, %20
  %22 = zext i1 %21 to i32
  %.q.1 = add nsw i32 %22, %q.1.lcssa
  %23 = icmp sgt i32 %.q.1, 3
  br i1 %23, label %24, label %31

; <label>:24                                      ; preds = %.thread
  %25 = load i32* %n_matches, align 4, !tbaa !1
  %26 = add nsw i32 %25, 1
  store i32 %26, i32* %n_matches, align 4, !tbaa !1
  %27 = add nsw i32 %.q.1, -1
  %28 = sext i32 %27 to i64
  %29 = getelementptr inbounds i32* %kmpNext, i64 %28
  %30 = load i32* %29, align 4, !tbaa !1
  br label %31

; <label>:31                                      ; preds = %.thread, %24
  %q.3 = phi i32 [ %30, %24 ], [ %.q.1, %.thread ]
  %indvars.iv.next = add nuw nsw i64 %indvars.iv, 1
  %exitcond = icmp eq i64 %indvars.iv.next, 32411
  br i1 %exitcond, label %32, label %.preheader

; <label>:32                                      ; preds = %31
  ret i32 0
}

attributes #0 = { nounwind uwtable "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nobuiltin }

!llvm.ident = !{!0}

!0 = metadata !{metadata !"clang version 3.4 (tags/RELEASE_34/final)"}
!1 = metadata !{metadata !2, metadata !2, i64 0}
!2 = metadata !{metadata !"int", metadata !3, i64 0}
!3 = metadata !{metadata !"omnipotent char", metadata !4, i64 0}
!4 = metadata !{metadata !"Simple C/C++ TBAA"}
!5 = metadata !{metadata !3, metadata !3, i64 0}
