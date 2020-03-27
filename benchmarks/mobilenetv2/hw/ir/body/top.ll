; ModuleID = 'top.bc'
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: norecurse nounwind uwtable
define void @top(i8 zeroext %stage, i64 %feat_rd_addr, i64 %res_rd_addr, i64 %feat_wr_addr, i64 %pw0_weights, i64 %pw0_quant, i64 %dw0_weights, i64 %dw0_quant, i64 %pw1_weights, i64 %pw1_quant) #0 {
  %1 = zext i8 %stage to i32
  switch i32 %1, label %18 [
    i32 0, label %2
    i32 1, label %3
    i32 2, label %4
    i32 3, label %5
    i32 4, label %6
    i32 5, label %7
    i32 6, label %8
    i32 7, label %9
    i32 8, label %10
    i32 9, label %11
    i32 10, label %12
    i32 11, label %13
    i32 12, label %14
    i32 13, label %15
    i32 14, label %16
    i32 15, label %17
  ]

; <label>:2                                       ; preds = %0
  store volatile i32 102400, i32* inttoptr (i64 788537520 to i32*), align 16
  store volatile i32 80, i32* inttoptr (i64 788537524 to i32*), align 4
  store volatile i32 80, i32* inttoptr (i64 788537528 to i32*), align 8
  store volatile i32 16, i32* inttoptr (i64 788537532 to i32*), align 4
  store volatile i32 96, i32* inttoptr (i64 788537536 to i32*), align 64
  store volatile i8 0, i8* inttoptr (i64 788537540 to i8*), align 4
  store volatile i8 0, i8* inttoptr (i64 788537541 to i8*), align 1
  store volatile i8 0, i8* inttoptr (i64 788537542 to i8*), align 2
  store volatile i32 40, i32* inttoptr (i64 788537543 to i32*), align 4
  store volatile i32 80, i32* inttoptr (i64 788537547 to i32*), align 4
  store volatile i32 96, i32* inttoptr (i64 788537551 to i32*), align 4
  store volatile i32 96, i32* inttoptr (i64 788537555 to i32*), align 4
  store volatile i32 2, i32* inttoptr (i64 788537559 to i32*), align 4
  store volatile i8 0, i8* inttoptr (i64 788537563 to i8*), align 1
  store volatile i8 0, i8* inttoptr (i64 788537564 to i8*), align 4
  store volatile i8 0, i8* inttoptr (i64 788537565 to i8*), align 1
  store volatile i32 40, i32* inttoptr (i64 788537566 to i32*), align 4
  store volatile i32 40, i32* inttoptr (i64 788537570 to i32*), align 4
  store volatile i32 96, i32* inttoptr (i64 788537574 to i32*), align 4
  store volatile i32 24, i32* inttoptr (i64 788537578 to i32*), align 4
  store volatile i8 0, i8* inttoptr (i64 788537582 to i8*), align 2
  store volatile i8 0, i8* inttoptr (i64 788537583 to i8*), align 1
  store volatile i8 0, i8* inttoptr (i64 788537584 to i8*), align 16
  br label %19

; <label>:3                                       ; preds = %0
  store volatile i32 38400, i32* inttoptr (i64 788537520 to i32*), align 16
  store volatile i32 40, i32* inttoptr (i64 788537524 to i32*), align 4
  store volatile i32 40, i32* inttoptr (i64 788537528 to i32*), align 8
  store volatile i32 24, i32* inttoptr (i64 788537532 to i32*), align 4
  store volatile i32 144, i32* inttoptr (i64 788537536 to i32*), align 64
  store volatile i8 0, i8* inttoptr (i64 788537540 to i8*), align 4
  store volatile i8 0, i8* inttoptr (i64 788537541 to i8*), align 1
  store volatile i8 0, i8* inttoptr (i64 788537542 to i8*), align 2
  store volatile i32 40, i32* inttoptr (i64 788537543 to i32*), align 4
  store volatile i32 40, i32* inttoptr (i64 788537547 to i32*), align 4
  store volatile i32 144, i32* inttoptr (i64 788537551 to i32*), align 4
  store volatile i32 144, i32* inttoptr (i64 788537555 to i32*), align 4
  store volatile i32 1, i32* inttoptr (i64 788537559 to i32*), align 4
  store volatile i8 0, i8* inttoptr (i64 788537563 to i8*), align 1
  store volatile i8 0, i8* inttoptr (i64 788537564 to i8*), align 4
  store volatile i8 0, i8* inttoptr (i64 788537565 to i8*), align 1
  store volatile i32 40, i32* inttoptr (i64 788537566 to i32*), align 4
  store volatile i32 40, i32* inttoptr (i64 788537570 to i32*), align 4
  store volatile i32 144, i32* inttoptr (i64 788537574 to i32*), align 4
  store volatile i32 24, i32* inttoptr (i64 788537578 to i32*), align 4
  store volatile i8 0, i8* inttoptr (i64 788537582 to i8*), align 2
  store volatile i8 0, i8* inttoptr (i64 788537583 to i8*), align 1
  store volatile i8 0, i8* inttoptr (i64 788537584 to i8*), align 16
  br label %19

