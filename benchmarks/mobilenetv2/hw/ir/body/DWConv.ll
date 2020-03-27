; ModuleID = 'DWConv.bc'
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: norecurse nounwind uwtable
define void @DWConv(i32 %o_size, i32 %i_size, i32 %o_c_size, i32 %i_c_size, i32 %t_STRIDE, i8 zeroext %bias_zp, i8 zeroext %input_zp, i8 zeroext %output_zp) #0 {
.preheader20:
  %0 = add nsw i32 %i_size, 2
  %1 = add nsw i32 %i_size, -2
  %2 = icmp sgt i32 %i_c_size, 0
  %3 = shl i32 %i_size, 1
  br i1 %2, label %.lr.ph53.preheader, label %.preheader18

.lr.ph53.preheader:                               ; preds = %.preheader20
  %4 = add i32 %i_c_size, -1
  %xtraiter145 = and i32 %i_c_size, 3
  %lcmp.mod146 = icmp eq i32 %xtraiter145, 0
  br i1 %lcmp.mod146, label %.lr.ph53.preheader.split, label %.lr.ph53.prol.preheader

.lr.ph53.prol.preheader:                          ; preds = %.lr.ph53.preheader
  br label %.lr.ph53.prol

.lr.ph53.prol:                                    ; preds = %.lr.ph53.prol.preheader, %.lr.ph53.prol
  %indvars.iv104.prol = phi i64 [ %indvars.iv.next105.prol, %.lr.ph53.prol ], [ 0, %.lr.ph53.prol.preheader ]
  %prol.iter147 = phi i32 [ %prol.iter147.sub, %.lr.ph53.prol ], [ %xtraiter145, %.lr.ph53.prol.preheader ]
  %5 = load volatile i8, i8* inttoptr (i64 788628425 to i8*), align 1
  %6 = trunc i64 %indvars.iv104.prol to i32
  %7 = mul i32 %3, %6
  %8 = add nsw i32 %7, %1
  %9 = sext i32 %8 to i64
  %10 = getelementptr inbounds i8, i8* inttoptr (i64 788628426 to i8*), i64 %9
  store volatile i8 %5, i8* %10, align 1
  %indvars.iv.next105.prol = add nuw nsw i64 %indvars.iv104.prol, 1
  %prol.iter147.sub = add i32 %prol.iter147, -1
  %prol.iter147.cmp = icmp eq i32 %prol.iter147.sub, 0
  br i1 %prol.iter147.cmp, label %.lr.ph53.preheader.split.loopexit, label %.lr.ph53.prol, !llvm.loop !1

.lr.ph53.preheader.split.loopexit:                ; preds = %.lr.ph53.prol
  %indvars.iv.next105.prol.lcssa = phi i64 [ %indvars.iv.next105.prol, %.lr.ph53.prol ]
  br label %.lr.ph53.preheader.split

.lr.ph53.preheader.split:                         ; preds = %.lr.ph53.preheader.split.loopexit, %.lr.ph53.preheader
  %indvars.iv104.unr = phi i64 [ 0, %.lr.ph53.preheader ], [ %indvars.iv.next105.prol.lcssa, %.lr.ph53.preheader.split.loopexit ]
  %11 = icmp ult i32 %4, 3
  br i1 %11, label %._crit_edge54, label %.lr.ph53.preheader154

.lr.ph53.preheader154:                            ; preds = %.lr.ph53.preheader.split
  br label %.lr.ph53

.lr.ph53:                                         ; preds = %.lr.ph53.preheader154, %.lr.ph53
  %indvars.iv104 = phi i64 [ %indvars.iv.next105.3, %.lr.ph53 ], [ %indvars.iv104.unr, %.lr.ph53.preheader154 ]
  %12 = load volatile i8, i8* inttoptr (i64 788628425 to i8*), align 1
  %13 = trunc i64 %indvars.iv104 to i32
  %14 = mul i32 %3, %13
  %15 = add nsw i32 %14, %1
  %16 = sext i32 %15 to i64
  %17 = getelementptr inbounds i8, i8* inttoptr (i64 788628426 to i8*), i64 %16
  store volatile i8 %12, i8* %17, align 1
  %indvars.iv.next105 = add nuw nsw i64 %indvars.iv104, 1
  %18 = load volatile i8, i8* inttoptr (i64 788628425 to i8*), align 1
  %19 = trunc i64 %indvars.iv.next105 to i32
  %20 = mul i32 %3, %19
  %21 = add nsw i32 %20, %1
  %22 = sext i32 %21 to i64
  %23 = getelementptr inbounds i8, i8* inttoptr (i64 788628426 to i8*), i64 %22
  store volatile i8 %18, i8* %23, align 1
  %indvars.iv.next105.1149 = add nsw i64 %indvars.iv104, 2
  %24 = load volatile i8, i8* inttoptr (i64 788628425 to i8*), align 1
  %25 = trunc i64 %indvars.iv.next105.1149 to i32
  %26 = mul i32 %3, %25
  %27 = add nsw i32 %26, %1
  %28 = sext i32 %27 to i64
  %29 = getelementptr inbounds i8, i8* inttoptr (i64 788628426 to i8*), i64 %28
  store volatile i8 %24, i8* %29, align 1
  %indvars.iv.next105.2 = add nsw i64 %indvars.iv104, 3
  %30 = load volatile i8, i8* inttoptr (i64 788628425 to i8*), align 1
  %31 = trunc i64 %indvars.iv.next105.2 to i32
  %32 = mul i32 %3, %31
  %33 = add nsw i32 %32, %1
  %34 = sext i32 %33 to i64
  %35 = getelementptr inbounds i8, i8* inttoptr (i64 788628426 to i8*), i64 %34
  store volatile i8 %30, i8* %35, align 1
  %indvars.iv.next105.3 = add nsw i64 %indvars.iv104, 4
  %lftr.wideiv106.3 = trunc i64 %indvars.iv.next105.3 to i32
  %exitcond107.3 = icmp eq i32 %lftr.wideiv106.3, %i_c_size
  br i1 %exitcond107.3, label %._crit_edge54.loopexit, label %.lr.ph53

._crit_edge54.loopexit:                           ; preds = %.lr.ph53
  br label %._crit_edge54

._crit_edge54:                                    ; preds = %._crit_edge54.loopexit, %.lr.ph53.preheader.split
  %36 = add nsw i32 %i_size, -1
  br i1 %2, label %.lr.ph53.1.preheader, label %.preheader18

