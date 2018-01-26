; ModuleID = 'viterbi.c'
target datalayout = "e-p:64:64:64-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:64:64-f32:32:32-f64:64:64-v64:64:64-v128:128:128-a0:0:64-s0:64:64-f80:128:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: nounwind uwtable
define i32 @viterbi(i8* nocapture readonly %obs, double* nocapture readonly %init, double* nocapture readonly %transition, double* nocapture readonly %emission, i8* nocapture %path) #0 {
  %llike = alloca [140 x [64 x double]], align 16
  %1 = bitcast [140 x [64 x double]]* %llike to i8*
  call void @llvm.lifetime.start(i64 71680, i8* %1) #1
  %2 = load i8* %obs, align 1, !tbaa !1
  %3 = zext i8 %2 to i32
  br label %4

; <label>:4                                       ; preds = %4, %0
  %indvars.iv28 = phi i64 [ 0, %0 ], [ %indvars.iv.next29, %4 ]
  %5 = trunc i64 %indvars.iv28 to i32
  %6 = getelementptr inbounds double* %init, i64 %indvars.iv28
  %7 = load double* %6, align 8, !tbaa !4
  %8 = shl nuw nsw i32 %5, 6
  %9 = add nsw i32 %3, %8
  %10 = sext i32 %9 to i64
  %11 = getelementptr inbounds double* %emission, i64 %10
  %12 = load double* %11, align 8, !tbaa !4
  %13 = fadd double %7, %12
  %14 = getelementptr inbounds [140 x [64 x double]]* %llike, i64 0, i64 0, i64 %indvars.iv28
  store double %13, double* %14, align 8, !tbaa !4
  %indvars.iv.next29 = add nuw nsw i64 %indvars.iv28, 1
  %exitcond30 = icmp eq i64 %indvars.iv.next29, 64
  br i1 %exitcond30, label %.preheader, label %4

.preheader:                                       ; preds = %4, %49
  %indvars.iv25 = phi i64 [ %indvars.iv.next26, %49 ], [ 1, %4 ]
  %15 = add nsw i64 %indvars.iv25, -1
  %16 = getelementptr inbounds [140 x [64 x double]]* %llike, i64 0, i64 %15, i64 0
  %17 = getelementptr inbounds i8* %obs, i64 %indvars.iv25
  %18 = load i8* %17, align 1, !tbaa !1
  %19 = zext i8 %18 to i64
  %20 = zext i8 %18 to i64
  br label %21

; <label>:21                                      ; preds = %47, %.preheader
  %indvars.iv22 = phi i64 [ 0, %.preheader ], [ %indvars.iv.next23, %47 ]
  %22 = load double* %16, align 16, !tbaa !4
  %23 = getelementptr inbounds double* %transition, i64 %indvars.iv22
  %24 = load double* %23, align 8, !tbaa !4
  %25 = fadd double %22, %24
  %26 = shl nuw nsw i64 %indvars.iv22, 6
  %27 = add nsw i64 %19, %26
  %28 = getelementptr inbounds double* %emission, i64 %27
  %29 = load double* %28, align 8, !tbaa !4
  %30 = fadd double %25, %29
  %31 = add nsw i64 %20, %26
  %32 = getelementptr inbounds double* %emission, i64 %31
  %33 = load double* %32, align 8, !tbaa !4
  br label %34

; <label>:34                                      ; preds = %34, %21
  %indvars.iv19 = phi i64 [ 1, %21 ], [ %indvars.iv.next20, %34 ]
  %min_p.09 = phi double [ %30, %21 ], [ %min_p.1, %34 ]
  %35 = trunc i64 %indvars.iv19 to i32
  %36 = getelementptr inbounds [140 x [64 x double]]* %llike, i64 0, i64 %15, i64 %indvars.iv19
  %37 = load double* %36, align 8, !tbaa !4
  %38 = shl nuw nsw i32 %35, 6
  %39 = trunc i64 %indvars.iv22 to i32
  %40 = add nsw i32 %38, %39
  %41 = sext i32 %40 to i64
  %42 = getelementptr inbounds double* %transition, i64 %41
  %43 = load double* %42, align 8, !tbaa !4
  %44 = fadd double %37, %43
  %45 = fadd double %44, %33
  %46 = fcmp olt double %45, %min_p.09
  %min_p.1 = select i1 %46, double %45, double %min_p.09
  %indvars.iv.next20 = add nuw nsw i64 %indvars.iv19, 1
  %exitcond21 = icmp eq i64 %indvars.iv.next20, 64
  br i1 %exitcond21, label %47, label %34

; <label>:47                                      ; preds = %34
  %48 = getelementptr inbounds [140 x [64 x double]]* %llike, i64 0, i64 %indvars.iv25, i64 %indvars.iv22
  store double %min_p.1, double* %48, align 8, !tbaa !4
  %indvars.iv.next23 = add nuw nsw i64 %indvars.iv22, 1
  %exitcond24 = icmp eq i64 %indvars.iv.next23, 64
  br i1 %exitcond24, label %49, label %21