; <label>:4                                       ; preds = %0
  store volatile i32 38400, i32* inttoptr (i64 788537520 to i32*), align 16
  store volatile i32 40, i32* inttoptr (i64 788537524 to i32*), align 4
  store volatile i32 40, i32* inttoptr (i64 788537528 to i32*), align 8
  store volatile i32 24, i32* inttoptr (i64 788537532 to i32*), align 4
  store volatile i32 144, i32* inttoptr (i64 788537536 to i32*), align 64
  store volatile i8 0, i8* inttoptr (i64 788537540 to i8*), align 4
  store volatile i8 0, i8* inttoptr (i64 788537541 to i8*), align 1
  store volatile i8 0, i8* inttoptr (i64 788537542 to i8*), align 2
  store volatile i32 20, i32* inttoptr (i64 788537543 to i32*), align 4
  store volatile i32 40, i32* inttoptr (i64 788537547 to i32*), align 4
  store volatile i32 144, i32* inttoptr (i64 788537551 to i32*), align 4
  store volatile i32 144, i32* inttoptr (i64 788537555 to i32*), align 4
  store volatile i32 2, i32* inttoptr (i64 788537559 to i32*), align 4
  store volatile i8 0, i8* inttoptr (i64 788537563 to i8*), align 1
  store volatile i8 0, i8* inttoptr (i64 788537564 to i8*), align 4
  store volatile i8 0, i8* inttoptr (i64 788537565 to i8*), align 1
  store volatile i32 20, i32* inttoptr (i64 788537566 to i32*), align 4
  store volatile i32 20, i32* inttoptr (i64 788537570 to i32*), align 4
  store volatile i32 144, i32* inttoptr (i64 788537574 to i32*), align 4
  store volatile i32 24, i32* inttoptr (i64 788537578 to i32*), align 4
  store volatile i8 0, i8* inttoptr (i64 788537582 to i8*), align 2
  store volatile i8 0, i8* inttoptr (i64 788537583 to i8*), align 1
  store volatile i8 0, i8* inttoptr (i64 788537584 to i8*), align 16
  br label %19

; <label>:5                                       ; preds = %0
  store volatile i32 9600, i32* inttoptr (i64 788537520 to i32*), align 16
  store volatile i32 20, i32* inttoptr (i64 788537524 to i32*), align 4
  store volatile i32 20, i32* inttoptr (i64 788537528 to i32*), align 8
  store volatile i32 24, i32* inttoptr (i64 788537532 to i32*), align 4
  store volatile i32 144, i32* inttoptr (i64 788537536 to i32*), align 64
  store volatile i8 0, i8* inttoptr (i64 788537540 to i8*), align 4
  store volatile i8 0, i8* inttoptr (i64 788537541 to i8*), align 1
  store volatile i8 0, i8* inttoptr (i64 788537542 to i8*), align 2
  store volatile i32 20, i32* inttoptr (i64 788537543 to i32*), align 4
  store volatile i32 20, i32* inttoptr (i64 788537547 to i32*), align 4
  store volatile i32 144, i32* inttoptr (i64 788537551 to i32*), align 4
  store volatile i32 144, i32* inttoptr (i64 788537555 to i32*), align 4
  store volatile i32 1, i32* inttoptr (i64 788537559 to i32*), align 4
  store volatile i8 0, i8* inttoptr (i64 788537563 to i8*), align 1
  store volatile i8 0, i8* inttoptr (i64 788537564 to i8*), align 4
  store volatile i8 0, i8* inttoptr (i64 788537565 to i8*), align 1
  store volatile i32 20, i32* inttoptr (i64 788537566 to i32*), align 4
  store volatile i32 20, i32* inttoptr (i64 788537570 to i32*), align 4
  store volatile i32 144, i32* inttoptr (i64 788537574 to i32*), align 4
  store volatile i32 24, i32* inttoptr (i64 788537578 to i32*), align 4
  store volatile i8 0, i8* inttoptr (i64 788537582 to i8*), align 2
  store volatile i8 0, i8* inttoptr (i64 788537583 to i8*), align 1
  store volatile i8 0, i8* inttoptr (i64 788537584 to i8*), align 16
  br label %19

; <label>:6                                       ; preds = %0
  store volatile i32 9600, i32* inttoptr (i64 788537520 to i32*), align 16
  store volatile i32 20, i32* inttoptr (i64 788537524 to i32*), align 4
  store volatile i32 20, i32* inttoptr (i64 788537528 to i32*), align 8
  store volatile i32 24, i32* inttoptr (i64 788537532 to i32*), align 4
  store volatile i32 144, i32* inttoptr (i64 788537536 to i32*), align 64
  store volatile i8 0, i8* inttoptr (i64 788537540 to i8*), align 4
  store volatile i8 0, i8* inttoptr (i64 788537541 to i8*), align 1
  store volatile i8 0, i8* inttoptr (i64 788537542 to i8*), align 2
  store volatile i32 20, i32* inttoptr (i64 788537543 to i32*), align 4
  store volatile i32 20, i32* inttoptr (i64 788537547 to i32*), align 4
  store volatile i32 144, i32* inttoptr (i64 788537551 to i32*), align 4
  store volatile i32 144, i32* inttoptr (i64 788537555 to i32*), align 4
  store volatile i32 1, i32* inttoptr (i64 788537559 to i32*), align 4
  store volatile i8 0, i8* inttoptr (i64 788537563 to i8*), align 1
  store volatile i8 0, i8* inttoptr (i64 788537564 to i8*), align 4
  store volatile i8 0, i8* inttoptr (i64 788537565 to i8*), align 1
  store volatile i32 20, i32* inttoptr (i64 788537566 to i32*), align 4
  store volatile i32 20, i32* inttoptr (i64 788537570 to i32*), align 4
  store volatile i32 144, i32* inttoptr (i64 788537574 to i32*), align 4
  store volatile i32 24, i32* inttoptr (i64 788537578 to i32*), align 4
  store volatile i8 0, i8* inttoptr (i64 788537582 to i8*), align 2
  store volatile i8 0, i8* inttoptr (i64 788537583 to i8*), align 1
  store volatile i8 0, i8* inttoptr (i64 788537584 to i8*), align 16
  br label %19