.lr.ph53.1.preheader:                             ; preds = %._crit_edge54
  %37 = add i32 %i_c_size, -1
  %xtraiter143 = and i32 %i_c_size, 3
  %lcmp.mod144 = icmp eq i32 %xtraiter143, 0
  br i1 %lcmp.mod144, label %.lr.ph53.1.preheader.split, label %.lr.ph53.1.prol.preheader

.lr.ph53.1.prol.preheader:                        ; preds = %.lr.ph53.1.preheader
  br label %.lr.ph53.1.prol

.lr.ph53.1.prol:                                  ; preds = %.lr.ph53.1.prol.preheader, %.lr.ph53.1.prol
  %indvars.iv104.1.prol = phi i64 [ %indvars.iv.next105.1.prol, %.lr.ph53.1.prol ], [ 0, %.lr.ph53.1.prol.preheader ]
  %prol.iter = phi i32 [ %prol.iter.sub, %.lr.ph53.1.prol ], [ %xtraiter143, %.lr.ph53.1.prol.preheader ]
  %38 = load volatile i8, i8* inttoptr (i64 788628425 to i8*), align 1
  %39 = trunc i64 %indvars.iv104.1.prol to i32
  %40 = mul i32 %3, %39
  %41 = add nsw i32 %40, %36
  %42 = sext i32 %41 to i64
  %43 = getelementptr inbounds i8, i8* inttoptr (i64 788628426 to i8*), i64 %42
  store volatile i8 %38, i8* %43, align 1
  %indvars.iv.next105.1.prol = add nuw nsw i64 %indvars.iv104.1.prol, 1
  %prol.iter.sub = add i32 %prol.iter, -1
  %prol.iter.cmp = icmp eq i32 %prol.iter.sub, 0
  br i1 %prol.iter.cmp, label %.lr.ph53.1.preheader.split.loopexit, label %.lr.ph53.1.prol, !llvm.loop !3

.lr.ph53.1.preheader.split.loopexit:              ; preds = %.lr.ph53.1.prol
  %indvars.iv.next105.1.prol.lcssa = phi i64 [ %indvars.iv.next105.1.prol, %.lr.ph53.1.prol ]
  br label %.lr.ph53.1.preheader.split

.lr.ph53.1.preheader.split:                       ; preds = %.lr.ph53.1.preheader.split.loopexit, %.lr.ph53.1.preheader
  %indvars.iv104.1.unr = phi i64 [ 0, %.lr.ph53.1.preheader ], [ %indvars.iv.next105.1.prol.lcssa, %.lr.ph53.1.preheader.split.loopexit ]
  %44 = icmp ult i32 %37, 3
  br i1 %44, label %.preheader18, label %.lr.ph53.1.preheader153

.lr.ph53.1.preheader153:                          ; preds = %.lr.ph53.1.preheader.split
  br label %.lr.ph53.1

.preheader18.loopexit:                            ; preds = %.lr.ph53.1
  br label %.preheader18

.preheader18:                                     ; preds = %.preheader18.loopexit, %.lr.ph53.1.preheader.split, %.preheader20, %._crit_edge54
  %45 = mul nsw i32 %0, %i_c_size
  %46 = icmp sgt i32 %i_c_size, 0
  %47 = icmp sgt i32 %i_size, 0
  br i1 %47, label %.preheader17.preheader, label %.preheader14

.preheader17.preheader:                           ; preds = %.preheader18
  %xtraiter141 = and i32 %i_c_size, 1
  %lcmp.mod142 = icmp eq i32 %xtraiter141, 0
  %48 = icmp eq i32 %i_c_size, 1
  br label %.preheader17

.preheader17:                                     ; preds = %.preheader17.preheader, %._crit_edge47
  %x1.048 = phi i32 [ %69, %._crit_edge47 ], [ 0, %.preheader17.preheader ]
  br i1 %46, label %.lr.ph46.preheader, label %._crit_edge47

.lr.ph46.preheader:                               ; preds = %.preheader17
  br i1 %lcmp.mod142, label %.lr.ph46.preheader.split, label %.lr.ph46.prol

.lr.ph46.prol:                                    ; preds = %.lr.ph46.preheader
  %49 = load volatile i8, i8* inttoptr (i64 788628425 to i8*), align 1
  %50 = add nsw i32 %x1.048, %i_size
  %51 = sext i32 %50 to i64
  %52 = getelementptr inbounds i8, i8* inttoptr (i64 788628426 to i8*), i64 %51
  store volatile i8 %49, i8* %52, align 1
  br label %.lr.ph46.preheader.split

.lr.ph46.preheader.split:                         ; preds = %.lr.ph46.preheader, %.lr.ph46.prol
  %indvars.iv97.unr = phi i64 [ 0, %.lr.ph46.preheader ], [ 1, %.lr.ph46.prol ]
  br i1 %48, label %._crit_edge47, label %.lr.ph46.preheader152

.lr.ph46.preheader152:                            ; preds = %.lr.ph46.preheader.split
  br label %.lr.ph46

.lr.ph46:                                         ; preds = %.lr.ph46.preheader152, %.lr.ph46
  %indvars.iv97 = phi i64 [ %indvars.iv.next98.1, %.lr.ph46 ], [ %indvars.iv97.unr, %.lr.ph46.preheader152 ]
  %53 = load volatile i8, i8* inttoptr (i64 788628425 to i8*), align 1
  %54 = trunc i64 %indvars.iv97 to i32
  %55 = shl i32 %54, 1
  %56 = or i32 %55, 1
  %57 = mul i32 %56, %i_size
  %58 = add nsw i32 %57, %x1.048
  %59 = sext i32 %58 to i64
  %60 = getelementptr inbounds i8, i8* inttoptr (i64 788628426 to i8*), i64 %59
  store volatile i8 %53, i8* %60, align 1
  %indvars.iv.next98 = add nuw nsw i64 %indvars.iv97, 1
  %61 = load volatile i8, i8* inttoptr (i64 788628425 to i8*), align 1
  %62 = trunc i64 %indvars.iv.next98 to i32
  %63 = shl i32 %62, 1
  %64 = or i32 %63, 1
  %65 = mul i32 %64, %i_size
  %66 = add nsw i32 %65, %x1.048
  %67 = sext i32 %66 to i64
  %68 = getelementptr inbounds i8, i8* inttoptr (i64 788628426 to i8*), i64 %67
  store volatile i8 %61, i8* %68, align 1
  %indvars.iv.next98.1 = add nsw i64 %indvars.iv97, 2
  %lftr.wideiv99.1 = trunc i64 %indvars.iv.next98.1 to i32
  %exitcond100.1 = icmp eq i32 %lftr.wideiv99.1, %i_c_size
  br i1 %exitcond100.1, label %._crit_edge47.loopexit, label %.lr.ph46

