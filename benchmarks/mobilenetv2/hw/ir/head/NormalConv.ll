; ModuleID = 'NormalConv.bc'
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: norecurse nounwind uwtable
define void @NormalConv() #0 {
.preheader20:
  %0 = load volatile i8, i8* inttoptr (i64 788529185 to i8*), align 1
  store volatile i8 %0, i8* inttoptr (i64 788529375 to i8*), align 1
  %1 = load volatile i8, i8* inttoptr (i64 788529185 to i8*), align 1
  store volatile i8 %1, i8* inttoptr (i64 788529695 to i8*), align 1
  %2 = load volatile i8, i8* inttoptr (i64 788529185 to i8*), align 1
  store volatile i8 %2, i8* inttoptr (i64 788530015 to i8*), align 1
  %3 = load volatile i8, i8* inttoptr (i64 788529185 to i8*), align 1
  store volatile i8 %3, i8* inttoptr (i64 788529376 to i8*), align 32
  %4 = load volatile i8, i8* inttoptr (i64 788529185 to i8*), align 1
  store volatile i8 %4, i8* inttoptr (i64 788529696 to i8*), align 32
  %5 = load volatile i8, i8* inttoptr (i64 788529185 to i8*), align 1
  store volatile i8 %5, i8* inttoptr (i64 788530016 to i8*), align 32
  br label %.preheader19

.preheader19:                                     ; preds = %.preheader19, %.preheader20
  %indvars.iv98 = phi i64 [ 0, %.preheader20 ], [ %indvars.iv.next99.1, %.preheader19 ]
  %6 = add nuw nsw i64 %indvars.iv98, 160
  %7 = load volatile i8, i8* inttoptr (i64 788529185 to i8*), align 1
  %8 = getelementptr inbounds i8, i8* inttoptr (i64 788529217 to i8*), i64 %6
  store volatile i8 %7, i8* %8, align 1
  %9 = load volatile i8, i8* inttoptr (i64 788529185 to i8*), align 1
  %10 = add nuw nsw i64 %indvars.iv98, 480
  %11 = getelementptr inbounds i8, i8* inttoptr (i64 788529217 to i8*), i64 %10
  store volatile i8 %9, i8* %11, align 1
  %12 = load volatile i8, i8* inttoptr (i64 788529185 to i8*), align 1
  %13 = add nuw nsw i64 %indvars.iv98, 800
  %14 = getelementptr inbounds i8, i8* inttoptr (i64 788529217 to i8*), i64 %13
  store volatile i8 %12, i8* %14, align 1
  %15 = add nsw i64 %indvars.iv98, 161
  %16 = load volatile i8, i8* inttoptr (i64 788529185 to i8*), align 1
  %17 = getelementptr inbounds i8, i8* inttoptr (i64 788529217 to i8*), i64 %15
  store volatile i8 %16, i8* %17, align 1
  %18 = load volatile i8, i8* inttoptr (i64 788529185 to i8*), align 1
  %19 = add nsw i64 %indvars.iv98, 481
  %20 = getelementptr inbounds i8, i8* inttoptr (i64 788529217 to i8*), i64 %19
  store volatile i8 %18, i8* %20, align 1
  %21 = load volatile i8, i8* inttoptr (i64 788529185 to i8*), align 1
  %22 = add nsw i64 %indvars.iv98, 801
  %23 = getelementptr inbounds i8, i8* inttoptr (i64 788529217 to i8*), i64 %22
  store volatile i8 %21, i8* %23, align 1
  %indvars.iv.next99.1 = add nsw i64 %indvars.iv98, 2
  %exitcond100.1 = icmp eq i64 %indvars.iv.next99.1, 160
  br i1 %exitcond100.1, label %.preheader14.preheader122, label %.preheader19

.preheader14:                                     ; preds = %111, %.preheader14.preheader122
  %indvars.iv79 = phi i64 [ 0, %.preheader14.preheader122 ], [ %indvars.iv.next80, %111 ]
  %read_count.040 = phi i32 [ 486, %.preheader14.preheader122 ], [ %read_count.3.2.lcssa, %111 ]
  br label %.preheader13

