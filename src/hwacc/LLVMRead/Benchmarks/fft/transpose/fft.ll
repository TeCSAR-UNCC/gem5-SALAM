; ModuleID = 'fft.c'
target datalayout = "e-p:64:64:64-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:64:64-f32:32:32-f64:64:64-v64:64:64-v128:128:128-a0:0:64-s0:64:64-f80:128:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@twiddles8.reversed8 = private unnamed_addr constant [8 x i32] [i32 0, i32 4, i32 2, i32 6, i32 1, i32 5, i32 3, i32 7], align 16

; Function Attrs: nounwind uwtable
define void @twiddles8(double* nocapture %a_x, double* nocapture %a_y, i32 %i, i32 %n) #0 {
  %1 = sitofp i32 %n to double
  %2 = sitofp i32 %i to double
  br label %3

; <label>:3                                       ; preds = %3, %0
  %indvars.iv = phi i64 [ 1, %0 ], [ %indvars.iv.next, %3 ]
  %4 = getelementptr inbounds [8 x i32]* @twiddles8.reversed8, i64 0, i64 %indvars.iv
  %5 = load i32* %4, align 4, !tbaa !1
  %6 = sitofp i32 %5 to double
  %7 = fmul double %6, 0xC01921FB54411744
  %8 = fdiv double %7, %1
  %9 = fmul double %2, %8
  %10 = tail call double @cos(double %9) #3
  %11 = tail call double @sin(double %9) #3
  %12 = getelementptr inbounds double* %a_x, i64 %indvars.iv
  %13 = load double* %12, align 8, !tbaa !5
  %14 = fmul double %10, %13
  %15 = getelementptr inbounds double* %a_y, i64 %indvars.iv
  %16 = load double* %15, align 8, !tbaa !5
  %17 = fmul double %11, %16
  %18 = fsub double %14, %17
  store double %18, double* %12, align 8, !tbaa !5
  %19 = fmul double %11, %13
  %20 = load double* %15, align 8, !tbaa !5
  %21 = fmul double %10, %20
  %22 = fadd double %19, %21
  store double %22, double* %15, align 8, !tbaa !5
  %indvars.iv.next = add nuw nsw i64 %indvars.iv, 1
  %exitcond = icmp eq i64 %indvars.iv.next, 8
  br i1 %exitcond, label %23, label %3

; <label>:23                                      ; preds = %3
  ret void
}

; Function Attrs: nounwind
declare double @cos(double) #1

; Function Attrs: nounwind
declare double @sin(double) #1

; Function Attrs: nounwind uwtable
define void @loadx8(double* nocapture %a_x, double* nocapture readonly %x, i32 %offset, i32 %sx) #0 {
  %1 = sext i32 %offset to i64
  %2 = getelementptr inbounds double* %x, i64 %1
  %3 = load double* %2, align 8, !tbaa !5
  store double %3, double* %a_x, align 8, !tbaa !5
  %4 = add nsw i32 %sx, %offset
  %5 = sext i32 %4 to i64
  %6 = getelementptr inbounds double* %x, i64 %5
  %7 = load double* %6, align 8, !tbaa !5
  %8 = getelementptr inbounds double* %a_x, i64 1
  store double %7, double* %8, align 8, !tbaa !5
  %9 = shl i32 %sx, 1
  %10 = add nsw i32 %9, %offset
  %11 = sext i32 %10 to i64
  %12 = getelementptr inbounds double* %x, i64 %11
  %13 = load double* %12, align 8, !tbaa !5
  %14 = getelementptr inbounds double* %a_x, i64 2
  store double %13, double* %14, align 8, !tbaa !5
  %15 = mul nsw i32 %sx, 3
  %16 = add nsw i32 %15, %offset
  %17 = sext i32 %16 to i64
  %18 = getelementptr inbounds double* %x, i64 %17
  %19 = load double* %18, align 8, !tbaa !5
  %20 = getelementptr inbounds double* %a_x, i64 3
  store double %19, double* %20, align 8, !tbaa !5
  %21 = shl i32 %sx, 2
  %22 = add nsw i32 %21, %offset
  %23 = sext i32 %22 to i64
  %24 = getelementptr inbounds double* %x, i64 %23
  %25 = load double* %24, align 8, !tbaa !5
  %26 = getelementptr inbounds double* %a_x, i64 4
  store double %25, double* %26, align 8, !tbaa !5
  %27 = mul nsw i32 %sx, 5
  %28 = add nsw i32 %27, %offset
  %29 = sext i32 %28 to i64
  %30 = getelementptr inbounds double* %x, i64 %29
  %31 = load double* %30, align 8, !tbaa !5
  %32 = getelementptr inbounds double* %a_x, i64 5
  store double %31, double* %32, align 8, !tbaa !5
  %33 = mul nsw i32 %sx, 6
  %34 = add nsw i32 %33, %offset
  %35 = sext i32 %34 to i64
  %36 = getelementptr inbounds double* %x, i64 %35
  %37 = load double* %36, align 8, !tbaa !5
  %38 = getelementptr inbounds double* %a_x, i64 6
  store double %37, double* %38, align 8, !tbaa !5
  %39 = mul nsw i32 %sx, 7
  %40 = add nsw i32 %39, %offset
  %41 = sext i32 %40 to i64
  %42 = getelementptr inbounds double* %x, i64 %41
  %43 = load double* %42, align 8, !tbaa !5
  %44 = getelementptr inbounds double* %a_x, i64 7
  store double %43, double* %44, align 8, !tbaa !5
  ret void
}

; Function Attrs: nounwind uwtable
define void @loady8(double* nocapture %a_y, double* nocapture readonly %x, i32 %offset, i32 %sx) #0 {
  %1 = sext i32 %offset to i64
  %2 = getelementptr inbounds double* %x, i64 %1
  %3 = load double* %2, align 8, !tbaa !5
  store double %3, double* %a_y, align 8, !tbaa !5
  %4 = add nsw i32 %sx, %offset
  %5 = sext i32 %4 to i64
  %6 = getelementptr inbounds double* %x, i64 %5
  %7 = load double* %6, align 8, !tbaa !5
  %8 = getelementptr inbounds double* %a_y, i64 1
  store double %7, double* %8, align 8, !tbaa !5
  %9 = shl i32 %sx, 1
  %10 = add nsw i32 %9, %offset
  %11 = sext i32 %10 to i64
  %12 = getelementptr inbounds double* %x, i64 %11
  %13 = load double* %12, align 8, !tbaa !5
  %14 = getelementptr inbounds double* %a_y, i64 2
  store double %13, double* %14, align 8, !tbaa !5
  %15 = mul nsw i32 %sx, 3
  %16 = add nsw i32 %15, %offset
  %17 = sext i32 %16 to i64
  %18 = getelementptr inbounds double* %x, i64 %17
  %19 = load double* %18, align 8, !tbaa !5
  %20 = getelementptr inbounds double* %a_y, i64 3
  store double %19, double* %20, align 8, !tbaa !5
  %21 = shl i32 %sx, 2
  %22 = add nsw i32 %21, %offset
  %23 = sext i32 %22 to i64
  %24 = getelementptr inbounds double* %x, i64 %23
  %25 = load double* %24, align 8, !tbaa !5
  %26 = getelementptr inbounds double* %a_y, i64 4
  store double %25, double* %26, align 8, !tbaa !5
  %27 = mul nsw i32 %sx, 5
  %28 = add nsw i32 %27, %offset
  %29 = sext i32 %28 to i64
  %30 = getelementptr inbounds double* %x, i64 %29
  %31 = load double* %30, align 8, !tbaa !5
  %32 = getelementptr inbounds double* %a_y, i64 5
  store double %31, double* %32, align 8, !tbaa !5
  %33 = mul nsw i32 %sx, 6
  %34 = add nsw i32 %33, %offset
  %35 = sext i32 %34 to i64
  %36 = getelementptr inbounds double* %x, i64 %35
  %37 = load double* %36, align 8, !tbaa !5
  %38 = getelementptr inbounds double* %a_y, i64 6
  store double %37, double* %38, align 8, !tbaa !5
  %39 = mul nsw i32 %sx, 7
  %40 = add nsw i32 %39, %offset
  %41 = sext i32 %40 to i64
  %42 = getelementptr inbounds double* %x, i64 %41
  %43 = load double* %42, align 8, !tbaa !5
  %44 = getelementptr inbounds double* %a_y, i64 7
  store double %43, double* %44, align 8, !tbaa !5
  ret void
}

; Function Attrs: nounwind uwtable
define void @fft1D_512(double* nocapture %work_x, double* nocapture %work_y, double* %smem, double* %DATA_x, double* %DATA_y) #0 {
  %data_x = alloca [8 x double], align 16
  %data_y = alloca [8 x double], align 16
  %1 = bitcast [8 x double]* %data_x to i8*
  call void @llvm.lifetime.start(i64 64, i8* %1) #2
  %2 = bitcast [8 x double]* %data_y to i8*
  call void @llvm.lifetime.start(i64 64, i8* %2) #2
  %3 = getelementptr inbounds [8 x double]* %data_x, i64 0, i64 0
  %4 = getelementptr inbounds [8 x double]* %data_x, i64 0, i64 1
  %5 = getelementptr inbounds [8 x double]* %data_x, i64 0, i64 2
  %6 = getelementptr inbounds [8 x double]* %data_x, i64 0, i64 3
  %7 = getelementptr inbounds [8 x double]* %data_x, i64 0, i64 4
  %8 = getelementptr inbounds [8 x double]* %data_x, i64 0, i64 5
  %9 = getelementptr inbounds [8 x double]* %data_x, i64 0, i64 6
  %10 = getelementptr inbounds [8 x double]* %data_x, i64 0, i64 7
  %11 = getelementptr inbounds [8 x double]* %data_y, i64 0, i64 0
  %12 = getelementptr inbounds [8 x double]* %data_y, i64 0, i64 1
  %13 = getelementptr inbounds [8 x double]* %data_y, i64 0, i64 2
  %14 = getelementptr inbounds [8 x double]* %data_y, i64 0, i64 3
  %15 = getelementptr inbounds [8 x double]* %data_y, i64 0, i64 4
  %16 = getelementptr inbounds [8 x double]* %data_y, i64 0, i64 5
  %17 = getelementptr inbounds [8 x double]* %data_y, i64 0, i64 6
  %18 = getelementptr inbounds [8 x double]* %data_y, i64 0, i64 7
  br label %19

