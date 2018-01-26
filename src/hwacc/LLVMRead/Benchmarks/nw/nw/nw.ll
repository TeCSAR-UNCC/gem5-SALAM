; ModuleID = 'nw.c'
target datalayout = "e-p:64:64:64-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:64:64-f32:32:32-f64:64:64-v64:64:64-v128:128:128-a0:0:64-s0:64:64-f80:128:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: nounwind uwtable
define void @needwun(i8* nocapture readonly %SEQA, i8* nocapture readonly %SEQB, i8* nocapture %alignedA, i8* nocapture %alignedB, i32* nocapture %M, i8* nocapture %ptr) #0 {
  br label %1

; <label>:1                                       ; preds = %1, %0
  %indvars.iv58 = phi i64 [ 0, %0 ], [ %indvars.iv.next59, %1 ]
  %2 = sub i64 0, %indvars.iv58
  %3 = getelementptr inbounds i32* %M, i64 %indvars.iv58
  %4 = trunc i64 %2 to i32
  store i32 %4, i32* %3, align 4, !tbaa !1
  %indvars.iv.next59 = add nuw nsw i64 %indvars.iv58, 1
  %exitcond60 = icmp eq i64 %indvars.iv.next59, 129
  br i1 %exitcond60, label %.preheader22, label %1

.preheader22:                                     ; preds = %1, %.preheader22
  %indvars.iv55 = phi i64 [ %indvars.iv.next56, %.preheader22 ], [ 0, %1 ]
  %5 = sub i64 0, %indvars.iv55
  %6 = mul nsw i64 %indvars.iv55, 129
  %7 = getelementptr inbounds i32* %M, i64 %6
  %8 = trunc i64 %5 to i32
  store i32 %8, i32* %7, align 4, !tbaa !1
  %indvars.iv.next56 = add nuw nsw i64 %indvars.iv55, 1
  %exitcond57 = icmp eq i64 %indvars.iv.next56, 129
  br i1 %exitcond57, label %.preheader18, label %.preheader22

.preheader18:                                     ; preds = %.preheader22, %46
  %indvars.iv52 = phi i64 [ %indvars.iv.next53, %46 ], [ 1, %.preheader22 ]
  %9 = add nsw i64 %indvars.iv52, -1
  %10 = getelementptr inbounds i8* %SEQB, i64 %9
  %11 = mul nsw i64 %9, 129
  %12 = mul nsw i64 %indvars.iv52, 129
  br label %13

; <label>:13                                      ; preds = %45, %.preheader18
  %indvars.iv49 = phi i64 [ 1, %.preheader18 ], [ %indvars.iv.next50, %45 ]
  %14 = add nsw i64 %indvars.iv49, -1
  %15 = getelementptr inbounds i8* %SEQA, i64 %14
  %16 = load i8* %15, align 1, !tbaa !5
  %17 = load i8* %10, align 1, !tbaa !5
  %18 = icmp eq i8 %16, %17
  %. = select i1 %18, i32 1, i32 -1
  %19 = add nsw i64 %14, %11
  %20 = getelementptr inbounds i32* %M, i64 %19
  %21 = load i32* %20, align 4, !tbaa !1
  %22 = add nsw i32 %., %21
  %23 = add nsw i64 %indvars.iv49, %11
  %24 = getelementptr inbounds i32* %M, i64 %23
  %25 = load i32* %24, align 4, !tbaa !1
  %26 = add nsw i32 %25, -1
  %27 = add nsw i64 %14, %12
  %28 = getelementptr inbounds i32* %M, i64 %27
  %29 = load i32* %28, align 4, !tbaa !1
  %30 = add nsw i32 %29, -1
  %31 = icmp sgt i32 %26, %30
  %32 = select i1 %31, i32 %26, i32 %30
  %33 = icmp sgt i32 %22, %32
  %34 = select i1 %33, i32 %22, i32 %32
  %35 = add nsw i64 %indvars.iv49, %12
  %36 = getelementptr inbounds i32* %M, i64 %35
  store i32 %34, i32* %36, align 4, !tbaa !1
  %37 = icmp eq i32 %34, %30
  br i1 %37, label %38, label %40

; <label>:38                                      ; preds = %13
  %39 = getelementptr inbounds i8* %ptr, i64 %35
  store i8 60, i8* %39, align 1, !tbaa !5
  br label %45

; <label>:40                                      ; preds = %13
  %41 = icmp eq i32 %34, %26
  %42 = getelementptr inbounds i8* %ptr, i64 %35
  br i1 %41, label %43, label %44

; <label>:43                                      ; preds = %40
  store i8 94, i8* %42, align 1, !tbaa !5
  br label %45

; <label>:44                                      ; preds = %40
  store i8 92, i8* %42, align 1, !tbaa !5
  br label %45

