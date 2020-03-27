; ModuleID = 'Residual.bc'
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: norecurse nounwind uwtable
define void @Residual(i8 zeroext %enable, i32 %length) #0 {
  %1 = icmp eq i8 %enable, 0
  %2 = icmp ne i32 %length, 0
  br i1 %1, label %.preheader, label %.preheader1

.preheader1:                                      ; preds = %0
  br i1 %2, label %.lr.ph5.preheader, label %.loopexit

.lr.ph5.preheader:                                ; preds = %.preheader1
  %3 = add i32 %length, -1
  %xtraiter11 = and i32 %length, 3
  %lcmp.mod12 = icmp eq i32 %xtraiter11, 0
  br i1 %lcmp.mod12, label %.lr.ph5.preheader.split, label %.lr.ph5.prol.preheader

.lr.ph5.prol.preheader:                           ; preds = %.lr.ph5.preheader
  br label %.lr.ph5.prol

.lr.ph5.prol:                                     ; preds = %.lr.ph5.prol.preheader, %.lr.ph5.prol
  %i1.04.prol = phi i32 [ %7, %.lr.ph5.prol ], [ 0, %.lr.ph5.prol.preheader ]
  %prol.iter13 = phi i32 [ %prol.iter13.sub, %.lr.ph5.prol ], [ %xtraiter11, %.lr.ph5.prol.preheader ]
  %4 = load volatile i8, i8* inttoptr (i64 788537449 to i8*), align 1
  %5 = load volatile i8, i8* inttoptr (i64 788537489 to i8*), align 1
  %6 = add i8 %5, %4
  store volatile i8 %6, i8* inttoptr (i64 788537584 to i8*), align 16
  %7 = add nuw nsw i32 %i1.04.prol, 1
  %prol.iter13.sub = add i32 %prol.iter13, -1
  %prol.iter13.cmp = icmp eq i32 %prol.iter13.sub, 0
  br i1 %prol.iter13.cmp, label %.lr.ph5.preheader.split.loopexit, label %.lr.ph5.prol, !llvm.loop !1

.lr.ph5.preheader.split.loopexit:                 ; preds = %.lr.ph5.prol
  %.lcssa18 = phi i32 [ %7, %.lr.ph5.prol ]
  br label %.lr.ph5.preheader.split

.lr.ph5.preheader.split:                          ; preds = %.lr.ph5.preheader.split.loopexit, %.lr.ph5.preheader
  %i1.04.unr = phi i32 [ 0, %.lr.ph5.preheader ], [ %.lcssa18, %.lr.ph5.preheader.split.loopexit ]
  %8 = icmp ult i32 %3, 3
  br i1 %8, label %.loopexit, label %.lr.ph5.preheader16

.lr.ph5.preheader16:                              ; preds = %.lr.ph5.preheader.split
  br label %.lr.ph5

.preheader:                                       ; preds = %0
  br i1 %2, label %.lr.ph.preheader, label %.loopexit

.lr.ph.preheader:                                 ; preds = %.preheader
  %9 = add i32 %length, -1
  %xtraiter = and i32 %length, 7
  %lcmp.mod = icmp eq i32 %xtraiter, 0
  br i1 %lcmp.mod, label %.lr.ph.preheader.split, label %.lr.ph.prol.preheader

.lr.ph.prol.preheader:                            ; preds = %.lr.ph.preheader
  br label %.lr.ph.prol

.lr.ph.prol:                                      ; preds = %.lr.ph.prol.preheader, %.lr.ph.prol
  %i.03.prol = phi i32 [ %11, %.lr.ph.prol ], [ 0, %.lr.ph.prol.preheader ]
  %prol.iter = phi i32 [ %prol.iter.sub, %.lr.ph.prol ], [ %xtraiter, %.lr.ph.prol.preheader ]
  %10 = load volatile i8, i8* inttoptr (i64 788537449 to i8*), align 1
  store volatile i8 %10, i8* inttoptr (i64 788537584 to i8*), align 16
  %11 = add nuw nsw i32 %i.03.prol, 1
  %prol.iter.sub = add i32 %prol.iter, -1
  %prol.iter.cmp = icmp eq i32 %prol.iter.sub, 0
  br i1 %prol.iter.cmp, label %.lr.ph.preheader.split.loopexit, label %.lr.ph.prol, !llvm.loop !3

.lr.ph.preheader.split.loopexit:                  ; preds = %.lr.ph.prol
  %.lcssa = phi i32 [ %11, %.lr.ph.prol ]
  br label %.lr.ph.preheader.split

