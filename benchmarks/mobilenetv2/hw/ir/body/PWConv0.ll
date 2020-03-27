; ModuleID = 'PWConv0.bc'
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: norecurse nounwind uwtable
define void @PWConv0(i32 %o_size, i32 %i_size, i32 %o_c_size, i32 %i_c_size, i8 zeroext %bias_zp, i8 zeroext %input_zp, i8 zeroext %output_zp) #0 {
  %1 = icmp sgt i32 %o_size, 0
  br i1 %1, label %.preheader5.lr.ph, label %._crit_edge15

.preheader5.lr.ph:                                ; preds = %0
  %2 = icmp sgt i32 %o_c_size, 0
  %3 = zext i8 %input_zp to i32
  %4 = zext i8 %output_zp to i32
  %5 = sext i32 %i_c_size to i64
  br label %.preheader4.preheader

.preheader4.preheader:                            ; preds = %.preheader5.lr.ph, %._crit_edge13
  %y.014 = phi i32 [ 0, %.preheader5.lr.ph ], [ %65, %._crit_edge13 ]
  br label %.preheader4

.preheader4:                                      ; preds = %.preheader4.preheader, %._crit_edge
  %x.012 = phi i32 [ %64, %._crit_edge ], [ 0, %.preheader4.preheader ]
  br label %6

.preheader3:                                      ; preds = %78
  br i1 %2, label %.preheader.preheader, label %._crit_edge

.preheader.preheader:                             ; preds = %.preheader3
  br label %.preheader

; <label>:6                                       ; preds = %78, %.preheader4
  %indvars.iv = phi i64 [ 0, %.preheader4 ], [ %indvars.iv.next.2, %78 ]
  %7 = icmp slt i64 %indvars.iv, %5
  br i1 %7, label %8, label %11

; <label>:8                                       ; preds = %6
  %9 = load volatile i8, i8* inttoptr (i64 788537584 to i8*), align 16
  %10 = getelementptr inbounds i8, i8* inttoptr (i64 788628305 to i8*), i64 %indvars.iv
  store volatile i8 %9, i8* %10, align 1
  br label %13

; <label>:11                                      ; preds = %6
  %12 = getelementptr inbounds i8, i8* inttoptr (i64 788628305 to i8*), i64 %indvars.iv
  store volatile i8 %input_zp, i8* %12, align 1
  br label %13

; <label>:13                                      ; preds = %8, %11
  %indvars.iv.next = add nuw nsw i64 %indvars.iv, 1
  %14 = icmp slt i64 %indvars.iv.next, %5
  br i1 %14, label %68, label %66

.preheader:                                       ; preds = %.preheader.preheader, %34
  %indvars.iv25 = phi i64 [ %indvars.iv.next26, %34 ], [ 0, %.preheader.preheader ]
  %15 = getelementptr inbounds i8, i8* inttoptr (i64 788623985 to i8*), i64 %indvars.iv25
  %16 = load volatile i8, i8* %15, align 1
  %17 = getelementptr inbounds i8, i8* inttoptr (i64 788627585 to i8*), i64 %indvars.iv25
  %18 = load volatile i8, i8* %17, align 1
  %19 = mul nuw nsw i64 %indvars.iv25, 120
  br label %20

; <label>:20                                      ; preds = %20, %.preheader
  %indvars.iv18 = phi i64 [ 0, %.preheader ], [ %indvars.iv.next19, %20 ]
  %sum.17 = phi i32 [ 0, %.preheader ], [ %33, %20 ]
  %21 = getelementptr inbounds i8, i8* inttoptr (i64 788628305 to i8*), i64 %indvars.iv18
  %22 = load volatile i8, i8* %21, align 1
  %23 = add i64 %indvars.iv18, %19
  %sext30 = shl i64 %23, 32
  %24 = ashr exact i64 %sext30, 32
  %25 = getelementptr inbounds i8, i8* inttoptr (i64 788537585 to i8*), i64 %24
  %26 = load volatile i8, i8* %25, align 1
  %27 = zext i8 %22 to i32
  %28 = sub nsw i32 %27, %3
  %29 = sub i8 %26, %18
  %30 = sext i8 %29 to i32
  %31 = shl nsw i32 %28, 16
  %sext = mul i32 %31, %30
  %32 = ashr exact i32 %sext, 16
  %33 = add nsw i32 %32, %sum.17
  %indvars.iv.next19 = add nuw nsw i64 %indvars.iv18, 1
  %exitcond20 = icmp eq i64 %indvars.iv.next19, 120
  br i1 %exitcond20, label %34, label %20