; <label>:19                                      ; preds = %19, %0
  %indvars.iv177 = phi i64 [ 0, %0 ], [ %indvars.iv.next178, %19 ]
  %20 = getelementptr inbounds double* %work_x, i64 %indvars.iv177
  %21 = load double* %20, align 8, !tbaa !5
  store double %21, double* %3, align 16, !tbaa !5
  %22 = add nsw i64 %indvars.iv177, 64
  %23 = getelementptr inbounds double* %work_x, i64 %22
  %24 = load double* %23, align 8, !tbaa !5
  store double %24, double* %4, align 8, !tbaa !5
  %25 = add nsw i64 %indvars.iv177, 128
  %26 = getelementptr inbounds double* %work_x, i64 %25
  %27 = load double* %26, align 8, !tbaa !5
  store double %27, double* %5, align 16, !tbaa !5
  %28 = add nsw i64 %indvars.iv177, 192
  %29 = getelementptr inbounds double* %work_x, i64 %28
  %30 = load double* %29, align 8, !tbaa !5
  store double %30, double* %6, align 8, !tbaa !5
  %31 = add nsw i64 %indvars.iv177, 256
  %32 = getelementptr inbounds double* %work_x, i64 %31
  %33 = load double* %32, align 8, !tbaa !5
  store double %33, double* %7, align 16, !tbaa !5
  %34 = add nsw i64 %indvars.iv177, 320
  %35 = getelementptr inbounds double* %work_x, i64 %34
  %36 = load double* %35, align 8, !tbaa !5
  store double %36, double* %8, align 8, !tbaa !5
  %37 = add nsw i64 %indvars.iv177, 384
  %38 = getelementptr inbounds double* %work_x, i64 %37
  %39 = load double* %38, align 8, !tbaa !5
  store double %39, double* %9, align 16, !tbaa !5
  %40 = add nsw i64 %indvars.iv177, 448
  %41 = getelementptr inbounds double* %work_x, i64 %40
  %42 = load double* %41, align 8, !tbaa !5
  store double %42, double* %10, align 8, !tbaa !5
  %43 = getelementptr inbounds double* %work_y, i64 %indvars.iv177
  %44 = load double* %43, align 8, !tbaa !5
  store double %44, double* %11, align 16, !tbaa !5
  %45 = getelementptr inbounds double* %work_y, i64 %22
  %46 = load double* %45, align 8, !tbaa !5
  store double %46, double* %12, align 8, !tbaa !5
  %47 = getelementptr inbounds double* %work_y, i64 %25
  %48 = load double* %47, align 8, !tbaa !5
  store double %48, double* %13, align 16, !tbaa !5
  %49 = getelementptr inbounds double* %work_y, i64 %28
  %50 = load double* %49, align 8, !tbaa !5
  store double %50, double* %14, align 8, !tbaa !5
  %51 = getelementptr inbounds double* %work_y, i64 %31
  %52 = load double* %51, align 8, !tbaa !5
  store double %52, double* %15, align 16, !tbaa !5
  %53 = getelementptr inbounds double* %work_y, i64 %34
  %54 = load double* %53, align 8, !tbaa !5
  store double %54, double* %16, align 8, !tbaa !5
  %55 = getelementptr inbounds double* %work_y, i64 %37
  %56 = load double* %55, align 8, !tbaa !5
  store double %56, double* %17, align 16, !tbaa !5
  %57 = getelementptr inbounds double* %work_y, i64 %40
  %58 = load double* %57, align 8, !tbaa !5
  store double %58, double* %18, align 8, !tbaa !5
  %59 = load double* %3, align 16, !tbaa !5
  %60 = load double* %11, align 16, !tbaa !5
  %61 = load double* %7, align 16, !tbaa !5
  %62 = fadd double %59, %61
  store double %62, double* %3, align 16, !tbaa !5
  %63 = load double* %15, align 16, !tbaa !5
  %64 = fadd double %60, %63
  store double %64, double* %11, align 16, !tbaa !5
  %65 = load double* %7, align 16, !tbaa !5
  %66 = fsub double %59, %65
  store double %66, double* %7, align 16, !tbaa !5
  %67 = load double* %15, align 16, !tbaa !5
  %68 = fsub double %60, %67
  store double %68, double* %15, align 16, !tbaa !5
  %69 = load double* %4, align 8, !tbaa !5
  %70 = load double* %12, align 8, !tbaa !5
  %71 = load double* %8, align 8, !tbaa !5
  %72 = fadd double %69, %71
  store double %72, double* %4, align 8, !tbaa !5
  %73 = load double* %16, align 8, !tbaa !5
  %74 = fadd double %70, %73
  store double %74, double* %12, align 8, !tbaa !5
  %75 = load double* %8, align 8, !tbaa !5
  %76 = fsub double %69, %75
  store double %76, double* %8, align 8, !tbaa !5
  %77 = load double* %16, align 8, !tbaa !5
  %78 = fsub double %70, %77
  store double %78, double* %16, align 8, !tbaa !5
  %79 = load double* %5, align 16, !tbaa !5
  %80 = load double* %13, align 16, !tbaa !5
  %81 = load double* %9, align 16, !tbaa !5
  %82 = fadd double %79, %81
  store double %82, double* %5, align 16, !tbaa !5
  %83 = load double* %17, align 16, !tbaa !5
  %84 = fadd double %80, %83
  store double %84, double* %13, align 16, !tbaa !5
  %85 = load double* %9, align 16, !tbaa !5
  %86 = fsub double %79, %85
  store double %86, double* %9, align 16, !tbaa !5
  %87 = load double* %17, align 16, !tbaa !5
  %88 = fsub double %80, %87
  store double %88, double* %17, align 16, !tbaa !5
  %89 = load double* %6, align 8, !tbaa !5
  %90 = load double* %14, align 8, !tbaa !5
  %91 = load double* %10, align 8, !tbaa !5
  %92 = fadd double %89, %91
  store double %92, double* %6, align 8, !tbaa !5
  %93 = load double* %18, align 8, !tbaa !5
  %94 = fadd double %90, %93
  store double %94, double* %14, align 8, !tbaa !5
  %95 = load double* %10, align 8, !tbaa !5
  %96 = fsub double %89, %95
  store double %96, double* %10, align 8, !tbaa !5
  %97 = load double* %18, align 8, !tbaa !5
  %98 = fsub double %90, %97
  store double %98, double* %18, align 8, !tbaa !5
  %99 = load double* %8, align 8, !tbaa !5
  %100 = load double* %16, align 8, !tbaa !5
  %101 = fmul double %100, -1.000000e+00
  %102 = fsub double %99, %101
  %103 = fmul double %102, 0x3FE6A09E667F3BCD
  store double %103, double* %8, align 8, !tbaa !5
  %104 = fmul double %99, -1.000000e+00
  %105 = load double* %16, align 8, !tbaa !5
  %106 = fadd double %104, %105
  %107 = fmul double %106, 0x3FE6A09E667F3BCD
  store double %107, double* %16, align 8, !tbaa !5
  %108 = load double* %9, align 16, !tbaa !5
  %109 = fmul double %108, 0.000000e+00
  %110 = load double* %17, align 16, !tbaa !5
  %111 = fmul double %110, -1.000000e+00
  %112 = fsub double %109, %111
  store double %112, double* %9, align 16, !tbaa !5
  %113 = fmul double %108, -1.000000e+00
  %114 = load double* %17, align 16, !tbaa !5
  %115 = fmul double %114, 0.000000e+00
  %116 = fadd double %113, %115
  store double %116, double* %17, align 16, !tbaa !5
  %117 = load double* %10, align 8, !tbaa !5
  %118 = fmul double %117, -1.000000e+00
  %119 = load double* %18, align 8, !tbaa !5
  %120 = fmul double %119, -1.000000e+00
  %121 = fsub double %118, %120
  %122 = fmul double %121, 0x3FE6A09E667F3BCD
  store double %122, double* %10, align 8, !tbaa !5
  %123 = load double* %18, align 8, !tbaa !5
  %124 = fmul double %123, -1.000000e+00
  %125 = fadd double %118, %124
  %126 = fmul double %125, 0x3FE6A09E667F3BCD
  store double %126, double* %18, align 8, !tbaa !5
  %127 = load double* %3, align 16, !tbaa !5
  %128 = load double* %11, align 16, !tbaa !5
  %129 = load double* %5, align 16, !tbaa !5
  %130 = fadd double %127, %129
  store double %130, double* %3, align 16, !tbaa !5
  %131 = load double* %13, align 16, !tbaa !5
  %132 = fadd double %128, %131
  store double %132, double* %11, align 16, !tbaa !5
  %133 = load double* %5, align 16, !tbaa !5
  %134 = fsub double %127, %133
  store double %134, double* %5, align 16, !tbaa !5
  %135 = load double* %13, align 16, !tbaa !5
  %136 = fsub double %128, %135
  store double %136, double* %13, align 16, !tbaa !5
  %137 = load double* %4, align 8, !tbaa !5
  %138 = load double* %12, align 8, !tbaa !5
  %139 = load double* %6, align 8, !tbaa !5
  %140 = fadd double %137, %139
  store double %140, double* %4, align 8, !tbaa !5
  %141 = load double* %14, align 8, !tbaa !5
  %142 = fadd double %138, %141
  store double %142, double* %12, align 8, !tbaa !5
  %143 = load double* %6, align 8, !tbaa !5
  %144 = fsub double %137, %143
  store double %144, double* %6, align 8, !tbaa !5
  %145 = load double* %14, align 8, !tbaa !5
  %146 = fsub double %138, %145
  store double %146, double* %14, align 8, !tbaa !5
  %147 = load double* %6, align 8, !tbaa !5
  %148 = fmul double %147, 0.000000e+00
  %149 = fmul double %146, -1.000000e+00
  %150 = fsub double %148, %149
  store double %150, double* %6, align 8, !tbaa !5
  %151 = fmul double %147, -1.000000e+00
  %152 = load double* %14, align 8, !tbaa !5
  %153 = fmul double %152, 0.000000e+00
  %154 = fsub double %151, %153
  store double %154, double* %14, align 8, !tbaa !5
  %155 = load double* %3, align 16, !tbaa !5
  %156 = load double* %11, align 16, !tbaa !5
  %157 = load double* %4, align 8, !tbaa !5
  %158 = fadd double %155, %157
  store double %158, double* %3, align 16, !tbaa !5
  %159 = load double* %12, align 8, !tbaa !5
  %160 = fadd double %156, %159
  store double %160, double* %11, align 16, !tbaa !5
  %161 = load double* %4, align 8, !tbaa !5
  %162 = fsub double %155, %161
  store double %162, double* %4, align 8, !tbaa !5
  %163 = load double* %12, align 8, !tbaa !5
  %164 = fsub double %156, %163
  store double %164, double* %12, align 8, !tbaa !5
  %165 = load double* %5, align 16, !tbaa !5
  %166 = load double* %13, align 16, !tbaa !5
  %167 = load double* %6, align 8, !tbaa !5
  %168 = fadd double %165, %167
  store double %168, double* %5, align 16, !tbaa !5
  %169 = load double* %14, align 8, !tbaa !5
  %170 = fadd double %166, %169
  store double %170, double* %13, align 16, !tbaa !5
  %171 = load double* %6, align 8, !tbaa !5
  %172 = fsub double %165, %171
  store double %172, double* %6, align 8, !tbaa !5
  %173 = load double* %14, align 8, !tbaa !5
  %174 = fsub double %166, %173
  store double %174, double* %14, align 8, !tbaa !5
  %175 = load double* %7, align 16, !tbaa !5
  %176 = load double* %15, align 16, !tbaa !5
  %177 = load double* %9, align 16, !tbaa !5
  %178 = fadd double %175, %177
  store double %178, double* %7, align 16, !tbaa !5
  %179 = load double* %17, align 16, !tbaa !5
  %180 = fadd double %176, %179
  store double %180, double* %15, align 16, !tbaa !5
  %181 = load double* %9, align 16, !tbaa !5
  %182 = fsub double %175, %181
  store double %182, double* %9, align 16, !tbaa !5
  %183 = load double* %17, align 16, !tbaa !5
  %184 = fsub double %176, %183
  store double %184, double* %17, align 16, !tbaa !5
  %185 = load double* %8, align 8, !tbaa !5
  %186 = load double* %16, align 8, !tbaa !5
  %187 = load double* %10, align 8, !tbaa !5
  %188 = fadd double %185, %187
  store double %188, double* %8, align 8, !tbaa !5
  %189 = load double* %18, align 8, !tbaa !5
  %190 = fadd double %186, %189
  store double %190, double* %16, align 8, !tbaa !5
  %191 = load double* %10, align 8, !tbaa !5
  %192 = fsub double %185, %191
  store double %192, double* %10, align 8, !tbaa !5
  %193 = load double* %18, align 8, !tbaa !5
  %194 = fsub double %186, %193
  store double %194, double* %18, align 8, !tbaa !5
  %195 = load double* %10, align 8, !tbaa !5
  %196 = fmul double %195, 0.000000e+00
  %197 = fmul double %194, -1.000000e+00
  %198 = fsub double %196, %197
  store double %198, double* %10, align 8, !tbaa !5
  %199 = fmul double %195, -1.000000e+00
  %200 = load double* %18, align 8, !tbaa !5
  %201 = fmul double %200, 0.000000e+00
  %202 = fsub double %199, %201
  store double %202, double* %18, align 8, !tbaa !5
  %203 = load double* %7, align 16, !tbaa !5
  %204 = load double* %15, align 16, !tbaa !5
  %205 = load double* %8, align 8, !tbaa !5
  %206 = fadd double %203, %205
  store double %206, double* %7, align 16, !tbaa !5
  %207 = load double* %16, align 8, !tbaa !5
  %208 = fadd double %204, %207
  store double %208, double* %15, align 16, !tbaa !5
  %209 = load double* %8, align 8, !tbaa !5
  %210 = fsub double %203, %209
  store double %210, double* %8, align 8, !tbaa !5
  %211 = load double* %16, align 8, !tbaa !5
  %212 = fsub double %204, %211
  store double %212, double* %16, align 8, !tbaa !5
  %213 = load double* %9, align 16, !tbaa !5
  %214 = load double* %17, align 16, !tbaa !5
  %215 = load double* %10, align 8, !tbaa !5
  %216 = fadd double %213, %215
  store double %216, double* %9, align 16, !tbaa !5
  %217 = load double* %18, align 8, !tbaa !5
  %218 = fadd double %214, %217
  store double %218, double* %17, align 16, !tbaa !5
  %219 = load double* %10, align 8, !tbaa !5
  %220 = fsub double %213, %219
  store double %220, double* %10, align 8, !tbaa !5
  %221 = load double* %18, align 8, !tbaa !5
  %222 = fsub double %214, %221
  store double %222, double* %18, align 8, !tbaa !5
  %223 = trunc i64 %indvars.iv177 to i32
  call void @twiddles8(double* %3, double* %11, i32 %223, i32 512) #4
  %224 = load double* %3, align 16, !tbaa !5
  %225 = shl nsw i64 %indvars.iv177, 3
  %226 = getelementptr inbounds double* %DATA_x, i64 %225
  store double %224, double* %226, align 8, !tbaa !5
  %227 = load double* %4, align 8, !tbaa !5
  %228 = or i64 %225, 1
  %229 = getelementptr inbounds double* %DATA_x, i64 %228
  store double %227, double* %229, align 8, !tbaa !5
  %230 = load double* %5, align 16, !tbaa !5
  %231 = or i64 %225, 2
  %232 = getelementptr inbounds double* %DATA_x, i64 %231
  store double %230, double* %232, align 8, !tbaa !5
  %233 = load double* %6, align 8, !tbaa !5
  %234 = or i64 %225, 3
  %235 = getelementptr inbounds double* %DATA_x, i64 %234
  store double %233, double* %235, align 8, !tbaa !5
  %236 = load double* %7, align 16, !tbaa !5
  %237 = or i64 %225, 4
  %238 = getelementptr inbounds double* %DATA_x, i64 %237
  store double %236, double* %238, align 8, !tbaa !5
  %239 = load double* %8, align 8, !tbaa !5
  %240 = or i64 %225, 5
  %241 = getelementptr inbounds double* %DATA_x, i64 %240
  store double %239, double* %241, align 8, !tbaa !5
  %242 = load double* %9, align 16, !tbaa !5
  %243 = or i64 %225, 6
  %244 = getelementptr inbounds double* %DATA_x, i64 %243
  store double %242, double* %244, align 8, !tbaa !5
  %245 = load double* %10, align 8, !tbaa !5
  %246 = or i64 %225, 7
  %247 = getelementptr inbounds double* %DATA_x, i64 %246
  store double %245, double* %247, align 8, !tbaa !5
  %248 = load double* %11, align 16, !tbaa !5
  %249 = getelementptr inbounds double* %DATA_y, i64 %225
  store double %248, double* %249, align 8, !tbaa !5
  %250 = load double* %12, align 8, !tbaa !5
  %251 = getelementptr inbounds double* %DATA_y, i64 %228
  store double %250, double* %251, align 8, !tbaa !5
  %252 = load double* %13, align 16, !tbaa !5
  %253 = getelementptr inbounds double* %DATA_y, i64 %231
  store double %252, double* %253, align 8, !tbaa !5
  %254 = load double* %14, align 8, !tbaa !5
  %255 = getelementptr inbounds double* %DATA_y, i64 %234
  store double %254, double* %255, align 8, !tbaa !5
  %256 = load double* %15, align 16, !tbaa !5
  %257 = getelementptr inbounds double* %DATA_y, i64 %237
  store double %256, double* %257, align 8, !tbaa !5
  %258 = load double* %16, align 8, !tbaa !5
  %259 = getelementptr inbounds double* %DATA_y, i64 %240
  store double %258, double* %259, align 8, !tbaa !5
  %260 = load double* %17, align 16, !tbaa !5
  %261 = getelementptr inbounds double* %DATA_y, i64 %243
  store double %260, double* %261, align 8, !tbaa !5
  %262 = load double* %18, align 8, !tbaa !5
  %263 = getelementptr inbounds double* %DATA_y, i64 %246
  store double %262, double* %263, align 8, !tbaa !5
  %indvars.iv.next178 = add nuw nsw i64 %indvars.iv177, 1
  %exitcond179 = icmp eq i64 %indvars.iv.next178, 64
  br i1 %exitcond179, label %.preheader132, label %19

