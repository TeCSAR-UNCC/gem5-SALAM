; NOTE: Assertions have been autogenerated by utils/update_llc_test_checks.py
; RUN: llc -mtriple=aarch64-unknown-linux-gnu < %s | FileCheck %s

;------------------------------------------------------------------------------;
; Odd divisors
;------------------------------------------------------------------------------;

; This tests the BuildREMEqFold optimization with UREM, i32, odd divisor, SETEQ.
; The corresponding pseudocode is:
; Q <- [N * multInv(5, 2^32)] <=> [N * 0xCCCCCCCD] <=> [N * (-858993459)]
; res <- [Q <= (2^32 - 1) / 5] <=> [Q <= 858993459] <=> [Q < 858993460]
define i32 @test_urem_odd(i32 %X) nounwind {
; CHECK-LABEL: test_urem_odd:
; CHECK:       // %bb.0:
; CHECK-NEXT:    mov w8, #52429
; CHECK-NEXT:    movk w8, #52428, lsl #16
; CHECK-NEXT:    mov w9, #13108
; CHECK-NEXT:    mul w8, w0, w8
; CHECK-NEXT:    movk w9, #13107, lsl #16
; CHECK-NEXT:    cmp w8, w9
; CHECK-NEXT:    cset w0, lo
; CHECK-NEXT:    ret
  %urem = urem i32 %X, 5
  %cmp = icmp eq i32 %urem, 0
  %ret = zext i1 %cmp to i32
  ret i32 %ret
}

define i32 @test_urem_odd_25(i32 %X) nounwind {
; CHECK-LABEL: test_urem_odd_25:
; CHECK:       // %bb.0:
; CHECK-NEXT:    mov w8, #23593
; CHECK-NEXT:    movk w8, #49807, lsl #16
; CHECK-NEXT:    mov w9, #28836
; CHECK-NEXT:    mul w8, w0, w8
; CHECK-NEXT:    movk w9, #2621, lsl #16
; CHECK-NEXT:    cmp w8, w9
; CHECK-NEXT:    cset w0, lo
; CHECK-NEXT:    ret
  %urem = urem i32 %X, 25
  %cmp = icmp eq i32 %urem, 0
  %ret = zext i1 %cmp to i32
  ret i32 %ret
}

; This is like test_urem_odd, except the divisor has bit 30 set.
define i32 @test_urem_odd_bit30(i32 %X) nounwind {
; CHECK-LABEL: test_urem_odd_bit30:
; CHECK:       // %bb.0:
; CHECK-NEXT:    mov w8, #43691
; CHECK-NEXT:    movk w8, #27306, lsl #16
; CHECK-NEXT:    mul w8, w0, w8
; CHECK-NEXT:    cmp w8, #4 // =4
; CHECK-NEXT:    cset w0, lo
; CHECK-NEXT:    ret
  %urem = urem i32 %X, 1073741827
  %cmp = icmp eq i32 %urem, 0
  %ret = zext i1 %cmp to i32
  ret i32 %ret
}

; This is like test_urem_odd, except the divisor has bit 31 set.
define i32 @test_urem_odd_bit31(i32 %X) nounwind {
; CHECK-LABEL: test_urem_odd_bit31:
; CHECK:       // %bb.0:
; CHECK-NEXT:    mov w8, #43691
; CHECK-NEXT:    movk w8, #10922, lsl #16
; CHECK-NEXT:    mul w8, w0, w8
; CHECK-NEXT:    cmp w8, #2 // =2
; CHECK-NEXT:    cset w0, lo
; CHECK-NEXT:    ret
  %urem = urem i32 %X, 2147483651
  %cmp = icmp eq i32 %urem, 0
  %ret = zext i1 %cmp to i32
  ret i32 %ret
}

;------------------------------------------------------------------------------;
; Even divisors
;------------------------------------------------------------------------------;

; This tests the BuildREMEqFold optimization with UREM, i16, even divisor, SETNE.
; In this case, D <=> 14 <=> 7 * 2^1, so D0 = 7 and K = 1.
; The corresponding pseudocode is:
; Q <- [N * multInv(D0, 2^16)] <=> [N * multInv(7, 2^16)] <=> [N * 28087]
; Q <- [Q >>rot K] <=> [Q >>rot 1]
; res <- ![Q <= (2^16 - 1) / 7] <=> ![Q <= 9362] <=> [Q > 9362]
define i16 @test_urem_even(i16 %X) nounwind {
; CHECK-LABEL: test_urem_even:
; CHECK:       // %bb.0:
; CHECK-NEXT:    mov w9, #28087
; CHECK-NEXT:    and w8, w0, #0xffff
; CHECK-NEXT:    movk w9, #46811, lsl #16
; CHECK-NEXT:    mul w8, w8, w9
; CHECK-NEXT:    mov w9, #9362
; CHECK-NEXT:    ror w8, w8, #1
; CHECK-NEXT:    movk w9, #4681, lsl #16
; CHECK-NEXT:    cmp w8, w9
; CHECK-NEXT:    cset w0, hi
; CHECK-NEXT:    ret
  %urem = urem i16 %X, 14
  %cmp = icmp ne i16 %urem, 0
  %ret = zext i1 %cmp to i16
  ret i16 %ret
}

