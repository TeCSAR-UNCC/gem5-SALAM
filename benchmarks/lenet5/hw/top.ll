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
  store volatile i64 268574751, i64* inttoptr (i64 268566537 to i64*), align 8
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
  store volatile i64 268575951, i64* inttoptr (i64 268566529 to i64*), align 8
  store volatile i64 2415919104, i64* inttoptr (i64 268566537 to i64*), align 8
  store volatile i32 37632, i32* inttoptr (i64 268566545 to i32*), align 4
  store volatile i8 1, i8* inttoptr (i64 268566528 to i8*), align 131072
  br label %while.cond16

while.cond16:                                     ; preds = %while.cond16, %while.end15
  %3 = load volatile i8, i8* inttoptr (i64 268566528 to i8*), align 131072
  %and18 = and i8 %3, 4
  %cmp19 = icmp eq i8 %and18, 0
  br i1 %cmp19, label %while.cond16, label %while.end22

while.end22:                                      ; preds = %while.cond16
  store volatile i64 2415919104, i64* inttoptr (i64 268566529 to i64*), align 8
  store volatile i64 268613584, i64* inttoptr (i64 268566537 to i64*), align 8
  store volatile i32 37632, i32* inttoptr (i64 268566545 to i32*), align 4
  store volatile i8 1, i8* inttoptr (i64 268566528 to i8*), align 131072
  br label %while.cond23

while.cond23:                                     ; preds = %while.cond23, %while.end22
  %4 = load volatile i8, i8* inttoptr (i64 268566528 to i8*), align 131072
  %and25 = and i8 %4, 4
  %cmp26 = icmp eq i8 %and25, 0
  br i1 %cmp26, label %while.cond23, label %while.end29

while.end29:                                      ; preds = %while.cond23
  store volatile i64 2415919104, i64* inttoptr (i64 268566529 to i64*), align 8
  store volatile i64 268651216, i64* inttoptr (i64 268566537 to i64*), align 8
  store volatile i32 192, i32* inttoptr (i64 268566545 to i32*), align 4
  store volatile i8 1, i8* inttoptr (i64 268566528 to i8*), align 131072
  br label %while.cond30

while.cond30:                                     ; preds = %while.cond30, %while.end29
  %5 = load volatile i8, i8* inttoptr (i64 268566528 to i8*), align 131072
  %and32 = and i8 %5, 4
  %cmp33 = icmp eq i8 %and32, 0
  br i1 %cmp33, label %while.cond30, label %while.end36

while.end36:                                      ; preds = %while.cond30
  store volatile i8 1, i8* inttoptr (i64 268613583 to i8*), align 1
  br label %while.cond37

while.cond37:                                     ; preds = %while.cond37, %while.end36
  %6 = load volatile i8, i8* inttoptr (i64 268566558 to i8*), align 2
  %and39 = and i8 %6, 4
  %cmp40 = icmp eq i8 %and39, 0
  br i1 %cmp40, label %while.cond37, label %while.end43

while.end43:                                      ; preds = %while.cond37
  store volatile i64 268651408, i64* inttoptr (i64 268566529 to i64*), align 8
  store volatile i64 2415919104, i64* inttoptr (i64 268566537 to i64*), align 8
  store volatile i32 9408, i32* inttoptr (i64 268566545 to i32*), align 4
  store volatile i8 1, i8* inttoptr (i64 268566528 to i8*), align 131072
  br label %while.cond44

while.cond44:                                     ; preds = %while.cond44, %while.end43
  %7 = load volatile i8, i8* inttoptr (i64 268566528 to i8*), align 131072
  %and46 = and i8 %7, 4
  %cmp47 = icmp eq i8 %and46, 0
  br i1 %cmp47, label %while.cond44, label %while.end50

while.end50:                                      ; preds = %while.cond44
  store volatile i64 2415919104, i64* inttoptr (i64 268566529 to i64*), align 8
  store volatile i64 268660817, i64* inttoptr (i64 268566537 to i64*), align 8
  store volatile i32 1176, i32* inttoptr (i64 268566545 to i32*), align 4
  store volatile i8 1, i8* inttoptr (i64 268566528 to i8*), align 131072
  br label %while.cond51