.preheader132:                                    ; preds = %19, %.preheader132
  %indvars.iv174 = phi i64 [ %indvars.iv.next175, %.preheader132 ], [ 0, %19 ]
  %264 = trunc i64 %indvars.iv174 to i32
  %265 = ashr i32 %264, 3
  %266 = trunc i64 %indvars.iv174 to i32
  %267 = and i32 %266, 7
  %268 = shl nsw i32 %265, 3
  %269 = or i32 %268, %267
  %270 = shl nsw i64 %indvars.iv174, 3
  %271 = getelementptr inbounds double* %DATA_x, i64 %270
  %272 = load double* %271, align 8, !tbaa !5
  %273 = sext i32 %269 to i64
  %274 = getelementptr inbounds double* %smem, i64 %273
  store double %272, double* %274, align 8, !tbaa !5
  %275 = or i64 %270, 1
  %276 = getelementptr inbounds double* %DATA_x, i64 %275
  %277 = load double* %276, align 8, !tbaa !5
  %278 = add nsw i32 %269, 264
  %279 = sext i32 %278 to i64
  %280 = getelementptr inbounds double* %smem, i64 %279
  store double %277, double* %280, align 8, !tbaa !5
  %281 = or i64 %270, 4
  %282 = getelementptr inbounds double* %DATA_x, i64 %281
  %283 = load double* %282, align 8, !tbaa !5
  %284 = add nsw i32 %269, 66
  %285 = sext i32 %284 to i64
  %286 = getelementptr inbounds double* %smem, i64 %285
  store double %283, double* %286, align 8, !tbaa !5
  %287 = or i64 %270, 5
  %288 = getelementptr inbounds double* %DATA_x, i64 %287
  %289 = load double* %288, align 8, !tbaa !5
  %290 = add nsw i32 %269, 330
  %291 = sext i32 %290 to i64
  %292 = getelementptr inbounds double* %smem, i64 %291
  store double %289, double* %292, align 8, !tbaa !5
  %293 = or i64 %270, 2
  %294 = getelementptr inbounds double* %DATA_x, i64 %293
  %295 = load double* %294, align 8, !tbaa !5
  %296 = add nsw i32 %269, 132
  %297 = sext i32 %296 to i64
  %298 = getelementptr inbounds double* %smem, i64 %297
  store double %295, double* %298, align 8, !tbaa !5
  %299 = or i64 %270, 3
  %300 = getelementptr inbounds double* %DATA_x, i64 %299
  %301 = load double* %300, align 8, !tbaa !5
  %302 = add nsw i32 %269, 396
  %303 = sext i32 %302 to i64
  %304 = getelementptr inbounds double* %smem, i64 %303
  store double %301, double* %304, align 8, !tbaa !5
  %305 = or i64 %270, 6
  %306 = getelementptr inbounds double* %DATA_x, i64 %305
  %307 = load double* %306, align 8, !tbaa !5
  %308 = add nsw i32 %269, 198
  %309 = sext i32 %308 to i64
  %310 = getelementptr inbounds double* %smem, i64 %309
  store double %307, double* %310, align 8, !tbaa !5
  %311 = or i64 %270, 7
  %312 = getelementptr inbounds double* %DATA_x, i64 %311
  %313 = load double* %312, align 8, !tbaa !5
  %314 = add nsw i32 %269, 462
  %315 = sext i32 %314 to i64
  %316 = getelementptr inbounds double* %smem, i64 %315
  store double %313, double* %316, align 8, !tbaa !5
  %indvars.iv.next175 = add nuw nsw i64 %indvars.iv174, 1
  %exitcond176 = icmp eq i64 %indvars.iv.next175, 64
  br i1 %exitcond176, label %.preheader130, label %.preheader132