.preheader13:                                     ; preds = %257, %.preheader14
  %indvars.iv76 = phi i64 [ 0, %.preheader14 ], [ %indvars.iv.next77, %257 ]
  %read_count.138 = phi i32 [ %read_count.040, %.preheader14 ], [ %read_count.3.2, %257 ]
  %24 = or i64 %indvars.iv76, %indvars.iv79
  %25 = and i64 %24, 1
  %26 = icmp eq i64 %25, 0
  %27 = add i64 %indvars.iv76, 4294967295
  %28 = trunc i64 %27 to i32
  %29 = icmp ult i32 %28, 160
  %30 = trunc i64 %indvars.iv76 to i32
  %31 = icmp ult i32 %30, 160
  %32 = add nsw i64 %indvars.iv76, 1
  %33 = trunc i64 %32 to i32
  %34 = icmp ult i32 %33, 160
  br label %49

.preheader12:                                     ; preds = %90
  %35 = add nuw nsw i64 %indvars.iv76, 160
  %36 = load volatile i8, i8* inttoptr (i64 788530178 to i8*), align 2
  store volatile i8 %36, i8* inttoptr (i64 788530177 to i8*), align 1
  %37 = load volatile i8, i8* inttoptr (i64 788530179 to i8*), align 1
  store volatile i8 %37, i8* inttoptr (i64 788530178 to i8*), align 2
  %38 = load volatile i8, i8* inttoptr (i64 788530181 to i8*), align 1
  store volatile i8 %38, i8* inttoptr (i64 788530180 to i8*), align 4
  %39 = load volatile i8, i8* inttoptr (i64 788530182 to i8*), align 2
  store volatile i8 %39, i8* inttoptr (i64 788530181 to i8*), align 1
  %40 = load volatile i8, i8* inttoptr (i64 788530184 to i8*), align 8
  store volatile i8 %40, i8* inttoptr (i64 788530183 to i8*), align 1
  %41 = load volatile i8, i8* inttoptr (i64 788530185 to i8*), align 1
  store volatile i8 %41, i8* inttoptr (i64 788530184 to i8*), align 8
  %42 = getelementptr inbounds i8, i8* inttoptr (i64 788529217 to i8*), i64 %indvars.iv76
  %43 = load volatile i8, i8* %42, align 1
  store volatile i8 %43, i8* inttoptr (i64 788530179 to i8*), align 1
  %44 = add nuw nsw i64 %indvars.iv76, 160
  %45 = getelementptr inbounds i8, i8* inttoptr (i64 788529217 to i8*), i64 %44
  %46 = load volatile i8, i8* %45, align 1
  %47 = getelementptr inbounds i8, i8* inttoptr (i64 788529217 to i8*), i64 %indvars.iv76
  store volatile i8 %46, i8* %47, align 1
  store volatile i8 %46, i8* inttoptr (i64 788530182 to i8*), align 2
  %48 = icmp slt i32 %read_count.138, 76800
  br i1 %48, label %91, label %94

; <label>:49                                      ; preds = %90, %.preheader13
  %indvars.iv58 = phi i64 [ 0, %.preheader13 ], [ %indvars.iv.next59, %90 ]
  %50 = getelementptr inbounds i8, i8* inttoptr (i64 788530852 to i8*), i64 %indvars.iv58
  %51 = load volatile i8, i8* %50, align 1
  %52 = getelementptr inbounds i8, i8* inttoptr (i64 788530972 to i8*), i64 %indvars.iv58
  %53 = load volatile i8, i8* %52, align 1
  %54 = zext i8 %53 to i32
  br label %55

; <label>:55                                      ; preds = %.thread137, %49
  %indvars.iv55 = phi i64 [ -1, %49 ], [ %indvars.iv.next56, %.thread137 ]
  %count.030 = phi i32 [ 0, %49 ], [ %220, %.thread137 ]
  %val_out.029 = phi i32 [ 0, %49 ], [ %val_out.3.2.2, %.thread137 ]
  %56 = add nsw i64 %indvars.iv55, %indvars.iv79
  %57 = trunc i64 %56 to i32
  %58 = icmp ult i32 %57, 160
  %59 = mul nsw i64 %indvars.iv55, 3
  %60 = add nsw i64 %59, 4
  %61 = and i1 %58, %29
  %62 = sext i32 %count.030 to i64
  br i1 %61, label %113, label %.thread129