while.cond51:                                     ; preds = %while.cond51, %while.end50
  %8 = load volatile i8, i8* inttoptr (i64 268566528 to i8*), align 131072
  %and53 = and i8 %8, 4
  %cmp54 = icmp eq i8 %and53, 0
  br i1 %cmp54, label %while.cond51, label %while.end57

while.end57:                                      ; preds = %while.cond51
  store volatile i64 2415919104, i64* inttoptr (i64 268566529 to i64*), align 8
  store volatile i64 268670225, i64* inttoptr (i64 268566537 to i64*), align 8
  store volatile i32 400, i32* inttoptr (i64 268566545 to i32*), align 4
  store volatile i8 1, i8* inttoptr (i64 268566528 to i8*), align 131072
  br label %while.cond58

while.cond58:                                     ; preds = %while.cond58, %while.end57
  %9 = load volatile i8, i8* inttoptr (i64 268566528 to i8*), align 131072
  %and60 = and i8 %9, 4
  %cmp61 = icmp eq i8 %and60, 0
  br i1 %cmp61, label %while.cond58, label %while.end64

while.end64:                                      ; preds = %while.cond58
  store volatile i8 1, i8* inttoptr (i64 268660816 to i8*), align 16
  br label %while.cond65

while.cond65:                                     ; preds = %while.cond65, %while.end64
  %10 = load volatile i8, i8* inttoptr (i64 268660816 to i8*), align 16
  %and67 = and i8 %10, 4
  %cmp68 = icmp eq i8 %and67, 0
  br i1 %cmp68, label %while.cond65, label %while.end71

while.end71:                                      ; preds = %while.cond65
  store volatile i64 268673425, i64* inttoptr (i64 268566529 to i64*), align 8
  store volatile i64 2415919104, i64* inttoptr (i64 268566537 to i64*), align 8
  store volatile i32 12800, i32* inttoptr (i64 268566545 to i32*), align 4
  store volatile i8 1, i8* inttoptr (i64 268566528 to i8*), align 131072
  br label %while.cond72

while.cond72:                                     ; preds = %while.cond72, %while.end71
  %11 = load volatile i8, i8* inttoptr (i64 268566528 to i8*), align 131072
  %and74 = and i8 %11, 4
  %cmp75 = icmp eq i8 %and74, 0
  br i1 %cmp75, label %while.cond72, label %while.end78

while.end78:                                      ; preds = %while.cond72
  store volatile i64 2415919104, i64* inttoptr (i64 268566529 to i64*), align 8
  store volatile i64 268686226, i64* inttoptr (i64 268566537 to i64*), align 8
  store volatile i32 12800, i32* inttoptr (i64 268566545 to i32*), align 4
  store volatile i8 1, i8* inttoptr (i64 268566528 to i8*), align 131072
  br label %while.cond79

while.cond79:                                     ; preds = %while.cond79, %while.end78
  %12 = load volatile i8, i8* inttoptr (i64 268566528 to i8*), align 131072
  %and81 = and i8 %12, 4
  %cmp82 = icmp eq i8 %and81, 0
  br i1 %cmp82, label %while.cond79, label %while.end85

while.end85:                                      ; preds = %while.cond79
  store volatile i64 2415919104, i64* inttoptr (i64 268566529 to i64*), align 8
  store volatile i64 268699026, i64* inttoptr (i64 268566537 to i64*), align 8
  store volatile i32 512, i32* inttoptr (i64 268566545 to i32*), align 4
  store volatile i8 1, i8* inttoptr (i64 268566528 to i8*), align 131072
  br label %while.cond86

while.cond86:                                     ; preds = %while.cond86, %while.end85
  %13 = load volatile i8, i8* inttoptr (i64 268566528 to i8*), align 131072
  %and88 = and i8 %13, 4
  %cmp89 = icmp eq i8 %and88, 0
  br i1 %cmp89, label %while.cond86, label %while.end92

while.end92:                                      ; preds = %while.cond86
  store volatile i8 1, i8* inttoptr (i64 268686225 to i8*), align 1
  br label %while.cond93