.preheader130:                                    ; preds = %.preheader132, %.preheader130
  %indvars.iv171 = phi i64 [ %indvars.iv.next172, %.preheader130 ], [ 0, %.preheader132 ]
  %317 = trunc i64 %indvars.iv171 to i32
  %318 = ashr i32 %317, 3
  %319 = trunc i64 %indvars.iv171 to i32
  %320 = and i32 %319, 7
  %321 = mul nsw i32 %320, 66
  %322 = add nsw i32 %321, %318
  %323 = sext i32 %322 to i64
  %324 = getelementptr inbounds double* %smem, i64 %323
  %325 = load double* %324, align 8, !tbaa !5
  %326 = shl nsw i64 %indvars.iv171, 3
  %327 = getelementptr inbounds double* %DATA_x, i64 %326
  store double %325, double* %327, align 8, !tbaa !5
  %328 = add nsw i32 %322, 32
  %329 = sext i32 %328 to i64
  %330 = getelementptr inbounds double* %smem, i64 %329
  %331 = load double* %330, align 8, !tbaa !5
  %332 = or i64 %326, 4
  %333 = getelementptr inbounds double* %DATA_x, i64 %332
  store double %331, double* %333, align 8, !tbaa !5
  %334 = add nsw i32 %322, 8
  %335 = sext i32 %334 to i64
  %336 = getelementptr inbounds double* %smem, i64 %335
  %337 = load double* %336, align 8, !tbaa !5
  %338 = or i64 %326, 1
  %339 = getelementptr inbounds double* %DATA_x, i64 %338
  store double %337, double* %339, align 8, !tbaa !5
  %340 = add nsw i32 %322, 40
  %341 = sext i32 %340 to i64
  %342 = getelementptr inbounds double* %smem, i64 %341
  %343 = load double* %342, align 8, !tbaa !5
  %344 = or i64 %326, 5
  %345 = getelementptr inbounds double* %DATA_x, i64 %344
  store double %343, double* %345, align 8, !tbaa !5
  %346 = add nsw i32 %322, 16
  %347 = sext i32 %346 to i64
  %348 = getelementptr inbounds double* %smem, i64 %347
  %349 = load double* %348, align 8, !tbaa !5
  %350 = or i64 %326, 2
  %351 = getelementptr inbounds double* %DATA_x, i64 %350
  store double %349, double* %351, align 8, !tbaa !5
  %352 = add nsw i32 %322, 48
  %353 = sext i32 %352 to i64
  %354 = getelementptr inbounds double* %smem, i64 %353
  %355 = load double* %354, align 8, !tbaa !5
  %356 = or i64 %326, 6
  %357 = getelementptr inbounds double* %DATA_x, i64 %356
  store double %355, double* %357, align 8, !tbaa !5
  %358 = add nsw i32 %322, 24
  %359 = sext i32 %358 to i64
  %360 = getelementptr inbounds double* %smem, i64 %359
  %361 = load double* %360, align 8, !tbaa !5
  %362 = or i64 %326, 3
  %363 = getelementptr inbounds double* %DATA_x, i64 %362
  store double %361, double* %363, align 8, !tbaa !5
  %364 = add nsw i32 %322, 56
  %365 = sext i32 %364 to i64
  %366 = getelementptr inbounds double* %smem, i64 %365
  %367 = load double* %366, align 8, !tbaa !5
  %368 = or i64 %326, 7
  %369 = getelementptr inbounds double* %DATA_x, i64 %368
  store double %367, double* %369, align 8, !tbaa !5
  %indvars.iv.next172 = add nuw nsw i64 %indvars.iv171, 1
  %exitcond173 = icmp eq i64 %indvars.iv.next172, 64
  br i1 %exitcond173, label %.preheader128, label %.preheader130

.preheader126:                                    ; preds = %.preheader128
  %370 = getelementptr inbounds [8 x double]* %data_y, i64 0, i64 0
  %371 = getelementptr inbounds [8 x double]* %data_y, i64 0, i64 1
  %372 = getelementptr inbounds [8 x double]* %data_y, i64 0, i64 2
  %373 = getelementptr inbounds [8 x double]* %data_y, i64 0, i64 3
  %374 = getelementptr inbounds [8 x double]* %data_y, i64 0, i64 4
  %375 = getelementptr inbounds [8 x double]* %data_y, i64 0, i64 5
  %376 = getelementptr inbounds [8 x double]* %data_y, i64 0, i64 6
  %377 = getelementptr inbounds [8 x double]* %data_y, i64 0, i64 7
  br label %447

.preheader128:                                    ; preds = %.preheader130, %.preheader128
  %indvars.iv168 = phi i64 [ %indvars.iv.next169, %.preheader128 ], [ 0, %.preheader130 ]
  %378 = trunc i64 %indvars.iv168 to i32
  %379 = ashr i32 %378, 3
  %380 = trunc i64 %indvars.iv168 to i32
  %381 = and i32 %380, 7
  %382 = shl nsw i32 %379, 3
  %383 = or i32 %382, %381
  %384 = shl nsw i64 %indvars.iv168, 3
  %385 = getelementptr inbounds double* %DATA_y, i64 %384
  %386 = load double* %385, align 8, !tbaa !5
  %387 = sext i32 %383 to i64
  %388 = getelementptr inbounds double* %smem, i64 %387
  store double %386, double* %388, align 8, !tbaa !5
  %389 = or i64 %384, 1
  %390 = getelementptr inbounds double* %DATA_y, i64 %389
  %391 = load double* %390, align 8, !tbaa !5
  %392 = add nsw i32 %383, 264
  %393 = sext i32 %392 to i64
  %394 = getelementptr inbounds double* %smem, i64 %393
  store double %391, double* %394, align 8, !tbaa !5
  %395 = or i64 %384, 4
  %396 = getelementptr inbounds double* %DATA_y, i64 %395
  %397 = load double* %396, align 8, !tbaa !5
  %398 = add nsw i32 %383, 66
  %399 = sext i32 %398 to i64
  %400 = getelementptr inbounds double* %smem, i64 %399
  store double %397, double* %400, align 8, !tbaa !5
  %401 = or i64 %384, 5
  %402 = getelementptr inbounds double* %DATA_y, i64 %401
  %403 = load double* %402, align 8, !tbaa !5
  %404 = add nsw i32 %383, 330
  %405 = sext i32 %404 to i64
  %406 = getelementptr inbounds double* %smem, i64 %405
  store double %403, double* %406, align 8, !tbaa !5
  %407 = or i64 %384, 2
  %408 = getelementptr inbounds double* %DATA_y, i64 %407
  %409 = load double* %408, align 8, !tbaa !5
  %410 = add nsw i32 %383, 132
  %411 = sext i32 %410 to i64
  %412 = getelementptr inbounds double* %smem, i64 %411
  store double %409, double* %412, align 8, !tbaa !5
  %413 = or i64 %384, 3
  %414 = getelementptr inbounds double* %DATA_y, i64 %413
  %415 = load double* %414, align 8, !tbaa !5
  %416 = add nsw i32 %383, 396
  %417 = sext i32 %416 to i64
  %418 = getelementptr inbounds double* %smem, i64 %417
  store double %415, double* %418, align 8, !tbaa !5
  %419 = or i64 %384, 6
  %420 = getelementptr inbounds double* %DATA_y, i64 %419
  %421 = load double* %420, align 8, !tbaa !5
  %422 = add nsw i32 %383, 198
  %423 = sext i32 %422 to i64
  %424 = getelementptr inbounds double* %smem, i64 %423
  store double %421, double* %424, align 8, !tbaa !5
  %425 = or i64 %384, 7
  %426 = getelementptr inbounds double* %DATA_y, i64 %425
  %427 = load double* %426, align 8, !tbaa !5
  %428 = add nsw i32 %383, 462
  %429 = sext i32 %428 to i64
  %430 = getelementptr inbounds double* %smem, i64 %429
  store double %427, double* %430, align 8, !tbaa !5
  %indvars.iv.next169 = add nuw nsw i64 %indvars.iv168, 1
  %exitcond170 = icmp eq i64 %indvars.iv.next169, 64
  br i1 %exitcond170, label %.preheader126, label %.preheader128

.preheader124:                                    ; preds = %447
  %431 = getelementptr inbounds [8 x double]* %data_x, i64 0, i64 0
  %432 = getelementptr inbounds [8 x double]* %data_x, i64 0, i64 1
  %433 = getelementptr inbounds [8 x double]* %data_x, i64 0, i64 2
  %434 = getelementptr inbounds [8 x double]* %data_x, i64 0, i64 3
  %435 = getelementptr inbounds [8 x double]* %data_x, i64 0, i64 4
  %436 = getelementptr inbounds [8 x double]* %data_x, i64 0, i64 5
  %437 = getelementptr inbounds [8 x double]* %data_x, i64 0, i64 6
  %438 = getelementptr inbounds [8 x double]* %data_x, i64 0, i64 7
  %439 = getelementptr inbounds [8 x double]* %data_y, i64 0, i64 0
  %440 = getelementptr inbounds [8 x double]* %data_y, i64 0, i64 1
  %441 = getelementptr inbounds [8 x double]* %data_y, i64 0, i64 2
  %442 = getelementptr inbounds [8 x double]* %data_y, i64 0, i64 3
  %443 = getelementptr inbounds [8 x double]* %data_y, i64 0, i64 4
  %444 = getelementptr inbounds [8 x double]* %data_y, i64 0, i64 5
  %445 = getelementptr inbounds [8 x double]* %data_y, i64 0, i64 6
  %446 = getelementptr inbounds [8 x double]* %data_y, i64 0, i64 7
  br label %486

; <label>:447                                     ; preds = %447, %.preheader126
  %indvars.iv165 = phi i64 [ 0, %.preheader126 ], [ %indvars.iv.next166, %447 ]
  %448 = shl nsw i64 %indvars.iv165, 3
  %449 = getelementptr inbounds double* %DATA_y, i64 %448
  %450 = load double* %449, align 8, !tbaa !5
  store double %450, double* %370, align 16, !tbaa !5
  %451 = or i64 %448, 1
  %452 = getelementptr inbounds double* %DATA_y, i64 %451
  %453 = load double* %452, align 8, !tbaa !5
  store double %453, double* %371, align 8, !tbaa !5
  %454 = or i64 %448, 2
  %455 = getelementptr inbounds double* %DATA_y, i64 %454
  %456 = load double* %455, align 8, !tbaa !5
  store double %456, double* %372, align 16, !tbaa !5
  %457 = or i64 %448, 3
  %458 = getelementptr inbounds double* %DATA_y, i64 %457
  %459 = load double* %458, align 8, !tbaa !5
  store double %459, double* %373, align 8, !tbaa !5
  %460 = or i64 %448, 4
  %461 = getelementptr inbounds double* %DATA_y, i64 %460
  %462 = load double* %461, align 8, !tbaa !5
  store double %462, double* %374, align 16, !tbaa !5
  %463 = or i64 %448, 5
  %464 = getelementptr inbounds double* %DATA_y, i64 %463
  %465 = load double* %464, align 8, !tbaa !5
  store double %465, double* %375, align 8, !tbaa !5
  %466 = or i64 %448, 6
  %467 = getelementptr inbounds double* %DATA_y, i64 %466
  %468 = load double* %467, align 8, !tbaa !5
  store double %468, double* %376, align 16, !tbaa !5
  %469 = or i64 %448, 7
  %470 = getelementptr inbounds double* %DATA_y, i64 %469
  %471 = load double* %470, align 8, !tbaa !5
  store double %471, double* %377, align 8, !tbaa !5
  %472 = trunc i64 %indvars.iv165 to i32
  %473 = ashr i32 %472, 3
  %474 = trunc i64 %indvars.iv165 to i32
  %475 = and i32 %474, 7
  %476 = mul nsw i32 %475, 66
  %477 = add nsw i32 %476, %473
  call void @loady8(double* %370, double* %smem, i32 %477, i32 8) #4
  %478 = load double* %370, align 16, !tbaa !5
  store double %478, double* %449, align 8, !tbaa !5
  %479 = load double* %371, align 8, !tbaa !5
  store double %479, double* %452, align 8, !tbaa !5
  %480 = load double* %372, align 16, !tbaa !5
  store double %480, double* %455, align 8, !tbaa !5
  %481 = load double* %373, align 8, !tbaa !5
  store double %481, double* %458, align 8, !tbaa !5
  %482 = load double* %374, align 16, !tbaa !5
  store double %482, double* %461, align 8, !tbaa !5
  %483 = load double* %375, align 8, !tbaa !5
  store double %483, double* %464, align 8, !tbaa !5
  %484 = load double* %376, align 16, !tbaa !5
  store double %484, double* %467, align 8, !tbaa !5
  %485 = load double* %377, align 8, !tbaa !5
  store double %485, double* %470, align 8, !tbaa !5
  %indvars.iv.next166 = add nuw nsw i64 %indvars.iv165, 1
  %exitcond167 = icmp eq i64 %indvars.iv.next166, 64
  br i1 %exitcond167, label %.preheader124, label %447