._crit_edge47.loopexit:                           ; preds = %.lr.ph46
  br label %._crit_edge47

._crit_edge47:                                    ; preds = %._crit_edge47.loopexit, %.lr.ph46.preheader.split, %.preheader17
  %69 = add nuw nsw i32 %x1.048, 1
  %exitcond101 = icmp eq i32 %69, %i_size
  br i1 %exitcond101, label %.preheader14.loopexit, label %.preheader17

.preheader14.loopexit:                            ; preds = %._crit_edge47
  br label %.preheader14

.preheader14:                                     ; preds = %.preheader14.loopexit, %.preheader18
  %70 = icmp sgt i32 %i_c_size, 0
  br i1 %70, label %.lr.ph41, label %._crit_edge42.1.1

.lr.ph41:                                         ; preds = %.preheader14
  %71 = add i32 %i_size, -2
  %xtraiter136 = and i32 %i_c_size, 1
  %lcmp.mod137 = icmp eq i32 %xtraiter136, 0
  br i1 %lcmp.mod137, label %.lr.ph41.split, label %72

; <label>:72                                      ; preds = %.lr.ph41
  %73 = sext i32 %71 to i64
  %74 = getelementptr inbounds i8, i8* inttoptr (i64 788628426 to i8*), i64 %73
  %75 = load volatile i8, i8* %74, align 1
  store volatile i8 %75, i8* inttoptr (i64 788743630 to i8*), align 2
  br label %.lr.ph41.split

.lr.ph41.split:                                   ; preds = %.lr.ph41, %72
  %indvars.iv87.unr = phi i64 [ 0, %.lr.ph41 ], [ 1, %72 ]
  %76 = icmp eq i32 %i_c_size, 1
  br i1 %76, label %._crit_edge42, label %.lr.ph41.split.split.preheader

.lr.ph41.split.split.preheader:                   ; preds = %.lr.ph41.split
  br label %.lr.ph41.split.split

.lr.ph41.split.split:                             ; preds = %.lr.ph41.split.split.preheader, %.lr.ph41.split.split
  %indvars.iv87 = phi i64 [ %indvars.iv.next88.1139, %.lr.ph41.split.split ], [ %indvars.iv87.unr, %.lr.ph41.split.split.preheader ]
  %77 = trunc i64 %indvars.iv87 to i32
  %78 = shl i32 %77, 1
  %79 = mul i32 %78, %i_size
  %80 = add i32 %71, %79
  %81 = sext i32 %80 to i64
  %82 = getelementptr inbounds i8, i8* inttoptr (i64 788628426 to i8*), i64 %81
  %83 = load volatile i8, i8* %82, align 1
  %84 = mul i64 %indvars.iv87, 38654705664
  %sext125 = add i64 %84, 17179869184
  %85 = ashr exact i64 %sext125, 32
  %86 = getelementptr inbounds i8, i8* inttoptr (i64 788743626 to i8*), i64 %85
  store volatile i8 %83, i8* %86, align 1
  %indvars.iv.next88 = add nuw nsw i64 %indvars.iv87, 1
  %87 = trunc i64 %indvars.iv.next88 to i32
  %88 = shl i32 %87, 1
  %89 = mul i32 %88, %i_size
  %90 = add i32 %71, %89
  %91 = sext i32 %90 to i64
  %92 = getelementptr inbounds i8, i8* inttoptr (i64 788628426 to i8*), i64 %91
  %93 = load volatile i8, i8* %92, align 1
  %94 = mul i64 %indvars.iv.next88, 38654705664
  %sext125.1 = add i64 %94, 17179869184
  %95 = ashr exact i64 %sext125.1, 32
  %96 = getelementptr inbounds i8, i8* inttoptr (i64 788743626 to i8*), i64 %95
  store volatile i8 %93, i8* %96, align 1
  %indvars.iv.next88.1139 = add nsw i64 %indvars.iv87, 2
  %lftr.wideiv89.1140 = trunc i64 %indvars.iv.next88.1139 to i32
  %exitcond90.1 = icmp eq i32 %lftr.wideiv89.1140, %i_c_size
  br i1 %exitcond90.1, label %._crit_edge42.loopexit, label %.lr.ph41.split.split

._crit_edge42.loopexit:                           ; preds = %.lr.ph41.split.split
  br label %._crit_edge42

._crit_edge42:                                    ; preds = %._crit_edge42.loopexit, %.lr.ph41.split
  br i1 %70, label %.lr.ph41.1, label %._crit_edge42.1.1

.preheader13.lr.ph:                               ; preds = %._crit_edge42.1.1
  %97 = icmp sgt i32 %o_c_size, 0
  %98 = icmp sgt i32 %i_c_size, 0
  %99 = mul nsw i32 %i_size, %i_size
  %100 = mul nsw i32 %99, %i_c_size
  %101 = zext i8 %bias_zp to i32
  %102 = zext i8 %output_zp to i32
  %103 = sext i32 %i_size to i64
  %104 = sext i32 %i_size to i64
  br label %.preheader12.preheader

.preheader12.preheader:                           ; preds = %.preheader13.lr.ph, %._crit_edge34
  %indvars.iv83 = phi i64 [ 0, %.preheader13.lr.ph ], [ %indvars.iv.next84, %._crit_edge34 ]
  %read_count.036 = phi i32 [ %45, %.preheader13.lr.ph ], [ %read_count.2.lcssa.lcssa, %._crit_edge34 ]
  br label %.preheader12

.preheader12:                                     ; preds = %.preheader12.preheader, %._crit_edge
  %indvars.iv79 = phi i64 [ %indvars.iv.next80, %._crit_edge ], [ 0, %.preheader12.preheader ]
  %read_count.131 = phi i32 [ %read_count.2.lcssa, %._crit_edge ], [ %read_count.036, %.preheader12.preheader ]
  br i1 %97, label %.lr.ph, label %.preheader11

.lr.ph:                                           ; preds = %.preheader12
  %105 = or i64 %indvars.iv79, %indvars.iv83
  %106 = and i64 %105, 1
  %107 = icmp eq i64 %106, 0
  %108 = add i64 %indvars.iv79, 4294967295
  %109 = icmp sle i64 %indvars.iv79, %103
  %110 = icmp slt i64 %indvars.iv79, %103
  %111 = add nsw i64 %indvars.iv79, 1
  %112 = icmp slt i64 %111, %103
  br label %118