; <label>:7                                       ; preds = %0
  store volatile i32 9600, i32* inttoptr (i64 788537520 to i32*), align 16
  store volatile i32 20, i32* inttoptr (i64 788537524 to i32*), align 4
  store volatile i32 20, i32* inttoptr (i64 788537528 to i32*), align 8
  store volatile i32 24, i32* inttoptr (i64 788537532 to i32*), align 4
  store volatile i32 144, i32* inttoptr (i64 788537536 to i32*), align 64
  store volatile i8 0, i8* inttoptr (i64 788537540 to i8*), align 4
  store volatile i8 0, i8* inttoptr (i64 788537541 to i8*), align 1
  store volatile i8 0, i8* inttoptr (i64 788537542 to i8*), align 2
  store volatile i32 10, i32* inttoptr (i64 788537543 to i32*), align 4
  store volatile i32 20, i32* inttoptr (i64 788537547 to i32*), align 4
  store volatile i32 144, i32* inttoptr (i64 788537551 to i32*), align 4
  store volatile i32 144, i32* inttoptr (i64 788537555 to i32*), align 4
  store volatile i32 2, i32* inttoptr (i64 788537559 to i32*), align 4
  store volatile i8 0, i8* inttoptr (i64 788537563 to i8*), align 1
  store volatile i8 0, i8* inttoptr (i64 788537564 to i8*), align 4
  store volatile i8 0, i8* inttoptr (i64 788537565 to i8*), align 1
  store volatile i32 10, i32* inttoptr (i64 788537566 to i32*), align 4
  store volatile i32 10, i32* inttoptr (i64 788537570 to i32*), align 4
  store volatile i32 144, i32* inttoptr (i64 788537574 to i32*), align 4
  store volatile i32 48, i32* inttoptr (i64 788537578 to i32*), align 4
  store volatile i8 0, i8* inttoptr (i64 788537582 to i8*), align 2
  store volatile i8 0, i8* inttoptr (i64 788537583 to i8*), align 1
  store volatile i8 0, i8* inttoptr (i64 788537584 to i8*), align 16
  br label %19

; <label>:8                                       ; preds = %0
  store volatile i32 4800, i32* inttoptr (i64 788537520 to i32*), align 16
  store volatile i32 10, i32* inttoptr (i64 788537524 to i32*), align 4
  store volatile i32 10, i32* inttoptr (i64 788537528 to i32*), align 8
  store volatile i32 48, i32* inttoptr (i64 788537532 to i32*), align 4
  store volatile i32 288, i32* inttoptr (i64 788537536 to i32*), align 64
  store volatile i8 0, i8* inttoptr (i64 788537540 to i8*), align 4
  store volatile i8 0, i8* inttoptr (i64 788537541 to i8*), align 1
  store volatile i8 0, i8* inttoptr (i64 788537542 to i8*), align 2
  store volatile i32 10, i32* inttoptr (i64 788537543 to i32*), align 4
  store volatile i32 10, i32* inttoptr (i64 788537547 to i32*), align 4
  store volatile i32 288, i32* inttoptr (i64 788537551 to i32*), align 4
  store volatile i32 288, i32* inttoptr (i64 788537555 to i32*), align 4
  store volatile i32 1, i32* inttoptr (i64 788537559 to i32*), align 4
  store volatile i8 0, i8* inttoptr (i64 788537563 to i8*), align 1
  store volatile i8 0, i8* inttoptr (i64 788537564 to i8*), align 4
  store volatile i8 0, i8* inttoptr (i64 788537565 to i8*), align 1
  store volatile i32 10, i32* inttoptr (i64 788537566 to i32*), align 4
  store volatile i32 10, i32* inttoptr (i64 788537570 to i32*), align 4
  store volatile i32 288, i32* inttoptr (i64 788537574 to i32*), align 4
  store volatile i32 48, i32* inttoptr (i64 788537578 to i32*), align 4
  store volatile i8 0, i8* inttoptr (i64 788537582 to i8*), align 2
  store volatile i8 0, i8* inttoptr (i64 788537583 to i8*), align 1
  store volatile i8 0, i8* inttoptr (i64 788537584 to i8*), align 16
  br label %19

; <label>:9                                       ; preds = %0
  store volatile i32 4800, i32* inttoptr (i64 788537520 to i32*), align 16
  store volatile i32 10, i32* inttoptr (i64 788537524 to i32*), align 4
  store volatile i32 10, i32* inttoptr (i64 788537528 to i32*), align 8
  store volatile i32 48, i32* inttoptr (i64 788537532 to i32*), align 4
  store volatile i32 288, i32* inttoptr (i64 788537536 to i32*), align 64
  store volatile i8 0, i8* inttoptr (i64 788537540 to i8*), align 4
  store volatile i8 0, i8* inttoptr (i64 788537541 to i8*), align 1
  store volatile i8 0, i8* inttoptr (i64 788537542 to i8*), align 2
  store volatile i32 10, i32* inttoptr (i64 788537543 to i32*), align 4
  store volatile i32 10, i32* inttoptr (i64 788537547 to i32*), align 4
  store volatile i32 288, i32* inttoptr (i64 788537551 to i32*), align 4
  store volatile i32 288, i32* inttoptr (i64 788537555 to i32*), align 4
  store volatile i32 1, i32* inttoptr (i64 788537559 to i32*), align 4
  store volatile i8 0, i8* inttoptr (i64 788537563 to i8*), align 1
  store volatile i8 0, i8* inttoptr (i64 788537564 to i8*), align 4
  store volatile i8 0, i8* inttoptr (i64 788537565 to i8*), align 1
  store volatile i32 10, i32* inttoptr (i64 788537566 to i32*), align 4
  store volatile i32 10, i32* inttoptr (i64 788537570 to i32*), align 4
  store volatile i32 288, i32* inttoptr (i64 788537574 to i32*), align 4
  store volatile i32 48, i32* inttoptr (i64 788537578 to i32*), align 4
  store volatile i8 0, i8* inttoptr (i64 788537582 to i8*), align 2
  store volatile i8 0, i8* inttoptr (i64 788537583 to i8*), align 1
  store volatile i8 0, i8* inttoptr (i64 788537584 to i8*), align 16
  br label %19

