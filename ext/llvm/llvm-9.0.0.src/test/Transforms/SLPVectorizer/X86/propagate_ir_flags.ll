; NOTE: Assertions have been autogenerated by utils/update_test_checks.py
; RUN: opt < %s -basicaa -slp-vectorizer -S | FileCheck %s

; Check propagation of optional IR flags (PR20802). For a flag to
; propagate from scalar instructions to their vector replacement,
; *all* scalar instructions must have the flag.

target datalayout = "e-m:o-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-unknown"

define void @exact(i32* %x) {
; CHECK-LABEL: @exact(
; CHECK-NEXT:    [[IDX1:%.*]] = getelementptr inbounds i32, i32* [[X:%.*]], i64 0
; CHECK-NEXT:    [[IDX2:%.*]] = getelementptr inbounds i32, i32* [[X]], i64 1
; CHECK-NEXT:    [[IDX3:%.*]] = getelementptr inbounds i32, i32* [[X]], i64 2
; CHECK-NEXT:    [[IDX4:%.*]] = getelementptr inbounds i32, i32* [[X]], i64 3
; CHECK-NEXT:    [[TMP1:%.*]] = bitcast i32* [[IDX1]] to <4 x i32>*
; CHECK-NEXT:    [[TMP2:%.*]] = load <4 x i32>, <4 x i32>* [[TMP1]], align 4
; CHECK-NEXT:    [[TMP3:%.*]] = lshr exact <4 x i32> [[TMP2]], <i32 1, i32 1, i32 1, i32 1>
; CHECK-NEXT:    [[TMP4:%.*]] = bitcast i32* [[IDX1]] to <4 x i32>*
; CHECK-NEXT:    store <4 x i32> [[TMP3]], <4 x i32>* [[TMP4]], align 4
; CHECK-NEXT:    ret void
;
  %idx1 = getelementptr inbounds i32, i32* %x, i64 0
  %idx2 = getelementptr inbounds i32, i32* %x, i64 1
  %idx3 = getelementptr inbounds i32, i32* %x, i64 2
  %idx4 = getelementptr inbounds i32, i32* %x, i64 3

  %load1 = load i32, i32* %idx1, align 4
  %load2 = load i32, i32* %idx2, align 4
  %load3 = load i32, i32* %idx3, align 4
  %load4 = load i32, i32* %idx4, align 4

  %op1 = lshr exact i32 %load1, 1
  %op2 = lshr exact i32 %load2, 1
  %op3 = lshr exact i32 %load3, 1
  %op4 = lshr exact i32 %load4, 1

  store i32 %op1, i32* %idx1, align 4
  store i32 %op2, i32* %idx2, align 4
  store i32 %op3, i32* %idx3, align 4
  store i32 %op4, i32* %idx4, align 4

  ret void
}

define void @not_exact(i32* %x) {
; CHECK-LABEL: @not_exact(
; CHECK-NEXT:    [[IDX1:%.*]] = getelementptr inbounds i32, i32* [[X:%.*]], i64 0
; CHECK-NEXT:    [[IDX2:%.*]] = getelementptr inbounds i32, i32* [[X]], i64 1
; CHECK-NEXT:    [[IDX3:%.*]] = getelementptr inbounds i32, i32* [[X]], i64 2
; CHECK-NEXT:    [[IDX4:%.*]] = getelementptr inbounds i32, i32* [[X]], i64 3
; CHECK-NEXT:    [[TMP1:%.*]] = bitcast i32* [[IDX1]] to <4 x i32>*
; CHECK-NEXT:    [[TMP2:%.*]] = load <4 x i32>, <4 x i32>* [[TMP1]], align 4
; CHECK-NEXT:    [[TMP3:%.*]] = lshr <4 x i32> [[TMP2]], <i32 1, i32 1, i32 1, i32 1>
; CHECK-NEXT:    [[TMP4:%.*]] = bitcast i32* [[IDX1]] to <4 x i32>*
; CHECK-NEXT:    store <4 x i32> [[TMP3]], <4 x i32>* [[TMP4]], align 4
; CHECK-NEXT:    ret void
;
  %idx1 = getelementptr inbounds i32, i32* %x, i64 0
  %idx2 = getelementptr inbounds i32, i32* %x, i64 1
  %idx3 = getelementptr inbounds i32, i32* %x, i64 2
  %idx4 = getelementptr inbounds i32, i32* %x, i64 3

  %load1 = load i32, i32* %idx1, align 4
  %load2 = load i32, i32* %idx2, align 4
  %load3 = load i32, i32* %idx3, align 4
  %load4 = load i32, i32* %idx4, align 4

  %op1 = lshr exact i32 %load1, 1
  %op2 = lshr i32 %load2, 1
  %op3 = lshr exact i32 %load3, 1
  %op4 = lshr exact i32 %load4, 1

  store i32 %op1, i32* %idx1, align 4
  store i32 %op2, i32* %idx2, align 4
  store i32 %op3, i32* %idx3, align 4
  store i32 %op4, i32* %idx4, align 4

  ret void
}

define void @nsw(i32* %x) {
; CHECK-LABEL: @nsw(
; CHECK-NEXT:    [[IDX1:%.*]] = getelementptr inbounds i32, i32* [[X:%.*]], i64 0
; CHECK-NEXT:    [[IDX2:%.*]] = getelementptr inbounds i32, i32* [[X]], i64 1
; CHECK-NEXT:    [[IDX3:%.*]] = getelementptr inbounds i32, i32* [[X]], i64 2
; CHECK-NEXT:    [[IDX4:%.*]] = getelementptr inbounds i32, i32* [[X]], i64 3
; CHECK-NEXT:    [[TMP1:%.*]] = bitcast i32* [[IDX1]] to <4 x i32>*
; CHECK-NEXT:    [[TMP2:%.*]] = load <4 x i32>, <4 x i32>* [[TMP1]], align 4
; CHECK-NEXT:    [[TMP3:%.*]] = add nsw <4 x i32> [[TMP2]], <i32 1, i32 1, i32 1, i32 1>
; CHECK-NEXT:    [[TMP4:%.*]] = bitcast i32* [[IDX1]] to <4 x i32>*
; CHECK-NEXT:    store <4 x i32> [[TMP3]], <4 x i32>* [[TMP4]], align 4
; CHECK-NEXT:    ret void
;
  %idx1 = getelementptr inbounds i32, i32* %x, i64 0
  %idx2 = getelementptr inbounds i32, i32* %x, i64 1
  %idx3 = getelementptr inbounds i32, i32* %x, i64 2
  %idx4 = getelementptr inbounds i32, i32* %x, i64 3

  %load1 = load i32, i32* %idx1, align 4
  %load2 = load i32, i32* %idx2, align 4
  %load3 = load i32, i32* %idx3, align 4
  %load4 = load i32, i32* %idx4, align 4

  %op1 = add nsw i32 %load1, 1
  %op2 = add nsw i32 %load2, 1
  %op3 = add nsw i32 %load3, 1
  %op4 = add nsw i32 %load4, 1

  store i32 %op1, i32* %idx1, align 4
  store i32 %op2, i32* %idx2, align 4
  store i32 %op3, i32* %idx3, align 4
  store i32 %op4, i32* %idx4, align 4

  ret void
}

define void @not_nsw(i32* %x) {
; CHECK-LABEL: @not_nsw(
; CHECK-NEXT:    [[IDX1:%.*]] = getelementptr inbounds i32, i32* [[X:%.*]], i64 0
; CHECK-NEXT:    [[IDX2:%.*]] = getelementptr inbounds i32, i32* [[X]], i64 1
; CHECK-NEXT:    [[IDX3:%.*]] = getelementptr inbounds i32, i32* [[X]], i64 2
; CHECK-NEXT:    [[IDX4:%.*]] = getelementptr inbounds i32, i32* [[X]], i64 3
; CHECK-NEXT:    [[TMP1:%.*]] = bitcast i32* [[IDX1]] to <4 x i32>*
; CHECK-NEXT:    [[TMP2:%.*]] = load <4 x i32>, <4 x i32>* [[TMP1]], align 4
; CHECK-NEXT:    [[TMP3:%.*]] = add <4 x i32> [[TMP2]], <i32 1, i32 1, i32 1, i32 1>
; CHECK-NEXT:    [[TMP4:%.*]] = bitcast i32* [[IDX1]] to <4 x i32>*
; CHECK-NEXT:    store <4 x i32> [[TMP3]], <4 x i32>* [[TMP4]], align 4
; CHECK-NEXT:    ret void
;
  %idx1 = getelementptr inbounds i32, i32* %x, i64 0
  %idx2 = getelementptr inbounds i32, i32* %x, i64 1
  %idx3 = getelementptr inbounds i32, i32* %x, i64 2
  %idx4 = getelementptr inbounds i32, i32* %x, i64 3

  %load1 = load i32, i32* %idx1, align 4
  %load2 = load i32, i32* %idx2, align 4
  %load3 = load i32, i32* %idx3, align 4
  %load4 = load i32, i32* %idx4, align 4

  %op1 = add nsw i32 %load1, 1
  %op2 = add nsw i32 %load2, 1
  %op3 = add nsw i32 %load3, 1
  %op4 = add i32 %load4, 1

  store i32 %op1, i32* %idx1, align 4
  store i32 %op2, i32* %idx2, align 4
  store i32 %op3, i32* %idx3, align 4
  store i32 %op4, i32* %idx4, align 4

  ret void
}

define void @nuw(i32* %x) {
; CHECK-LABEL: @nuw(
; CHECK-NEXT:    [[IDX1:%.*]] = getelementptr inbounds i32, i32* [[X:%.*]], i64 0
; CHECK-NEXT:    [[IDX2:%.*]] = getelementptr inbounds i32, i32* [[X]], i64 1
; CHECK-NEXT:    [[IDX3:%.*]] = getelementptr inbounds i32, i32* [[X]], i64 2
; CHECK-NEXT:    [[IDX4:%.*]] = getelementptr inbounds i32, i32* [[X]], i64 3
; CHECK-NEXT:    [[TMP1:%.*]] = bitcast i32* [[IDX1]] to <4 x i32>*
; CHECK-NEXT:    [[TMP2:%.*]] = load <4 x i32>, <4 x i32>* [[TMP1]], align 4
; CHECK-NEXT:    [[TMP3:%.*]] = add nuw <4 x i32> [[TMP2]], <i32 1, i32 1, i32 1, i32 1>
; CHECK-NEXT:    [[TMP4:%.*]] = bitcast i32* [[IDX1]] to <4 x i32>*
; CHECK-NEXT:    store <4 x i32> [[TMP3]], <4 x i32>* [[TMP4]], align 4
; CHECK-NEXT:    ret void
;
  %idx1 = getelementptr inbounds i32, i32* %x, i64 0
  %idx2 = getelementptr inbounds i32, i32* %x, i64 1
  %idx3 = getelementptr inbounds i32, i32* %x, i64 2
  %idx4 = getelementptr inbounds i32, i32* %x, i64 3

  %load1 = load i32, i32* %idx1, align 4
  %load2 = load i32, i32* %idx2, align 4
  %load3 = load i32, i32* %idx3, align 4
  %load4 = load i32, i32* %idx4, align 4

  %op1 = add nuw i32 %load1, 1
  %op2 = add nuw i32 %load2, 1
  %op3 = add nuw i32 %load3, 1
  %op4 = add nuw i32 %load4, 1

  store i32 %op1, i32* %idx1, align 4
  store i32 %op2, i32* %idx2, align 4
  store i32 %op3, i32* %idx3, align 4
  store i32 %op4, i32* %idx4, align 4

  ret void
}

define void @not_nuw(i32* %x) {
; CHECK-LABEL: @not_nuw(
; CHECK-NEXT:    [[IDX1:%.*]] = getelementptr inbounds i32, i32* [[X:%.*]], i64 0
; CHECK-NEXT:    [[IDX2:%.*]] = getelementptr inbounds i32, i32* [[X]], i64 1
; CHECK-NEXT:    [[IDX3:%.*]] = getelementptr inbounds i32, i32* [[X]], i64 2
; CHECK-NEXT:    [[IDX4:%.*]] = getelementptr inbounds i32, i32* [[X]], i64 3
; CHECK-NEXT:    [[TMP1:%.*]] = bitcast i32* [[IDX1]] to <4 x i32>*
; CHECK-NEXT:    [[TMP2:%.*]] = load <4 x i32>, <4 x i32>* [[TMP1]], align 4
; CHECK-NEXT:    [[TMP3:%.*]] = add <4 x i32> [[TMP2]], <i32 1, i32 1, i32 1, i32 1>
; CHECK-NEXT:    [[TMP4:%.*]] = bitcast i32* [[IDX1]] to <4 x i32>*
; CHECK-NEXT:    store <4 x i32> [[TMP3]], <4 x i32>* [[TMP4]], align 4
; CHECK-NEXT:    ret void
;
  %idx1 = getelementptr inbounds i32, i32* %x, i64 0
  %idx2 = getelementptr inbounds i32, i32* %x, i64 1
  %idx3 = getelementptr inbounds i32, i32* %x, i64 2
  %idx4 = getelementptr inbounds i32, i32* %x, i64 3

  %load1 = load i32, i32* %idx1, align 4
  %load2 = load i32, i32* %idx2, align 4
  %load3 = load i32, i32* %idx3, align 4
  %load4 = load i32, i32* %idx4, align 4

  %op1 = add nuw i32 %load1, 1
  %op2 = add i32 %load2, 1
  %op3 = add i32 %load3, 1
  %op4 = add nuw i32 %load4, 1

  store i32 %op1, i32* %idx1, align 4
  store i32 %op2, i32* %idx2, align 4
  store i32 %op3, i32* %idx3, align 4
  store i32 %op4, i32* %idx4, align 4

  ret void
}

define void @not_nsw_but_nuw(i32* %x) {
; CHECK-LABEL: @not_nsw_but_nuw(
; CHECK-NEXT:    [[IDX1:%.*]] = getelementptr inbounds i32, i32* [[X:%.*]], i64 0
; CHECK-NEXT:    [[IDX2:%.*]] = getelementptr inbounds i32, i32* [[X]], i64 1
; CHECK-NEXT:    [[IDX3:%.*]] = getelementptr inbounds i32, i32* [[X]], i64 2
; CHECK-NEXT:    [[IDX4:%.*]] = getelementptr inbounds i32, i32* [[X]], i64 3
; CHECK-NEXT:    [[TMP1:%.*]] = bitcast i32* [[IDX1]] to <4 x i32>*
; CHECK-NEXT:    [[TMP2:%.*]] = load <4 x i32>, <4 x i32>* [[TMP1]], align 4
; CHECK-NEXT:    [[TMP3:%.*]] = add nuw <4 x i32> [[TMP2]], <i32 1, i32 1, i32 1, i32 1>
; CHECK-NEXT:    [[TMP4:%.*]] = bitcast i32* [[IDX1]] to <4 x i32>*
; CHECK-NEXT:    store <4 x i32> [[TMP3]], <4 x i32>* [[TMP4]], align 4
; CHECK-NEXT:    ret void
;
  %idx1 = getelementptr inbounds i32, i32* %x, i64 0
  %idx2 = getelementptr inbounds i32, i32* %x, i64 1
  %idx3 = getelementptr inbounds i32, i32* %x, i64 2
  %idx4 = getelementptr inbounds i32, i32* %x, i64 3

  %load1 = load i32, i32* %idx1, align 4
  %load2 = load i32, i32* %idx2, align 4
  %load3 = load i32, i32* %idx3, align 4
  %load4 = load i32, i32* %idx4, align 4

  %op1 = add nuw i32 %load1, 1
  %op2 = add nuw nsw i32 %load2, 1
  %op3 = add nuw nsw i32 %load3, 1
  %op4 = add nuw i32 %load4, 1

  store i32 %op1, i32* %idx1, align 4
  store i32 %op2, i32* %idx2, align 4
  store i32 %op3, i32* %idx3, align 4
  store i32 %op4, i32* %idx4, align 4

  ret void
}

define void @nnan(float* %x) {
; CHECK-LABEL: @nnan(
; CHECK-NEXT:    [[IDX1:%.*]] = getelementptr inbounds float, float* [[X:%.*]], i64 0
; CHECK-NEXT:    [[IDX2:%.*]] = getelementptr inbounds float, float* [[X]], i64 1
; CHECK-NEXT:    [[IDX3:%.*]] = getelementptr inbounds float, float* [[X]], i64 2
; CHECK-NEXT:    [[IDX4:%.*]] = getelementptr inbounds float, float* [[X]], i64 3
; CHECK-NEXT:    [[TMP1:%.*]] = bitcast float* [[IDX1]] to <4 x float>*
; CHECK-NEXT:    [[TMP2:%.*]] = load <4 x float>, <4 x float>* [[TMP1]], align 4
; CHECK-NEXT:    [[TMP3:%.*]] = fadd nnan <4 x float> [[TMP2]], <float 1.000000e+00, float 1.000000e+00, float 1.000000e+00, float 1.000000e+00>
; CHECK-NEXT:    [[TMP4:%.*]] = bitcast float* [[IDX1]] to <4 x float>*
; CHECK-NEXT:    store <4 x float> [[TMP3]], <4 x float>* [[TMP4]], align 4
; CHECK-NEXT:    ret void
;
  %idx1 = getelementptr inbounds float, float* %x, i64 0
  %idx2 = getelementptr inbounds float, float* %x, i64 1
  %idx3 = getelementptr inbounds float, float* %x, i64 2
  %idx4 = getelementptr inbounds float, float* %x, i64 3

  %load1 = load float, float* %idx1, align 4
  %load2 = load float, float* %idx2, align 4
  %load3 = load float, float* %idx3, align 4
  %load4 = load float, float* %idx4, align 4

  %op1 = fadd fast nnan float %load1, 1.0
  %op2 = fadd nnan ninf float %load2, 1.0
  %op3 = fadd nsz nnan float %load3, 1.0
  %op4 = fadd arcp nnan float %load4, 1.0

  store float %op1, float* %idx1, align 4
  store float %op2, float* %idx2, align 4
  store float %op3, float* %idx3, align 4
  store float %op4, float* %idx4, align 4

  ret void
}

define void @not_nnan(float* %x) {
; CHECK-LABEL: @not_nnan(
; CHECK-NEXT:    [[IDX1:%.*]] = getelementptr inbounds float, float* [[X:%.*]], i64 0
; CHECK-NEXT:    [[IDX2:%.*]] = getelementptr inbounds float, float* [[X]], i64 1
; CHECK-NEXT:    [[IDX3:%.*]] = getelementptr inbounds float, float* [[X]], i64 2
; CHECK-NEXT:    [[IDX4:%.*]] = getelementptr inbounds float, float* [[X]], i64 3
; CHECK-NEXT:    [[TMP1:%.*]] = bitcast float* [[IDX1]] to <4 x float>*
; CHECK-NEXT:    [[TMP2:%.*]] = load <4 x float>, <4 x float>* [[TMP1]], align 4
; CHECK-NEXT:    [[TMP3:%.*]] = fadd <4 x float> [[TMP2]], <float 1.000000e+00, float 1.000000e+00, float 1.000000e+00, float 1.000000e+00>
; CHECK-NEXT:    [[TMP4:%.*]] = bitcast float* [[IDX1]] to <4 x float>*
; CHECK-NEXT:    store <4 x float> [[TMP3]], <4 x float>* [[TMP4]], align 4
; CHECK-NEXT:    ret void
;
  %idx1 = getelementptr inbounds float, float* %x, i64 0
  %idx2 = getelementptr inbounds float, float* %x, i64 1
  %idx3 = getelementptr inbounds float, float* %x, i64 2
  %idx4 = getelementptr inbounds float, float* %x, i64 3

  %load1 = load float, float* %idx1, align 4
  %load2 = load float, float* %idx2, align 4
  %load3 = load float, float* %idx3, align 4
  %load4 = load float, float* %idx4, align 4

  %op1 = fadd nnan float %load1, 1.0
  %op2 = fadd ninf float %load2, 1.0
  %op3 = fadd nsz float %load3, 1.0
  %op4 = fadd arcp float %load4, 1.0

  store float %op1, float* %idx1, align 4
  store float %op2, float* %idx2, align 4
  store float %op3, float* %idx3, align 4
  store float %op4, float* %idx4, align 4

  ret void
}

define void @only_fast(float* %x) {
; CHECK-LABEL: @only_fast(
; CHECK-NEXT:    [[IDX1:%.*]] = getelementptr inbounds float, float* [[X:%.*]], i64 0
; CHECK-NEXT:    [[IDX2:%.*]] = getelementptr inbounds float, float* [[X]], i64 1
; CHECK-NEXT:    [[IDX3:%.*]] = getelementptr inbounds float, float* [[X]], i64 2
; CHECK-NEXT:    [[IDX4:%.*]] = getelementptr inbounds float, float* [[X]], i64 3
; CHECK-NEXT:    [[TMP1:%.*]] = bitcast float* [[IDX1]] to <4 x float>*
; CHECK-NEXT:    [[TMP2:%.*]] = load <4 x float>, <4 x float>* [[TMP1]], align 4
; CHECK-NEXT:    [[TMP3:%.*]] = fadd fast <4 x float> [[TMP2]], <float 1.000000e+00, float 1.000000e+00, float 1.000000e+00, float 1.000000e+00>
; CHECK-NEXT:    [[TMP4:%.*]] = bitcast float* [[IDX1]] to <4 x float>*
; CHECK-NEXT:    store <4 x float> [[TMP3]], <4 x float>* [[TMP4]], align 4
; CHECK-NEXT:    ret void
;
  %idx1 = getelementptr inbounds float, float* %x, i64 0
  %idx2 = getelementptr inbounds float, float* %x, i64 1
  %idx3 = getelementptr inbounds float, float* %x, i64 2
  %idx4 = getelementptr inbounds float, float* %x, i64 3

  %load1 = load float, float* %idx1, align 4
  %load2 = load float, float* %idx2, align 4
  %load3 = load float, float* %idx3, align 4
  %load4 = load float, float* %idx4, align 4

  %op1 = fadd fast nnan float %load1, 1.0
  %op2 = fadd fast nnan ninf float %load2, 1.0
  %op3 = fadd fast nsz nnan float %load3, 1.0
  %op4 = fadd arcp nnan fast float %load4, 1.0

  store float %op1, float* %idx1, align 4
  store float %op2, float* %idx2, align 4
  store float %op3, float* %idx3, align 4
  store float %op4, float* %idx4, align 4

  ret void
}

define void @only_arcp(float* %x) {
; CHECK-LABEL: @only_arcp(
; CHECK-NEXT:    [[IDX1:%.*]] = getelementptr inbounds float, float* [[X:%.*]], i64 0
; CHECK-NEXT:    [[IDX2:%.*]] = getelementptr inbounds float, float* [[X]], i64 1
; CHECK-NEXT:    [[IDX3:%.*]] = getelementptr inbounds float, float* [[X]], i64 2
; CHECK-NEXT:    [[IDX4:%.*]] = getelementptr inbounds float, float* [[X]], i64 3
; CHECK-NEXT:    [[TMP1:%.*]] = bitcast float* [[IDX1]] to <4 x float>*
; CHECK-NEXT:    [[TMP2:%.*]] = load <4 x float>, <4 x float>* [[TMP1]], align 4
; CHECK-NEXT:    [[TMP3:%.*]] = fadd arcp <4 x float> [[TMP2]], <float 1.000000e+00, float 1.000000e+00, float 1.000000e+00, float 1.000000e+00>
; CHECK-NEXT:    [[TMP4:%.*]] = bitcast float* [[IDX1]] to <4 x float>*
; CHECK-NEXT:    store <4 x float> [[TMP3]], <4 x float>* [[TMP4]], align 4
; CHECK-NEXT:    ret void
;
  %idx1 = getelementptr inbounds float, float* %x, i64 0
  %idx2 = getelementptr inbounds float, float* %x, i64 1
  %idx3 = getelementptr inbounds float, float* %x, i64 2
  %idx4 = getelementptr inbounds float, float* %x, i64 3

  %load1 = load float, float* %idx1, align 4
  %load2 = load float, float* %idx2, align 4
  %load3 = load float, float* %idx3, align 4
  %load4 = load float, float* %idx4, align 4

  %op1 = fadd fast float %load1, 1.0
  %op2 = fadd fast float %load2, 1.0
  %op3 = fadd fast float %load3, 1.0
  %op4 = fadd arcp float %load4, 1.0

  store float %op1, float* %idx1, align 4
  store float %op2, float* %idx2, align 4
  store float %op3, float* %idx3, align 4
  store float %op4, float* %idx4, align 4

  ret void
}

define void @addsub_all_nsw(i32* %x) {
; CHECK-LABEL: @addsub_all_nsw(
; CHECK-NEXT:    [[IDX1:%.*]] = getelementptr inbounds i32, i32* [[X:%.*]], i64 0
; CHECK-NEXT:    [[IDX2:%.*]] = getelementptr inbounds i32, i32* [[X]], i64 1
; CHECK-NEXT:    [[IDX3:%.*]] = getelementptr inbounds i32, i32* [[X]], i64 2
; CHECK-NEXT:    [[IDX4:%.*]] = getelementptr inbounds i32, i32* [[X]], i64 3
; CHECK-NEXT:    [[TMP1:%.*]] = bitcast i32* [[IDX1]] to <4 x i32>*
; CHECK-NEXT:    [[TMP2:%.*]] = load <4 x i32>, <4 x i32>* [[TMP1]], align 4
; CHECK-NEXT:    [[TMP3:%.*]] = add nsw <4 x i32> [[TMP2]], <i32 1, i32 1, i32 1, i32 1>
; CHECK-NEXT:    [[TMP4:%.*]] = sub nsw <4 x i32> [[TMP2]], <i32 1, i32 1, i32 1, i32 1>
; CHECK-NEXT:    [[TMP5:%.*]] = shufflevector <4 x i32> [[TMP3]], <4 x i32> [[TMP4]], <4 x i32> <i32 0, i32 5, i32 2, i32 7>
; CHECK-NEXT:    [[TMP6:%.*]] = bitcast i32* [[IDX1]] to <4 x i32>*
; CHECK-NEXT:    store <4 x i32> [[TMP5]], <4 x i32>* [[TMP6]], align 4
; CHECK-NEXT:    ret void
;
  %idx1 = getelementptr inbounds i32, i32* %x, i64 0
  %idx2 = getelementptr inbounds i32, i32* %x, i64 1
  %idx3 = getelementptr inbounds i32, i32* %x, i64 2
  %idx4 = getelementptr inbounds i32, i32* %x, i64 3

  %load1 = load i32, i32* %idx1, align 4
  %load2 = load i32, i32* %idx2, align 4
  %load3 = load i32, i32* %idx3, align 4
  %load4 = load i32, i32* %idx4, align 4

  %op1 = add nsw i32 %load1, 1
  %op2 = sub nsw i32 %load2, 1
  %op3 = add nsw i32 %load3, 1
  %op4 = sub nsw i32 %load4, 1

  store i32 %op1, i32* %idx1, align 4
  store i32 %op2, i32* %idx2, align 4
  store i32 %op3, i32* %idx3, align 4
  store i32 %op4, i32* %idx4, align 4

  ret void
}

define void @addsub_some_nsw(i32* %x) {
; CHECK-LABEL: @addsub_some_nsw(
; CHECK-NEXT:    [[IDX1:%.*]] = getelementptr inbounds i32, i32* [[X:%.*]], i64 0
; CHECK-NEXT:    [[IDX2:%.*]] = getelementptr inbounds i32, i32* [[X]], i64 1
; CHECK-NEXT:    [[IDX3:%.*]] = getelementptr inbounds i32, i32* [[X]], i64 2
; CHECK-NEXT:    [[IDX4:%.*]] = getelementptr inbounds i32, i32* [[X]], i64 3
; CHECK-NEXT:    [[TMP1:%.*]] = bitcast i32* [[IDX1]] to <4 x i32>*
; CHECK-NEXT:    [[TMP2:%.*]] = load <4 x i32>, <4 x i32>* [[TMP1]], align 4
; CHECK-NEXT:    [[TMP3:%.*]] = add nsw <4 x i32> [[TMP2]], <i32 1, i32 1, i32 1, i32 1>
; CHECK-NEXT:    [[TMP4:%.*]] = sub <4 x i32> [[TMP2]], <i32 1, i32 1, i32 1, i32 1>
; CHECK-NEXT:    [[TMP5:%.*]] = shufflevector <4 x i32> [[TMP3]], <4 x i32> [[TMP4]], <4 x i32> <i32 0, i32 5, i32 2, i32 7>
; CHECK-NEXT:    [[TMP6:%.*]] = bitcast i32* [[IDX1]] to <4 x i32>*
; CHECK-NEXT:    store <4 x i32> [[TMP5]], <4 x i32>* [[TMP6]], align 4
; CHECK-NEXT:    ret void
;
  %idx1 = getelementptr inbounds i32, i32* %x, i64 0
  %idx2 = getelementptr inbounds i32, i32* %x, i64 1
  %idx3 = getelementptr inbounds i32, i32* %x, i64 2
  %idx4 = getelementptr inbounds i32, i32* %x, i64 3

  %load1 = load i32, i32* %idx1, align 4
  %load2 = load i32, i32* %idx2, align 4
  %load3 = load i32, i32* %idx3, align 4
  %load4 = load i32, i32* %idx4, align 4

  %op1 = add nsw i32 %load1, 1
  %op2 = sub nsw i32 %load2, 1
  %op3 = add nsw i32 %load3, 1
  %op4 = sub i32 %load4, 1

  store i32 %op1, i32* %idx1, align 4
  store i32 %op2, i32* %idx2, align 4
  store i32 %op3, i32* %idx3, align 4
  store i32 %op4, i32* %idx4, align 4

  ret void
}

define void @addsub_no_nsw(i32* %x) {
; CHECK-LABEL: @addsub_no_nsw(
; CHECK-NEXT:    [[IDX1:%.*]] = getelementptr inbounds i32, i32* [[X:%.*]], i64 0
; CHECK-NEXT:    [[IDX2:%.*]] = getelementptr inbounds i32, i32* [[X]], i64 1
; CHECK-NEXT:    [[IDX3:%.*]] = getelementptr inbounds i32, i32* [[X]], i64 2
; CHECK-NEXT:    [[IDX4:%.*]] = getelementptr inbounds i32, i32* [[X]], i64 3
; CHECK-NEXT:    [[TMP1:%.*]] = bitcast i32* [[IDX1]] to <4 x i32>*
; CHECK-NEXT:    [[TMP2:%.*]] = load <4 x i32>, <4 x i32>* [[TMP1]], align 4
; CHECK-NEXT:    [[TMP3:%.*]] = add <4 x i32> [[TMP2]], <i32 1, i32 1, i32 1, i32 1>
; CHECK-NEXT:    [[TMP4:%.*]] = sub <4 x i32> [[TMP2]], <i32 1, i32 1, i32 1, i32 1>
; CHECK-NEXT:    [[TMP5:%.*]] = shufflevector <4 x i32> [[TMP3]], <4 x i32> [[TMP4]], <4 x i32> <i32 0, i32 5, i32 2, i32 7>
; CHECK-NEXT:    [[TMP6:%.*]] = bitcast i32* [[IDX1]] to <4 x i32>*
; CHECK-NEXT:    store <4 x i32> [[TMP5]], <4 x i32>* [[TMP6]], align 4
; CHECK-NEXT:    ret void
;
  %idx1 = getelementptr inbounds i32, i32* %x, i64 0
  %idx2 = getelementptr inbounds i32, i32* %x, i64 1
  %idx3 = getelementptr inbounds i32, i32* %x, i64 2
  %idx4 = getelementptr inbounds i32, i32* %x, i64 3

  %load1 = load i32, i32* %idx1, align 4
  %load2 = load i32, i32* %idx2, align 4
  %load3 = load i32, i32* %idx3, align 4
  %load4 = load i32, i32* %idx4, align 4

  %op1 = add i32 %load1, 1
  %op2 = sub nsw i32 %load2, 1
  %op3 = add nsw i32 %load3, 1
  %op4 = sub i32 %load4, 1

  store i32 %op1, i32* %idx1, align 4
  store i32 %op2, i32* %idx2, align 4
  store i32 %op3, i32* %idx3, align 4
  store i32 %op4, i32* %idx4, align 4

  ret void
}

define void @fcmp_fast(double* %x) #1 {
; CHECK-LABEL: @fcmp_fast(
; CHECK-NEXT:    [[IDX1:%.*]] = getelementptr inbounds double, double* [[X:%.*]], i64 0
; CHECK-NEXT:    [[IDX2:%.*]] = getelementptr inbounds double, double* [[X]], i64 1
; CHECK-NEXT:    [[TMP1:%.*]] = bitcast double* [[IDX1]] to <2 x double>*
; CHECK-NEXT:    [[TMP2:%.*]] = load <2 x double>, <2 x double>* [[TMP1]], align 8
; CHECK-NEXT:    [[TMP3:%.*]] = fcmp fast oge <2 x double> [[TMP2]], zeroinitializer
; CHECK-NEXT:    [[TMP4:%.*]] = fsub fast <2 x double> <double -0.000000e+00, double -0.000000e+00>, [[TMP2]]
; CHECK-NEXT:    [[TMP5:%.*]] = select <2 x i1> [[TMP3]], <2 x double> [[TMP2]], <2 x double> [[TMP4]]
; CHECK-NEXT:    [[TMP6:%.*]] = bitcast double* [[IDX1]] to <2 x double>*
; CHECK-NEXT:    store <2 x double> [[TMP5]], <2 x double>* [[TMP6]], align 8
; CHECK-NEXT:    ret void
;
  %idx1 = getelementptr inbounds double, double* %x, i64 0
  %idx2 = getelementptr inbounds double, double* %x, i64 1

  %load1 = load double, double* %idx1, align 8
  %load2 = load double, double* %idx2, align 8

  %cmp1 = fcmp fast oge double %load1, 0.000000e+00
  %cmp2 = fcmp fast oge double %load2, 0.000000e+00

  %sub1 = fsub fast double -0.000000e+00, %load1
  %sub2 = fsub fast double -0.000000e+00, %load2

  %sel1 = select i1 %cmp1, double %load1, double %sub1
  %sel2 = select i1 %cmp2, double %load2, double %sub2

  store double %sel1, double* %idx1, align 8
  store double %sel2, double* %idx2, align 8

  ret void
}

define void @fcmp_fast_unary_fneg(double* %x) #1 {
; CHECK-LABEL: @fcmp_fast_unary_fneg(
; CHECK-NEXT:    [[IDX1:%.*]] = getelementptr inbounds double, double* [[X:%.*]], i64 0
; CHECK-NEXT:    [[IDX2:%.*]] = getelementptr inbounds double, double* [[X]], i64 1
; CHECK-NEXT:    [[TMP1:%.*]] = bitcast double* [[IDX1]] to <2 x double>*
; CHECK-NEXT:    [[TMP2:%.*]] = load <2 x double>, <2 x double>* [[TMP1]], align 8
; CHECK-NEXT:    [[TMP3:%.*]] = fcmp fast oge <2 x double> [[TMP2]], zeroinitializer
; CHECK-NEXT:    [[TMP4:%.*]] = fneg fast <2 x double> [[TMP2]]
; CHECK-NEXT:    [[TMP5:%.*]] = select <2 x i1> [[TMP3]], <2 x double> [[TMP2]], <2 x double> [[TMP4]]
; CHECK-NEXT:    [[TMP6:%.*]] = bitcast double* [[IDX1]] to <2 x double>*
; CHECK-NEXT:    store <2 x double> [[TMP5]], <2 x double>* [[TMP6]], align 8
; CHECK-NEXT:    ret void
;
  %idx1 = getelementptr inbounds double, double* %x, i64 0
  %idx2 = getelementptr inbounds double, double* %x, i64 1

  %load1 = load double, double* %idx1, align 8
  %load2 = load double, double* %idx2, align 8

  %cmp1 = fcmp fast oge double %load1, 0.000000e+00
  %cmp2 = fcmp fast oge double %load2, 0.000000e+00

  %sub1 = fneg fast double %load1
  %sub2 = fneg fast double %load2

  %sel1 = select i1 %cmp1, double %load1, double %sub1
  %sel2 = select i1 %cmp2, double %load2, double %sub2

  store double %sel1, double* %idx1, align 8
  store double %sel2, double* %idx2, align 8

  ret void
}

define void @fcmp_no_fast(double* %x) #1 {
; CHECK-LABEL: @fcmp_no_fast(
; CHECK-NEXT:    [[IDX1:%.*]] = getelementptr inbounds double, double* [[X:%.*]], i64 0
; CHECK-NEXT:    [[IDX2:%.*]] = getelementptr inbounds double, double* [[X]], i64 1
; CHECK-NEXT:    [[TMP1:%.*]] = bitcast double* [[IDX1]] to <2 x double>*
; CHECK-NEXT:    [[TMP2:%.*]] = load <2 x double>, <2 x double>* [[TMP1]], align 8
; CHECK-NEXT:    [[TMP3:%.*]] = fcmp oge <2 x double> [[TMP2]], zeroinitializer
; CHECK-NEXT:    [[TMP4:%.*]] = fsub <2 x double> <double -0.000000e+00, double -0.000000e+00>, [[TMP2]]
; CHECK-NEXT:    [[TMP5:%.*]] = select <2 x i1> [[TMP3]], <2 x double> [[TMP2]], <2 x double> [[TMP4]]
; CHECK-NEXT:    [[TMP6:%.*]] = bitcast double* [[IDX1]] to <2 x double>*
; CHECK-NEXT:    store <2 x double> [[TMP5]], <2 x double>* [[TMP6]], align 8
; CHECK-NEXT:    ret void
;
  %idx1 = getelementptr inbounds double, double* %x, i64 0
  %idx2 = getelementptr inbounds double, double* %x, i64 1

  %load1 = load double, double* %idx1, align 8
  %load2 = load double, double* %idx2, align 8

  %cmp1 = fcmp fast oge double %load1, 0.000000e+00
  %cmp2 = fcmp oge double %load2, 0.000000e+00

  %sub1 = fsub fast double -0.000000e+00, %load1
  %sub2 = fsub double -0.000000e+00, %load2

  %sel1 = select i1 %cmp1, double %load1, double %sub1
  %sel2 = select i1 %cmp2, double %load2, double %sub2

  store double %sel1, double* %idx1, align 8
  store double %sel2, double* %idx2, align 8

  ret void
}

define void @fcmp_no_fast_unary_fneg(double* %x) #1 {
; CHECK-LABEL: @fcmp_no_fast_unary_fneg(
; CHECK-NEXT:    [[IDX1:%.*]] = getelementptr inbounds double, double* [[X:%.*]], i64 0
; CHECK-NEXT:    [[IDX2:%.*]] = getelementptr inbounds double, double* [[X]], i64 1
; CHECK-NEXT:    [[TMP1:%.*]] = bitcast double* [[IDX1]] to <2 x double>*
; CHECK-NEXT:    [[TMP2:%.*]] = load <2 x double>, <2 x double>* [[TMP1]], align 8
; CHECK-NEXT:    [[TMP3:%.*]] = fcmp oge <2 x double> [[TMP2]], zeroinitializer
; CHECK-NEXT:    [[TMP4:%.*]] = fneg <2 x double> [[TMP2]]
; CHECK-NEXT:    [[TMP5:%.*]] = select <2 x i1> [[TMP3]], <2 x double> [[TMP2]], <2 x double> [[TMP4]]
; CHECK-NEXT:    [[TMP6:%.*]] = bitcast double* [[IDX1]] to <2 x double>*
; CHECK-NEXT:    store <2 x double> [[TMP5]], <2 x double>* [[TMP6]], align 8
; CHECK-NEXT:    ret void
;
  %idx1 = getelementptr inbounds double, double* %x, i64 0
  %idx2 = getelementptr inbounds double, double* %x, i64 1

  %load1 = load double, double* %idx1, align 8
  %load2 = load double, double* %idx2, align 8

  %cmp1 = fcmp fast oge double %load1, 0.000000e+00
  %cmp2 = fcmp oge double %load2, 0.000000e+00

  %sub1 = fneg double %load1
  %sub2 = fneg double %load2

  %sel1 = select i1 %cmp1, double %load1, double %sub1
  %sel2 = select i1 %cmp2, double %load2, double %sub2

  store double %sel1, double* %idx1, align 8
  store double %sel2, double* %idx2, align 8

  ret void
}

declare double @llvm.fabs.f64(double) nounwind readnone

define void @call_fast(double* %x) {
; CHECK-LABEL: @call_fast(
; CHECK-NEXT:    [[IDX1:%.*]] = getelementptr inbounds double, double* [[X:%.*]], i64 0
; CHECK-NEXT:    [[IDX2:%.*]] = getelementptr inbounds double, double* [[X]], i64 1
; CHECK-NEXT:    [[TMP1:%.*]] = bitcast double* [[IDX1]] to <2 x double>*
; CHECK-NEXT:    [[TMP2:%.*]] = load <2 x double>, <2 x double>* [[TMP1]], align 8
; CHECK-NEXT:    [[TMP3:%.*]] = call fast <2 x double> @llvm.fabs.v2f64(<2 x double> [[TMP2]])
; CHECK-NEXT:    [[TMP4:%.*]] = bitcast double* [[IDX1]] to <2 x double>*
; CHECK-NEXT:    store <2 x double> [[TMP3]], <2 x double>* [[TMP4]], align 8
; CHECK-NEXT:    ret void
;
  %idx1 = getelementptr inbounds double, double* %x, i64 0
  %idx2 = getelementptr inbounds double, double* %x, i64 1

  %load1 = load double, double* %idx1, align 8
  %load2 = load double, double* %idx2, align 8

  %call1 = tail call fast double @llvm.fabs.f64(double %load1) nounwind readnone
  %call2 = tail call fast double @llvm.fabs.f64(double %load2) nounwind readnone

  store double %call1, double* %idx1, align 8
  store double %call2, double* %idx2, align 8

  ret void
}

define void @call_no_fast(double* %x) {
; CHECK-LABEL: @call_no_fast(
; CHECK-NEXT:    [[IDX1:%.*]] = getelementptr inbounds double, double* [[X:%.*]], i64 0
; CHECK-NEXT:    [[IDX2:%.*]] = getelementptr inbounds double, double* [[X]], i64 1
; CHECK-NEXT:    [[TMP1:%.*]] = bitcast double* [[IDX1]] to <2 x double>*
; CHECK-NEXT:    [[TMP2:%.*]] = load <2 x double>, <2 x double>* [[TMP1]], align 8
; CHECK-NEXT:    [[TMP3:%.*]] = call <2 x double> @llvm.fabs.v2f64(<2 x double> [[TMP2]])
; CHECK-NEXT:    [[TMP4:%.*]] = bitcast double* [[IDX1]] to <2 x double>*
; CHECK-NEXT:    store <2 x double> [[TMP3]], <2 x double>* [[TMP4]], align 8
; CHECK-NEXT:    ret void
;
  %idx1 = getelementptr inbounds double, double* %x, i64 0
  %idx2 = getelementptr inbounds double, double* %x, i64 1

  %load1 = load double, double* %idx1, align 8
  %load2 = load double, double* %idx2, align 8

  %call1 = tail call fast double @llvm.fabs.f64(double %load1) nounwind readnone
  %call2 = tail call double @llvm.fabs.f64(double %load2) nounwind readnone

  store double %call1, double* %idx1, align 8
  store double %call2, double* %idx2, align 8

  ret void
}

attributes #1 = { "target-features"="+avx" }