; <label>:45                                      ; preds = %38, %44, %43
  %indvars.iv.next50 = add nuw nsw i64 %indvars.iv49, 1
  %exitcond51 = icmp eq i64 %indvars.iv.next50, 129
  br i1 %exitcond51, label %46, label %13

; <label>:46                                      ; preds = %45
  %indvars.iv.next53 = add nuw nsw i64 %indvars.iv52, 1
  %exitcond54 = icmp eq i64 %indvars.iv.next53, 129
  br i1 %exitcond54, label %.critedge.lr.ph, label %.preheader18

.preheader3:                                      ; preds = %.outer.backedge, %83
  %b_str_idx.0.lcssa = phi i32 [ %89, %83 ], [ %b_str_idx.0.ph.be, %.outer.backedge ]
  %a_str_idx.0.lcssa = phi i32 [ %84, %83 ], [ %a_str_idx.0.ph.be, %.outer.backedge ]
  %47 = icmp slt i32 %a_str_idx.0.lcssa, 256
  br i1 %47, label %.lr.ph5, label %.preheader

.lr.ph5:                                          ; preds = %.preheader3
  %48 = sext i32 %a_str_idx.0.lcssa to i64
  br label %95

.critedge:                                        ; preds = %.critedge.lr.ph, %83
  %indvars.iv47 = phi i64 [ %73, %.critedge.lr.ph ], [ %indvars.iv.next48, %83 ]
  %indvars.iv45 = phi i64 [ %72, %.critedge.lr.ph ], [ %indvars.iv.next46, %83 ]
  %indvars.iv43 = phi i64 [ %71, %.critedge.lr.ph ], [ %indvars.iv.next44, %83 ]
  %b_str_idx.016 = phi i32 [ %b_str_idx.0.ph30, %.critedge.lr.ph ], [ %89, %83 ]
  %a_str_idx.015 = phi i32 [ %a_str_idx.0.ph29, %.critedge.lr.ph ], [ %84, %83 ]
  %b_idx.214 = phi i32 [ %b_idx.2.ph28, %.critedge.lr.ph ], [ %86, %83 ]
  %49 = trunc i64 %indvars.iv47 to i32
  %50 = mul nsw i32 %49, 129
  %51 = add nsw i32 %50, %a_idx.2.ph27
  %52 = sext i32 %51 to i64
  %53 = getelementptr inbounds i8* %ptr, i64 %52
  %54 = load i8* %53, align 1, !tbaa !5
  switch i8 %54, label %83 [
    i8 92, label %55
    i8 60, label %74
  ]

; <label>:55                                      ; preds = %.critedge
  %56 = add nsw i32 %a_idx.2.ph27, -1
  %57 = sext i32 %56 to i64
  %58 = getelementptr inbounds i8* %SEQA, i64 %57
  %59 = load i8* %58, align 1, !tbaa !5
  %60 = sext i32 %a_str_idx.015 to i64
  %61 = getelementptr inbounds i8* %alignedA, i64 %60
  store i8 %59, i8* %61, align 1, !tbaa !5
  %62 = add nsw i32 %b_idx.214, -1
  %63 = sext i32 %62 to i64
  %64 = getelementptr inbounds i8* %SEQB, i64 %63
  %65 = load i8* %64, align 1, !tbaa !5
  %66 = sext i32 %b_str_idx.016 to i64
  %67 = getelementptr inbounds i8* %alignedB, i64 %66
  store i8 %65, i8* %67, align 1, !tbaa !5
  br label %.outer.backedge

.outer.backedge:                                  ; preds = %55, %74
  %a_idx.2.ph.be = phi i32 [ %56, %55 ], [ %75, %74 ]
  %b_idx.2.ph.be = phi i32 [ %62, %55 ], [ %b_idx.214, %74 ]
  %b_str_idx.0.ph.be = add nsw i32 %b_str_idx.016, 1
  %a_str_idx.0.ph.be = add nsw i32 %a_str_idx.015, 1
  %68 = icmp sgt i32 %a_idx.2.ph.be, 0
  %69 = icmp sgt i32 %b_idx.2.ph.be, 0
  %or.cond13 = or i1 %68, %69
  br i1 %or.cond13, label %.critedge.lr.ph, label %.preheader3

.critedge.lr.ph:                                  ; preds = %46, %.outer.backedge
  %70 = phi i1 [ %68, %.outer.backedge ], [ true, %46 ]
  %b_str_idx.0.ph30 = phi i32 [ %b_str_idx.0.ph.be, %.outer.backedge ], [ 0, %46 ]
  %a_str_idx.0.ph29 = phi i32 [ %a_str_idx.0.ph.be, %.outer.backedge ], [ 0, %46 ]
  %b_idx.2.ph28 = phi i32 [ %b_idx.2.ph.be, %.outer.backedge ], [ 128, %46 ]
  %a_idx.2.ph27 = phi i32 [ %a_idx.2.ph.be, %.outer.backedge ], [ 128, %46 ]
  %71 = sext i32 %b_str_idx.0.ph30 to i64
  %72 = sext i32 %a_str_idx.0.ph29 to i64
  %73 = sext i32 %b_idx.2.ph28 to i64
  br label %.critedge