; <label>:10                                      ; preds = %0
  store volatile i32 4800, i32* inttoptr (i64 788537520 to i32*), align 16
  store volatile i32 10, i32* inttoptr (i64 788537524 to i32*), align 4
  store volatile i32 10, i32* inttoptr (i64 788537528 to i32*), align 8
  store volatile i32 48, i32* inttoptr (i64 788537532 to i32*), align 4
  store volatile i32 288, i32* inttoptr (i64 788537536 to i32*), align 64
  store volatile i8 0, i8* inttoptr (i64 788537540 to i8*), align 4
  store volatile i8 0, i8* inttoptr (i64 788537541 to i8*), align 1
  store volatile i8 0, i8* inttoptr (i64 788537542 to i8*), align 2
  store volatile i32 10, i32* inttoptr (i64 788537543 to i32*), align 4
  store volatile i32 10, i32* inttoptr (i64 788537547 to i32*), align 4
  store volatile i32 288, i32* inttoptr (i64 788537551 to i32*), align 4
  store volatile i32 288, i32* inttoptr (i64 788537555 to i32*), align 4
  store volatile i32 1, i32* inttoptr (i64 788537559 to i32*), align 4
  store volatile i8 0, i8* inttoptr (i64 788537563 to i8*), align 1
  store volatile i8 0, i8* inttoptr (i64 788537564 to i8*), align 4
  store volatile i8 0, i8* inttoptr (i64 788537565 to i8*), align 1
  store volatile i32 10, i32* inttoptr (i64 788537566 to i32*), align 4
  store volatile i32 10, i32* inttoptr (i64 788537570 to i32*), align 4
  store volatile i32 288, i32* inttoptr (i64 788537574 to i32*), align 4
  store volatile i32 48, i32* inttoptr (i64 788537578 to i32*), align 4
  store volatile i8 0, i8* inttoptr (i64 788537582 to i8*), align 2
  store volatile i8 0, i8* inttoptr (i64 788537583 to i8*), align 1
  store volatile i8 0, i8* inttoptr (i64 788537584 to i8*), align 16
  br label %19

; <label>:11                                      ; preds = %0
  store volatile i32 4800, i32* inttoptr (i64 788537520 to i32*), align 16
  store volatile i32 10, i32* inttoptr (i64 788537524 to i32*), align 4
  store volatile i32 10, i32* inttoptr (i64 788537528 to i32*), align 8
  store volatile i32 48, i32* inttoptr (i64 788537532 to i32*), align 4
  store volatile i32 288, i32* inttoptr (i64 788537536 to i32*), align 64
  store volatile i8 0, i8* inttoptr (i64 788537540 to i8*), align 4
  store volatile i8 0, i8* inttoptr (i64 788537541 to i8*), align 1
  store volatile i8 0, i8* inttoptr (i64 788537542 to i8*), align 2
  store volatile i32 10, i32* inttoptr (i64 788537543 to i32*), align 4
  store volatile i32 10, i32* inttoptr (i64 788537547 to i32*), align 4
  store volatile i32 288, i32* inttoptr (i64 788537551 to i32*), align 4
  store volatile i32 288, i32* inttoptr (i64 788537555 to i32*), align 4
  store volatile i32 1, i32* inttoptr (i64 788537559 to i32*), align 4
  store volatile i8 0, i8* inttoptr (i64 788537563 to i8*), align 1
  store volatile i8 0, i8* inttoptr (i64 788537564 to i8*), align 4
  store volatile i8 0, i8* inttoptr (i64 788537565 to i8*), align 1
  store volatile i32 10, i32* inttoptr (i64 788537566 to i32*), align 4
  store volatile i32 10, i32* inttoptr (i64 788537570 to i32*), align 4
  store volatile i32 288, i32* inttoptr (i64 788537574 to i32*), align 4
  store volatile i32 72, i32* inttoptr (i64 788537578 to i32*), align 4
  store volatile i8 0, i8* inttoptr (i64 788537582 to i8*), align 2
  store volatile i8 0, i8* inttoptr (i64 788537583 to i8*), align 1
  store volatile i8 0, i8* inttoptr (i64 788537584 to i8*), align 16
  br label %19