.preheader11.loopexit:                            ; preds = %191
  br label %.preheader11

.preheader11:                                     ; preds = %.preheader11.loopexit, %.preheader12
  br i1 %98, label %.preheader10.lr.ph, label %._crit_edge

.preheader10.lr.ph:                               ; preds = %.preheader11
  %113 = trunc i64 %indvars.iv79 to i32
  %114 = add i32 %113, %i_size
  %115 = trunc i64 %indvars.iv79 to i32
  %116 = trunc i64 %indvars.iv79 to i32
  %117 = trunc i64 %indvars.iv79 to i32
  br label %192

; <label>:118                                     ; preds = %191, %.lr.ph
  %indvars.iv60 = phi i64 [ 0, %.lr.ph ], [ %indvars.iv.next61, %191 ]
  %119 = getelementptr inbounds i8, i8* inttoptr (i64 788757306 to i8*), i64 %indvars.iv60
  %120 = load volatile i8, i8* %119, align 1
  %121 = getelementptr inbounds i8, i8* inttoptr (i64 788760906 to i8*), i64 %indvars.iv60
  %122 = load volatile i8, i8* %121, align 1
  %123 = trunc i64 %indvars.iv60 to i32
  %124 = mul i32 %123, 9
  %125 = add i32 %124, 4
  br label %126

; <label>:126                                     ; preds = %118, %296
  %indvars.iv57 = phi i64 [ -1, %118 ], [ %indvars.iv.next58, %296 ]
  %val_out.023 = phi i32 [ 0, %118 ], [ %val_out.2.2, %296 ]
  %127 = add nsw i64 %indvars.iv57, %indvars.iv83
  %128 = icmp slt i64 %127, %104
  %129 = mul nsw i64 %indvars.iv57, 3
  %130 = trunc i64 %129 to i32
  %131 = add i32 %125, %130
  %132 = add nsw i64 %129, 4
  %133 = or i64 %108, %127
  %134 = trunc i64 %133 to i32
  %135 = icmp sgt i32 %134, -1
  %136 = and i1 %128, %135
  %or.cond5 = and i1 %109, %136
  br i1 %or.cond5, label %137, label %152

; <label>:137                                     ; preds = %126
  %138 = add i32 %131, -1
  %139 = sext i32 %138 to i64
  %140 = getelementptr inbounds i8, i8* inttoptr (i64 788743626 to i8*), i64 %139
  %141 = load volatile i8, i8* %140, align 1
  %142 = sub i8 %141, %input_zp
  %143 = add nsw i64 %129, 3
  %144 = getelementptr inbounds i8, i8* inttoptr (i64 788750106 to i8*), i64 %143
  %145 = load volatile i8, i8* %144, align 1
  %146 = sub i8 %145, %122
  %147 = sext i8 %142 to i32
  %148 = sext i8 %146 to i32
  %149 = shl nsw i32 %147, 16
  %sext = mul nsw i32 %149, %148
  %150 = ashr exact i32 %sext, 16
  %151 = add nsw i32 %150, %val_out.023
  br label %152

; <label>:152                                     ; preds = %126, %137
  %val_out.2 = phi i32 [ %151, %137 ], [ %val_out.023, %126 ]
  %153 = or i64 %indvars.iv79, %127
  %154 = trunc i64 %153 to i32
  %155 = icmp sgt i32 %154, -1
  %156 = and i1 %128, %155
  %or.cond5.1 = and i1 %110, %156
  br i1 %or.cond5.1, label %263, label %276

; <label>:157                                     ; preds = %296
  %val_out.2.2.lcssa = phi i32 [ %val_out.2.2, %296 ]
  %158 = zext i8 %120 to i32
  %159 = sub nsw i32 %158, %101
  %160 = getelementptr inbounds i8, i8* inttoptr (i64 788758746 to i8*), i64 %indvars.iv60
  %161 = load volatile i8, i8* %160, align 1
  %162 = icmp sgt i8 %161, -1
  %163 = getelementptr inbounds i8, i8* inttoptr (i64 788758026 to i8*), i64 %indvars.iv60
  %164 = load volatile i8, i8* %163, align 1
  %165 = zext i8 %164 to i32
  %166 = mul nsw i32 %165, %159
  %167 = load volatile i8, i8* %160, align 1
  %168 = sext i8 %167 to i32
  %169 = sub nsw i32 0, %168
  %.sink = select i1 %162, i32 %168, i32 %169
  %170 = ashr i32 %166, %.sink
  %171 = add nsw i32 %170, %val_out.2.2.lcssa
  %172 = getelementptr inbounds i8, i8* inttoptr (i64 788759466 to i8*), i64 %indvars.iv60
  %173 = load volatile i8, i8* %172, align 1
  %174 = zext i8 %173 to i32
  %175 = mul nsw i32 %171, %174
  %176 = getelementptr inbounds i8, i8* inttoptr (i64 788760186 to i8*), i64 %indvars.iv60
  %177 = load volatile i8, i8* %176, align 1
  %178 = zext i8 %177 to i32
  %179 = ashr i32 %175, %178
  %180 = add nsw i32 %179, %102
  %181 = icmp slt i32 %180, 255
  %. = select i1 %181, i32 %180, i32 255
  %182 = icmp slt i32 %., 0
  %183 = select i1 %182, i32 0, i32 %.
  %184 = trunc i32 %183 to i8
  %185 = getelementptr inbounds i8, i8* inttoptr (i64 788761626 to i8*), i64 %indvars.iv60
  store volatile i8 %184, i8* %185, align 1
  br i1 %381, label %186, label %189

; <label>:186                                     ; preds = %157
  br i1 %107, label %187, label %191

; <label>:187                                     ; preds = %186
  %188 = load volatile i8, i8* %185, align 1
  store volatile i8 %188, i8* inttoptr (i64 788762346 to i8*), align 2
  br label %191

; <label>:189                                     ; preds = %157
  %190 = load volatile i8, i8* %185, align 1
  store volatile i8 %190, i8* inttoptr (i64 788762346 to i8*), align 2
  br label %191

; <label>:191                                     ; preds = %189, %186, %187
  %indvars.iv.next61 = add nuw nsw i64 %indvars.iv60, 1
  %lftr.wideiv = trunc i64 %indvars.iv.next61 to i32
  %exitcond62 = icmp eq i32 %lftr.wideiv, %o_c_size
  br i1 %exitcond62, label %.preheader11.loopexit, label %118