; <label>:63                                      ; preds = %.thread137
  %val_out.3.2.2.lcssa = phi i32 [ %val_out.3.2.2, %.thread137 ]
  %64 = getelementptr inbounds i8, i8* inttoptr (i64 788530900 to i8*), i64 %indvars.iv58
  %65 = load volatile i8, i8* %64, align 1
  %66 = icmp sgt i8 %65, -1
  %67 = zext i8 %51 to i32
  %68 = getelementptr inbounds i8, i8* inttoptr (i64 788530876 to i8*), i64 %indvars.iv58
  %69 = load volatile i8, i8* %68, align 1
  %70 = zext i8 %69 to i32
  %71 = mul nuw nsw i32 %70, %67
  %72 = load volatile i8, i8* %64, align 1
  %73 = sext i8 %72 to i32
  %74 = sub nsw i32 0, %73
  %.sink = select i1 %66, i32 %73, i32 %74
  %75 = lshr i32 %71, %.sink
  %76 = add nsw i32 %75, %val_out.3.2.2.lcssa
  %77 = getelementptr inbounds i8, i8* inttoptr (i64 788530924 to i8*), i64 %indvars.iv58
  %78 = load volatile i8, i8* %77, align 1
  %79 = zext i8 %78 to i32
  %80 = mul nsw i32 %76, %79
  %81 = getelementptr inbounds i8, i8* inttoptr (i64 788530948 to i8*), i64 %indvars.iv58
  %82 = load volatile i8, i8* %81, align 1
  %83 = zext i8 %82 to i32
  %84 = ashr i32 %80, %83
  %85 = icmp slt i32 %84, 255
  %. = select i1 %85, i32 %84, i32 255
  br i1 %26, label %86, label %90

; <label>:86                                      ; preds = %63
  %87 = icmp slt i32 %., 0
  %88 = select i1 %87, i32 0, i32 %.
  %89 = trunc i32 %88 to i8
  store volatile i8 %89, i8* inttoptr (i64 788530996 to i8*), align 4
  br label %90

; <label>:90                                      ; preds = %86, %63
  %indvars.iv.next59 = add nuw nsw i64 %indvars.iv58, 1
  %exitcond60 = icmp eq i64 %indvars.iv.next59, 24
  br i1 %exitcond60, label %.preheader12, label %49

; <label>:91                                      ; preds = %.preheader12
  %92 = load volatile i8, i8* inttoptr (i64 788529185 to i8*), align 1
  %93 = add nsw i32 %read_count.138, 1
  br label %94

; <label>:94                                      ; preds = %91, %.preheader12
  %read_count.3 = phi i32 [ %93, %91 ], [ %read_count.138, %.preheader12 ]
  %val_in.0 = phi i8 [ %92, %91 ], [ 0, %.preheader12 ]
  %95 = getelementptr inbounds i8, i8* inttoptr (i64 788529217 to i8*), i64 %35
  store volatile i8 %val_in.0, i8* %95, align 1
  store volatile i8 %val_in.0, i8* inttoptr (i64 788530185 to i8*), align 1
  %96 = load volatile i8, i8* inttoptr (i64 788530187 to i8*), align 1
  store volatile i8 %96, i8* inttoptr (i64 788530186 to i8*), align 2
  %97 = load volatile i8, i8* inttoptr (i64 788530188 to i8*), align 4
  store volatile i8 %97, i8* inttoptr (i64 788530187 to i8*), align 1
  %98 = load volatile i8, i8* inttoptr (i64 788530190 to i8*), align 2
  store volatile i8 %98, i8* inttoptr (i64 788530189 to i8*), align 1
  %99 = load volatile i8, i8* inttoptr (i64 788530191 to i8*), align 1
  store volatile i8 %99, i8* inttoptr (i64 788530190 to i8*), align 2
  %100 = load volatile i8, i8* inttoptr (i64 788530193 to i8*), align 1
  store volatile i8 %100, i8* inttoptr (i64 788530192 to i8*), align 16
  %101 = load volatile i8, i8* inttoptr (i64 788530194 to i8*), align 2
  store volatile i8 %101, i8* inttoptr (i64 788530193 to i8*), align 1
  %102 = add nuw nsw i64 %indvars.iv76, 320
  %103 = getelementptr inbounds i8, i8* inttoptr (i64 788529217 to i8*), i64 %102
  %104 = load volatile i8, i8* %103, align 1
  store volatile i8 %104, i8* inttoptr (i64 788530188 to i8*), align 4
  %105 = add i64 %indvars.iv76, 320
  %106 = add nsw i64 %indvars.iv76, 480
  %107 = getelementptr inbounds i8, i8* inttoptr (i64 788529217 to i8*), i64 %106
  %108 = load volatile i8, i8* %107, align 1
  %109 = getelementptr inbounds i8, i8* inttoptr (i64 788529217 to i8*), i64 %105
  store volatile i8 %108, i8* %109, align 1
  store volatile i8 %108, i8* inttoptr (i64 788530191 to i8*), align 1
  %110 = icmp slt i32 %read_count.3, 76800
  br i1 %110, label %233, label %236

