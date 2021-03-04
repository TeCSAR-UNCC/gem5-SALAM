; ModuleID = 'top.bc'
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: norecurse nounwind uwtable
define void @top(i64 %mainMem) #0 {
entry:
  store volatile i64 2415919104, i64* inttoptr (i64 268566529 to i64*), align 8
  store volatile i64 268566559, i64* inttoptr (i64 268566537 to i64*), align 8
  store volatile i32 1024, i32* inttoptr (i64 268566545 to i32*), align 4
  store volatile i8 1, i8* inttoptr (i64 268566528 to i8*), align 131072
  br label %while.cond

while.cond:                                       ; preds = %while.cond, %entry
  %0 = load volatile i8, i8* inttoptr (i64 268566528 to i8*), align 131072
  %and = and i8 %0, 4
  %cmp = icmp eq i8 %and, 0
  br i1 %cmp, label %while.cond, label %while.end

while.end:                                        ; preds = %while.cond
  store volatile i64 2415919104, i64* inttoptr (i64 268566529 to i64*), align 8
  store volatile i64 268567583, i64* inttoptr (i64 268566537 to i64*), align 8
  store volatile i32 150, i32* inttoptr (i64 268566545 to i32*), align 4
  store volatile i8 1, i8* inttoptr (i64 268566528 to i8*), align 131072
  br label %while.cond2

while.cond2:                                      ; preds = %while.cond2, %while.end
  %1 = load volatile i8, i8* inttoptr (i64 268566528 to i8*), align 131072
  %and4 = and i8 %1, 4
  %cmp5 = icmp eq i8 %and4, 0
  br i1 %cmp5, label %while.cond2, label %while.end8

while.end8:                                       ; preds = %while.cond2
  store volatile i8 1, i8* inttoptr (i64 268566558 to i8*), align 2
  br label %while.cond9

while.cond9:                                      ; preds = %while.cond9, %while.end8
  %2 = load volatile i8, i8* inttoptr (i64 268566558 to i8*), align 2
  %and11 = and i8 %2, 4
  %cmp12 = icmp eq i8 %and11, 0
  br i1 %cmp12, label %while.cond9, label %while.end15

while.end15:                                      ; preds = %while.cond9
  store volatile i64 268567733, i64* inttoptr (i64 268566529 to i64*), align 8
  store volatile i64 2415919104, i64* inttoptr (i64 268566537 to i64*), align 8
  store volatile i32 4704, i32* inttoptr (i64 268566545 to i32*), align 4
  store volatile i8 1, i8* inttoptr (i64 268566528 to i8*), align 131072
  br label %while.cond16

while.cond16:                                     ; preds = %while.cond16, %while.end15
  %3 = load volatile i8, i8* inttoptr (i64 268566528 to i8*), align 131072
  %and18 = and i8 %3, 4
  %cmp19 = icmp eq i8 %and18, 0
  br i1 %cmp19, label %while.cond16, label %while.end22

while.end22:                                      ; preds = %while.cond16
  store volatile i64 2415919104, i64* inttoptr (i64 268566529 to i64*), align 8
  store volatile i64 268572438, i64* inttoptr (i64 268566537 to i64*), align 8
  store volatile i32 4704, i32* inttoptr (i64 268566545 to i32*), align 4
  store volatile i8 1, i8* inttoptr (i64 268566528 to i8*), align 131072
  br label %while.cond23

while.cond23:                                     ; preds = %while.cond23, %while.end22
  %4 = load volatile i8, i8* inttoptr (i64 268566528 to i8*), align 131072
  %and25 = and i8 %4, 4
  %cmp26 = icmp eq i8 %and25, 0
  br i1 %cmp26, label %while.cond23, label %while.end29

while.end29:                                      ; preds = %while.cond23
  store volatile i8 1, i8* inttoptr (i64 268572437 to i8*), align 1
  br label %while.cond30

while.cond30:                                     ; preds = %while.cond30, %while.end29
  %5 = load volatile i8, i8* inttoptr (i64 268566558 to i8*), align 2
  %and32 = and i8 %5, 4
  %cmp33 = icmp eq i8 %and32, 0
  br i1 %cmp33, label %while.cond30, label %while.end36

while.end36:                                      ; preds = %while.cond30
  store volatile i64 268577142, i64* inttoptr (i64 268566529 to i64*), align 8
  store volatile i64 2415919104, i64* inttoptr (i64 268566537 to i64*), align 8
  store volatile i32 1176, i32* inttoptr (i64 268566545 to i32*), align 4
  store volatile i8 1, i8* inttoptr (i64 268566528 to i8*), align 131072
  br label %while.cond37

while.cond37:                                     ; preds = %while.cond37, %while.end36
  %6 = load volatile i8, i8* inttoptr (i64 268566528 to i8*), align 131072
  %and39 = and i8 %6, 4
  %cmp40 = icmp eq i8 %and39, 0
  br i1 %cmp40, label %while.cond37, label %while.end43