; <label>:192                                     ; preds = %.preheader10.lr.ph, %256
  %indvars.iv75 = phi i64 [ 0, %.preheader10.lr.ph ], [ %indvars.iv.next76, %256 ]
  %read_count.229 = phi i32 [ %read_count.131, %.preheader10.lr.ph ], [ %read_count.3, %256 ]
  %sext118 = mul i64 %indvars.iv75, 38654705664
  %193 = ashr exact i64 %sext118, 32
  %sext126 = add i64 %sext118, 4294967296
  %194 = ashr exact i64 %sext126, 32
  %195 = getelementptr inbounds i8, i8* inttoptr (i64 788743626 to i8*), i64 %194
  %196 = load volatile i8, i8* %195, align 1
  %197 = getelementptr inbounds i8, i8* inttoptr (i64 788743626 to i8*), i64 %193
  store volatile i8 %196, i8* %197, align 1
  %sext119 = add i64 %sext118, 8589934592
  %198 = ashr exact i64 %sext119, 32
  %199 = getelementptr inbounds i8, i8* inttoptr (i64 788743626 to i8*), i64 %198
  %200 = load volatile i8, i8* %199, align 1
  %201 = add nsw i64 %193, 1
  %202 = getelementptr inbounds i8, i8* inttoptr (i64 788743626 to i8*), i64 %201
  store volatile i8 %200, i8* %202, align 1
  %203 = add nsw i64 %193, 3
  %204 = shl i64 %203, 32
  %sext127 = add i64 %204, 4294967296
  %205 = ashr exact i64 %sext127, 32
  %206 = getelementptr inbounds i8, i8* inttoptr (i64 788743626 to i8*), i64 %205
  %207 = load volatile i8, i8* %206, align 1
  %208 = getelementptr inbounds i8, i8* inttoptr (i64 788743626 to i8*), i64 %203
  store volatile i8 %207, i8* %208, align 1
  %209 = shl i64 %203, 32
  %sext120 = add i64 %209, 8589934592
  %210 = ashr exact i64 %sext120, 32
  %211 = getelementptr inbounds i8, i8* inttoptr (i64 788743626 to i8*), i64 %210
  %212 = load volatile i8, i8* %211, align 1
  %213 = add nsw i64 %193, 4
  %214 = getelementptr inbounds i8, i8* inttoptr (i64 788743626 to i8*), i64 %213
  store volatile i8 %212, i8* %214, align 1
  %215 = add nsw i64 %193, 6
  %216 = shl i64 %215, 32
  %sext128 = add i64 %216, 4294967296
  %217 = ashr exact i64 %sext128, 32
  %218 = getelementptr inbounds i8, i8* inttoptr (i64 788743626 to i8*), i64 %217
  %219 = load volatile i8, i8* %218, align 1
  %220 = getelementptr inbounds i8, i8* inttoptr (i64 788743626 to i8*), i64 %215
  store volatile i8 %219, i8* %220, align 1
  %221 = shl i64 %215, 32
  %sext121 = add i64 %221, 8589934592
  %222 = ashr exact i64 %sext121, 32
  %223 = getelementptr inbounds i8, i8* inttoptr (i64 788743626 to i8*), i64 %222
  %224 = load volatile i8, i8* %223, align 1
  %225 = add nsw i64 %193, 7
  %226 = getelementptr inbounds i8, i8* inttoptr (i64 788743626 to i8*), i64 %225
  store volatile i8 %224, i8* %226, align 1
  %227 = trunc i64 %indvars.iv75 to i32
  %228 = shl nsw i32 %227, 1
  %229 = mul nsw i32 %228, %i_size
  %230 = add nsw i32 %229, %115
  %231 = sext i32 %230 to i64
  %232 = getelementptr inbounds i8, i8* inttoptr (i64 788628426 to i8*), i64 %231
  %233 = load volatile i8, i8* %232, align 1
  %234 = trunc i64 %indvars.iv75 to i32
  %235 = mul i32 %234, 9
  %236 = add nsw i32 %235, 2
  %237 = sext i32 %236 to i64
  %238 = getelementptr inbounds i8, i8* inttoptr (i64 788743626 to i8*), i64 %237
  store volatile i8 %233, i8* %238, align 1
  %239 = add i32 %235, 5
  %240 = mul i32 %228, %i_size
  %241 = add nsw i32 %240, %116
  %242 = or i32 %228, 1
  %243 = mul i32 %242, %i_size
  %244 = add nsw i32 %243, %117
  %245 = sext i32 %244 to i64
  %246 = getelementptr inbounds i8, i8* inttoptr (i64 788628426 to i8*), i64 %245
  %247 = load volatile i8, i8* %246, align 1
  %248 = sext i32 %241 to i64
  %249 = getelementptr inbounds i8, i8* inttoptr (i64 788628426 to i8*), i64 %248
  store volatile i8 %247, i8* %249, align 1
  %250 = sext i32 %239 to i64
  %251 = getelementptr inbounds i8, i8* inttoptr (i64 788743626 to i8*), i64 %250
  store volatile i8 %247, i8* %251, align 1
  %252 = icmp slt i32 %read_count.229, %100
  br i1 %252, label %253, label %256

; <label>:253                                     ; preds = %192
  %254 = load volatile i8, i8* inttoptr (i64 788628425 to i8*), align 1
  %255 = add nsw i32 %read_count.229, 1
  br label %256

; <label>:256                                     ; preds = %253, %192
  %read_count.3 = phi i32 [ %255, %253 ], [ %read_count.229, %192 ]
  %val_in.0 = phi i8 [ %254, %253 ], [ 0, %192 ]
  %257 = add nsw i32 %235, 8
  %258 = add i32 %114, %229
  %259 = sext i32 %258 to i64
  %260 = getelementptr inbounds i8, i8* inttoptr (i64 788628426 to i8*), i64 %259
  store volatile i8 %val_in.0, i8* %260, align 1
  %261 = sext i32 %257 to i64
  %262 = getelementptr inbounds i8, i8* inttoptr (i64 788743626 to i8*), i64 %261
  store volatile i8 %val_in.0, i8* %262, align 1
  %indvars.iv.next76 = add nuw nsw i64 %indvars.iv75, 1
  %lftr.wideiv77 = trunc i64 %indvars.iv.next76 to i32
  %exitcond78 = icmp eq i32 %lftr.wideiv77, %i_c_size
  br i1 %exitcond78, label %._crit_edge.loopexit, label %192

._crit_edge.loopexit:                             ; preds = %256
  %read_count.3.lcssa = phi i32 [ %read_count.3, %256 ]
  br label %._crit_edge