; <label>:111                                     ; preds = %257
  %read_count.3.2.lcssa = phi i32 [ %read_count.3.2, %257 ]
  %indvars.iv.next80 = add nuw nsw i64 %indvars.iv79, 1
  %exitcond81 = icmp eq i64 %indvars.iv.next80, 160
  br i1 %exitcond81, label %112, label %.preheader14

; <label>:112                                     ; preds = %111
  ret void

; <label>:113                                     ; preds = %55
  %114 = add nsw i64 %59, 3
  %115 = getelementptr inbounds i8, i8* inttoptr (i64 788530177 to i8*), i64 %114
  %116 = load volatile i8, i8* %115, align 1
  %117 = zext i8 %116 to i32
  %118 = getelementptr inbounds i8, i8* inttoptr (i64 788530204 to i8*), i64 %62
  %119 = load volatile i8, i8* %118, align 1
  %120 = zext i8 %119 to i32
  %121 = sub nsw i32 %120, %54
  %122 = shl nuw nsw i32 %117, 16
  %sext = mul i32 %122, %121
  %123 = ashr exact i32 %sext, 16
  %124 = add nsw i32 %123, %val_out.029
  %indvars.iv.next51 = add nsw i64 %62, 1
  %125 = add nsw i64 %59, 12
  %126 = getelementptr inbounds i8, i8* inttoptr (i64 788530177 to i8*), i64 %125
  %127 = load volatile i8, i8* %126, align 1
  %128 = zext i8 %127 to i32
  %129 = getelementptr inbounds i8, i8* inttoptr (i64 788530204 to i8*), i64 %indvars.iv.next51
  %130 = load volatile i8, i8* %129, align 1
  %131 = zext i8 %130 to i32
  %132 = sub nsw i32 %131, %54
  %133 = shl nuw nsw i32 %128, 16
  %sext.1 = mul i32 %133, %132
  %134 = ashr exact i32 %sext.1, 16
  %135 = add nsw i32 %134, %124
  %indvars.iv.next51.1 = add nsw i64 %62, 2
  %136 = add nsw i64 %59, 21
  %137 = getelementptr inbounds i8, i8* inttoptr (i64 788530177 to i8*), i64 %136
  %138 = load volatile i8, i8* %137, align 1
  %139 = zext i8 %138 to i32
  %140 = getelementptr inbounds i8, i8* inttoptr (i64 788530204 to i8*), i64 %indvars.iv.next51.1
  %141 = load volatile i8, i8* %140, align 1
  %142 = zext i8 %141 to i32
  %143 = sub nsw i32 %142, %54
  %144 = shl nuw nsw i32 %139, 16
  %sext.2 = mul i32 %144, %143
  %145 = ashr exact i32 %sext.2, 16
  %146 = add nsw i32 %145, %135
  br label %.thread129

