; ModuleID = 'md.c'
target datalayout = "e-p:64:64:64-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:64:64-f32:32:32-f64:64:64-v64:64:64-v128:128:128-a0:0:64-s0:64:64-f80:128:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%struct.dvector_t = type { double, double, double }

; Function Attrs: nounwind uwtable
define void @md([4 x [4 x i32]]* nocapture readonly %n_points, [4 x [4 x [10 x %struct.dvector_t]]]* nocapture %force, [4 x [4 x [10 x %struct.dvector_t]]]* nocapture readonly %position) #0 {
  br label %.preheader30

.preheader30:                                     ; preds = %103, %0
  %indvars.iv67 = phi i64 [ 0, %0 ], [ %indvars.iv.next68, %103 ]
  %indvars.iv57 = phi i32 [ -3, %0 ], [ %indvars.iv.next58, %103 ]
  %indvars.iv52 = phi i32 [ 0, %0 ], [ %indvars.iv.next53, %103 ]
  %1 = icmp sgt i32 %indvars.iv52, 1
  %indvars.iv52.op = add i32 %indvars.iv52, -1
  %2 = zext i32 %indvars.iv52.op to i64
  %3 = select i1 %1, i64 %2, i64 0
  %4 = icmp sgt i32 %indvars.iv57, -5
  %indvars.iv57.op = xor i32 %indvars.iv57, -1
  %5 = select i1 %4, i32 %indvars.iv57.op, i32 4
  %6 = trunc i64 %indvars.iv67 to i32
  %7 = icmp slt i32 %6, 1
  %8 = add nsw i64 %indvars.iv67, 4294967295
  %9 = trunc i64 %8 to i32
  %. = select i1 %7, i32 0, i32 %9
  %10 = add nsw i64 %indvars.iv67, 2
  %11 = trunc i64 %10 to i32
  %12 = icmp sgt i32 %11, 4
  %13 = trunc i64 %10 to i32
  %.1 = select i1 %12, i32 4, i32 %13
  %14 = icmp slt i32 %., %.1
  br label %.preheader

.preheader:                                       ; preds = %102, %.preheader30
  %indvars.iv64 = phi i64 [ 0, %.preheader30 ], [ %indvars.iv.next65, %102 ]
  %indvars.iv48 = phi i32 [ -3, %.preheader30 ], [ %indvars.iv.next49, %102 ]
  %indvars.iv43 = phi i32 [ 0, %.preheader30 ], [ %indvars.iv.next44, %102 ]
  %15 = icmp sgt i32 %indvars.iv43, 1
  %indvars.iv43.op = add i32 %indvars.iv43, -1
  %16 = zext i32 %indvars.iv43.op to i64
  %17 = select i1 %15, i64 %16, i64 0
  %18 = icmp sgt i32 %indvars.iv48, -5
  %indvars.iv48.op = xor i32 %indvars.iv48, -1
  %19 = select i1 %18, i32 %indvars.iv48.op, i32 4
  %20 = trunc i64 %indvars.iv64 to i32
  %21 = icmp slt i32 %20, 1
  %22 = add nsw i64 %indvars.iv64, 4294967295
  %23 = trunc i64 %22 to i32
  %.2 = select i1 %21, i32 0, i32 %23
  %24 = add nsw i64 %indvars.iv64, 2
  %25 = trunc i64 %24 to i32
  %26 = icmp sgt i32 %25, 4
  %27 = trunc i64 %24 to i32
  %.3 = select i1 %26, i32 4, i32 %27
  %28 = icmp slt i32 %.2, %.3
  br label %29

; <label>:29                                      ; preds = %._crit_edge28, %.preheader
  %indvars.iv61 = phi i64 [ 0, %.preheader ], [ %indvars.iv.next62, %._crit_edge28 ]
  %indvars.iv39 = phi i32 [ -3, %.preheader ], [ %indvars.iv.next40, %._crit_edge28 ]
  %indvars.iv35 = phi i32 [ 0, %.preheader ], [ %indvars.iv.next36, %._crit_edge28 ]
  %30 = icmp sgt i32 %indvars.iv35, 1
  %indvars.iv35.op = add i32 %indvars.iv35, -1
  %31 = zext i32 %indvars.iv35.op to i64
  %32 = select i1 %30, i64 %31, i64 0
  %33 = icmp sgt i32 %indvars.iv39, -5
  %indvars.iv39.op = xor i32 %indvars.iv39, -1
  %34 = select i1 %33, i32 %indvars.iv39.op, i32 4
  br i1 %14, label %.lr.ph27, label %._crit_edge28