; <label>:34                                      ; preds = %20
  %.lcssa = phi i32 [ %33, %20 ]
  %35 = sub i8 %16, %bias_zp
  %36 = sext i8 %35 to i32
  %37 = getelementptr inbounds i8, i8* inttoptr (i64 788624705 to i8*), i64 %indvars.iv25
  %38 = load volatile i8, i8* %37, align 1
  %39 = zext i8 %38 to i32
  %40 = getelementptr inbounds i8, i8* inttoptr (i64 788625425 to i8*), i64 %indvars.iv25
  %41 = load volatile i8, i8* %40, align 1
  %42 = icmp sgt i8 %41, -1
  %43 = shl nsw i32 %36, 16
  %sext2 = mul nsw i32 %43, %39
  %44 = ashr exact i32 %sext2, 16
  %45 = load volatile i8, i8* %40, align 1
  %46 = sext i8 %45 to i32
  %47 = ashr i32 %44, %46
  %48 = sub nsw i32 0, %46
  %49 = shl i32 %44, %48
  %scaled_bias.0 = select i1 %42, i32 %47, i32 %49
  %50 = add nsw i32 %scaled_bias.0, %.lcssa
  %51 = getelementptr inbounds i8, i8* inttoptr (i64 788626145 to i8*), i64 %indvars.iv25
  %52 = load volatile i8, i8* %51, align 1
  %53 = zext i8 %52 to i32
  %54 = mul nsw i32 %50, %53
  %55 = getelementptr inbounds i8, i8* inttoptr (i64 788626865 to i8*), i64 %indvars.iv25
  %56 = load volatile i8, i8* %55, align 1
  %57 = zext i8 %56 to i32
  %58 = ashr i32 %54, %57
  %59 = add nsw i32 %58, %4
  %60 = icmp slt i32 %59, 255
  %. = select i1 %60, i32 %59, i32 255
  %61 = icmp slt i32 %., 0
  %62 = select i1 %61, i32 0, i32 %.
  %63 = trunc i32 %62 to i8
  store volatile i8 %63, i8* inttoptr (i64 788628425 to i8*), align 1
  %indvars.iv.next26 = add nuw nsw i64 %indvars.iv25, 1
  %lftr.wideiv = trunc i64 %indvars.iv.next26 to i32
  %exitcond27 = icmp eq i32 %lftr.wideiv, %o_c_size
  br i1 %exitcond27, label %._crit_edge.loopexit, label %.preheader

._crit_edge.loopexit:                             ; preds = %34
  br label %._crit_edge

._crit_edge:                                      ; preds = %._crit_edge.loopexit, %.preheader3
  %64 = add nuw nsw i32 %x.012, 1
  %exitcond28 = icmp eq i32 %64, %o_size
  br i1 %exitcond28, label %._crit_edge13, label %.preheader4

._crit_edge13:                                    ; preds = %._crit_edge
  %65 = add nuw nsw i32 %y.014, 1
  %exitcond29 = icmp eq i32 %65, %o_size
  br i1 %exitcond29, label %._crit_edge15.loopexit, label %.preheader4.preheader

._crit_edge15.loopexit:                           ; preds = %._crit_edge13
  br label %._crit_edge15

._crit_edge15:                                    ; preds = %._crit_edge15.loopexit, %0
  ret void

; <label>:66                                      ; preds = %13
  %67 = getelementptr inbounds i8, i8* inttoptr (i64 788628305 to i8*), i64 %indvars.iv.next
  store volatile i8 %input_zp, i8* %67, align 1
  br label %71

; <label>:68                                      ; preds = %13
  %69 = load volatile i8, i8* inttoptr (i64 788537584 to i8*), align 16
  %70 = getelementptr inbounds i8, i8* inttoptr (i64 788628305 to i8*), i64 %indvars.iv.next
  store volatile i8 %69, i8* %70, align 1
  br label %71

; <label>:71                                      ; preds = %68, %66
  %indvars.iv.next.1 = add nsw i64 %indvars.iv, 2
  %72 = icmp slt i64 %indvars.iv.next.1, %5
  br i1 %72, label %75, label %73

; <label>:73                                      ; preds = %71
  %74 = getelementptr inbounds i8, i8* inttoptr (i64 788628305 to i8*), i64 %indvars.iv.next.1
  store volatile i8 %input_zp, i8* %74, align 1
  br label %78

; <label>:75                                      ; preds = %71
  %76 = load volatile i8, i8* inttoptr (i64 788537584 to i8*), align 16
  %77 = getelementptr inbounds i8, i8* inttoptr (i64 788628305 to i8*), i64 %indvars.iv.next.1
  store volatile i8 %76, i8* %77, align 1
  br label %78

; <label>:78                                      ; preds = %75, %73
  %indvars.iv.next.2 = add nsw i64 %indvars.iv, 3
  %exitcond.2 = icmp eq i64 %indvars.iv.next.2, 120
  br i1 %exitcond.2, label %.preheader3, label %6
}

attributes #0 = { norecurse nounwind uwtable "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.ident = !{!0}

!0 = !{!"clang version 3.8.1 "}