; <label>:12                                      ; preds = %0
  store volatile i32 7200, i32* inttoptr (i64 788537520 to i32*), align 16
  store volatile i32 10, i32* inttoptr (i64 788537524 to i32*), align 4
  store volatile i32 10, i32* inttoptr (i64 788537528 to i32*), align 8
  store volatile i32 72, i32* inttoptr (i64 788537532 to i32*), align 4
  store volatile i32 432, i32* inttoptr (i64 788537536 to i32*), align 64
  store volatile i8 0, i8* inttoptr (i64 788537540 to i8*), align 4
  store volatile i8 0, i8* inttoptr (i64 788537541 to i8*), align 1
  store volatile i8 0, i8* inttoptr (i64 788537542 to i8*), align 2
  store volatile i32 10, i32* inttoptr (i64 788537543 to i32*), align 4
  store volatile i32 10, i32* inttoptr (i64 788537547 to i32*), align 4
  store volatile i32 432, i32* inttoptr (i64 788537551 to i32*), align 4
  store volatile i32 432, i32* inttoptr (i64 788537555 to i32*), align 4
  store volatile i32 1, i32* inttoptr (i64 788537559 to i32*), align 4
  store volatile i8 0, i8* inttoptr (i64 788537563 to i8*), align 1
  store volatile i8 0, i8* inttoptr (i64 788537564 to i8*), align 4
  store volatile i8 0, i8* inttoptr (i64 788537565 to i8*), align 1
  store volatile i32 10, i32* inttoptr (i64 788537566 to i32*), align 4
  store volatile i32 10, i32* inttoptr (i64 788537570 to i32*), align 4
  store volatile i32 432, i32* inttoptr (i64 788537574 to i32*), align 4
  store volatile i32 72, i32* inttoptr (i64 788537578 to i32*), align 4
  store volatile i8 0, i8* inttoptr (i64 788537582 to i8*), align 2
  store volatile i8 0, i8* inttoptr (i64 788537583 to i8*), align 1
  store volatile i8 0, i8* inttoptr (i64 788537584 to i8*), align 16
  br label %19

; <label>:13                                      ; preds = %0
  store volatile i32 7200, i32* inttoptr (i64 788537520 to i32*), align 16
  store volatile i32 10, i32* inttoptr (i64 788537524 to i32*), align 4
  store volatile i32 10, i32* inttoptr (i64 788537528 to i32*), align 8
  store volatile i32 72, i32* inttoptr (i64 788537532 to i32*), align 4
  store volatile i32 432, i32* inttoptr (i64 788537536 to i32*), align 64
  store volatile i8 0, i8* inttoptr (i64 788537540 to i8*), align 4
  store volatile i8 0, i8* inttoptr (i64 788537541 to i8*), align 1
  store volatile i8 0, i8* inttoptr (i64 788537542 to i8*), align 2
  store volatile i32 10, i32* inttoptr (i64 788537543 to i32*), align 4
  store volatile i32 10, i32* inttoptr (i64 788537547 to i32*), align 4
  store volatile i32 432, i32* inttoptr (i64 788537551 to i32*), align 4
  store volatile i32 432, i32* inttoptr (i64 788537555 to i32*), align 4
  store volatile i32 1, i32* inttoptr (i64 788537559 to i32*), align 4
  store volatile i8 0, i8* inttoptr (i64 788537563 to i8*), align 1
  store volatile i8 0, i8* inttoptr (i64 788537564 to i8*), align 4
  store volatile i8 0, i8* inttoptr (i64 788537565 to i8*), align 1
  store volatile i32 10, i32* inttoptr (i64 788537566 to i32*), align 4
  store volatile i32 10, i32* inttoptr (i64 788537570 to i32*), align 4
  store volatile i32 432, i32* inttoptr (i64 788537574 to i32*), align 4
  store volatile i32 72, i32* inttoptr (i64 788537578 to i32*), align 4
  store volatile i8 0, i8* inttoptr (i64 788537582 to i8*), align 2
  store volatile i8 0, i8* inttoptr (i64 788537583 to i8*), align 1
  store volatile i8 0, i8* inttoptr (i64 788537584 to i8*), align 16
  br label %19

; <label>:14                                      ; preds = %0
  store volatile i32 7200, i32* inttoptr (i64 788537520 to i32*), align 16
  store volatile i32 10, i32* inttoptr (i64 788537524 to i32*), align 4
  store volatile i32 10, i32* inttoptr (i64 788537528 to i32*), align 8
  store volatile i32 72, i32* inttoptr (i64 788537532 to i32*), align 4
  store volatile i32 432, i32* inttoptr (i64 788537536 to i32*), align 64
  store volatile i8 0, i8* inttoptr (i64 788537540 to i8*), align 4
  store volatile i8 0, i8* inttoptr (i64 788537541 to i8*), align 1
  store volatile i8 0, i8* inttoptr (i64 788537542 to i8*), align 2
  store volatile i32 5, i32* inttoptr (i64 788537543 to i32*), align 4
  store volatile i32 10, i32* inttoptr (i64 788537547 to i32*), align 4
  store volatile i32 432, i32* inttoptr (i64 788537551 to i32*), align 4
  store volatile i32 432, i32* inttoptr (i64 788537555 to i32*), align 4
  store volatile i32 2, i32* inttoptr (i64 788537559 to i32*), align 4
  store volatile i8 0, i8* inttoptr (i64 788537563 to i8*), align 1
  store volatile i8 0, i8* inttoptr (i64 788537564 to i8*), align 4
  store volatile i8 0, i8* inttoptr (i64 788537565 to i8*), align 1
  store volatile i32 5, i32* inttoptr (i64 788537566 to i32*), align 4
  store volatile i32 5, i32* inttoptr (i64 788537570 to i32*), align 4
  store volatile i32 432, i32* inttoptr (i64 788537574 to i32*), align 4
  store volatile i32 120, i32* inttoptr (i64 788537578 to i32*), align 4
  store volatile i8 0, i8* inttoptr (i64 788537582 to i8*), align 2
  store volatile i8 0, i8* inttoptr (i64 788537583 to i8*), align 1
  store volatile i8 0, i8* inttoptr (i64 788537584 to i8*), align 16
  br label %19