.thread129:                                       ; preds = %55, %113
  %val_out.3.2 = phi i32 [ %146, %113 ], [ %val_out.029, %55 ]
  %147 = add i32 %count.030, 3
  %148 = and i1 %58, %31
  %149 = sext i32 %147 to i64
  br i1 %148, label %150, label %.thread133

; <label>:150                                     ; preds = %.thread129
  %151 = getelementptr inbounds i8, i8* inttoptr (i64 788530177 to i8*), i64 %60
  %152 = load volatile i8, i8* %151, align 1
  %153 = zext i8 %152 to i32
  %154 = getelementptr inbounds i8, i8* inttoptr (i64 788530204 to i8*), i64 %149
  %155 = load volatile i8, i8* %154, align 1
  %156 = zext i8 %155 to i32
  %157 = sub nsw i32 %156, %54
  %158 = shl nuw nsw i32 %153, 16
  %sext.1111 = mul i32 %158, %157
  %159 = ashr exact i32 %sext.1111, 16
  %160 = add nsw i32 %159, %val_out.3.2
  %indvars.iv.next51.1113 = add nsw i64 %149, 1
  %161 = add nsw i64 %59, 13
  %162 = getelementptr inbounds i8, i8* inttoptr (i64 788530177 to i8*), i64 %161
  %163 = load volatile i8, i8* %162, align 1
  %164 = zext i8 %163 to i32
  %165 = getelementptr inbounds i8, i8* inttoptr (i64 788530204 to i8*), i64 %indvars.iv.next51.1113
  %166 = load volatile i8, i8* %165, align 1
  %167 = zext i8 %166 to i32
  %168 = sub nsw i32 %167, %54
  %169 = shl nuw nsw i32 %164, 16
  %sext.1.1 = mul i32 %169, %168
  %170 = ashr exact i32 %sext.1.1, 16
  %171 = add nsw i32 %170, %160
  %indvars.iv.next51.1.1 = add nsw i64 %149, 2
  %172 = add nsw i64 %59, 22
  %173 = getelementptr inbounds i8, i8* inttoptr (i64 788530177 to i8*), i64 %172
  %174 = load volatile i8, i8* %173, align 1
  %175 = zext i8 %174 to i32
  %176 = getelementptr inbounds i8, i8* inttoptr (i64 788530204 to i8*), i64 %indvars.iv.next51.1.1
  %177 = load volatile i8, i8* %176, align 1
  %178 = zext i8 %177 to i32
  %179 = sub nsw i32 %178, %54
  %180 = shl nuw nsw i32 %175, 16
  %sext.2.1 = mul i32 %180, %179
  %181 = ashr exact i32 %sext.2.1, 16
  %182 = add nsw i32 %181, %171
  br label %.thread133

.thread133:                                       ; preds = %.thread129, %150
  %val_out.3.2.1 = phi i32 [ %182, %150 ], [ %val_out.3.2, %.thread129 ]
  %183 = add i32 %count.030, 6
  %184 = and i1 %58, %34
  %185 = sext i32 %183 to i64
  br i1 %184, label %186, label %.thread137

