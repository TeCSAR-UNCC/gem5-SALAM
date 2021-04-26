; ModuleID = 'top.bc'
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: norecurse nounwind uwtable
define void @top(i64 %mainMem) #0 {
entry:
  store volatile i64 2415919104, i64* inttoptr (i64 268566553 to i64*), align 8
  store volatile i32 4096, i32* inttoptr (i64 268566569 to i32*), align 4
  store volatile i8 2, i8* inttoptr (i64 268566573 to i8*), align 1
  store volatile i8 1, i8* inttoptr (i64 268566574 to i8*), align 2
  store volatile i64 2415919119, i64* inttoptr (i64 268566561 to i64*), align 8
  store volatile i32 336, i32* inttoptr (i64 268566575 to i32*), align 4
  store volatile i8 1, i8* inttoptr (i64 268566579 to i8*), align 1
  store volatile i8 1, i8* inttoptr (i64 268566580 to i8*), align 4
  store volatile i8 3, i8* inttoptr (i64 268566549 to i8*), align 1
  store volatile i64 2415919104, i64* inttoptr (i64 268566529 to i64*), align 8
  store volatile i64 268566699, i64* inttoptr (i64 268566537 to i64*), align 8
  store volatile i32 600, i32* inttoptr (i64 268566545 to i32*), align 4
  store volatile i8 1, i8* inttoptr (i64 268566528 to i8*), align 131072
  br label %while.cond

while.cond:                                       ; preds = %while.cond, %entry
  %0 = load volatile i8, i8* inttoptr (i64 268566528 to i8*), align 131072
  %and = and i8 %0, 4
  %cmp = icmp eq i8 %and, 0
  br i1 %cmp, label %while.cond, label %while.end

while.end:                                        ; preds = %while.cond
  store volatile i64 2415919104, i64* inttoptr (i64 268566529 to i64*), align 8
  store volatile i64 268570337, i64* inttoptr (i64 268566537 to i64*), align 8
  store volatile i32 9600, i32* inttoptr (i64 268566545 to i32*), align 4
  store volatile i8 1, i8* inttoptr (i64 268566528 to i8*), align 131072
  br label %while.cond2

while.cond2:                                      ; preds = %while.cond2, %while.end
  %1 = load volatile i8, i8* inttoptr (i64 268566528 to i8*), align 131072
  %and4 = and i8 %1, 4
  %cmp5 = icmp eq i8 %and4, 0
  br i1 %cmp5, label %while.cond2, label %while.end8

while.end8:                                       ; preds = %while.cond2
  store volatile i8 1, i8* inttoptr (i64 268583154 to i8*), align 2
  store volatile i8 1, i8* inttoptr (i64 268777239 to i8*), align 1
  br label %while.cond9

while.cond9:                                      ; preds = %while.cond9, %while.end8
  %2 = load volatile i8, i8* inttoptr (i64 268777239 to i8*), align 1
  %and11 = and i8 %2, 4
  %cmp12 = icmp eq i8 %and11, 0
  br i1 %cmp12, label %while.cond9, label %while.cond16.preheader

while.cond16.preheader:                           ; preds = %while.cond9
  br label %while.cond16

while.cond16:                                     ; preds = %while.cond16.preheader, %while.cond16
  %3 = load volatile i8, i8* inttoptr (i64 268566549 to i8*), align 1
  %and18 = and i8 %3, 8
  %cmp19 = icmp eq i8 %and18, 0
  br i1 %cmp19, label %while.end22, label %while.cond16

while.end22:                                      ; preds = %while.cond16
  ret void
}

attributes #0 = { norecurse nounwind uwtable "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.ident = !{!0}

!0 = !{!"clang version 3.8.1 "}