; <label>:49                                      ; preds = %47
  %indvars.iv.next26 = add nuw nsw i64 %indvars.iv25, 1
  %exitcond27 = icmp eq i64 %indvars.iv.next26, 140
  br i1 %exitcond27, label %50, label %.preheader

; <label>:50                                      ; preds = %49
  %51 = getelementptr inbounds [140 x [64 x double]]* %llike, i64 0, i64 139, i64 0
  %52 = load double* %51, align 16, !tbaa !4
  br label %53

; <label>:53                                      ; preds = %53, %50
  %indvars.iv16 = phi i64 [ 1, %50 ], [ %indvars.iv.next17, %53 ]
  %min_s.06 = phi i8 [ 0, %50 ], [ %min_s.1, %53 ]
  %min_p.25 = phi double [ %52, %50 ], [ %min_p.3, %53 ]
  %54 = getelementptr inbounds [140 x [64 x double]]* %llike, i64 0, i64 139, i64 %indvars.iv16
  %55 = load double* %54, align 8, !tbaa !4
  %56 = fcmp olt double %55, %min_p.25
  %min_p.3 = select i1 %56, double %55, double %min_p.25
  %57 = trunc i64 %indvars.iv16 to i8
  %min_s.1 = select i1 %56, i8 %57, i8 %min_s.06
  %indvars.iv.next17 = add nuw nsw i64 %indvars.iv16, 1
  %exitcond18 = icmp eq i64 %indvars.iv.next17, 64
  br i1 %exitcond18, label %58, label %53

; <label>:58                                      ; preds = %53
  %59 = getelementptr inbounds i8* %path, i64 139
  store i8 %min_s.1, i8* %59, align 1, !tbaa !1
  br label %60

; <label>:60                                      ; preds = %58, %83
  %indvars.iv14 = phi i64 [ 138, %58 ], [ %indvars.iv.next15, %83 ]
  %61 = getelementptr inbounds [140 x [64 x double]]* %llike, i64 0, i64 %indvars.iv14, i64 0
  %62 = load double* %61, align 16, !tbaa !4
  %63 = add nsw i64 %indvars.iv14, 1
  %64 = getelementptr inbounds i8* %path, i64 %63
  %65 = load i8* %64, align 1, !tbaa !1
  %66 = zext i8 %65 to i64
  %67 = getelementptr inbounds double* %transition, i64 %66
  %68 = load double* %67, align 8, !tbaa !4
  %69 = fadd double %62, %68
  %70 = zext i8 %65 to i32
  br label %71

; <label>:71                                      ; preds = %71, %60
  %indvars.iv = phi i64 [ 1, %60 ], [ %indvars.iv.next, %71 ]
  %min_s.22 = phi i8 [ 0, %60 ], [ %min_s.3, %71 ]
  %min_p.41 = phi double [ %69, %60 ], [ %min_p.5, %71 ]
  %72 = trunc i64 %indvars.iv to i32
  %73 = getelementptr inbounds [140 x [64 x double]]* %llike, i64 0, i64 %indvars.iv14, i64 %indvars.iv
  %74 = load double* %73, align 8, !tbaa !4
  %75 = shl nuw nsw i32 %72, 6
  %76 = add nsw i32 %70, %75
  %77 = sext i32 %76 to i64
  %78 = getelementptr inbounds double* %transition, i64 %77
  %79 = load double* %78, align 8, !tbaa !4
  %80 = fadd double %74, %79
  %81 = fcmp olt double %80, %min_p.41
  %min_p.5 = select i1 %81, double %80, double %min_p.41
  %82 = trunc i64 %indvars.iv to i8
  %min_s.3 = select i1 %81, i8 %82, i8 %min_s.22
  %indvars.iv.next = add nuw nsw i64 %indvars.iv, 1
  %exitcond = icmp eq i64 %indvars.iv.next, 64
  br i1 %exitcond, label %83, label %71

; <label>:83                                      ; preds = %71
  %84 = getelementptr inbounds i8* %path, i64 %indvars.iv14
  store i8 %min_s.3, i8* %84, align 1, !tbaa !1
  %indvars.iv.next15 = add nsw i64 %indvars.iv14, -1
  %85 = trunc i64 %indvars.iv14 to i32
  %86 = icmp sgt i32 %85, 0
  br i1 %86, label %60, label %87

; <label>:87                                      ; preds = %83
  call void @llvm.lifetime.end(i64 71680, i8* %1) #1
  ret i32 0
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
!2 = metadata !{metadata !"omnipotent char", metadata !3, i64 0}
!3 = metadata !{metadata !"Simple C/C++ TBAA"}
!4 = metadata !{metadata !5, metadata !5, i64 0}
!5 = metadata !{metadata !"double", metadata !2, i64 0}