; <label>:15                                      ; preds = %0
  store volatile i32 3000, i32* inttoptr (i64 788537520 to i32*), align 16
  store volatile i32 5, i32* inttoptr (i64 788537524 to i32*), align 4
  store volatile i32 5, i32* inttoptr (i64 788537528 to i32*), align 8
  store volatile i32 120, i32* inttoptr (i64 788537532 to i32*), align 4
  store volatile i32 720, i32* inttoptr (i64 788537536 to i32*), align 64
  store volatile i8 0, i8* inttoptr (i64 788537540 to i8*), align 4
  store volatile i8 0, i8* inttoptr (i64 788537541 to i8*), align 1
  store volatile i8 0, i8* inttoptr (i64 788537542 to i8*), align 2
  store volatile i32 5, i32* inttoptr (i64 788537543 to i32*), align 4
  store volatile i32 5, i32* inttoptr (i64 788537547 to i32*), align 4
  store volatile i32 720, i32* inttoptr (i64 788537551 to i32*), align 4
  store volatile i32 720, i32* inttoptr (i64 788537555 to i32*), align 4
  store volatile i32 1, i32* inttoptr (i64 788537559 to i32*), align 4
  store volatile i8 0, i8* inttoptr (i64 788537563 to i8*), align 1
  store volatile i8 0, i8* inttoptr (i64 788537564 to i8*), align 4
  store volatile i8 0, i8* inttoptr (i64 788537565 to i8*), align 1
  store volatile i32 5, i32* inttoptr (i64 788537566 to i32*), align 4
  store volatile i32 5, i32* inttoptr (i64 788537570 to i32*), align 4
  store volatile i32 720, i32* inttoptr (i64 788537574 to i32*), align 4
  store volatile i32 120, i32* inttoptr (i64 788537578 to i32*), align 4
  store volatile i8 0, i8* inttoptr (i64 788537582 to i8*), align 2
  store volatile i8 0, i8* inttoptr (i64 788537583 to i8*), align 1
  store volatile i8 0, i8* inttoptr (i64 788537584 to i8*), align 16
  br label %19

; <label>:16                                      ; preds = %0
  store volatile i32 3000, i32* inttoptr (i64 788537520 to i32*), align 16
  store volatile i32 5, i32* inttoptr (i64 788537524 to i32*), align 4
  store volatile i32 5, i32* inttoptr (i64 788537528 to i32*), align 8
  store volatile i32 120, i32* inttoptr (i64 788537532 to i32*), align 4
  store volatile i32 720, i32* inttoptr (i64 788537536 to i32*), align 64
  store volatile i8 0, i8* inttoptr (i64 788537540 to i8*), align 4
  store volatile i8 0, i8* inttoptr (i64 788537541 to i8*), align 1
  store volatile i8 0, i8* inttoptr (i64 788537542 to i8*), align 2
  store volatile i32 5, i32* inttoptr (i64 788537543 to i32*), align 4
  store volatile i32 5, i32* inttoptr (i64 788537547 to i32*), align 4
  store volatile i32 720, i32* inttoptr (i64 788537551 to i32*), align 4
  store volatile i32 720, i32* inttoptr (i64 788537555 to i32*), align 4
  store volatile i32 1, i32* inttoptr (i64 788537559 to i32*), align 4
  store volatile i8 0, i8* inttoptr (i64 788537563 to i8*), align 1
  store volatile i8 0, i8* inttoptr (i64 788537564 to i8*), align 4
  store volatile i8 0, i8* inttoptr (i64 788537565 to i8*), align 1
  store volatile i32 5, i32* inttoptr (i64 788537566 to i32*), align 4
  store volatile i32 5, i32* inttoptr (i64 788537570 to i32*), align 4
  store volatile i32 720, i32* inttoptr (i64 788537574 to i32*), align 4
  store volatile i32 120, i32* inttoptr (i64 788537578 to i32*), align 4
  store volatile i8 0, i8* inttoptr (i64 788537582 to i8*), align 2
  store volatile i8 0, i8* inttoptr (i64 788537583 to i8*), align 1
  store volatile i8 0, i8* inttoptr (i64 788537584 to i8*), align 16
  br label %19

; <label>:17                                      ; preds = %0
  store volatile i32 3000, i32* inttoptr (i64 788537520 to i32*), align 16
  store volatile i32 5, i32* inttoptr (i64 788537524 to i32*), align 4
  store volatile i32 5, i32* inttoptr (i64 788537528 to i32*), align 8
  store volatile i32 120, i32* inttoptr (i64 788537532 to i32*), align 4
  store volatile i32 720, i32* inttoptr (i64 788537536 to i32*), align 64
  store volatile i8 0, i8* inttoptr (i64 788537540 to i8*), align 4
  store volatile i8 0, i8* inttoptr (i64 788537541 to i8*), align 1
  store volatile i8 0, i8* inttoptr (i64 788537542 to i8*), align 2
  store volatile i32 5, i32* inttoptr (i64 788537543 to i32*), align 4
  store volatile i32 5, i32* inttoptr (i64 788537547 to i32*), align 4
  store volatile i32 720, i32* inttoptr (i64 788537551 to i32*), align 4
  store volatile i32 720, i32* inttoptr (i64 788537555 to i32*), align 4
  store volatile i32 1, i32* inttoptr (i64 788537559 to i32*), align 4
  store volatile i8 0, i8* inttoptr (i64 788537563 to i8*), align 1
  store volatile i8 0, i8* inttoptr (i64 788537564 to i8*), align 4
  store volatile i8 0, i8* inttoptr (i64 788537565 to i8*), align 1
  store volatile i32 5, i32* inttoptr (i64 788537566 to i32*), align 4
  store volatile i32 5, i32* inttoptr (i64 788537570 to i32*), align 4
  store volatile i32 720, i32* inttoptr (i64 788537574 to i32*), align 4
  store volatile i32 240, i32* inttoptr (i64 788537578 to i32*), align 4
  store volatile i8 0, i8* inttoptr (i64 788537582 to i8*), align 2
  store volatile i8 0, i8* inttoptr (i64 788537583 to i8*), align 1
  store volatile i8 0, i8* inttoptr (i64 788537584 to i8*), align 16
  br label %19

