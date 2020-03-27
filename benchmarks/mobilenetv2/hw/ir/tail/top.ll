; ModuleID = 'top.bc'
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: norecurse nounwind uwtable
define void @top(i64 %feat_rd_addr, i64 %feat_wr_addr, i64 %pw_weights, i64 %pw_quant) #0 {
  store volatile i8 1, i8* inttoptr (i64 788942941 to i8*), align 1
  store volatile i8 1, i8* inttoptr (i64 788942942 to i8*), align 2
  store volatile i8 1, i8* inttoptr (i64 788942943 to i8*), align 1
  store volatile i64 %feat_rd_addr, i64* inttoptr (i64 788942884 to i64*), align 8
  store volatile i32 6000, i32* inttoptr (i64 788942900 to i32*), align 4
  store volatile i8 1, i8* inttoptr (i64 788942904 to i8*), align 8
  store volatile i8 1, i8* inttoptr (i64 788942905 to i8*), align 1
  store volatile i64 %feat_wr_addr, i64* inttoptr (i64 788942892 to i64*), align 8
  store volatile i32 1280, i32* inttoptr (i64 788942906 to i32*), align 4
  store volatile i8 1, i8* inttoptr (i64 788942910 to i8*), align 2
  store volatile i8 1, i8* inttoptr (i64 788942911 to i8*), align 1
  store volatile i8 3, i8* inttoptr (i64 788942880 to i8*), align 32
  store volatile i64 %pw_weights, i64* inttoptr (i64 788942921 to i64*), align 8
  store volatile i64 788942944, i64* inttoptr (i64 788942929 to i64*), align 8
  store volatile i32 307200, i32* inttoptr (i64 788942937 to i32*), align 4
  store volatile i8 1, i8* inttoptr (i64 788942920 to i8*), align 8
  br label %1

; <label>:1                                       ; preds = %1, %0
  %2 = load volatile i8, i8* inttoptr (i64 788942920 to i8*), align 8
  %3 = and i8 %2, 4
  %4 = icmp eq i8 %3, 0
  br i1 %4, label %1, label %5

; <label>:5                                       ; preds = %1
  store volatile i64 %pw_quant, i64* inttoptr (i64 788942921 to i64*), align 8
  store volatile i64 789250144, i64* inttoptr (i64 788942929 to i64*), align 8
  store volatile i32 7680, i32* inttoptr (i64 788942937 to i32*), align 4
  store volatile i8 1, i8* inttoptr (i64 788942920 to i8*), align 8
  br label %6

; <label>:6                                       ; preds = %6, %5
  %7 = load volatile i8, i8* inttoptr (i64 788942880 to i8*), align 32
  %8 = and i8 %7, 6
  %9 = icmp eq i8 %8, 6
  br i1 %9, label %10, label %6

; <label>:10                                      ; preds = %6
  ret void
}

attributes #0 = { norecurse nounwind uwtable "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.ident = !{!0}

!0 = !{!"clang version 3.8.1 "}