.lr.ph27:                                         ; preds = %29
  %35 = trunc i64 %indvars.iv61 to i32
  %36 = icmp slt i32 %35, 1
  %37 = add nsw i64 %indvars.iv61, 4294967295
  %38 = trunc i64 %37 to i32
  %.4 = select i1 %36, i32 0, i32 %38
  %39 = add nsw i64 %indvars.iv61, 2
  %40 = trunc i64 %39 to i32
  %41 = icmp sgt i32 %40, 4
  %42 = trunc i64 %39 to i32
  %.5 = select i1 %41, i32 4, i32 %42
  %43 = icmp slt i32 %.4, %.5
  %44 = getelementptr inbounds [4 x [4 x i32]]* %n_points, i64 %indvars.iv67, i64 %indvars.iv64, i64 %indvars.iv61
  br label %45

; <label>:45                                      ; preds = %._crit_edge24, %.lr.ph27
  %indvars.iv55 = phi i64 [ %3, %.lr.ph27 ], [ %indvars.iv.next56, %._crit_edge24 ]
  %b1.sroa.0.025 = phi i32 [ %., %.lr.ph27 ], [ %101, %._crit_edge24 ]
  br i1 %28, label %.lr.ph23, label %._crit_edge24

.lr.ph23:                                         ; preds = %45, %._crit_edge20
  %indvars.iv46 = phi i64 [ %indvars.iv.next47, %._crit_edge20 ], [ %17, %45 ]
  %b1.sroa.6.021 = phi i32 [ %100, %._crit_edge20 ], [ %.2, %45 ]
  br i1 %43, label %.lr.ph19, label %._crit_edge20

.lr.ph19:                                         ; preds = %.lr.ph23, %._crit_edge16
  %indvars.iv37 = phi i64 [ %indvars.iv.next38, %._crit_edge16 ], [ %32, %.lr.ph23 ]
  %b1.sroa.12.017 = phi i32 [ %99, %._crit_edge16 ], [ %.4, %.lr.ph23 ]
  %46 = getelementptr inbounds [4 x [4 x i32]]* %n_points, i64 %indvars.iv55, i64 %indvars.iv46, i64 %indvars.iv37
  %47 = load i32* %46, align 4, !tbaa !1
  %48 = load i32* %44, align 4, !tbaa !1
  %49 = icmp sgt i32 %48, 0
  br i1 %49, label %.lr.ph15, label %._crit_edge16

.lr.ph15:                                         ; preds = %.lr.ph19
  %50 = icmp sgt i32 %47, 0
  br label %51

; <label>:51                                      ; preds = %.lr.ph15, %._crit_edge
  %indvars.iv33 = phi i64 [ 0, %.lr.ph15 ], [ %indvars.iv.next34, %._crit_edge ]
  %52 = getelementptr inbounds [4 x [4 x [10 x %struct.dvector_t]]]* %position, i64 %indvars.iv67, i64 %indvars.iv64, i64 %indvars.iv61, i64 %indvars.iv33, i32 0
  %53 = load double* %52, align 8
  %54 = getelementptr inbounds [4 x [4 x [10 x %struct.dvector_t]]]* %position, i64 %indvars.iv67, i64 %indvars.iv64, i64 %indvars.iv61, i64 %indvars.iv33, i32 1
  %55 = load double* %54, align 8
  %56 = getelementptr inbounds [4 x [4 x [10 x %struct.dvector_t]]]* %position, i64 %indvars.iv67, i64 %indvars.iv64, i64 %indvars.iv61, i64 %indvars.iv33, i32 2
  %57 = load double* %56, align 8
  %58 = getelementptr inbounds [4 x [4 x [10 x %struct.dvector_t]]]* %force, i64 %indvars.iv67, i64 %indvars.iv64, i64 %indvars.iv61, i64 %indvars.iv33, i32 0
  %59 = load double* %58, align 8, !tbaa !5
  %60 = getelementptr inbounds [4 x [4 x [10 x %struct.dvector_t]]]* %force, i64 %indvars.iv67, i64 %indvars.iv64, i64 %indvars.iv61, i64 %indvars.iv33, i32 1
  %61 = load double* %60, align 8, !tbaa !8
  %62 = getelementptr inbounds [4 x [4 x [10 x %struct.dvector_t]]]* %force, i64 %indvars.iv67, i64 %indvars.iv64, i64 %indvars.iv61, i64 %indvars.iv33, i32 2
  %63 = load double* %62, align 8, !tbaa !9
  br i1 %50, label %.lr.ph, label %._crit_edge