; <label>:186                                     ; preds = %.thread133
  %187 = add nsw i64 %59, 5
  %188 = getelementptr inbounds i8, i8* inttoptr (i64 788530177 to i8*), i64 %187
  %189 = load volatile i8, i8* %188, align 1
  %190 = zext i8 %189 to i32
  %191 = getelementptr inbounds i8, i8* inttoptr (i64 788530204 to i8*), i64 %185
  %192 = load volatile i8, i8* %191, align 1
  %193 = zext i8 %192 to i32
  %194 = sub nsw i32 %193, %54
  %195 = shl nuw nsw i32 %190, 16
  %sext.2114 = mul i32 %195, %194
  %196 = ashr exact i32 %sext.2114, 16
  %197 = add nsw i32 %196, %val_out.3.2.1
  %indvars.iv.next51.2 = add nsw i64 %185, 1
  %198 = add nsw i64 %59, 14
  %199 = getelementptr inbounds i8, i8* inttoptr (i64 788530177 to i8*), i64 %198
  %200 = load volatile i8, i8* %199, align 1
  %201 = zext i8 %200 to i32
  %202 = getelementptr inbounds i8, i8* inttoptr (i64 788530204 to i8*), i64 %indvars.iv.next51.2
  %203 = load volatile i8, i8* %202, align 1
  %204 = zext i8 %203 to i32
  %205 = sub nsw i32 %204, %54
  %206 = shl nuw nsw i32 %201, 16
  %sext.1.2 = mul i32 %206, %205
  %207 = ashr exact i32 %sext.1.2, 16
  %208 = add nsw i32 %207, %197
  %indvars.iv.next51.1.2 = add nsw i64 %185, 2
  %209 = add nsw i64 %59, 23
  %210 = getelementptr inbounds i8, i8* inttoptr (i64 788530177 to i8*), i64 %209
  %211 = load volatile i8, i8* %210, align 1
  %212 = zext i8 %211 to i32
  %213 = getelementptr inbounds i8, i8* inttoptr (i64 788530204 to i8*), i64 %indvars.iv.next51.1.2
  %214 = load volatile i8, i8* %213, align 1
  %215 = zext i8 %214 to i32
  %216 = sub nsw i32 %215, %54
  %217 = shl nuw nsw i32 %212, 16
  %sext.2.2 = mul i32 %217, %216
  %218 = ashr exact i32 %sext.2.2, 16
  %219 = add nsw i32 %218, %208
  br label %.thread137

.thread137:                                       ; preds = %.thread133, %186
  %val_out.3.2.2 = phi i32 [ %219, %186 ], [ %val_out.3.2.1, %.thread133 ]
  %220 = add nuw nsw i32 %count.030, 9
  %indvars.iv.next56 = add nsw i64 %indvars.iv55, 1
  %exitcond57 = icmp eq i64 %indvars.iv.next56, 2
  br i1 %exitcond57, label %63, label %55

.preheader14.preheader122:                        ; preds = %.preheader19
  %221 = load volatile i8, i8* inttoptr (i64 788529375 to i8*), align 1
  store volatile i8 %221, i8* inttoptr (i64 788530181 to i8*), align 1
  %222 = load volatile i8, i8* inttoptr (i64 788529695 to i8*), align 1
  store volatile i8 %222, i8* inttoptr (i64 788530190 to i8*), align 2
  %223 = load volatile i8, i8* inttoptr (i64 788530015 to i8*), align 1
  store volatile i8 %223, i8* inttoptr (i64 788530199 to i8*), align 1
  %224 = load volatile i8, i8* inttoptr (i64 788529376 to i8*), align 32
  store volatile i8 %224, i8* inttoptr (i64 788530182 to i8*), align 2
  %225 = load volatile i8, i8* inttoptr (i64 788529696 to i8*), align 32
  store volatile i8 %225, i8* inttoptr (i64 788530191 to i8*), align 1
  %226 = load volatile i8, i8* inttoptr (i64 788530016 to i8*), align 32
  store volatile i8 %226, i8* inttoptr (i64 788530200 to i8*), align 8
  %227 = load volatile i8, i8* inttoptr (i64 788529535 to i8*), align 1
  store volatile i8 %227, i8* inttoptr (i64 788530184 to i8*), align 8
  %228 = load volatile i8, i8* inttoptr (i64 788529855 to i8*), align 1
  store volatile i8 %228, i8* inttoptr (i64 788530193 to i8*), align 1
  %229 = load volatile i8, i8* inttoptr (i64 788530175 to i8*), align 1
  store volatile i8 %229, i8* inttoptr (i64 788530202 to i8*), align 2
  %230 = load volatile i8, i8* inttoptr (i64 788529536 to i8*), align 128
  store volatile i8 %230, i8* inttoptr (i64 788530185 to i8*), align 1
  %231 = load volatile i8, i8* inttoptr (i64 788529856 to i8*), align 64
  store volatile i8 %231, i8* inttoptr (i64 788530194 to i8*), align 2
  %232 = load volatile i8, i8* inttoptr (i64 788530176 to i8*), align 1024
  store volatile i8 %232, i8* inttoptr (i64 788530203 to i8*), align 1
  br label %.preheader14

