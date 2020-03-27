; ModuleID = 'top.bc'
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: norecurse nounwind uwtable
define void @top(i64 %img_rd_addr, i64 %feat_wr_addr, i64 %conv_weights, i64 %conv_quant, i64 %dw_weights, i64 %dw_quant, i64 %pw_weights, i64 %pw_quant) #0 {
  store volatile i8 1, i8* inttoptr (i64 788529214 to i8*), align 2
  store volatile i8 1, i8* inttoptr (i64 788529215 to i8*), align 1
  store volatile i8 1, i8* inttoptr (i64 788529216 to i8*), align 64
  store volatile i64 %img_rd_addr, i64* inttoptr (i64 788529157 to i64*), align 8
  store volatile i32 76800, i32* inttoptr (i64 788529173 to i32*), align 4
  store volatile i8 1, i8* inttoptr (i64 788529177 to i8*), align 1
  store volatile i8 1, i8* inttoptr (i64 788529178 to i8*), align 2
  store volatile i64 %feat_wr_addr, i64* inttoptr (i64 788529165 to i64*), align 8
  store volatile i32 102400, i32* inttoptr (i64 788529179 to i32*), align 4
  store volatile i8 1, i8* inttoptr (i64 788529183 to i8*), align 1
  store volatile i8 1, i8* inttoptr (i64 788529184 to i8*), align 32
  store volatile i8 3, i8* inttoptr (i64 788529153 to i8*), align 1
  store volatile i64 %conv_weights, i64* inttoptr (i64 788529194 to i64*), align 8
  store volatile i64 788530204, i64* inttoptr (i64 788529202 to i64*), align 8
  store volatile i32 648, i32* inttoptr (i64 788529210 to i32*), align 4
  store volatile i8 1, i8* inttoptr (i64 788529193 to i8*), align 1
  br label %1

; <label>:1                                       ; preds = %1, %0
  %2 = load volatile i8, i8* inttoptr (i64 788529193 to i8*), align 1
  %3 = and i8 %2, 4
  %4 = icmp eq i8 %3, 0
  br i1 %4, label %1, label %5

; <label>:5                                       ; preds = %1
  store volatile i64 %conv_quant, i64* inttoptr (i64 788529194 to i64*), align 8
  store volatile i64 788530852, i64* inttoptr (i64 788529202 to i64*), align 8
  store volatile i32 144, i32* inttoptr (i64 788529210 to i32*), align 4
  store volatile i8 1, i8* inttoptr (i64 788529193 to i8*), align 1
  br label %6

; <label>:6                                       ; preds = %6, %5
  %7 = load volatile i8, i8* inttoptr (i64 788529193 to i8*), align 1
  %8 = and i8 %7, 4
  %9 = icmp eq i8 %8, 0
  br i1 %9, label %6, label %10

; <label>:10                                      ; preds = %6
  store volatile i64 %dw_weights, i64* inttoptr (i64 788529194 to i64*), align 8
  store volatile i64 788535053, i64* inttoptr (i64 788529202 to i64*), align 8
  store volatile i32 240, i32* inttoptr (i64 788529210 to i32*), align 4
  store volatile i8 1, i8* inttoptr (i64 788529193 to i8*), align 1
  br label %11

; <label>:11                                      ; preds = %11, %10
  %12 = load volatile i8, i8* inttoptr (i64 788529193 to i8*), align 1
  %13 = and i8 %12, 4
  %14 = icmp eq i8 %13, 0
  br i1 %14, label %11, label %15

; <label>:15                                      ; preds = %11
  store volatile i64 %dw_quant, i64* inttoptr (i64 788529194 to i64*), align 8
  store volatile i64 788535293, i64* inttoptr (i64 788529202 to i64*), align 8
  store volatile i32 144, i32* inttoptr (i64 788529210 to i32*), align 4
  store volatile i8 1, i8* inttoptr (i64 788529193 to i8*), align 1
  br label %16

; <label>:16                                      ; preds = %16, %15
  %17 = load volatile i8, i8* inttoptr (i64 788529193 to i8*), align 1
  %18 = and i8 %17, 4
  %19 = icmp eq i8 %18, 0
  br i1 %19, label %16, label %20

; <label>:20                                      ; preds = %16
  store volatile i64 %pw_weights, i64* inttoptr (i64 788529194 to i64*), align 8
  store volatile i64 788535462, i64* inttoptr (i64 788529202 to i64*), align 8
  store volatile i32 384, i32* inttoptr (i64 788529210 to i32*), align 4
  store volatile i8 1, i8* inttoptr (i64 788529193 to i8*), align 1
  br label %21

; <label>:21                                      ; preds = %21, %20
  %22 = load volatile i8, i8* inttoptr (i64 788529193 to i8*), align 1
  %23 = and i8 %22, 4
  %24 = icmp eq i8 %23, 0
  br i1 %24, label %21, label %25

; <label>:25                                      ; preds = %21
  store volatile i64 %pw_quant, i64* inttoptr (i64 788529194 to i64*), align 8
  store volatile i64 788535846, i64* inttoptr (i64 788529202 to i64*), align 8
  store volatile i32 96, i32* inttoptr (i64 788529210 to i32*), align 4
  store volatile i8 1, i8* inttoptr (i64 788529193 to i8*), align 1
  br label %26

; <label>:26                                      ; preds = %26, %25
  %27 = load volatile i8, i8* inttoptr (i64 788529153 to i8*), align 1
  %28 = and i8 %27, 6
  %29 = icmp eq i8 %28, 6
  br i1 %29, label %30, label %26

; <label>:30                                      ; preds = %26
  ret void
}

attributes #0 = { norecurse nounwind uwtable "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.ident = !{!0}

!0 = !{!"clang version 3.8.1 "}