._crit_edge:                                      ; preds = %._crit_edge.loopexit, %.preheader11
  %read_count.2.lcssa = phi i32 [ %read_count.131, %.preheader11 ], [ %read_count.3.lcssa, %._crit_edge.loopexit ]
  %indvars.iv.next80 = add nuw nsw i64 %indvars.iv79, 1
  %lftr.wideiv81 = trunc i64 %indvars.iv.next80 to i32
  %exitcond82 = icmp eq i32 %lftr.wideiv81, %x_looper.0
  br i1 %exitcond82, label %._crit_edge34, label %.preheader12

._crit_edge34:                                    ; preds = %._crit_edge
  %read_count.2.lcssa.lcssa = phi i32 [ %read_count.2.lcssa, %._crit_edge ]
  %indvars.iv.next84 = add nuw nsw i64 %indvars.iv83, 1
  %lftr.wideiv85 = trunc i64 %indvars.iv.next84 to i32
  %exitcond86 = icmp eq i32 %lftr.wideiv85, %x_looper.0
  br i1 %exitcond86, label %._crit_edge39.loopexit, label %.preheader12.preheader

._crit_edge39.loopexit:                           ; preds = %._crit_edge34
  br label %._crit_edge39

._crit_edge39:                                    ; preds = %._crit_edge39.loopexit, %._crit_edge42.1.1
  ret void

; <label>:263                                     ; preds = %152
  %264 = sext i32 %131 to i64
  %265 = getelementptr inbounds i8, i8* inttoptr (i64 788743626 to i8*), i64 %264
  %266 = load volatile i8, i8* %265, align 1
  %267 = sub i8 %266, %input_zp
  %268 = getelementptr inbounds i8, i8* inttoptr (i64 788750106 to i8*), i64 %132
  %269 = load volatile i8, i8* %268, align 1
  %270 = sub i8 %269, %122
  %271 = sext i8 %267 to i32
  %272 = sext i8 %270 to i32
  %273 = shl nsw i32 %271, 16
  %sext.1 = mul nsw i32 %273, %272
  %274 = ashr exact i32 %sext.1, 16
  %275 = add nsw i32 %274, %val_out.2
  br label %276

; <label>:276                                     ; preds = %263, %152
  %val_out.2.1 = phi i32 [ %275, %263 ], [ %val_out.2, %152 ]
  %277 = or i64 %111, %127
  %278 = trunc i64 %277 to i32
  %279 = icmp sgt i32 %278, -1
  %280 = and i1 %128, %279
  %or.cond5.2 = and i1 %112, %280
  br i1 %or.cond5.2, label %281, label %296

; <label>:281                                     ; preds = %276
  %282 = add i32 %131, 1
  %283 = sext i32 %282 to i64
  %284 = getelementptr inbounds i8, i8* inttoptr (i64 788743626 to i8*), i64 %283
  %285 = load volatile i8, i8* %284, align 1
  %286 = sub i8 %285, %input_zp
  %287 = add nsw i64 %129, 5
  %288 = getelementptr inbounds i8, i8* inttoptr (i64 788750106 to i8*), i64 %287
  %289 = load volatile i8, i8* %288, align 1
  %290 = sub i8 %289, %122
  %291 = sext i8 %286 to i32
  %292 = sext i8 %290 to i32
  %293 = shl nsw i32 %291, 16
  %sext.2 = mul nsw i32 %293, %292
  %294 = ashr exact i32 %sext.2, 16
  %295 = add nsw i32 %294, %val_out.2.1
  br label %296

; <label>:296                                     ; preds = %281, %276
  %val_out.2.2 = phi i32 [ %295, %281 ], [ %val_out.2.1, %276 ]
  %indvars.iv.next58 = add nsw i64 %indvars.iv57, 1
  %exitcond59 = icmp eq i64 %indvars.iv.next58, 2
  br i1 %exitcond59, label %157, label %126

.lr.ph41.1:                                       ; preds = %._crit_edge42
  %297 = add i32 %i_size, -1
  %xtraiter131 = and i32 %i_c_size, 1
  %lcmp.mod132 = icmp eq i32 %xtraiter131, 0
  br i1 %lcmp.mod132, label %.lr.ph41.1.split, label %298

; <label>:298                                     ; preds = %.lr.ph41.1
  %299 = sext i32 %297 to i64
  %300 = getelementptr inbounds i8, i8* inttoptr (i64 788628426 to i8*), i64 %299
  %301 = load volatile i8, i8* %300, align 1
  store volatile i8 %301, i8* inttoptr (i64 788743631 to i8*), align 1
  br label %.lr.ph41.1.split

.lr.ph41.1.split:                                 ; preds = %.lr.ph41.1, %298
  %indvars.iv87.1.unr = phi i64 [ 0, %.lr.ph41.1 ], [ 1, %298 ]
  %302 = icmp eq i32 %i_c_size, 1
  br i1 %302, label %.preheader14.1, label %.lr.ph41.1.split.split.preheader

.lr.ph41.1.split.split.preheader:                 ; preds = %.lr.ph41.1.split
  br label %.lr.ph41.1.split.split

.lr.ph41.1.split.split:                           ; preds = %.lr.ph41.1.split.split.preheader, %.lr.ph41.1.split.split
  %indvars.iv87.1 = phi i64 [ %indvars.iv.next88.1.1134, %.lr.ph41.1.split.split ], [ %indvars.iv87.1.unr, %.lr.ph41.1.split.split.preheader ]
  %303 = trunc i64 %indvars.iv87.1 to i32
  %304 = shl i32 %303, 1
  %305 = mul i32 %304, %i_size
  %306 = add i32 %297, %305
  %307 = sext i32 %306 to i64
  %308 = getelementptr inbounds i8, i8* inttoptr (i64 788628426 to i8*), i64 %307
  %309 = load volatile i8, i8* %308, align 1
  %310 = mul i64 %indvars.iv87.1, 38654705664
  %sext124 = add i64 %310, 21474836480
  %311 = ashr exact i64 %sext124, 32
  %312 = getelementptr inbounds i8, i8* inttoptr (i64 788743626 to i8*), i64 %311
  store volatile i8 %309, i8* %312, align 1
  %indvars.iv.next88.1 = add nuw nsw i64 %indvars.iv87.1, 1
  %313 = trunc i64 %indvars.iv.next88.1 to i32
  %314 = shl i32 %313, 1
  %315 = mul i32 %314, %i_size
  %316 = add i32 %297, %315
  %317 = sext i32 %316 to i64
  %318 = getelementptr inbounds i8, i8* inttoptr (i64 788628426 to i8*), i64 %317
  %319 = load volatile i8, i8* %318, align 1
  %320 = mul i64 %indvars.iv.next88.1, 38654705664
  %sext124.1 = add i64 %320, 21474836480
  %321 = ashr exact i64 %sext124.1, 32
  %322 = getelementptr inbounds i8, i8* inttoptr (i64 788743626 to i8*), i64 %321
  store volatile i8 %319, i8* %322, align 1
  %indvars.iv.next88.1.1134 = add nsw i64 %indvars.iv87.1, 2
  %lftr.wideiv89.1.1135 = trunc i64 %indvars.iv.next88.1.1134 to i32
  %exitcond90.1.1 = icmp eq i32 %lftr.wideiv89.1.1135, %i_c_size
  br i1 %exitcond90.1.1, label %.preheader14.1.loopexit, label %.lr.ph41.1.split.split