while.cond93:                                     ; preds = %while.cond93, %while.end92
  %14 = load volatile i8, i8* inttoptr (i64 268686225 to i8*), align 1
  %and95 = and i8 %14, 4
  %cmp96 = icmp eq i8 %and95, 0
  br i1 %cmp96, label %while.cond93, label %while.end99

while.end99:                                      ; preds = %while.cond93
  store volatile i64 268699538, i64* inttoptr (i64 268566529 to i64*), align 8
  store volatile i64 2415919104, i64* inttoptr (i64 268566537 to i64*), align 8
  store volatile i32 3200, i32* inttoptr (i64 268566545 to i32*), align 4
  store volatile i8 1, i8* inttoptr (i64 268566528 to i8*), align 131072
  br label %while.cond100

while.cond100:                                    ; preds = %while.cond100, %while.end99
  %15 = load volatile i8, i8* inttoptr (i64 268566528 to i8*), align 131072
  %and102 = and i8 %15, 4
  %cmp103 = icmp eq i8 %and102, 0
  br i1 %cmp103, label %while.cond100, label %while.end106

while.end106:                                     ; preds = %while.cond100
  store volatile i64 2415919104, i64* inttoptr (i64 268566529 to i64*), align 8
  store volatile i64 268702739, i64* inttoptr (i64 268566537 to i64*), align 8
  store volatile i32 400, i32* inttoptr (i64 268566545 to i32*), align 4
  store volatile i8 1, i8* inttoptr (i64 268566528 to i8*), align 131072
  br label %while.cond107

while.cond107:                                    ; preds = %while.cond107, %while.end106
  %16 = load volatile i8, i8* inttoptr (i64 268566528 to i8*), align 131072
  %and109 = and i8 %16, 4
  %cmp110 = icmp eq i8 %and109, 0
  br i1 %cmp110, label %while.cond107, label %while.end113

while.end113:                                     ; preds = %while.cond107
  store volatile i64 2415919104, i64* inttoptr (i64 268566529 to i64*), align 8
  store volatile i64 268705939, i64* inttoptr (i64 268566537 to i64*), align 8
  store volatile i32 25, i32* inttoptr (i64 268566545 to i32*), align 4
  store volatile i8 1, i8* inttoptr (i64 268566528 to i8*), align 131072
  br label %while.cond114

while.cond114:                                    ; preds = %while.cond114, %while.end113
  %17 = load volatile i8, i8* inttoptr (i64 268566528 to i8*), align 131072
  %and116 = and i8 %17, 4
  %cmp117 = icmp eq i8 %and116, 0
  br i1 %cmp117, label %while.cond114, label %while.end120

while.end120:                                     ; preds = %while.cond114
  store volatile i8 1, i8* inttoptr (i64 268702738 to i8*), align 2
  br label %while.cond121

while.cond121:                                    ; preds = %while.cond121, %while.end120
  %18 = load volatile i8, i8* inttoptr (i64 268702738 to i8*), align 2
  %and123 = and i8 %18, 4
  %cmp124 = icmp eq i8 %and123, 0
  br i1 %cmp124, label %while.cond121, label %while.end127

while.end127:                                     ; preds = %while.cond121
  store volatile i64 268729939, i64* inttoptr (i64 268566529 to i64*), align 8
  store volatile i64 2415919104, i64* inttoptr (i64 268566537 to i64*), align 8
  store volatile i32 960, i32* inttoptr (i64 268566545 to i32*), align 4
  store volatile i8 1, i8* inttoptr (i64 268566528 to i8*), align 131072
  br label %while.cond128

while.cond128:                                    ; preds = %while.cond128, %while.end127
  %19 = load volatile i8, i8* inttoptr (i64 268566528 to i8*), align 131072
  %and130 = and i8 %19, 4
  %cmp131 = icmp eq i8 %and130, 0
  br i1 %cmp131, label %while.cond128, label %while.end134

while.end134:                                     ; preds = %while.cond128
  ret void
}

attributes #0 = { norecurse nounwind uwtable "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.ident = !{!0}

!0 = !{!"clang version 3.8.1 "}