; <label>:18                                      ; preds = %0
  store volatile i32 0, i32* inttoptr (i64 788537520 to i32*), align 16
  store volatile i32 0, i32* inttoptr (i64 788537524 to i32*), align 4
  store volatile i32 0, i32* inttoptr (i64 788537528 to i32*), align 8
  store volatile i32 0, i32* inttoptr (i64 788537532 to i32*), align 4
  store volatile i32 0, i32* inttoptr (i64 788537536 to i32*), align 64
  store volatile i8 0, i8* inttoptr (i64 788537540 to i8*), align 4
  store volatile i8 0, i8* inttoptr (i64 788537541 to i8*), align 1
  store volatile i8 0, i8* inttoptr (i64 788537542 to i8*), align 2
  store volatile i32 0, i32* inttoptr (i64 788537543 to i32*), align 4
  store volatile i32 0, i32* inttoptr (i64 788537547 to i32*), align 4
  store volatile i32 0, i32* inttoptr (i64 788537551 to i32*), align 4
  store volatile i32 0, i32* inttoptr (i64 788537555 to i32*), align 4
  store volatile i32 0, i32* inttoptr (i64 788537559 to i32*), align 4
  store volatile i8 0, i8* inttoptr (i64 788537563 to i8*), align 1
  store volatile i8 0, i8* inttoptr (i64 788537564 to i8*), align 4
  store volatile i8 0, i8* inttoptr (i64 788537565 to i8*), align 1
  store volatile i32 0, i32* inttoptr (i64 788537566 to i32*), align 4
  store volatile i32 0, i32* inttoptr (i64 788537570 to i32*), align 4
  store volatile i32 0, i32* inttoptr (i64 788537574 to i32*), align 4
  store volatile i32 0, i32* inttoptr (i64 788537578 to i32*), align 4
  store volatile i8 0, i8* inttoptr (i64 788537582 to i8*), align 2
  store volatile i8 0, i8* inttoptr (i64 788537583 to i8*), align 1
  store volatile i8 0, i8* inttoptr (i64 788537584 to i8*), align 16
  br label %19

; <label>:19                                      ; preds = %18, %17, %16, %15, %14, %13, %12, %11, %10, %9, %8, %7, %6, %5, %4, %3, %2
  %InputSize.0 = phi i32 [ 0, %18 ], [ 3000, %17 ], [ 3000, %16 ], [ 3000, %15 ], [ 7200, %14 ], [ 7200, %13 ], [ 7200, %12 ], [ 4800, %11 ], [ 4800, %10 ], [ 4800, %9 ], [ 4800, %8 ], [ 9600, %7 ], [ 9600, %6 ], [ 9600, %5 ], [ 38400, %4 ], [ 38400, %3 ], [ 102400, %2 ]
  %OutputSize.0 = phi i32 [ 0, %18 ], [ 6000, %17 ], [ 3000, %16 ], [ 3000, %15 ], [ 3000, %14 ], [ 7200, %13 ], [ 7200, %12 ], [ 7200, %11 ], [ 4800, %10 ], [ 4800, %9 ], [ 4800, %8 ], [ 4800, %7 ], [ 9600, %6 ], [ 9600, %5 ], [ 9600, %4 ], [ 38400, %3 ], [ 38400, %2 ]
  %resEnable.0 = phi i8 [ 0, %18 ], [ 1, %17 ], [ 1, %16 ], [ 0, %15 ], [ 1, %14 ], [ 1, %13 ], [ 0, %12 ], [ 1, %11 ], [ 1, %10 ], [ 1, %9 ], [ 0, %8 ], [ 1, %7 ], [ 1, %6 ], [ 0, %5 ], [ 1, %4 ], [ 0, %3 ], [ 0, %2 ]
  store volatile i8 %resEnable.0, i8* inttoptr (i64 788537519 to i8*), align 1
  store volatile i8 1, i8* inttoptr (i64 788537518 to i8*), align 2
  store volatile i8 1, i8* inttoptr (i64 788537523 to i8*), align 1
  store volatile i8 1, i8* inttoptr (i64 788537542 to i8*), align 2
  store volatile i8 1, i8* inttoptr (i64 788537565 to i8*), align 1
  store volatile i64 %feat_rd_addr, i64* inttoptr (i64 788537421 to i64*), align 8
  store volatile i32 %InputSize.0, i32* inttoptr (i64 788537437 to i32*), align 4
  store volatile i8 1, i8* inttoptr (i64 788537441 to i8*), align 1
  store volatile i8 1, i8* inttoptr (i64 788537442 to i8*), align 2
  store volatile i64 %feat_wr_addr, i64* inttoptr (i64 788537429 to i64*), align 8
  store volatile i32 %OutputSize.0, i32* inttoptr (i64 788537443 to i32*), align 4
  store volatile i8 1, i8* inttoptr (i64 788537447 to i8*), align 1
  store volatile i8 1, i8* inttoptr (i64 788537448 to i8*), align 8
  store volatile i64 %res_rd_addr, i64* inttoptr (i64 788537461 to i64*), align 8
  store volatile i32 %InputSize.0, i32* inttoptr (i64 788537477 to i32*), align 4
  store volatile i8 1, i8* inttoptr (i64 788537481 to i8*), align 1
  store volatile i8 1, i8* inttoptr (i64 788537482 to i8*), align 2
  store volatile i8 3, i8* inttoptr (i64 788537417 to i8*), align 1
  %20 = icmp eq i8 %resEnable.0, 1
  br i1 %20, label %21, label %22

