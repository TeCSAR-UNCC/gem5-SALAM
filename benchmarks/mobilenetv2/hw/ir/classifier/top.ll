; ModuleID = 'top.bc'
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: norecurse nounwind uwtable
define void @top(i64 %feat_rd_addr, i64 %feat_wr_addr, i64 %weights, i64 %quant) #0 {
  store volatile i8 1, i8* inttoptr (i64 789311581 to i8*), align 1
  store volatile i64 %feat_wr_addr, i64* inttoptr (i64 789311532 to i64*), align 8
  store volatile i32 1000, i32* inttoptr (i64 789311546 to i32*), align 4
  store volatile i8 1, i8* inttoptr (i64 789311550 to i8*), align 2
  store volatile i8 1, i8* inttoptr (i64 789311551 to i8*), align 1
  store volatile i8 2, i8* inttoptr (i64 789311520 to i8*), align 32
  store volatile i64 %feat_rd_addr, i64* inttoptr (i64 789311561 to i64*), align 8
  store volatile i64 789311582, i64* inttoptr (i64 789311569 to i64*), align 8
  store volatile i32 1280, i32* inttoptr (i64 789311577 to i32*), align 4
  store volatile i8 1, i8* inttoptr (i64 789311560 to i8*), align 8
  br label %1

; <label>:1                                       ; preds = %1, %0
  %2 = load volatile i8, i8* inttoptr (i64 789311560 to i8*), align 8
  %3 = and i8 %2, 4
  %4 = icmp eq i8 %3, 0
  br i1 %4, label %1, label %5

; <label>:5                                       ; preds = %1
  store volatile i64 %weights, i64* inttoptr (i64 789311561 to i64*), align 8
  store volatile i64 789312862, i64* inttoptr (i64 789311569 to i64*), align 8
  store volatile i32 1280000, i32* inttoptr (i64 789311577 to i32*), align 4
  store volatile i8 1, i8* inttoptr (i64 789311560 to i8*), align 8
  br label %6

; <label>:6                                       ; preds = %6, %5
  %7 = load volatile i8, i8* inttoptr (i64 789311560 to i8*), align 8
  %8 = and i8 %7, 4
  %9 = icmp eq i8 %8, 0
  br i1 %9, label %6, label %10

; <label>:10                                      ; preds = %6
  store volatile i64 %quant, i64* inttoptr (i64 789311561 to i64*), align 8
  store volatile i64 790592862, i64* inttoptr (i64 789311569 to i64*), align 8
  store volatile i32 6000, i32* inttoptr (i64 789311577 to i32*), align 4
  store volatile i8 1, i8* inttoptr (i64 789311560 to i8*), align 8
  br label %11

; <label>:11                                      ; preds = %11, %10
  %12 = load volatile i8, i8* inttoptr (i64 789311520 to i8*), align 32
  %13 = and i8 %12, 6
  %14 = icmp eq i8 %13, 6
  br i1 %14, label %15, label %11

; <label>:15                                      ; preds = %11
  ret void
}

attributes #0 = { norecurse nounwind uwtable "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.ident = !{!0}

!0 = !{!"clang version 3.8.1 "}
