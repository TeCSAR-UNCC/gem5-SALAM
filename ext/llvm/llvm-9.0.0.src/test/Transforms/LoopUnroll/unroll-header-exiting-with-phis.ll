; NOTE: Assertions have been autogenerated by utils/update_test_checks.py
; RUN: opt < %s -S -loop-unroll -unroll-allow-partial | FileCheck %s

; The phi which acts as input to func should not be undef. It should
; have its loop-carried value (the load in for.cond) replaced accordingly
; after unrolling the loop.

define i16 @full_unroll(i16* %A) {
; CHECK-LABEL: @full_unroll(
; CHECK-NEXT:  entry:
; CHECK-NEXT:    br label [[FOR_COND:%.*]]
; CHECK:       for.cond:
; CHECK-NEXT:    [[TMP2:%.*]] = load i16, i16* [[A:%.*]]
; CHECK-NEXT:    br label [[FOR_COND_CLEANUP3:%.*]]
; CHECK:       for.cond.cleanup:
; CHECK-NEXT:    [[DOTLCSSA10_LCSSA:%.*]] = phi i16 [ [[TMP2_2:%.*]], [[FOR_COND_CLEANUP3_2:%.*]] ]
; CHECK-NEXT:    [[TMP3:%.*]] = call i16 @func(i16 [[DOTLCSSA10_LCSSA]])
; CHECK-NEXT:    ret i16 0
; CHECK:       for.cond.cleanup3:
; CHECK-NEXT:    [[PTR_1:%.*]] = getelementptr inbounds i16, i16* [[A]], i64 1
; CHECK-NEXT:    [[TMP2_1:%.*]] = load i16, i16* [[PTR_1]]
; CHECK-NEXT:    br label [[FOR_COND_CLEANUP3_1:%.*]]
; CHECK:       for.cond.cleanup3.1:
; CHECK-NEXT:    [[PTR_2:%.*]] = getelementptr inbounds i16, i16* [[A]], i64 2
; CHECK-NEXT:    [[TMP2_2]] = load i16, i16* [[PTR_2]]
; CHECK-NEXT:    br label [[FOR_COND_CLEANUP3_2]]
; CHECK:       for.cond.cleanup3.2:
; CHECK-NEXT:    [[PTR_3:%.*]] = getelementptr inbounds i16, i16* [[A]], i64 3
; CHECK-NEXT:    [[TMP2_3:%.*]] = load i16, i16* [[PTR_3]]
; CHECK-NEXT:    br i1 false, label [[FOR_COND_CLEANUP3_3:%.*]], label [[FOR_COND_CLEANUP:%.*]]
; CHECK:       for.cond.cleanup3.3:
; CHECK-NEXT:    unreachable
;
entry:
  br label %for.cond

for.cond:                                         ; preds = %for.cond.cleanup3, %entry
  %.lcssa10 = phi i16 [ 123, %entry ], [ %.lcssa, %for.cond.cleanup3 ]
  %i.0 = phi i64 [ 0, %entry ], [ %inc9, %for.cond.cleanup3 ]
  %ptr = getelementptr inbounds i16, i16* %A, i64 %i.0
  %tmp2 = load i16, i16* %ptr
  %cmp = icmp ult i64 %i.0, 3
  br i1 %cmp, label %for.cond.cleanup3, label %for.cond.cleanup

for.cond.cleanup:                                 ; preds = %for.cond
  %.lcssa10.lcssa = phi i16 [ %.lcssa10, %for.cond ]
  %tmp3 = call i16 (i16) @func(i16 %.lcssa10.lcssa)
  ret i16 0

for.cond.cleanup3:                                ; preds = %for.cond
  %.lcssa = phi i16 [ %tmp2, %for.cond ]
  %inc9 = add i64 %i.0, 1
  br label %for.cond
}

define i16 @partial_unroll(i16* %A) {
; CHECK-LABEL: @partial_unroll(
; CHECK-NEXT:  entry:
; CHECK-NEXT:    br label [[FOR_COND:%.*]]
; CHECK:       for.cond:
; CHECK-NEXT:    [[I_0:%.*]] = phi i64 [ 0, [[ENTRY:%.*]] ], [ [[INC9_2:%.*]], [[FOR_COND_CLEANUP3_2:%.*]] ]
; CHECK-NEXT:    [[PTR:%.*]] = getelementptr inbounds i16, i16* [[A:%.*]], i64 [[I_0]]
; CHECK-NEXT:    [[TMP2:%.*]] = load i16, i16* [[PTR]]
; CHECK-NEXT:    br label [[FOR_COND_CLEANUP3:%.*]]
; CHECK:       for.cond.cleanup:
; CHECK-NEXT:    [[DOTLCSSA10_LCSSA:%.*]] = phi i16 [ [[TMP2_1:%.*]], [[FOR_COND_CLEANUP3_1:%.*]] ]
; CHECK-NEXT:    [[TMP3:%.*]] = call i16 @func(i16 [[DOTLCSSA10_LCSSA]])
; CHECK-NEXT:    ret i16 0
; CHECK:       for.cond.cleanup3:
; CHECK-NEXT:    [[INC9:%.*]] = add nuw nsw i64 [[I_0]], 1
; CHECK-NEXT:    [[PTR_1:%.*]] = getelementptr inbounds i16, i16* [[A]], i64 [[INC9]]
; CHECK-NEXT:    [[TMP2_1]] = load i16, i16* [[PTR_1]]
; CHECK-NEXT:    br label [[FOR_COND_CLEANUP3_1]]
; CHECK:       for.cond.cleanup3.1:
; CHECK-NEXT:    [[INC9_1:%.*]] = add nuw nsw i64 [[INC9]], 1
; CHECK-NEXT:    [[PTR_2:%.*]] = getelementptr inbounds i16, i16* [[A]], i64 [[INC9_1]]
; CHECK-NEXT:    [[TMP2_2:%.*]] = load i16, i16* [[PTR_2]]
; CHECK-NEXT:    [[CMP_2:%.*]] = icmp ult i64 [[INC9_1]], 200
; CHECK-NEXT:    br i1 [[CMP_2]], label [[FOR_COND_CLEANUP3_2]], label [[FOR_COND_CLEANUP:%.*]]
; CHECK:       for.cond.cleanup3.2:
; CHECK-NEXT:    [[INC9_2]] = add nuw nsw i64 [[INC9_1]], 1
; CHECK-NEXT:    br label [[FOR_COND]]
;
entry:
  br label %for.cond

for.cond:                                         ; preds = %for.cond.cleanup3, %entry
  %.lcssa10 = phi i16 [ 123, %entry ], [ %.lcssa, %for.cond.cleanup3 ]
  %i.0 = phi i64 [ 0, %entry ], [ %inc9, %for.cond.cleanup3 ]
  %ptr = getelementptr inbounds i16, i16* %A, i64 %i.0
  %tmp2 = load i16, i16* %ptr
  %cmp = icmp ult i64 %i.0, 200
  br i1 %cmp, label %for.cond.cleanup3, label %for.cond.cleanup

for.cond.cleanup:                                 ; preds = %for.cond
  %.lcssa10.lcssa = phi i16 [ %.lcssa10, %for.cond ]
  %tmp3 = call i16 (i16) @func(i16 %.lcssa10.lcssa)
  ret i16 0

for.cond.cleanup3:                                ; preds = %for.cond
  %.lcssa = phi i16 [ %tmp2, %for.cond ]
  %inc9 = add i64 %i.0, 1
  br label %for.cond
}

declare i16 @func(i16)