; <label>:233                                     ; preds = %94
  %234 = load volatile i8, i8* inttoptr (i64 788529185 to i8*), align 1
  %235 = add nsw i32 %read_count.3, 1
  br label %236

; <label>:236                                     ; preds = %233, %94
  %read_count.3.1 = phi i32 [ %235, %233 ], [ %read_count.3, %94 ]
  %val_in.0.1 = phi i8 [ %234, %233 ], [ 0, %94 ]
  %237 = add nsw i64 %indvars.iv76, 480
  %238 = getelementptr inbounds i8, i8* inttoptr (i64 788529217 to i8*), i64 %237
  store volatile i8 %val_in.0.1, i8* %238, align 1
  store volatile i8 %val_in.0.1, i8* inttoptr (i64 788530194 to i8*), align 2
  %239 = load volatile i8, i8* inttoptr (i64 788530196 to i8*), align 4
  store volatile i8 %239, i8* inttoptr (i64 788530195 to i8*), align 1
  %240 = load volatile i8, i8* inttoptr (i64 788530197 to i8*), align 1
  store volatile i8 %240, i8* inttoptr (i64 788530196 to i8*), align 4
  %241 = load volatile i8, i8* inttoptr (i64 788530199 to i8*), align 1
  store volatile i8 %241, i8* inttoptr (i64 788530198 to i8*), align 2
  %242 = load volatile i8, i8* inttoptr (i64 788530200 to i8*), align 8
  store volatile i8 %242, i8* inttoptr (i64 788530199 to i8*), align 1
  %243 = load volatile i8, i8* inttoptr (i64 788530202 to i8*), align 2
  store volatile i8 %243, i8* inttoptr (i64 788530201 to i8*), align 1
  %244 = load volatile i8, i8* inttoptr (i64 788530203 to i8*), align 1
  store volatile i8 %244, i8* inttoptr (i64 788530202 to i8*), align 2
  %245 = add nuw nsw i64 %indvars.iv76, 640
  %246 = getelementptr inbounds i8, i8* inttoptr (i64 788529217 to i8*), i64 %245
  %247 = load volatile i8, i8* %246, align 1
  store volatile i8 %247, i8* inttoptr (i64 788530197 to i8*), align 1
  %248 = add i64 %indvars.iv76, 640
  %249 = add nsw i64 %indvars.iv76, 800
  %250 = getelementptr inbounds i8, i8* inttoptr (i64 788529217 to i8*), i64 %249
  %251 = load volatile i8, i8* %250, align 1
  %252 = getelementptr inbounds i8, i8* inttoptr (i64 788529217 to i8*), i64 %248
  store volatile i8 %251, i8* %252, align 1
  store volatile i8 %251, i8* inttoptr (i64 788530200 to i8*), align 8
  %253 = icmp slt i32 %read_count.3.1, 76800
  br i1 %253, label %254, label %257

; <label>:254                                     ; preds = %236
  %255 = load volatile i8, i8* inttoptr (i64 788529185 to i8*), align 1
  %256 = add nsw i32 %read_count.3.1, 1
  br label %257

; <label>:257                                     ; preds = %254, %236
  %read_count.3.2 = phi i32 [ %256, %254 ], [ %read_count.3.1, %236 ]
  %val_in.0.2 = phi i8 [ %255, %254 ], [ 0, %236 ]
  %258 = add nsw i64 %indvars.iv76, 800
  %259 = getelementptr inbounds i8, i8* inttoptr (i64 788529217 to i8*), i64 %258
  store volatile i8 %val_in.0.2, i8* %259, align 1
  store volatile i8 %val_in.0.2, i8* inttoptr (i64 788530203 to i8*), align 1
  %indvars.iv.next77 = add nuw nsw i64 %indvars.iv76, 1
  %exitcond78 = icmp eq i64 %indvars.iv.next77, 160
  br i1 %exitcond78, label %111, label %.preheader13
}

attributes #0 = { norecurse nounwind uwtable "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.ident = !{!0}

!0 = !{!"clang version 3.8.1 "}