; <label>:74                                      ; preds = %.critedge
  %75 = add nsw i32 %a_idx.2.ph27, -1
  %76 = sext i32 %75 to i64
  %77 = getelementptr inbounds i8* %SEQA, i64 %76
  %78 = load i8* %77, align 1, !tbaa !5
  %79 = sext i32 %a_str_idx.015 to i64
  %80 = getelementptr inbounds i8* %alignedA, i64 %79
  store i8 %78, i8* %80, align 1, !tbaa !5
  %81 = sext i32 %b_str_idx.016 to i64
  %82 = getelementptr inbounds i8* %alignedB, i64 %81
  store i8 45, i8* %82, align 1, !tbaa !5
  br label %.outer.backedge

; <label>:83                                      ; preds = %.critedge
  %indvars.iv.next46 = add nsw i64 %indvars.iv45, 1
  %84 = add nsw i32 %a_str_idx.015, 1
  %85 = getelementptr inbounds i8* %alignedA, i64 %indvars.iv45
  store i8 45, i8* %85, align 1, !tbaa !5
  %indvars.iv.next48 = add nsw i64 %indvars.iv47, -1
  %86 = add nsw i32 %b_idx.214, -1
  %87 = getelementptr inbounds i8* %SEQB, i64 %indvars.iv.next48
  %88 = load i8* %87, align 1, !tbaa !5
  %indvars.iv.next44 = add nsw i64 %indvars.iv43, 1
  %89 = add nsw i32 %b_str_idx.016, 1
  %90 = getelementptr inbounds i8* %alignedB, i64 %indvars.iv43
  store i8 %88, i8* %90, align 1, !tbaa !5
  %91 = trunc i64 %indvars.iv.next48 to i32
  %92 = icmp sgt i32 %91, 0
  %or.cond = or i1 %70, %92
  br i1 %or.cond, label %.critedge, label %.preheader3

.preheader:                                       ; preds = %95, %.preheader3
  %93 = icmp slt i32 %b_str_idx.0.lcssa, 256
  br i1 %93, label %.lr.ph, label %._crit_edge

.lr.ph:                                           ; preds = %.preheader
  %94 = sext i32 %b_str_idx.0.lcssa to i64
  br label %98

; <label>:95                                      ; preds = %95, %.lr.ph5
  %indvars.iv31 = phi i64 [ %48, %.lr.ph5 ], [ %indvars.iv.next32, %95 ]
  %a_str_idx.14 = phi i32 [ %a_str_idx.0.lcssa, %.lr.ph5 ], [ %97, %95 ]
  %96 = getelementptr inbounds i8* %alignedA, i64 %indvars.iv31
  store i8 95, i8* %96, align 1, !tbaa !5
  %indvars.iv.next32 = add nsw i64 %indvars.iv31, 1
  %97 = add nsw i32 %a_str_idx.14, 1
  %exitcond33 = icmp eq i32 %97, 256
  br i1 %exitcond33, label %.preheader, label %95

; <label>:98                                      ; preds = %98, %.lr.ph
  %indvars.iv = phi i64 [ %94, %.lr.ph ], [ %indvars.iv.next, %98 ]
  %b_str_idx.12 = phi i32 [ %b_str_idx.0.lcssa, %.lr.ph ], [ %100, %98 ]
  %99 = getelementptr inbounds i8* %alignedB, i64 %indvars.iv
  store i8 95, i8* %99, align 1, !tbaa !5
  %indvars.iv.next = add nsw i64 %indvars.iv, 1
  %100 = add nsw i32 %b_str_idx.12, 1
  %exitcond = icmp eq i32 %100, 256
  br i1 %exitcond, label %._crit_edge, label %98

._crit_edge:                                      ; preds = %98, %.preheader
  ret void
}

attributes #0 = { nounwind uwtable "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.ident = !{!0}

!0 = metadata !{metadata !"clang version 3.4 (tags/RELEASE_34/final)"}
!1 = metadata !{metadata !2, metadata !2, i64 0}
!2 = metadata !{metadata !"int", metadata !3, i64 0}
!3 = metadata !{metadata !"omnipotent char", metadata !4, i64 0}
!4 = metadata !{metadata !"Simple C/C++ TBAA"}
!5 = metadata !{metadata !3, metadata !3, i64 0}