.preheader14.1.loopexit:                          ; preds = %.lr.ph41.1.split.split
  br label %.preheader14.1

.preheader14.1:                                   ; preds = %.preheader14.1.loopexit, %.lr.ph41.1.split
  br i1 %70, label %.lr.ph41.1111, label %._crit_edge42.1.1

.lr.ph41.1111:                                    ; preds = %.preheader14.1
  %323 = add i32 %i_size, -2
  %xtraiter129 = and i32 %i_c_size, 1
  %lcmp.mod130 = icmp eq i32 %xtraiter129, 0
  br i1 %lcmp.mod130, label %.lr.ph41.1111.split, label %324

; <label>:324                                     ; preds = %.lr.ph41.1111
  %325 = add i32 %323, %i_size
  %326 = sext i32 %325 to i64
  %327 = getelementptr inbounds i8, i8* inttoptr (i64 788628426 to i8*), i64 %326
  %328 = load volatile i8, i8* %327, align 1
  store volatile i8 %328, i8* inttoptr (i64 788743633 to i8*), align 1
  br label %.lr.ph41.1111.split

.lr.ph41.1111.split:                              ; preds = %.lr.ph41.1111, %324
  %indvars.iv87.1112.unr = phi i64 [ 0, %.lr.ph41.1111 ], [ 1, %324 ]
  %329 = icmp eq i32 %i_c_size, 1
  br i1 %329, label %._crit_edge42.1117, label %.lr.ph41.1111.split.split.preheader

.lr.ph41.1111.split.split.preheader:              ; preds = %.lr.ph41.1111.split
  br label %.lr.ph41.1111.split.split

.lr.ph41.1111.split.split:                        ; preds = %.lr.ph41.1111.split.split.preheader, %.lr.ph41.1111.split.split
  %indvars.iv87.1112 = phi i64 [ %indvars.iv.next88.1113.1, %.lr.ph41.1111.split.split ], [ %indvars.iv87.1112.unr, %.lr.ph41.1111.split.split.preheader ]
  %330 = trunc i64 %indvars.iv87.1112 to i32
  %331 = shl i32 %330, 1
  %332 = or i32 %331, 1
  %333 = mul i32 %332, %i_size
  %334 = add i32 %323, %333
  %335 = sext i32 %334 to i64
  %336 = getelementptr inbounds i8, i8* inttoptr (i64 788628426 to i8*), i64 %335
  %337 = load volatile i8, i8* %336, align 1
  %338 = mul i64 %indvars.iv87.1112, 38654705664
  %sext123 = add i64 %338, 30064771072
  %339 = ashr exact i64 %sext123, 32
  %340 = getelementptr inbounds i8, i8* inttoptr (i64 788743626 to i8*), i64 %339
  store volatile i8 %337, i8* %340, align 1
  %indvars.iv.next88.1113 = add nuw nsw i64 %indvars.iv87.1112, 1
  %341 = trunc i64 %indvars.iv.next88.1113 to i32
  %342 = shl i32 %341, 1
  %343 = or i32 %342, 1
  %344 = mul i32 %343, %i_size
  %345 = add i32 %323, %344
  %346 = sext i32 %345 to i64
  %347 = getelementptr inbounds i8, i8* inttoptr (i64 788628426 to i8*), i64 %346
  %348 = load volatile i8, i8* %347, align 1
  %349 = mul i64 %indvars.iv.next88.1113, 38654705664
  %sext123.1 = add i64 %349, 30064771072
  %350 = ashr exact i64 %sext123.1, 32
  %351 = getelementptr inbounds i8, i8* inttoptr (i64 788743626 to i8*), i64 %350
  store volatile i8 %348, i8* %351, align 1
  %indvars.iv.next88.1113.1 = add nsw i64 %indvars.iv87.1112, 2
  %lftr.wideiv89.1114.1 = trunc i64 %indvars.iv.next88.1113.1 to i32
  %exitcond90.1115.1 = icmp eq i32 %lftr.wideiv89.1114.1, %i_c_size
  br i1 %exitcond90.1115.1, label %._crit_edge42.1117.loopexit, label %.lr.ph41.1111.split.split

._crit_edge42.1117.loopexit:                      ; preds = %.lr.ph41.1111.split.split
  br label %._crit_edge42.1117

._crit_edge42.1117:                               ; preds = %._crit_edge42.1117.loopexit, %.lr.ph41.1111.split
  br i1 %70, label %.lr.ph41.1.1, label %._crit_edge42.1.1

.lr.ph41.1.1:                                     ; preds = %._crit_edge42.1117
  %352 = add i32 %i_size, -1
  %xtraiter = and i32 %i_c_size, 1
  %lcmp.mod = icmp eq i32 %xtraiter, 0
  br i1 %lcmp.mod, label %.lr.ph41.1.1.split, label %353

; <label>:353                                     ; preds = %.lr.ph41.1.1
  %354 = add i32 %352, %i_size
  %355 = sext i32 %354 to i64
  %356 = getelementptr inbounds i8, i8* inttoptr (i64 788628426 to i8*), i64 %355
  %357 = load volatile i8, i8* %356, align 1
  store volatile i8 %357, i8* inttoptr (i64 788743634 to i8*), align 2
  br label %.lr.ph41.1.1.split

.lr.ph41.1.1.split:                               ; preds = %.lr.ph41.1.1, %353
  %indvars.iv87.1.1.unr = phi i64 [ 0, %.lr.ph41.1.1 ], [ 1, %353 ]
  %358 = icmp eq i32 %i_c_size, 1
  br i1 %358, label %._crit_edge42.1.1, label %.lr.ph41.1.1.split.split.preheader

