; ModuleID = 'bfs.c'
target datalayout = "e-p:64:64:64-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:64:64-f32:32:32-f64:64:64-v64:64:64-v128:128:128-a0:0:64-s0:64:64-f80:128:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%struct.node_t_struct = type { i64, i64 }
%struct.edge_t_struct = type { i64 }

; Function Attrs: nounwind uwtable
define void @bfs(%struct.node_t_struct* nocapture readonly %nodes, %struct.edge_t_struct* nocapture readonly %edges, i64 %starting_node, i8* nocapture %level, i64* nocapture %level_counts) #0 {
  %queue = alloca [256 x i64], align 16
  %1 = bitcast [256 x i64]* %queue to i8*
  call void @llvm.lifetime.start(i64 2048, i8* %1) #1
  br label %2

; <label>:2                                       ; preds = %2, %0
  %indvars.iv = phi i64 [ 0, %0 ], [ %indvars.iv.next, %2 ]
  %3 = getelementptr inbounds i64* %level_counts, i64 %indvars.iv
  store i64 0, i64* %3, align 8, !tbaa !1
  %indvars.iv.next = add nuw nsw i64 %indvars.iv, 1
  %exitcond9 = icmp eq i64 %indvars.iv.next, 10
  br i1 %exitcond9, label %4, label %2

; <label>:4                                       ; preds = %2
  %5 = getelementptr inbounds i8* %level, i64 %starting_node
  store i8 0, i8* %5, align 1, !tbaa !5
  store i64 1, i64* %level_counts, align 8, !tbaa !1
  %6 = getelementptr inbounds [256 x i64]* %queue, i64 0, i64 0
  store i64 %starting_node, i64* %6, align 16, !tbaa !1
  br label %7

; <label>:7                                       ; preds = %4, %._crit_edge
  %q_in.07 = phi i64 [ 2, %4 ], [ %q_in.1.lcssa, %._crit_edge ]
  %q_out.06 = phi i64 [ 0, %4 ], [ %19, %._crit_edge ]
  %dummy.05 = phi i64 [ 0, %4 ], [ %46, %._crit_edge ]
  %8 = icmp ugt i64 %q_in.07, %q_out.06
  br i1 %8, label %9, label %12

; <label>:9                                       ; preds = %7
  %10 = add i64 %q_out.06, 1
  %11 = icmp eq i64 %q_in.07, %10
  br i1 %11, label %48, label %15

; <label>:12                                      ; preds = %7
  %13 = icmp eq i64 %q_in.07, 0
  %14 = icmp eq i64 %q_out.06, 255
  %or.cond = and i1 %13, %14
  br i1 %or.cond, label %48, label %15

; <label>:15                                      ; preds = %12, %9
  %16 = getelementptr inbounds [256 x i64]* %queue, i64 0, i64 %q_out.06
  %17 = load i64* %16, align 8, !tbaa !1
  %18 = add i64 %q_out.06, 1
  %19 = and i64 %18, 255
  %20 = getelementptr inbounds %struct.node_t_struct* %nodes, i64 %17, i32 0
  %21 = load i64* %20, align 8, !tbaa !6
  %22 = getelementptr inbounds %struct.node_t_struct* %nodes, i64 %17, i32 1
  %23 = load i64* %22, align 8, !tbaa !8
  %24 = icmp ult i64 %21, %23
  br i1 %24, label %.lr.ph, label %._crit_edge

.lr.ph:                                           ; preds = %15
  %25 = getelementptr inbounds i8* %level, i64 %17
  br label %26

; <label>:26                                      ; preds = %44, %.lr.ph
  %q_in.14 = phi i64 [ %q_in.07, %.lr.ph ], [ %q_in.2, %44 ]
  %e.03 = phi i64 [ %21, %.lr.ph ], [ %45, %44 ]
  %27 = getelementptr inbounds %struct.edge_t_struct* %edges, i64 %e.03, i32 0
  %28 = load i64* %27, align 8, !tbaa !9
  %29 = getelementptr inbounds i8* %level, i64 %28
  %30 = load i8* %29, align 1, !tbaa !5
  %31 = icmp eq i8 %30, 127
  br i1 %31, label %32, label %44

; <label>:32                                      ; preds = %26
  %33 = load i8* %25, align 1, !tbaa !5
  %34 = add i8 %33, 1
  store i8 %34, i8* %29, align 1, !tbaa !5
  %35 = sext i8 %34 to i64
  %36 = getelementptr inbounds i64* %level_counts, i64 %35
  %37 = load i64* %36, align 8, !tbaa !1
  %38 = add i64 %37, 1
  store i64 %38, i64* %36, align 8, !tbaa !1
  %39 = icmp eq i64 %q_in.14, 0
  %40 = add i64 %q_in.14, -1
  %. = select i1 %39, i64 255, i64 %40
  %41 = getelementptr inbounds [256 x i64]* %queue, i64 0, i64 %.
  store i64 %28, i64* %41, align 8, !tbaa !1
  %42 = add i64 %q_in.14, 1
  %43 = and i64 %42, 255
  br label %44

; <label>:44                                      ; preds = %26, %32
  %q_in.2 = phi i64 [ %43, %32 ], [ %q_in.14, %26 ]
  %45 = add i64 %e.03, 1
  %exitcond = icmp eq i64 %45, %23
  br i1 %exitcond, label %._crit_edge, label %26

._crit_edge:                                      ; preds = %44, %15
  %q_in.1.lcssa = phi i64 [ %q_in.07, %15 ], [ %q_in.2, %44 ]
  %46 = add i64 %dummy.05, 1
  %47 = icmp ult i64 %46, 256
  br i1 %47, label %7, label %48

; <label>:48                                      ; preds = %9, %12, %._crit_edge
  call void @llvm.lifetime.end(i64 2048, i8* %1) #1
  ret void
}

; Function Attrs: nounwind
declare void @llvm.lifetime.start(i64, i8* nocapture) #1

; Function Attrs: nounwind
declare void @llvm.lifetime.end(i64, i8* nocapture) #1

attributes #0 = { nounwind uwtable "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nounwind }

!llvm.ident = !{!0}

!0 = metadata !{metadata !"clang version 3.4 (tags/RELEASE_34/final)"}
!1 = metadata !{metadata !2, metadata !2, i64 0}
!2 = metadata !{metadata !"long", metadata !3, i64 0}
!3 = metadata !{metadata !"omnipotent char", metadata !4, i64 0}
!4 = metadata !{metadata !"Simple C/C++ TBAA"}
!5 = metadata !{metadata !3, metadata !3, i64 0}
!6 = metadata !{metadata !7, metadata !2, i64 0}
!7 = metadata !{metadata !"node_t_struct", metadata !2, i64 0, metadata !2, i64 8}
!8 = metadata !{metadata !7, metadata !2, i64 8}
!9 = metadata !{metadata !10, metadata !2, i64 0}
!10 = metadata !{metadata !"edge_t_struct", metadata !2, i64 0}
