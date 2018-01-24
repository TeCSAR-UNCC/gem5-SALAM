; ModuleID = 'bfs.c'
target datalayout = "e-p:64:64:64-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:64:64-f32:32:32-f64:64:64-v64:64:64-v128:128:128-a0:0:64-s0:64:64-f80:128:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%struct.node_t_struct = type { i64, i64 }
%struct.edge_t_struct = type { i64 }

; Function Attrs: nounwind uwtable
define void @bfs(%struct.node_t_struct* nocapture readonly %nodes, %struct.edge_t_struct* nocapture readonly %edges, i64 %starting_node, i8* nocapture %level, i64* nocapture %level_counts) #0 {
  %1 = getelementptr inbounds i8* %level, i64 %starting_node
  store i8 0, i8* %1, align 1, !tbaa !1
  store i64 1, i64* %level_counts, align 8, !tbaa !4
  br label %.preheader

.preheader:                                       ; preds = %24, %0
  %indvars.iv = phi i64 [ 0, %0 ], [ %indvars.iv.next, %24 ]
  %indvars.iv.next = add nuw nsw i64 %indvars.iv, 1
  br label %2

; <label>:2                                       ; preds = %.loopexit, %.preheader
  %n.04 = phi i64 [ 0, %.preheader ], [ %23, %.loopexit ]
  %cnt.03 = phi i64 [ 0, %.preheader ], [ %cnt.3, %.loopexit ]
  %3 = getelementptr inbounds i8* %level, i64 %n.04
  %4 = load i8* %3, align 1, !tbaa !1
  %5 = trunc i64 %indvars.iv to i8
  %6 = icmp eq i8 %4, %5
  br i1 %6, label %7, label %.loopexit

; <label>:7                                       ; preds = %2
  %8 = getelementptr inbounds %struct.node_t_struct* %nodes, i64 %n.04, i32 0
  %9 = load i64* %8, align 8, !tbaa !6
  %10 = getelementptr inbounds %struct.node_t_struct* %nodes, i64 %n.04, i32 1
  %11 = load i64* %10, align 8, !tbaa !8
  %12 = icmp ult i64 %9, %11
  br i1 %12, label %.lr.ph, label %.loopexit

.lr.ph:                                           ; preds = %7, %21
  %e.02 = phi i64 [ %22, %21 ], [ %9, %7 ]
  %cnt.11 = phi i64 [ %cnt.2, %21 ], [ %cnt.03, %7 ]
  %13 = getelementptr inbounds %struct.edge_t_struct* %edges, i64 %e.02, i32 0
  %14 = load i64* %13, align 8, !tbaa !9
  %15 = getelementptr inbounds i8* %level, i64 %14
  %16 = load i8* %15, align 1, !tbaa !1
  %17 = icmp eq i8 %16, 127
  br i1 %17, label %18, label %21

; <label>:18                                      ; preds = %.lr.ph
  %19 = trunc i64 %indvars.iv.next to i8
  store i8 %19, i8* %15, align 1, !tbaa !1
  %20 = add i64 %cnt.11, 1
  br label %21

; <label>:21                                      ; preds = %.lr.ph, %18
  %cnt.2 = phi i64 [ %20, %18 ], [ %cnt.11, %.lr.ph ]
  %22 = add i64 %e.02, 1
  %exitcond = icmp eq i64 %22, %11
  br i1 %exitcond, label %.loopexit, label %.lr.ph

.loopexit:                                        ; preds = %7, %21, %2
  %cnt.3 = phi i64 [ %cnt.03, %2 ], [ %cnt.03, %7 ], [ %cnt.2, %21 ]
  %23 = add i64 %n.04, 1
  %exitcond6 = icmp eq i64 %23, 256
  br i1 %exitcond6, label %24, label %2

; <label>:24                                      ; preds = %.loopexit
  %25 = add i64 %indvars.iv, 1
  %26 = getelementptr inbounds i64* %level_counts, i64 %25
  store i64 %cnt.3, i64* %26, align 8, !tbaa !4
  %27 = icmp ne i64 %cnt.3, 0
  %28 = trunc i64 %indvars.iv.next to i8
  %29 = icmp slt i8 %28, 10
  %or.cond = and i1 %27, %29
  br i1 %or.cond, label %.preheader, label %30

; <label>:30                                      ; preds = %24
  ret void
}

attributes #0 = { nounwind uwtable "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.ident = !{!0}

!0 = metadata !{metadata !"clang version 3.4 (tags/RELEASE_34/final)"}
!1 = metadata !{metadata !2, metadata !2, i64 0}
!2 = metadata !{metadata !"omnipotent char", metadata !3, i64 0}
!3 = metadata !{metadata !"Simple C/C++ TBAA"}
!4 = metadata !{metadata !5, metadata !5, i64 0}
!5 = metadata !{metadata !"long", metadata !2, i64 0}
!6 = metadata !{metadata !7, metadata !5, i64 0}
!7 = metadata !{metadata !"node_t_struct", metadata !5, i64 0, metadata !5, i64 8}
!8 = metadata !{metadata !7, metadata !5, i64 8}
!9 = metadata !{metadata !10, metadata !5, i64 0}
!10 = metadata !{metadata !"edge_t_struct", metadata !5, i64 0}