.lr.ph.preheader.split:                           ; preds = %.lr.ph.preheader.split.loopexit, %.lr.ph.preheader
  %i.03.unr = phi i32 [ 0, %.lr.ph.preheader ], [ %.lcssa, %.lr.ph.preheader.split.loopexit ]
  %12 = icmp ult i32 %9, 7
  br i1 %12, label %.loopexit, label %.lr.ph.preheader15

.lr.ph.preheader15:                               ; preds = %.lr.ph.preheader.split
  br label %.lr.ph

.lr.ph:                                           ; preds = %.lr.ph.preheader15, %.lr.ph
  %i.03 = phi i32 [ %21, %.lr.ph ], [ %i.03.unr, %.lr.ph.preheader15 ]
  %13 = load volatile i8, i8* inttoptr (i64 788537449 to i8*), align 1
  store volatile i8 %13, i8* inttoptr (i64 788537584 to i8*), align 16
  %14 = load volatile i8, i8* inttoptr (i64 788537449 to i8*), align 1
  store volatile i8 %14, i8* inttoptr (i64 788537584 to i8*), align 16
  %15 = load volatile i8, i8* inttoptr (i64 788537449 to i8*), align 1
  store volatile i8 %15, i8* inttoptr (i64 788537584 to i8*), align 16
  %16 = load volatile i8, i8* inttoptr (i64 788537449 to i8*), align 1
  store volatile i8 %16, i8* inttoptr (i64 788537584 to i8*), align 16
  %17 = load volatile i8, i8* inttoptr (i64 788537449 to i8*), align 1
  store volatile i8 %17, i8* inttoptr (i64 788537584 to i8*), align 16
  %18 = load volatile i8, i8* inttoptr (i64 788537449 to i8*), align 1
  store volatile i8 %18, i8* inttoptr (i64 788537584 to i8*), align 16
  %19 = load volatile i8, i8* inttoptr (i64 788537449 to i8*), align 1
  store volatile i8 %19, i8* inttoptr (i64 788537584 to i8*), align 16
  %20 = load volatile i8, i8* inttoptr (i64 788537449 to i8*), align 1
  store volatile i8 %20, i8* inttoptr (i64 788537584 to i8*), align 16
  %21 = add nsw i32 %i.03, 8
  %exitcond.7 = icmp eq i32 %21, %length
  br i1 %exitcond.7, label %.loopexit.loopexit, label %.lr.ph

.lr.ph5:                                          ; preds = %.lr.ph5.preheader16, %.lr.ph5
  %i1.04 = phi i32 [ %34, %.lr.ph5 ], [ %i1.04.unr, %.lr.ph5.preheader16 ]
  %22 = load volatile i8, i8* inttoptr (i64 788537449 to i8*), align 1
  %23 = load volatile i8, i8* inttoptr (i64 788537489 to i8*), align 1
  %24 = add i8 %23, %22
  store volatile i8 %24, i8* inttoptr (i64 788537584 to i8*), align 16
  %25 = load volatile i8, i8* inttoptr (i64 788537449 to i8*), align 1
  %26 = load volatile i8, i8* inttoptr (i64 788537489 to i8*), align 1
  %27 = add i8 %26, %25
  store volatile i8 %27, i8* inttoptr (i64 788537584 to i8*), align 16
  %28 = load volatile i8, i8* inttoptr (i64 788537449 to i8*), align 1
  %29 = load volatile i8, i8* inttoptr (i64 788537489 to i8*), align 1
  %30 = add i8 %29, %28
  store volatile i8 %30, i8* inttoptr (i64 788537584 to i8*), align 16
  %31 = load volatile i8, i8* inttoptr (i64 788537449 to i8*), align 1
  %32 = load volatile i8, i8* inttoptr (i64 788537489 to i8*), align 1
  %33 = add i8 %32, %31
  store volatile i8 %33, i8* inttoptr (i64 788537584 to i8*), align 16
  %34 = add nsw i32 %i1.04, 4
  %exitcond7.3 = icmp eq i32 %34, %length
  br i1 %exitcond7.3, label %.loopexit.loopexit17, label %.lr.ph5

.loopexit.loopexit:                               ; preds = %.lr.ph
  br label %.loopexit

.loopexit.loopexit17:                             ; preds = %.lr.ph5
  br label %.loopexit

.loopexit:                                        ; preds = %.loopexit.loopexit17, %.loopexit.loopexit, %.lr.ph5.preheader.split, %.lr.ph.preheader.split, %.preheader1, %.preheader
  ret void
}

attributes #0 = { norecurse nounwind uwtable "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.ident = !{!0}

!0 = !{!"clang version 3.8.1 "}
!1 = distinct !{!1, !2}
!2 = !{!"llvm.loop.unroll.disable"}
!3 = distinct !{!3, !2}