define i32 @test_urem_even_100(i32 %X) nounwind {
; CHECK-LABEL: test_urem_even_100:
; CHECK:       // %bb.0:
; CHECK-NEXT:    mov w8, #23593
; CHECK-NEXT:    movk w8, #49807, lsl #16
; CHECK-NEXT:    mul w8, w0, w8
; CHECK-NEXT:    mov w9, #23593
; CHECK-NEXT:    ror w8, w8, #2
; CHECK-NEXT:    movk w9, #655, lsl #16
; CHECK-NEXT:    cmp w8, w9
; CHECK-NEXT:    cset w0, lo
; CHECK-NEXT:    ret
  %urem = urem i32 %X, 100
  %cmp = icmp eq i32 %urem, 0
  %ret = zext i1 %cmp to i32
  ret i32 %ret
}

; This is like test_urem_even, except the divisor has bit 30 set.
define i32 @test_urem_even_bit30(i32 %X) nounwind {
; CHECK-LABEL: test_urem_even_bit30:
; CHECK:       // %bb.0:
; CHECK-NEXT:    mov w8, #20165
; CHECK-NEXT:    movk w8, #64748, lsl #16
; CHECK-NEXT:    mul w8, w0, w8
; CHECK-NEXT:    ror w8, w8, #3
; CHECK-NEXT:    cmp w8, #4 // =4
; CHECK-NEXT:    cset w0, lo
; CHECK-NEXT:    ret
  %urem = urem i32 %X, 1073741928
  %cmp = icmp eq i32 %urem, 0
  %ret = zext i1 %cmp to i32
  ret i32 %ret
}

; This is like test_urem_odd, except the divisor has bit 31 set.
define i32 @test_urem_even_bit31(i32 %X) nounwind {
; CHECK-LABEL: test_urem_even_bit31:
; CHECK:       // %bb.0:
; CHECK-NEXT:    mov w8, #64251
; CHECK-NEXT:    movk w8, #47866, lsl #16
; CHECK-NEXT:    mul w8, w0, w8
; CHECK-NEXT:    ror w8, w8, #1
; CHECK-NEXT:    cmp w8, #2 // =2
; CHECK-NEXT:    cset w0, lo
; CHECK-NEXT:    ret
  %urem = urem i32 %X, 2147483750
  %cmp = icmp eq i32 %urem, 0
  %ret = zext i1 %cmp to i32
  ret i32 %ret
}

;------------------------------------------------------------------------------;
; Special case
;------------------------------------------------------------------------------;

; 'NE' predicate is fine too.
define i32 @test_urem_odd_setne(i32 %X) nounwind {
; CHECK-LABEL: test_urem_odd_setne:
; CHECK:       // %bb.0:
; CHECK-NEXT:    mov w8, #52429
; CHECK-NEXT:    movk w8, #52428, lsl #16
; CHECK-NEXT:    mul w8, w0, w8
; CHECK-NEXT:    mov w9, #858993459
; CHECK-NEXT:    cmp w8, w9
; CHECK-NEXT:    cset w0, hi
; CHECK-NEXT:    ret
  %urem = urem i32 %X, 5
  %cmp = icmp ne i32 %urem, 0
  %ret = zext i1 %cmp to i32
  ret i32 %ret
}

;------------------------------------------------------------------------------;
; Negative tests
;------------------------------------------------------------------------------;

; The fold is invalid if divisor is 1.
define i32 @test_urem_one(i32 %X) nounwind {
; CHECK-LABEL: test_urem_one:
; CHECK:       // %bb.0:
; CHECK-NEXT:    mov w0, #1
; CHECK-NEXT:    ret
  %urem = urem i32 %X, 1
  %cmp = icmp eq i32 %urem, 0
  %ret = zext i1 %cmp to i32
  ret i32 %ret
}

; We can lower remainder of division by all-ones much better elsewhere.
define i32 @test_urem_allones(i32 %X) nounwind {
; CHECK-LABEL: test_urem_allones:
; CHECK:       // %bb.0:
; CHECK-NEXT:    neg w8, w0
; CHECK-NEXT:    cmp w8, #2 // =2
; CHECK-NEXT:    cset w0, lo
; CHECK-NEXT:    ret
  %urem = urem i32 %X, 4294967295
  %cmp = icmp eq i32 %urem, 0
  %ret = zext i1 %cmp to i32
  ret i32 %ret
}

; We can lower remainder of division by powers of two much better elsewhere.
define i32 @test_urem_pow2(i32 %X) nounwind {
; CHECK-LABEL: test_urem_pow2:
; CHECK:       // %bb.0:
; CHECK-NEXT:    tst w0, #0xf
; CHECK-NEXT:    cset w0, eq
; CHECK-NEXT:    ret
  %urem = urem i32 %X, 16
  %cmp = icmp eq i32 %urem, 0
  %ret = zext i1 %cmp to i32
  ret i32 %ret
}