; <label>:486                                     ; preds = %486, %.preheader124
  %indvars.iv162 = phi i64 [ 0, %.preheader124 ], [ %indvars.iv.next163, %486 ]
  %487 = shl nsw i64 %indvars.iv162, 3
  %488 = getelementptr inbounds double* %DATA_x, i64 %487
  %489 = load double* %488, align 8, !tbaa !5
  store double %489, double* %431, align 16, !tbaa !5
  %490 = or i64 %487, 1
  %491 = getelementptr inbounds double* %DATA_x, i64 %490
  %492 = load double* %491, align 8, !tbaa !5
  store double %492, double* %432, align 8, !tbaa !5
  %493 = or i64 %487, 2
  %494 = getelementptr inbounds double* %DATA_x, i64 %493
  %495 = load double* %494, align 8, !tbaa !5
  store double %495, double* %433, align 16, !tbaa !5
  %496 = or i64 %487, 3
  %497 = getelementptr inbounds double* %DATA_x, i64 %496
  %498 = load double* %497, align 8, !tbaa !5
  store double %498, double* %434, align 8, !tbaa !5
  %499 = or i64 %487, 4
  %500 = getelementptr inbounds double* %DATA_x, i64 %499
  %501 = load double* %500, align 8, !tbaa !5
  store double %501, double* %435, align 16, !tbaa !5
  %502 = or i64 %487, 5
  %503 = getelementptr inbounds double* %DATA_x, i64 %502
  %504 = load double* %503, align 8, !tbaa !5
  store double %504, double* %436, align 8, !tbaa !5
  %505 = or i64 %487, 6
  %506 = getelementptr inbounds double* %DATA_x, i64 %505
  %507 = load double* %506, align 8, !tbaa !5
  store double %507, double* %437, align 16, !tbaa !5
  %508 = or i64 %487, 7
  %509 = getelementptr inbounds double* %DATA_x, i64 %508
  %510 = load double* %509, align 8, !tbaa !5
  store double %510, double* %438, align 8, !tbaa !5
  %511 = getelementptr inbounds double* %DATA_y, i64 %487
  %512 = load double* %511, align 8, !tbaa !5
  store double %512, double* %439, align 16, !tbaa !5
  %513 = getelementptr inbounds double* %DATA_y, i64 %490
  %514 = load double* %513, align 8, !tbaa !5
  store double %514, double* %440, align 8, !tbaa !5
  %515 = getelementptr inbounds double* %DATA_y, i64 %493
  %516 = load double* %515, align 8, !tbaa !5
  store double %516, double* %441, align 16, !tbaa !5
  %517 = getelementptr inbounds double* %DATA_y, i64 %496
  %518 = load double* %517, align 8, !tbaa !5
  store double %518, double* %442, align 8, !tbaa !5
  %519 = getelementptr inbounds double* %DATA_y, i64 %499
  %520 = load double* %519, align 8, !tbaa !5
  store double %520, double* %443, align 16, !tbaa !5
  %521 = getelementptr inbounds double* %DATA_y, i64 %502
  %522 = load double* %521, align 8, !tbaa !5
  store double %522, double* %444, align 8, !tbaa !5
  %523 = getelementptr inbounds double* %DATA_y, i64 %505
  %524 = load double* %523, align 8, !tbaa !5
  store double %524, double* %445, align 16, !tbaa !5
  %525 = getelementptr inbounds double* %DATA_y, i64 %508
  %526 = load double* %525, align 8, !tbaa !5
  store double %526, double* %446, align 8, !tbaa !5
  %527 = load double* %431, align 16, !tbaa !5
  %528 = load double* %439, align 16, !tbaa !5
  %529 = load double* %435, align 16, !tbaa !5
  %530 = fadd double %527, %529
  store double %530, double* %431, align 16, !tbaa !5
  %531 = load double* %443, align 16, !tbaa !5
  %532 = fadd double %528, %531
  store double %532, double* %439, align 16, !tbaa !5
  %533 = load double* %435, align 16, !tbaa !5
  %534 = fsub double %527, %533
  store double %534, double* %435, align 16, !tbaa !5
  %535 = load double* %443, align 16, !tbaa !5
  %536 = fsub double %528, %535
  store double %536, double* %443, align 16, !tbaa !5
  %537 = load double* %432, align 8, !tbaa !5
  %538 = load double* %440, align 8, !tbaa !5
  %539 = load double* %436, align 8, !tbaa !5
  %540 = fadd double %537, %539
  store double %540, double* %432, align 8, !tbaa !5
  %541 = load double* %444, align 8, !tbaa !5
  %542 = fadd double %538, %541
  store double %542, double* %440, align 8, !tbaa !5
  %543 = load double* %436, align 8, !tbaa !5
  %544 = fsub double %537, %543
  store double %544, double* %436, align 8, !tbaa !5
  %545 = load double* %444, align 8, !tbaa !5
  %546 = fsub double %538, %545
  store double %546, double* %444, align 8, !tbaa !5
  %547 = load double* %433, align 16, !tbaa !5
  %548 = load double* %441, align 16, !tbaa !5
  %549 = load double* %437, align 16, !tbaa !5
  %550 = fadd double %547, %549
  store double %550, double* %433, align 16, !tbaa !5
  %551 = load double* %445, align 16, !tbaa !5
  %552 = fadd double %548, %551
  store double %552, double* %441, align 16, !tbaa !5
  %553 = load double* %437, align 16, !tbaa !5
  %554 = fsub double %547, %553
  store double %554, double* %437, align 16, !tbaa !5
  %555 = load double* %445, align 16, !tbaa !5
  %556 = fsub double %548, %555
  store double %556, double* %445, align 16, !tbaa !5
  %557 = load double* %434, align 8, !tbaa !5
  %558 = load double* %442, align 8, !tbaa !5
  %559 = load double* %438, align 8, !tbaa !5
  %560 = fadd double %557, %559
  store double %560, double* %434, align 8, !tbaa !5
  %561 = load double* %446, align 8, !tbaa !5
  %562 = fadd double %558, %561
  store double %562, double* %442, align 8, !tbaa !5
  %563 = load double* %438, align 8, !tbaa !5
  %564 = fsub double %557, %563
  store double %564, double* %438, align 8, !tbaa !5
  %565 = load double* %446, align 8, !tbaa !5
  %566 = fsub double %558, %565
  store double %566, double* %446, align 8, !tbaa !5
  %567 = load double* %436, align 8, !tbaa !5
  %568 = load double* %444, align 8, !tbaa !5
  %569 = fmul double %568, -1.000000e+00
  %570 = fsub double %567, %569
  %571 = fmul double %570, 0x3FE6A09E667F3BCD
  store double %571, double* %436, align 8, !tbaa !5
  %572 = fmul double %567, -1.000000e+00
  %573 = load double* %444, align 8, !tbaa !5
  %574 = fadd double %572, %573
  %575 = fmul double %574, 0x3FE6A09E667F3BCD
  store double %575, double* %444, align 8, !tbaa !5
  %576 = load double* %437, align 16, !tbaa !5
  %577 = fmul double %576, 0.000000e+00
  %578 = load double* %445, align 16, !tbaa !5
  %579 = fmul double %578, -1.000000e+00
  %580 = fsub double %577, %579
  store double %580, double* %437, align 16, !tbaa !5
  %581 = fmul double %576, -1.000000e+00
  %582 = load double* %445, align 16, !tbaa !5
  %583 = fmul double %582, 0.000000e+00
  %584 = fadd double %581, %583
  store double %584, double* %445, align 16, !tbaa !5
  %585 = load double* %438, align 8, !tbaa !5
  %586 = fmul double %585, -1.000000e+00
  %587 = load double* %446, align 8, !tbaa !5
  %588 = fmul double %587, -1.000000e+00
  %589 = fsub double %586, %588
  %590 = fmul double %589, 0x3FE6A09E667F3BCD
  store double %590, double* %438, align 8, !tbaa !5
  %591 = load double* %446, align 8, !tbaa !5
  %592 = fmul double %591, -1.000000e+00
  %593 = fadd double %586, %592
  %594 = fmul double %593, 0x3FE6A09E667F3BCD
  store double %594, double* %446, align 8, !tbaa !5
  %595 = load double* %431, align 16, !tbaa !5
  %596 = load double* %439, align 16, !tbaa !5
  %597 = load double* %433, align 16, !tbaa !5
  %598 = fadd double %595, %597
  store double %598, double* %431, align 16, !tbaa !5
  %599 = load double* %441, align 16, !tbaa !5
  %600 = fadd double %596, %599
  store double %600, double* %439, align 16, !tbaa !5
  %601 = load double* %433, align 16, !tbaa !5
  %602 = fsub double %595, %601
  store double %602, double* %433, align 16, !tbaa !5
  %603 = load double* %441, align 16, !tbaa !5
  %604 = fsub double %596, %603
  store double %604, double* %441, align 16, !tbaa !5
  %605 = load double* %432, align 8, !tbaa !5
  %606 = load double* %440, align 8, !tbaa !5
  %607 = load double* %434, align 8, !tbaa !5
  %608 = fadd double %605, %607
  store double %608, double* %432, align 8, !tbaa !5
  %609 = load double* %442, align 8, !tbaa !5
  %610 = fadd double %606, %609
  store double %610, double* %440, align 8, !tbaa !5
  %611 = load double* %434, align 8, !tbaa !5
  %612 = fsub double %605, %611
  store double %612, double* %434, align 8, !tbaa !5
  %613 = load double* %442, align 8, !tbaa !5
  %614 = fsub double %606, %613
  store double %614, double* %442, align 8, !tbaa !5
  %615 = load double* %434, align 8, !tbaa !5
  %616 = fmul double %615, 0.000000e+00
  %617 = fmul double %614, -1.000000e+00
  %618 = fsub double %616, %617
  store double %618, double* %434, align 8, !tbaa !5
  %619 = fmul double %615, -1.000000e+00
  %620 = load double* %442, align 8, !tbaa !5
  %621 = fmul double %620, 0.000000e+00
  %622 = fsub double %619, %621
  store double %622, double* %442, align 8, !tbaa !5
  %623 = load double* %431, align 16, !tbaa !5
  %624 = load double* %439, align 16, !tbaa !5
  %625 = load double* %432, align 8, !tbaa !5
  %626 = fadd double %623, %625
  store double %626, double* %431, align 16, !tbaa !5
  %627 = load double* %440, align 8, !tbaa !5
  %628 = fadd double %624, %627
  store double %628, double* %439, align 16, !tbaa !5
  %629 = load double* %432, align 8, !tbaa !5
  %630 = fsub double %623, %629
  store double %630, double* %432, align 8, !tbaa !5
  %631 = load double* %440, align 8, !tbaa !5
  %632 = fsub double %624, %631
  store double %632, double* %440, align 8, !tbaa !5
  %633 = load double* %433, align 16, !tbaa !5
  %634 = load double* %441, align 16, !tbaa !5
  %635 = load double* %434, align 8, !tbaa !5
  %636 = fadd double %633, %635
  store double %636, double* %433, align 16, !tbaa !5
  %637 = load double* %442, align 8, !tbaa !5
  %638 = fadd double %634, %637
  store double %638, double* %441, align 16, !tbaa !5
  %639 = load double* %434, align 8, !tbaa !5
  %640 = fsub double %633, %639
  store double %640, double* %434, align 8, !tbaa !5
  %641 = load double* %442, align 8, !tbaa !5
  %642 = fsub double %634, %641
  store double %642, double* %442, align 8, !tbaa !5
  %643 = load double* %435, align 16, !tbaa !5
  %644 = load double* %443, align 16, !tbaa !5
  %645 = load double* %437, align 16, !tbaa !5
  %646 = fadd double %643, %645
  store double %646, double* %435, align 16, !tbaa !5
  %647 = load double* %445, align 16, !tbaa !5
  %648 = fadd double %644, %647
  store double %648, double* %443, align 16, !tbaa !5
  %649 = load double* %437, align 16, !tbaa !5
  %650 = fsub double %643, %649
  store double %650, double* %437, align 16, !tbaa !5
  %651 = load double* %445, align 16, !tbaa !5
  %652 = fsub double %644, %651
  store double %652, double* %445, align 16, !tbaa !5
  %653 = load double* %436, align 8, !tbaa !5
  %654 = load double* %444, align 8, !tbaa !5
  %655 = load double* %438, align 8, !tbaa !5
  %656 = fadd double %653, %655
  store double %656, double* %436, align 8, !tbaa !5
  %657 = load double* %446, align 8, !tbaa !5
  %658 = fadd double %654, %657
  store double %658, double* %444, align 8, !tbaa !5
  %659 = load double* %438, align 8, !tbaa !5
  %660 = fsub double %653, %659
  store double %660, double* %438, align 8, !tbaa !5
  %661 = load double* %446, align 8, !tbaa !5
  %662 = fsub double %654, %661
  store double %662, double* %446, align 8, !tbaa !5
  %663 = load double* %438, align 8, !tbaa !5
  %664 = fmul double %663, 0.000000e+00
  %665 = fmul double %662, -1.000000e+00
  %666 = fsub double %664, %665
  store double %666, double* %438, align 8, !tbaa !5
  %667 = fmul double %663, -1.000000e+00
  %668 = load double* %446, align 8, !tbaa !5
  %669 = fmul double %668, 0.000000e+00
  %670 = fsub double %667, %669
  store double %670, double* %446, align 8, !tbaa !5
  %671 = load double* %435, align 16, !tbaa !5
  %672 = load double* %443, align 16, !tbaa !5
  %673 = load double* %436, align 8, !tbaa !5
  %674 = fadd double %671, %673
  store double %674, double* %435, align 16, !tbaa !5
  %675 = load double* %444, align 8, !tbaa !5
  %676 = fadd double %672, %675
  store double %676, double* %443, align 16, !tbaa !5
  %677 = load double* %436, align 8, !tbaa !5
  %678 = fsub double %671, %677
  store double %678, double* %436, align 8, !tbaa !5
  %679 = load double* %444, align 8, !tbaa !5
  %680 = fsub double %672, %679
  store double %680, double* %444, align 8, !tbaa !5
  %681 = load double* %437, align 16, !tbaa !5
  %682 = load double* %445, align 16, !tbaa !5
  %683 = load double* %438, align 8, !tbaa !5
  %684 = fadd double %681, %683
  store double %684, double* %437, align 16, !tbaa !5
  %685 = load double* %446, align 8, !tbaa !5
  %686 = fadd double %682, %685
  store double %686, double* %445, align 16, !tbaa !5
  %687 = load double* %438, align 8, !tbaa !5
  %688 = fsub double %681, %687
  store double %688, double* %438, align 8, !tbaa !5
  %689 = load double* %446, align 8, !tbaa !5
  %690 = fsub double %682, %689
  store double %690, double* %446, align 8, !tbaa !5
  %691 = trunc i64 %indvars.iv162 to i32
  %692 = ashr i32 %691, 3
  call void @twiddles8(double* %431, double* %439, i32 %692, i32 64) #4
  %693 = load double* %431, align 16, !tbaa !5
  store double %693, double* %488, align 8, !tbaa !5
  %694 = load double* %432, align 8, !tbaa !5
  store double %694, double* %491, align 8, !tbaa !5
  %695 = load double* %433, align 16, !tbaa !5
  store double %695, double* %494, align 8, !tbaa !5
  %696 = load double* %434, align 8, !tbaa !5
  store double %696, double* %497, align 8, !tbaa !5
  %697 = load double* %435, align 16, !tbaa !5
  store double %697, double* %500, align 8, !tbaa !5
  %698 = load double* %436, align 8, !tbaa !5
  store double %698, double* %503, align 8, !tbaa !5
  %699 = load double* %437, align 16, !tbaa !5
  store double %699, double* %506, align 8, !tbaa !5
  %700 = load double* %438, align 8, !tbaa !5
  store double %700, double* %509, align 8, !tbaa !5
  %701 = load double* %439, align 16, !tbaa !5
  store double %701, double* %511, align 8, !tbaa !5
  %702 = load double* %440, align 8, !tbaa !5
  store double %702, double* %513, align 8, !tbaa !5
  %703 = load double* %441, align 16, !tbaa !5
  store double %703, double* %515, align 8, !tbaa !5
  %704 = load double* %442, align 8, !tbaa !5
  store double %704, double* %517, align 8, !tbaa !5
  %705 = load double* %443, align 16, !tbaa !5
  store double %705, double* %519, align 8, !tbaa !5
  %706 = load double* %444, align 8, !tbaa !5
  store double %706, double* %521, align 8, !tbaa !5
  %707 = load double* %445, align 16, !tbaa !5
  store double %707, double* %523, align 8, !tbaa !5
  %708 = load double* %446, align 8, !tbaa !5
  store double %708, double* %525, align 8, !tbaa !5
  %indvars.iv.next163 = add nuw nsw i64 %indvars.iv162, 1
  %exitcond164 = icmp eq i64 %indvars.iv.next163, 64
  br i1 %exitcond164, label %.preheader122, label %486