; <label>:21                                      ; preds = %19
  store volatile i8 1, i8* inttoptr (i64 788537457 to i8*), align 1
  br label %22

; <label>:22                                      ; preds = %21, %19
  store volatile i64 %pw0_weights, i64* inttoptr (i64 788537498 to i64*), align 8
  store volatile i64 788537585, i64* inttoptr (i64 788537506 to i64*), align 8
  store volatile i32 86400, i32* inttoptr (i64 788537514 to i32*), align 4
  store volatile i8 1, i8* inttoptr (i64 788537497 to i8*), align 1
  br label %23

; <label>:23                                      ; preds = %23, %22
  %24 = load volatile i8, i8* inttoptr (i64 788537497 to i8*), align 1
  %25 = and i8 %24, 4
  %26 = icmp eq i8 %25, 0
  br i1 %26, label %23, label %27

; <label>:27                                      ; preds = %23
  store volatile i64 %pw0_quant, i64* inttoptr (i64 788537498 to i64*), align 8
  store volatile i64 788623985, i64* inttoptr (i64 788537506 to i64*), align 8
  store volatile i32 4320, i32* inttoptr (i64 788537514 to i32*), align 4
  store volatile i8 1, i8* inttoptr (i64 788537497 to i8*), align 1
  br label %28

; <label>:28                                      ; preds = %28, %27
  %29 = load volatile i8, i8* inttoptr (i64 788537497 to i8*), align 1
  %30 = and i8 %29, 4
  %31 = icmp eq i8 %30, 0
  br i1 %31, label %28, label %32

; <label>:32                                      ; preds = %28
  store volatile i64 %dw0_weights, i64* inttoptr (i64 788537498 to i64*), align 8
  store volatile i64 788750106, i64* inttoptr (i64 788537506 to i64*), align 8
  store volatile i32 7200, i32* inttoptr (i64 788537514 to i32*), align 4
  store volatile i8 1, i8* inttoptr (i64 788537497 to i8*), align 1
  br label %33

; <label>:33                                      ; preds = %33, %32
  %34 = load volatile i8, i8* inttoptr (i64 788537497 to i8*), align 1
  %35 = and i8 %34, 4
  %36 = icmp eq i8 %35, 0
  br i1 %36, label %33, label %37

; <label>:37                                      ; preds = %33
  store volatile i64 %dw0_quant, i64* inttoptr (i64 788537498 to i64*), align 8
  store volatile i64 788757306, i64* inttoptr (i64 788537506 to i64*), align 8
  store volatile i32 4320, i32* inttoptr (i64 788537514 to i32*), align 4
  store volatile i8 1, i8* inttoptr (i64 788537497 to i8*), align 1
  br label %38

; <label>:38                                      ; preds = %38, %37
  %39 = load volatile i8, i8* inttoptr (i64 788537497 to i8*), align 1
  %40 = and i8 %39, 4
  %41 = icmp eq i8 %40, 0
  br i1 %41, label %38, label %42

; <label>:42                                      ; preds = %38
  store volatile i64 %pw1_weights, i64* inttoptr (i64 788537498 to i64*), align 8
  store volatile i64 788762347, i64* inttoptr (i64 788537506 to i64*), align 8
  store volatile i32 172800, i32* inttoptr (i64 788537514 to i32*), align 4
  store volatile i8 1, i8* inttoptr (i64 788537497 to i8*), align 1
  br label %43

; <label>:43                                      ; preds = %43, %42
  %44 = load volatile i8, i8* inttoptr (i64 788537497 to i8*), align 1
  %45 = and i8 %44, 4
  %46 = icmp eq i8 %45, 0
  br i1 %46, label %43, label %47

; <label>:47                                      ; preds = %43
  store volatile i64 %pw1_quant, i64* inttoptr (i64 788537498 to i64*), align 8
  store volatile i64 788935147, i64* inttoptr (i64 788537506 to i64*), align 8
  store volatile i32 1440, i32* inttoptr (i64 788537514 to i32*), align 4
  store volatile i8 1, i8* inttoptr (i64 788537497 to i8*), align 1
  br label %48

; <label>:48                                      ; preds = %48, %47
  %49 = load volatile i8, i8* inttoptr (i64 788537417 to i8*), align 1
  %50 = and i8 %49, 6
  %51 = icmp eq i8 %50, 6
  br i1 %51, label %52, label %48

; <label>:52                                      ; preds = %48
  ret void
}

attributes #0 = { norecurse nounwind uwtable "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.ident = !{!0}

!0 = !{!"clang version 3.8.1 "}