.lr.ph41.1.1.split.split.preheader:               ; preds = %.lr.ph41.1.1.split
  br label %.lr.ph41.1.1.split.split

.lr.ph41.1.1.split.split:                         ; preds = %.lr.ph41.1.1.split.split.preheader, %.lr.ph41.1.1.split.split
  %indvars.iv87.1.1 = phi i64 [ %indvars.iv.next88.1.1.1, %.lr.ph41.1.1.split.split ], [ %indvars.iv87.1.1.unr, %.lr.ph41.1.1.split.split.preheader ]
  %359 = trunc i64 %indvars.iv87.1.1 to i32
  %360 = shl i32 %359, 1
  %361 = or i32 %360, 1
  %362 = mul i32 %361, %i_size
  %363 = add i32 %352, %362
  %364 = sext i32 %363 to i64
  %365 = getelementptr inbounds i8, i8* inttoptr (i64 788628426 to i8*), i64 %364
  %366 = load volatile i8, i8* %365, align 1
  %367 = mul i64 %indvars.iv87.1.1, 38654705664
  %sext122 = add i64 %367, 34359738368
  %368 = ashr exact i64 %sext122, 32
  %369 = getelementptr inbounds i8, i8* inttoptr (i64 788743626 to i8*), i64 %368
  store volatile i8 %366, i8* %369, align 1
  %indvars.iv.next88.1.1 = add nuw nsw i64 %indvars.iv87.1.1, 1
  %370 = trunc i64 %indvars.iv.next88.1.1 to i32
  %371 = shl i32 %370, 1
  %372 = or i32 %371, 1
  %373 = mul i32 %372, %i_size
  %374 = add i32 %352, %373
  %375 = sext i32 %374 to i64
  %376 = getelementptr inbounds i8, i8* inttoptr (i64 788628426 to i8*), i64 %375
  %377 = load volatile i8, i8* %376, align 1
  %378 = mul i64 %indvars.iv.next88.1.1, 38654705664
  %sext122.1 = add i64 %378, 34359738368
  %379 = ashr exact i64 %sext122.1, 32
  %380 = getelementptr inbounds i8, i8* inttoptr (i64 788743626 to i8*), i64 %379
  store volatile i8 %377, i8* %380, align 1
  %indvars.iv.next88.1.1.1 = add nsw i64 %indvars.iv87.1.1, 2
  %lftr.wideiv89.1.1.1 = trunc i64 %indvars.iv.next88.1.1.1 to i32
  %exitcond90.1.1.1 = icmp eq i32 %lftr.wideiv89.1.1.1, %i_c_size
  br i1 %exitcond90.1.1.1, label %._crit_edge42.1.1.loopexit, label %.lr.ph41.1.1.split.split

._crit_edge42.1.1.loopexit:                       ; preds = %.lr.ph41.1.1.split.split
  br label %._crit_edge42.1.1

._crit_edge42.1.1:                                ; preds = %._crit_edge42.1.1.loopexit, %.lr.ph41.1.1.split, %.preheader14, %._crit_edge42, %.preheader14.1, %._crit_edge42.1117
  %381 = icmp eq i32 %t_STRIDE, 2
  %382 = zext i1 %381 to i32
  %x_looper.0 = shl nsw i32 %o_size, %382
  %383 = icmp sgt i32 %x_looper.0, 0
  br i1 %383, label %.preheader13.lr.ph, label %._crit_edge39

.lr.ph53.1:                                       ; preds = %.lr.ph53.1.preheader153, %.lr.ph53.1
  %indvars.iv104.1 = phi i64 [ %indvars.iv.next105.1.3, %.lr.ph53.1 ], [ %indvars.iv104.1.unr, %.lr.ph53.1.preheader153 ]
  %384 = load volatile i8, i8* inttoptr (i64 788628425 to i8*), align 1
  %385 = trunc i64 %indvars.iv104.1 to i32
  %386 = mul i32 %3, %385
  %387 = add nsw i32 %386, %36
  %388 = sext i32 %387 to i64
  %389 = getelementptr inbounds i8, i8* inttoptr (i64 788628426 to i8*), i64 %388
  store volatile i8 %384, i8* %389, align 1
  %indvars.iv.next105.1 = add nuw nsw i64 %indvars.iv104.1, 1
  %390 = load volatile i8, i8* inttoptr (i64 788628425 to i8*), align 1
  %391 = trunc i64 %indvars.iv.next105.1 to i32
  %392 = mul i32 %3, %391
  %393 = add nsw i32 %392, %36
  %394 = sext i32 %393 to i64
  %395 = getelementptr inbounds i8, i8* inttoptr (i64 788628426 to i8*), i64 %394
  store volatile i8 %390, i8* %395, align 1
  %indvars.iv.next105.1.1 = add nsw i64 %indvars.iv104.1, 2
  %396 = load volatile i8, i8* inttoptr (i64 788628425 to i8*), align 1
  %397 = trunc i64 %indvars.iv.next105.1.1 to i32
  %398 = mul i32 %3, %397
  %399 = add nsw i32 %398, %36
  %400 = sext i32 %399 to i64
  %401 = getelementptr inbounds i8, i8* inttoptr (i64 788628426 to i8*), i64 %400
  store volatile i8 %396, i8* %401, align 1
  %indvars.iv.next105.1.2 = add nsw i64 %indvars.iv104.1, 3
  %402 = load volatile i8, i8* inttoptr (i64 788628425 to i8*), align 1
  %403 = trunc i64 %indvars.iv.next105.1.2 to i32
  %404 = mul i32 %3, %403
  %405 = add nsw i32 %404, %36
  %406 = sext i32 %405 to i64
  %407 = getelementptr inbounds i8, i8* inttoptr (i64 788628426 to i8*), i64 %406
  store volatile i8 %402, i8* %407, align 1
  %indvars.iv.next105.1.3 = add nsw i64 %indvars.iv104.1, 4
  %lftr.wideiv106.1.3 = trunc i64 %indvars.iv.next105.1.3 to i32
  %exitcond107.1.3 = icmp eq i32 %lftr.wideiv106.1.3, %i_c_size
  br i1 %exitcond107.1.3, label %.preheader18.loopexit, label %.lr.ph53.1
}

attributes #0 = { norecurse nounwind uwtable "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.ident = !{!0}

!0 = !{!"clang version 3.8.1 "}
!1 = distinct !{!1, !2}
!2 = !{!"llvm.loop.unroll.disable"}
!3 = distinct !{!3, !2}