.preheader122:                                    ; preds = %486, %.preheader122
  %indvars.iv159 = phi i64 [ %indvars.iv.next160, %.preheader122 ], [ 0, %486 ]
  %709 = trunc i64 %indvars.iv159 to i32
  %710 = ashr i32 %709, 3
  %711 = trunc i64 %indvars.iv159 to i32
  %712 = and i32 %711, 7
  %713 = shl nsw i32 %710, 3
  %714 = or i32 %713, %712
  %715 = shl nsw i64 %indvars.iv159, 3
  %716 = getelementptr inbounds double* %DATA_x, i64 %715
  %717 = load double* %716, align 8, !tbaa !5
  %718 = sext i32 %714 to i64
  %719 = getelementptr inbounds double* %smem, i64 %718
  store double %717, double* %719, align 8, !tbaa !5
  %720 = or i64 %715, 1
  %721 = getelementptr inbounds double* %DATA_x, i64 %720
  %722 = load double* %721, align 8, !tbaa !5
  %723 = add nsw i32 %714, 288
  %724 = sext i32 %723 to i64
  %725 = getelementptr inbounds double* %smem, i64 %724
  store double %722, double* %725, align 8, !tbaa !5
  %726 = or i64 %715, 4
  %727 = getelementptr inbounds double* %DATA_x, i64 %726
  %728 = load double* %727, align 8, !tbaa !5
  %729 = add nsw i32 %714, 72
  %730 = sext i32 %729 to i64
  %731 = getelementptr inbounds double* %smem, i64 %730
  store double %728, double* %731, align 8, !tbaa !5
  %732 = or i64 %715, 5
  %733 = getelementptr inbounds double* %DATA_x, i64 %732
  %734 = load double* %733, align 8, !tbaa !5
  %735 = add nsw i32 %714, 360
  %736 = sext i32 %735 to i64
  %737 = getelementptr inbounds double* %smem, i64 %736
  store double %734, double* %737, align 8, !tbaa !5
  %738 = or i64 %715, 2
  %739 = getelementptr inbounds double* %DATA_x, i64 %738
  %740 = load double* %739, align 8, !tbaa !5
  %741 = add nsw i32 %714, 144
  %742 = sext i32 %741 to i64
  %743 = getelementptr inbounds double* %smem, i64 %742
  store double %740, double* %743, align 8, !tbaa !5
  %744 = or i64 %715, 3
  %745 = getelementptr inbounds double* %DATA_x, i64 %744
  %746 = load double* %745, align 8, !tbaa !5
  %747 = add nsw i32 %714, 432
  %748 = sext i32 %747 to i64
  %749 = getelementptr inbounds double* %smem, i64 %748
  store double %746, double* %749, align 8, !tbaa !5
  %750 = or i64 %715, 6
  %751 = getelementptr inbounds double* %DATA_x, i64 %750
  %752 = load double* %751, align 8, !tbaa !5
  %753 = add nsw i32 %714, 216
  %754 = sext i32 %753 to i64
  %755 = getelementptr inbounds double* %smem, i64 %754
  store double %752, double* %755, align 8, !tbaa !5
  %756 = or i64 %715, 7
  %757 = getelementptr inbounds double* %DATA_x, i64 %756
  %758 = load double* %757, align 8, !tbaa !5
  %759 = add nsw i32 %714, 504
  %760 = sext i32 %759 to i64
  %761 = getelementptr inbounds double* %smem, i64 %760
  store double %758, double* %761, align 8, !tbaa !5
  %indvars.iv.next160 = add nuw nsw i64 %indvars.iv159, 1
  %exitcond161 = icmp eq i64 %indvars.iv.next160, 64
  br i1 %exitcond161, label %.preheader120, label %.preheader122