while.end43:                                      ; preds = %while.cond37
  store volatile i64 2415919104, i64* inttoptr (i64 268566529 to i64*), align 8
  store volatile i64 268578319, i64* inttoptr (i64 268566537 to i64*), align 8
  store volatile i32 1176, i32* inttoptr (i64 268566545 to i32*), align 4
  store volatile i8 1, i8* inttoptr (i64 268566528 to i8*), align 131072
  br label %while.cond44

while.cond44:                                     ; preds = %while.cond44, %while.end43
  %7 = load volatile i8, i8* inttoptr (i64 268566528 to i8*), align 131072
  %and46 = and i8 %7, 4
  %cmp47 = icmp eq i8 %and46, 0
  br i1 %cmp47, label %while.cond44, label %while.end50

while.end50:                                      ; preds = %while.cond44
  store volatile i64 2415919104, i64* inttoptr (i64 268566529 to i64*), align 8
  store volatile i64 268579495, i64* inttoptr (i64 268566537 to i64*), align 8
  store volatile i32 400, i32* inttoptr (i64 268566545 to i32*), align 4
  store volatile i8 1, i8* inttoptr (i64 268566528 to i8*), align 131072
  br label %while.cond51

while.cond51:                                     ; preds = %while.cond51, %while.end50
  %8 = load volatile i8, i8* inttoptr (i64 268566528 to i8*), align 131072
  %and53 = and i8 %8, 4
  %cmp54 = icmp eq i8 %and53, 0
  br i1 %cmp54, label %while.cond51, label %while.end57

while.end57:                                      ; preds = %while.cond51
  store volatile i8 1, i8* inttoptr (i64 268578318 to i8*), align 2
  br label %while.cond58

while.cond58:                                     ; preds = %while.cond58, %while.end57
  %9 = load volatile i8, i8* inttoptr (i64 268578318 to i8*), align 2
  %and60 = and i8 %9, 4
  %cmp61 = icmp eq i8 %and60, 0
  br i1 %cmp61, label %while.cond58, label %while.end64

while.end64:                                      ; preds = %while.cond58
  store volatile i64 268579895, i64* inttoptr (i64 268566529 to i64*), align 8
  store volatile i64 2415919104, i64* inttoptr (i64 268566537 to i64*), align 8
  store volatile i32 1600, i32* inttoptr (i64 268566545 to i32*), align 4
  store volatile i8 1, i8* inttoptr (i64 268566528 to i8*), align 131072
  br label %while.cond65

while.cond65:                                     ; preds = %while.cond65, %while.end64
  %10 = load volatile i8, i8* inttoptr (i64 268566528 to i8*), align 131072
  %and67 = and i8 %10, 4
  %cmp68 = icmp eq i8 %and67, 0
  br i1 %cmp68, label %while.cond65, label %while.end71

while.end71:                                      ; preds = %while.cond65
  store volatile i64 2415919104, i64* inttoptr (i64 268566529 to i64*), align 8
  store volatile i64 268581496, i64* inttoptr (i64 268566537 to i64*), align 8
  store volatile i32 1600, i32* inttoptr (i64 268566545 to i32*), align 4
  store volatile i8 1, i8* inttoptr (i64 268566528 to i8*), align 131072
  br label %while.cond72

while.cond72:                                     ; preds = %while.cond72, %while.end71
  %11 = load volatile i8, i8* inttoptr (i64 268566528 to i8*), align 131072
  %and74 = and i8 %11, 4
  %cmp75 = icmp eq i8 %and74, 0
  br i1 %cmp75, label %while.cond72, label %while.end78

while.end78:                                      ; preds = %while.cond72
  store volatile i8 1, i8* inttoptr (i64 268581495 to i8*), align 1
  br label %while.cond79

while.cond79:                                     ; preds = %while.cond79, %while.end78
  %12 = load volatile i8, i8* inttoptr (i64 268581495 to i8*), align 1
  %and81 = and i8 %12, 4
  %cmp82 = icmp eq i8 %and81, 0
  br i1 %cmp82, label %while.cond79, label %while.end85

while.end85:                                      ; preds = %while.cond79
  store volatile i64 268583096, i64* inttoptr (i64 268566529 to i64*), align 8
  store volatile i64 2415919104, i64* inttoptr (i64 268566537 to i64*), align 8
  store volatile i32 400, i32* inttoptr (i64 268566545 to i32*), align 4
  store volatile i8 1, i8* inttoptr (i64 268566528 to i8*), align 131072
  br label %while.cond86

while.cond86:                                     ; preds = %while.cond86, %while.end85
  %13 = load volatile i8, i8* inttoptr (i64 268566528 to i8*), align 131072
  %and88 = and i8 %13, 4
  %cmp89 = icmp eq i8 %and88, 0
  br i1 %cmp89, label %while.cond86, label %while.end92

while.end92:                                      ; preds = %while.cond86
  ret void
}

attributes #0 = { norecurse nounwind uwtable "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.ident = !{!0}

!0 = !{!"clang version 3.8.1 "}