.lr.ph:                                           ; preds = %51, %95
  %indvars.iv = phi i64 [ %indvars.iv.next, %95 ], [ 0, %51 ]
  %sum_z.010 = phi double [ %sum_z.1, %95 ], [ %63, %51 ]
  %sum_y.09 = phi double [ %sum_y.1, %95 ], [ %61, %51 ]
  %sum_x.08 = phi double [ %sum_x.1, %95 ], [ %59, %51 ]
  %64 = getelementptr inbounds [4 x [4 x [10 x %struct.dvector_t]]]* %position, i64 %indvars.iv55, i64 %indvars.iv46, i64 %indvars.iv37, i64 %indvars.iv, i32 0
  %65 = load double* %64, align 8
  %66 = getelementptr inbounds [4 x [4 x [10 x %struct.dvector_t]]]* %position, i64 %indvars.iv55, i64 %indvars.iv46, i64 %indvars.iv37, i64 %indvars.iv, i32 1
  %67 = load double* %66, align 8
  %68 = getelementptr inbounds [4 x [4 x [10 x %struct.dvector_t]]]* %position, i64 %indvars.iv55, i64 %indvars.iv46, i64 %indvars.iv37, i64 %indvars.iv, i32 2
  %69 = load double* %68, align 8
  %70 = fcmp une double %65, %53
  %71 = fcmp une double %67, %55
  %or.cond = or i1 %70, %71
  %72 = fcmp une double %69, %57
  %or.cond6 = or i1 %or.cond, %72
  br i1 %or.cond6, label %73, label %95

; <label>:73                                      ; preds = %.lr.ph
  %74 = fsub double %53, %65
  %75 = fsub double %55, %67
  %76 = fsub double %57, %69
  %77 = fmul double %74, %74
  %78 = fmul double %75, %75
  %79 = fadd double %77, %78
  %80 = fmul double %76, %76
  %81 = fadd double %79, %80
  %82 = fdiv double 1.000000e+00, %81
  %83 = fmul double %82, %82
  %84 = fmul double %82, %83
  %85 = fmul double %84, 1.500000e+00
  %86 = fadd double %85, -2.000000e+00
  %87 = fmul double %84, %86
  %88 = fmul double %82, %87
  %89 = fmul double %74, %88
  %90 = fadd double %sum_x.08, %89
  %91 = fmul double %75, %88
  %92 = fadd double %sum_y.09, %91
  %93 = fmul double %76, %88
  %94 = fadd double %sum_z.010, %93
  br label %95

; <label>:95                                      ; preds = %.lr.ph, %73
  %sum_x.1 = phi double [ %90, %73 ], [ %sum_x.08, %.lr.ph ]
  %sum_y.1 = phi double [ %92, %73 ], [ %sum_y.09, %.lr.ph ]
  %sum_z.1 = phi double [ %94, %73 ], [ %sum_z.010, %.lr.ph ]
  %indvars.iv.next = add nuw nsw i64 %indvars.iv, 1
  %lftr.wideiv = trunc i64 %indvars.iv.next to i32
  %exitcond = icmp eq i32 %lftr.wideiv, %47
  br i1 %exitcond, label %._crit_edge, label %.lr.ph