.preheader120:                                    ; preds = %.preheader122, %.preheader120
  %indvars.iv156 = phi i64 [ %indvars.iv.next157, %.preheader120 ], [ 0, %.preheader122 ]
  %762 = trunc i64 %indvars.iv156 to i32
  %763 = ashr i32 %762, 3
  %764 = trunc i64 %indvars.iv156 to i32
  %765 = and i32 %764, 7
  %766 = mul nsw i32 %763, 72
  %767 = or i32 %766, %765
  %768 = sext i32 %767 to i64
  %769 = getelementptr inbounds double* %smem, i64 %768
  %770 = load double* %769, align 8, !tbaa !5
  %771 = shl nsw i64 %indvars.iv156, 3
  %772 = getelementptr inbounds double* %DATA_x, i64 %771
  store double %770, double* %772, align 8, !tbaa !5
  %773 = add nsw i32 %767, 32
  %774 = sext i32 %773 to i64
  %775 = getelementptr inbounds double* %smem, i64 %774
  %776 = load double* %775, align 8, !tbaa !5
  %777 = or i64 %771, 4
  %778 = getelementptr inbounds double* %DATA_x, i64 %777
  store double %776, double* %778, align 8, !tbaa !5
  %779 = add nsw i32 %767, 8
  %780 = sext i32 %779 to i64
  %781 = getelementptr inbounds double* %smem, i64 %780
  %782 = load double* %781, align 8, !tbaa !5
  %783 = or i64 %771, 1
  %784 = getelementptr inbounds double* %DATA_x, i64 %783
  store double %782, double* %784, align 8, !tbaa !5
  %785 = add nsw i32 %767, 40
  %786 = sext i32 %785 to i64
  %787 = getelementptr inbounds double* %smem, i64 %786
  %788 = load double* %787, align 8, !tbaa !5
  %789 = or i64 %771, 5
  %790 = getelementptr inbounds double* %DATA_x, i64 %789
  store double %788, double* %790, align 8, !tbaa !5
  %791 = add nsw i32 %767, 16
  %792 = sext i32 %791 to i64
  %793 = getelementptr inbounds double* %smem, i64 %792
  %794 = load double* %793, align 8, !tbaa !5
  %795 = or i64 %771, 2
  %796 = getelementptr inbounds double* %DATA_x, i64 %795
  store double %794, double* %796, align 8, !tbaa !5
  %797 = add nsw i32 %767, 48
  %798 = sext i32 %797 to i64
  %799 = getelementptr inbounds double* %smem, i64 %798
  %800 = load double* %799, align 8, !tbaa !5
  %801 = or i64 %771, 6
  %802 = getelementptr inbounds double* %DATA_x, i64 %801
  store double %800, double* %802, align 8, !tbaa !5
  %803 = add nsw i32 %767, 24
  %804 = sext i32 %803 to i64
  %805 = getelementptr inbounds double* %smem, i64 %804
  %806 = load double* %805, align 8, !tbaa !5
  %807 = or i64 %771, 3
  %808 = getelementptr inbounds double* %DATA_x, i64 %807
  store double %806, double* %808, align 8, !tbaa !5
  %809 = add nsw i32 %767, 56
  %810 = sext i32 %809 to i64
  %811 = getelementptr inbounds double* %smem, i64 %810
  %812 = load double* %811, align 8, !tbaa !5
  %813 = or i64 %771, 7
  %814 = getelementptr inbounds double* %DATA_x, i64 %813
  store double %812, double* %814, align 8, !tbaa !5
  %indvars.iv.next157 = add nuw nsw i64 %indvars.iv156, 1
  %exitcond158 = icmp eq i64 %indvars.iv.next157, 64
  br i1 %exitcond158, label %.preheader118, label %.preheader120

.preheader116:                                    ; preds = %.preheader118
  %815 = getelementptr inbounds [8 x double]* %data_y, i64 0, i64 0
  %816 = getelementptr inbounds [8 x double]* %data_y, i64 0, i64 1
  %817 = getelementptr inbounds [8 x double]* %data_y, i64 0, i64 2
  %818 = getelementptr inbounds [8 x double]* %data_y, i64 0, i64 3
  %819 = getelementptr inbounds [8 x double]* %data_y, i64 0, i64 4
  %820 = getelementptr inbounds [8 x double]* %data_y, i64 0, i64 5
  %821 = getelementptr inbounds [8 x double]* %data_y, i64 0, i64 6
  %822 = getelementptr inbounds [8 x double]* %data_y, i64 0, i64 7
  br label %876

.preheader118:                                    ; preds = %.preheader120, %.preheader118
  %indvars.iv153 = phi i64 [ %indvars.iv.next154, %.preheader118 ], [ 0, %.preheader120 ]
  %823 = trunc i64 %indvars.iv153 to i32
  %824 = ashr i32 %823, 3
  %825 = trunc i64 %indvars.iv153 to i32
  %826 = and i32 %825, 7
  %827 = shl nsw i32 %824, 3
  %828 = or i32 %827, %826
  %829 = shl nsw i64 %indvars.iv153, 3
  %830 = getelementptr inbounds double* %DATA_y, i64 %829
  %831 = load double* %830, align 8, !tbaa !5
  %832 = sext i32 %828 to i64
  %833 = getelementptr inbounds double* %smem, i64 %832
  store double %831, double* %833, align 8, !tbaa !5
  %834 = or i64 %829, 1
  %835 = getelementptr inbounds double* %DATA_y, i64 %834
  %836 = load double* %835, align 8, !tbaa !5
  %837 = add nsw i32 %828, 288
  %838 = sext i32 %837 to i64
  %839 = getelementptr inbounds double* %smem, i64 %838
  store double %836, double* %839, align 8, !tbaa !5
  %840 = or i64 %829, 4
  %841 = getelementptr inbounds double* %DATA_y, i64 %840
  %842 = load double* %841, align 8, !tbaa !5
  %843 = add nsw i32 %828, 72
  %844 = sext i32 %843 to i64
  %845 = getelementptr inbounds double* %smem, i64 %844
  store double %842, double* %845, align 8, !tbaa !5
  %846 = or i64 %829, 5
  %847 = getelementptr inbounds double* %DATA_y, i64 %846
  %848 = load double* %847, align 8, !tbaa !5
  %849 = add nsw i32 %828, 360
  %850 = sext i32 %849 to i64
  %851 = getelementptr inbounds double* %smem, i64 %850
  store double %848, double* %851, align 8, !tbaa !5
  %852 = or i64 %829, 2
  %853 = getelementptr inbounds double* %DATA_y, i64 %852
  %854 = load double* %853, align 8, !tbaa !5
  %855 = add nsw i32 %828, 144
  %856 = sext i32 %855 to i64
  %857 = getelementptr inbounds double* %smem, i64 %856
  store double %854, double* %857, align 8, !tbaa !5
  %858 = or i64 %829, 3
  %859 = getelementptr inbounds double* %DATA_y, i64 %858
  %860 = load double* %859, align 8, !tbaa !5
  %861 = add nsw i32 %828, 432
  %862 = sext i32 %861 to i64
  %863 = getelementptr inbounds double* %smem, i64 %862
  store double %860, double* %863, align 8, !tbaa !5
  %864 = or i64 %829, 6
  %865 = getelementptr inbounds double* %DATA_y, i64 %864
  %866 = load double* %865, align 8, !tbaa !5
  %867 = add nsw i32 %828, 216
  %868 = sext i32 %867 to i64
  %869 = getelementptr inbounds double* %smem, i64 %868
  store double %866, double* %869, align 8, !tbaa !5
  %870 = or i64 %829, 7
  %871 = getelementptr inbounds double* %DATA_y, i64 %870
  %872 = load double* %871, align 8, !tbaa !5
  %873 = add nsw i32 %828, 504
  %874 = sext i32 %873 to i64
  %875 = getelementptr inbounds double* %smem, i64 %874
  store double %872, double* %875, align 8, !tbaa !5
  %indvars.iv.next154 = add nuw nsw i64 %indvars.iv153, 1
  %exitcond155 = icmp eq i64 %indvars.iv.next154, 64
  br i1 %exitcond155, label %.preheader116, label %.preheader118

; <label>:876                                     ; preds = %876, %.preheader116
  %indvars.iv150 = phi i64 [ 0, %.preheader116 ], [ %indvars.iv.next151, %876 ]
  %877 = shl nsw i64 %indvars.iv150, 3
  %878 = getelementptr inbounds double* %DATA_y, i64 %877
  %879 = load double* %878, align 8, !tbaa !5
  store double %879, double* %815, align 16, !tbaa !5
  %880 = or i64 %877, 1
  %881 = getelementptr inbounds double* %DATA_y, i64 %880
  %882 = load double* %881, align 8, !tbaa !5
  store double %882, double* %816, align 8, !tbaa !5
  %883 = or i64 %877, 2
  %884 = getelementptr inbounds double* %DATA_y, i64 %883
  %885 = load double* %884, align 8, !tbaa !5
  store double %885, double* %817, align 16, !tbaa !5
  %886 = or i64 %877, 3
  %887 = getelementptr inbounds double* %DATA_y, i64 %886
  %888 = load double* %887, align 8, !tbaa !5
  store double %888, double* %818, align 8, !tbaa !5
  %889 = or i64 %877, 4
  %890 = getelementptr inbounds double* %DATA_y, i64 %889
  %891 = load double* %890, align 8, !tbaa !5
  store double %891, double* %819, align 16, !tbaa !5
  %892 = or i64 %877, 5
  %893 = getelementptr inbounds double* %DATA_y, i64 %892
  %894 = load double* %893, align 8, !tbaa !5
  store double %894, double* %820, align 8, !tbaa !5
  %895 = or i64 %877, 6
  %896 = getelementptr inbounds double* %DATA_y, i64 %895
  %897 = load double* %896, align 8, !tbaa !5
  store double %897, double* %821, align 16, !tbaa !5
  %898 = or i64 %877, 7
  %899 = getelementptr inbounds double* %DATA_y, i64 %898
  %900 = load double* %899, align 8, !tbaa !5
  store double %900, double* %822, align 8, !tbaa !5
  %901 = trunc i64 %indvars.iv150 to i32
  %902 = ashr i32 %901, 3
  %903 = trunc i64 %indvars.iv150 to i32
  %904 = and i32 %903, 7
  %905 = mul nsw i32 %902, 72
  %906 = or i32 %905, %904
  call void @loady8(double* %815, double* %smem, i32 %906, i32 8) #4
  %907 = load double* %815, align 16, !tbaa !5
  store double %907, double* %878, align 8, !tbaa !5
  %908 = load double* %816, align 8, !tbaa !5
  store double %908, double* %881, align 8, !tbaa !5
  %909 = load double* %817, align 16, !tbaa !5
  store double %909, double* %884, align 8, !tbaa !5
  %910 = load double* %818, align 8, !tbaa !5
  store double %910, double* %887, align 8, !tbaa !5
  %911 = load double* %819, align 16, !tbaa !5
  store double %911, double* %890, align 8, !tbaa !5
  %912 = load double* %820, align 8, !tbaa !5
  store double %912, double* %893, align 8, !tbaa !5
  %913 = load double* %821, align 16, !tbaa !5
  store double %913, double* %896, align 8, !tbaa !5
  %914 = load double* %822, align 8, !tbaa !5
  store double %914, double* %899, align 8, !tbaa !5
  %indvars.iv.next151 = add nuw nsw i64 %indvars.iv150, 1
  %exitcond152 = icmp eq i64 %indvars.iv.next151, 64
  br i1 %exitcond152, label %.preheader, label %876

.preheader:                                       ; preds = %876, %.preheader
  %indvars.iv = phi i64 [ %indvars.iv.next, %.preheader ], [ 0, %876 ]
  %915 = shl nsw i64 %indvars.iv, 3
  %916 = getelementptr inbounds double* %DATA_y, i64 %915
  %917 = load double* %916, align 8, !tbaa !5
  %918 = or i64 %915, 1
  %919 = getelementptr inbounds double* %DATA_y, i64 %918
  %920 = load double* %919, align 8, !tbaa !5
  %921 = or i64 %915, 2
  %922 = getelementptr inbounds double* %DATA_y, i64 %921
  %923 = load double* %922, align 8, !tbaa !5
  %924 = or i64 %915, 3
  %925 = getelementptr inbounds double* %DATA_y, i64 %924
  %926 = load double* %925, align 8, !tbaa !5
  %927 = or i64 %915, 4
  %928 = getelementptr inbounds double* %DATA_y, i64 %927
  %929 = load double* %928, align 8, !tbaa !5
  %930 = or i64 %915, 5
  %931 = getelementptr inbounds double* %DATA_y, i64 %930
  %932 = load double* %931, align 8, !tbaa !5
  %933 = or i64 %915, 6
  %934 = getelementptr inbounds double* %DATA_y, i64 %933
  %935 = load double* %934, align 8, !tbaa !5
  %936 = or i64 %915, 7
  %937 = getelementptr inbounds double* %DATA_y, i64 %936
  %938 = load double* %937, align 8, !tbaa !5
  %939 = getelementptr inbounds double* %DATA_x, i64 %915
  %940 = load double* %939, align 8, !tbaa !5
  %941 = getelementptr inbounds double* %DATA_x, i64 %918
  %942 = load double* %941, align 8, !tbaa !5
  %943 = getelementptr inbounds double* %DATA_x, i64 %921
  %944 = load double* %943, align 8, !tbaa !5
  %945 = getelementptr inbounds double* %DATA_x, i64 %924
  %946 = load double* %945, align 8, !tbaa !5
  %947 = getelementptr inbounds double* %DATA_x, i64 %927
  %948 = load double* %947, align 8, !tbaa !5
  %949 = getelementptr inbounds double* %DATA_x, i64 %930
  %950 = load double* %949, align 8, !tbaa !5
  %951 = getelementptr inbounds double* %DATA_x, i64 %933
  %952 = load double* %951, align 8, !tbaa !5
  %953 = getelementptr inbounds double* %DATA_x, i64 %936
  %954 = load double* %953, align 8, !tbaa !5
  %955 = fadd double %940, %948
  %956 = fadd double %917, %929
  %957 = fsub double %940, %948
  %958 = fsub double %917, %929
  %959 = fadd double %942, %950
  %960 = fadd double %920, %932
  %961 = fsub double %942, %950
  %962 = fsub double %920, %932
  %963 = fadd double %944, %952
  %964 = fadd double %923, %935
  %965 = fsub double %944, %952
  %966 = fsub double %923, %935
  %967 = fadd double %946, %954
  %968 = fadd double %926, %938
  %969 = fsub double %946, %954
  %970 = fsub double %926, %938
  %971 = fmul double %962, -1.000000e+00
  %972 = fsub double %961, %971
  %973 = fmul double %972, 0x3FE6A09E667F3BCD
  %974 = fmul double %961, -1.000000e+00
  %975 = fadd double %974, %962
  %976 = fmul double %975, 0x3FE6A09E667F3BCD
  %977 = fmul double %965, 0.000000e+00
  %978 = fmul double %966, -1.000000e+00
  %979 = fsub double %977, %978
  %980 = fmul double %965, -1.000000e+00
  %981 = fmul double %966, 0.000000e+00
  %982 = fadd double %980, %981
  %983 = fmul double %969, -1.000000e+00
  %984 = fmul double %970, -1.000000e+00
  %985 = fsub double %983, %984
  %986 = fmul double %985, 0x3FE6A09E667F3BCD
  %987 = fmul double %970, -1.000000e+00
  %988 = fadd double %983, %987
  %989 = fmul double %988, 0x3FE6A09E667F3BCD
  %990 = fadd double %955, %963
  %991 = fadd double %956, %964
  %992 = fsub double %955, %963
  %993 = fsub double %956, %964
  %994 = fadd double %959, %967
  %995 = fadd double %960, %968
  %996 = fsub double %959, %967
  %997 = fsub double %960, %968
  %998 = fmul double %996, 0.000000e+00
  %999 = fmul double %997, -1.000000e+00
  %1000 = fsub double %998, %999
  %1001 = fmul double %996, -1.000000e+00
  %1002 = fmul double %997, 0.000000e+00
  %1003 = fsub double %1001, %1002
  %1004 = fadd double %990, %994
  %1005 = fadd double %991, %995
  %1006 = fsub double %990, %994
  %1007 = fsub double %991, %995
  %1008 = fadd double %992, %1000
  %1009 = fadd double %993, %1003
  %1010 = fsub double %992, %1000
  %1011 = fsub double %993, %1003
  %1012 = fadd double %957, %979
  %1013 = fadd double %958, %982
  %1014 = fsub double %957, %979
  %1015 = fsub double %958, %982
  %1016 = fadd double %973, %986
  %1017 = fadd double %976, %989
  %1018 = fsub double %973, %986
  %1019 = fsub double %976, %989
  %1020 = fmul double %1018, 0.000000e+00
  %1021 = fmul double %1019, -1.000000e+00
  %1022 = fsub double %1020, %1021
  %1023 = fmul double %1018, -1.000000e+00
  %1024 = fmul double %1019, 0.000000e+00
  %1025 = fsub double %1023, %1024
  %1026 = fadd double %1012, %1016
  %1027 = fadd double %1013, %1017
  %1028 = fsub double %1012, %1016
  %1029 = fsub double %1013, %1017
  %1030 = fadd double %1014, %1022
  %1031 = fadd double %1015, %1025
  %1032 = fsub double %1014, %1022
  %1033 = fsub double %1015, %1025
  %1034 = getelementptr inbounds double* %work_x, i64 %indvars.iv
  store double %1004, double* %1034, align 8, !tbaa !5
  %1035 = add nsw i64 %indvars.iv, 64
  %1036 = getelementptr inbounds double* %work_x, i64 %1035
  store double %1026, double* %1036, align 8, !tbaa !5
  %1037 = add nsw i64 %indvars.iv, 128
  %1038 = getelementptr inbounds double* %work_x, i64 %1037
  store double %1008, double* %1038, align 8, !tbaa !5
  %1039 = add nsw i64 %indvars.iv, 192
  %1040 = getelementptr inbounds double* %work_x, i64 %1039
  store double %1030, double* %1040, align 8, !tbaa !5
  %1041 = add nsw i64 %indvars.iv, 256
  %1042 = getelementptr inbounds double* %work_x, i64 %1041
  store double %1006, double* %1042, align 8, !tbaa !5
  %1043 = add nsw i64 %indvars.iv, 320
  %1044 = getelementptr inbounds double* %work_x, i64 %1043
  store double %1028, double* %1044, align 8, !tbaa !5
  %1045 = add nsw i64 %indvars.iv, 384
  %1046 = getelementptr inbounds double* %work_x, i64 %1045
  store double %1010, double* %1046, align 8, !tbaa !5
  %1047 = add nsw i64 %indvars.iv, 448
  %1048 = getelementptr inbounds double* %work_x, i64 %1047
  store double %1032, double* %1048, align 8, !tbaa !5
  %1049 = getelementptr inbounds double* %work_y, i64 %indvars.iv
  store double %1005, double* %1049, align 8, !tbaa !5
  %1050 = getelementptr inbounds double* %work_y, i64 %1035
  store double %1027, double* %1050, align 8, !tbaa !5
  %1051 = getelementptr inbounds double* %work_y, i64 %1037
  store double %1009, double* %1051, align 8, !tbaa !5
  %1052 = getelementptr inbounds double* %work_y, i64 %1039
  store double %1031, double* %1052, align 8, !tbaa !5
  %1053 = getelementptr inbounds double* %work_y, i64 %1041
  store double %1007, double* %1053, align 8, !tbaa !5
  %1054 = getelementptr inbounds double* %work_y, i64 %1043
  store double %1029, double* %1054, align 8, !tbaa !5
  %1055 = getelementptr inbounds double* %work_y, i64 %1045
  store double %1011, double* %1055, align 8, !tbaa !5
  %1056 = getelementptr inbounds double* %work_y, i64 %1047
  store double %1033, double* %1056, align 8, !tbaa !5
  %indvars.iv.next = add nuw nsw i64 %indvars.iv, 1
  %exitcond = icmp eq i64 %indvars.iv.next, 64
  br i1 %exitcond, label %1057, label %.preheader

; <label>:1057                                    ; preds = %.preheader
  call void @llvm.lifetime.end(i64 64, i8* %2) #2
  call void @llvm.lifetime.end(i64 64, i8* %1) #2
  ret void
}

; Function Attrs: nounwind
declare void @llvm.lifetime.start(i64, i8* nocapture) #2

; Function Attrs: nounwind
declare void @llvm.lifetime.end(i64, i8* nocapture) #2

attributes #0 = { nounwind uwtable "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nounwind "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #2 = { nounwind }
attributes #3 = { nobuiltin nounwind }
attributes #4 = { nobuiltin }

!llvm.ident = !{!0}

!0 = metadata !{metadata !"clang version 3.4 (tags/RELEASE_34/final)"}
!1 = metadata !{metadata !2, metadata !2, i64 0}
!2 = metadata !{metadata !"int", metadata !3, i64 0}
!3 = metadata !{metadata !"omnipotent char", metadata !4, i64 0}
!4 = metadata !{metadata !"Simple C/C++ TBAA"}
!5 = metadata !{metadata !6, metadata !6, i64 0}
!6 = metadata !{metadata !"double", metadata !3, i64 0}