._crit_edge:                                      ; preds = %95, %51
  %sum_z.0.lcssa = phi double [ %63, %51 ], [ %sum_z.1, %95 ]
  %sum_y.0.lcssa = phi double [ %61, %51 ], [ %sum_y.1, %95 ]
  %sum_x.0.lcssa = phi double [ %59, %51 ], [ %sum_x.1, %95 ]
  store double %sum_x.0.lcssa, double* %58, align 8, !tbaa !5
  store double %sum_y.0.lcssa, double* %60, align 8, !tbaa !8
  store double %sum_z.0.lcssa, double* %62, align 8, !tbaa !9
  %indvars.iv.next34 = add nuw nsw i64 %indvars.iv33, 1
  %96 = load i32* %44, align 4, !tbaa !1
  %97 = trunc i64 %indvars.iv.next34 to i32
  %98 = icmp slt i32 %97, %96
  br i1 %98, label %51, label %._crit_edge16

._crit_edge16:                                    ; preds = %._crit_edge, %.lr.ph19
  %99 = add nsw i32 %b1.sroa.12.017, 1
  %indvars.iv.next38 = add nuw nsw i64 %indvars.iv37, 1
  %exitcond42 = icmp eq i32 %99, %34
  br i1 %exitcond42, label %._crit_edge20, label %.lr.ph19

._crit_edge20:                                    ; preds = %._crit_edge16, %.lr.ph23
  %100 = add nsw i32 %b1.sroa.6.021, 1
  %indvars.iv.next47 = add nuw nsw i64 %indvars.iv46, 1
  %exitcond51 = icmp eq i32 %100, %19
  br i1 %exitcond51, label %._crit_edge24, label %.lr.ph23

._crit_edge24:                                    ; preds = %._crit_edge20, %45
  %101 = add nsw i32 %b1.sroa.0.025, 1
  %indvars.iv.next56 = add nuw nsw i64 %indvars.iv55, 1
  %exitcond60 = icmp eq i32 %101, %5
  br i1 %exitcond60, label %._crit_edge28, label %45

._crit_edge28:                                    ; preds = %._crit_edge24, %29
  %indvars.iv.next62 = add nuw nsw i64 %indvars.iv61, 1
  %indvars.iv.next36 = add nuw nsw i32 %indvars.iv35, 1
  %indvars.iv.next40 = add i32 %indvars.iv39, -1
  %exitcond63 = icmp eq i64 %indvars.iv.next62, 4
  br i1 %exitcond63, label %102, label %29

; <label>:102                                     ; preds = %._crit_edge28
  %indvars.iv.next65 = add nuw nsw i64 %indvars.iv64, 1
  %indvars.iv.next44 = add nuw nsw i32 %indvars.iv43, 1
  %indvars.iv.next49 = add i32 %indvars.iv48, -1
  %exitcond66 = icmp eq i64 %indvars.iv.next65, 4
  br i1 %exitcond66, label %103, label %.preheader

; <label>:103                                     ; preds = %102
  %indvars.iv.next68 = add nuw nsw i64 %indvars.iv67, 1
  %indvars.iv.next53 = add nuw nsw i32 %indvars.iv52, 1
  %indvars.iv.next58 = add i32 %indvars.iv57, -1
  %exitcond69 = icmp eq i64 %indvars.iv.next68, 4
  br i1 %exitcond69, label %104, label %.preheader30

; <label>:104                                     ; preds = %103
  ret void
}

attributes #0 = { nounwind uwtable "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.ident = !{!0}

!0 = metadata !{metadata !"clang version 3.4 (tags/RELEASE_34/final)"}
!1 = metadata !{metadata !2, metadata !2, i64 0}
!2 = metadata !{metadata !"int", metadata !3, i64 0}
!3 = metadata !{metadata !"omnipotent char", metadata !4, i64 0}
!4 = metadata !{metadata !"Simple C/C++ TBAA"}
!5 = metadata !{metadata !6, metadata !7, i64 0}
!6 = metadata !{metadata !"", metadata !7, i64 0, metadata !7, i64 8, metadata !7, i64 16}
!7 = metadata !{metadata !"double", metadata !3, i64 0}
!8 = metadata !{metadata !6, metadata !7, i64 8}
!9 = metadata !{metadata !6, metadata !7, i64 16}
