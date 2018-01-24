; ModuleID = 'aes.c'
target datalayout = "e-p:64:64:64-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:64:64-f32:32:32-f64:64:64-v64:64:64-v128:128:128-a0:0:64-s0:64:64-f80:128:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%struct.aes256_context = type { [32 x i8], [32 x i8], [32 x i8] }

@sbox = constant [256 x i8] c"c|w{\F2ko\C50\01g+\FE\D7\ABv\CA\82\C9}\FAYG\F0\AD\D4\A2\AF\9C\A4r\C0\B7\FD\93&6?\F7\CC4\A5\E5\F1q\D81\15\04\C7#\C3\18\96\05\9A\07\12\80\E2\EB'\B2u\09\83,\1A\1BnZ\A0R;\D6\B3)\E3/\84S\D1\00\ED \FC\B1[j\CB\BE9JLX\CF\D0\EF\AA\FBCM3\85E\F9\02\7FP<\9F\A8Q\A3@\8F\92\9D8\F5\BC\B6\DA!\10\FF\F3\D2\CD\0C\13\EC_\97D\17\C4\A7~=d]\19s`\81O\DC\22*\90\88F\EE\B8\14\DE^\0B\DB\E02:\0AI\06$\5C\C2\D3\ACb\91\95\E4y\E7\C87m\8D\D5N\A9lV\F4\EAez\AE\08\BAx%.\1C\A6\B4\C6\E8\DDt\1FK\BD\8B\8Ap>\B5fH\03\F6\0Ea5W\B9\86\C1\1D\9E\E1\F8\98\11i\D9\8E\94\9B\1E\87\E9\CEU(\DF\8C\A1\89\0D\BF\E6BhA\99-\0F\B0T\BB\16", align 16

; Function Attrs: nounwind readnone uwtable
define zeroext i8 @rj_xtime(i8 zeroext %x) #0 {
  %1 = zext i8 %x to i32
  %2 = and i32 %1, 128
  %3 = icmp ne i32 %2, 0
  %4 = shl nuw nsw i32 %1, 1
  %5 = xor i32 %4, 27
  %6 = select i1 %3, i32 %5, i32 %4
  %7 = trunc i32 %6 to i8
  ret i8 %7
}

; Function Attrs: nounwind uwtable
define void @aes_subBytes(i8* nocapture %buf) #1 {
  br label %1

; <label>:1                                       ; preds = %0, %1
  %indvars.iv = phi i64 [ 15, %0 ], [ %indvars.iv.next, %1 ]
  %2 = getelementptr inbounds i8* %buf, i64 %indvars.iv
  %3 = load i8* %2, align 1, !tbaa !1
  %4 = zext i8 %3 to i64
  %5 = getelementptr inbounds [256 x i8]* @sbox, i64 0, i64 %4
  %6 = load i8* %5, align 1, !tbaa !1
  store i8 %6, i8* %2, align 1, !tbaa !1
  %7 = trunc i64 %indvars.iv to i8
  %8 = icmp eq i8 %7, 0
  %indvars.iv.next = add i64 %indvars.iv, -1
  br i1 %8, label %9, label %1

; <label>:9                                       ; preds = %1
  ret void
}

; Function Attrs: nounwind uwtable
define void @aes_addRoundKey(i8* nocapture %buf, i8* nocapture readonly %key) #1 {
  br label %1

; <label>:1                                       ; preds = %0, %1
  %indvars.iv = phi i64 [ 15, %0 ], [ %indvars.iv.next, %1 ]
  %2 = getelementptr inbounds i8* %key, i64 %indvars.iv
  %3 = load i8* %2, align 1, !tbaa !1
  %4 = getelementptr inbounds i8* %buf, i64 %indvars.iv
  %5 = load i8* %4, align 1, !tbaa !1
  %6 = xor i8 %5, %3
  store i8 %6, i8* %4, align 1, !tbaa !1
  %7 = trunc i64 %indvars.iv to i8
  %8 = icmp eq i8 %7, 0
  %indvars.iv.next = add i64 %indvars.iv, -1
  br i1 %8, label %9, label %1

; <label>:9                                       ; preds = %1
  ret void
}

; Function Attrs: nounwind uwtable
define void @aes_addRoundKey_cpy(i8* nocapture %buf, i8* nocapture readonly %key, i8* nocapture %cpk) #1 {
  br label %1

; <label>:1                                       ; preds = %0, %1
  %indvars.iv = phi i64 [ 15, %0 ], [ %indvars.iv.next, %1 ]
  %2 = getelementptr inbounds i8* %key, i64 %indvars.iv
  %3 = load i8* %2, align 1, !tbaa !1
  %4 = getelementptr inbounds i8* %cpk, i64 %indvars.iv
  store i8 %3, i8* %4, align 1, !tbaa !1
  %5 = getelementptr inbounds i8* %buf, i64 %indvars.iv
  %6 = load i8* %5, align 1, !tbaa !1
  %7 = xor i8 %6, %3
  store i8 %7, i8* %5, align 1, !tbaa !1
  %8 = add i64 %indvars.iv, 16
  %9 = getelementptr inbounds i8* %key, i64 %8
  %10 = load i8* %9, align 1, !tbaa !1
  %11 = getelementptr inbounds i8* %cpk, i64 %8
  store i8 %10, i8* %11, align 1, !tbaa !1
  %12 = trunc i64 %indvars.iv to i8
  %13 = icmp eq i8 %12, 0
  %indvars.iv.next = add i64 %indvars.iv, -1
  br i1 %13, label %14, label %1

; <label>:14                                      ; preds = %1
  ret void
}

; Function Attrs: nounwind uwtable
define void @aes_shiftRows(i8* nocapture %buf) #1 {
  %1 = getelementptr inbounds i8* %buf, i64 1
  %2 = load i8* %1, align 1, !tbaa !1
  %3 = getelementptr inbounds i8* %buf, i64 5
  %4 = load i8* %3, align 1, !tbaa !1
  store i8 %4, i8* %1, align 1, !tbaa !1
  %5 = getelementptr inbounds i8* %buf, i64 9
  %6 = load i8* %5, align 1, !tbaa !1
  store i8 %6, i8* %3, align 1, !tbaa !1
  %7 = getelementptr inbounds i8* %buf, i64 13
  %8 = load i8* %7, align 1, !tbaa !1
  store i8 %8, i8* %5, align 1, !tbaa !1
  store i8 %2, i8* %7, align 1, !tbaa !1
  %9 = getelementptr inbounds i8* %buf, i64 10
  %10 = load i8* %9, align 1, !tbaa !1
  %11 = getelementptr inbounds i8* %buf, i64 2
  %12 = load i8* %11, align 1, !tbaa !1
  store i8 %12, i8* %9, align 1, !tbaa !1
  store i8 %10, i8* %11, align 1, !tbaa !1
  %13 = getelementptr inbounds i8* %buf, i64 3
  %14 = load i8* %13, align 1, !tbaa !1
  %15 = getelementptr inbounds i8* %buf, i64 15
  %16 = load i8* %15, align 1, !tbaa !1
  store i8 %16, i8* %13, align 1, !tbaa !1
  %17 = getelementptr inbounds i8* %buf, i64 11
  %18 = load i8* %17, align 1, !tbaa !1
  store i8 %18, i8* %15, align 1, !tbaa !1
  %19 = getelementptr inbounds i8* %buf, i64 7
  %20 = load i8* %19, align 1, !tbaa !1
  store i8 %20, i8* %17, align 1, !tbaa !1
  store i8 %14, i8* %19, align 1, !tbaa !1
  %21 = getelementptr inbounds i8* %buf, i64 14
  %22 = load i8* %21, align 1, !tbaa !1
  %23 = getelementptr inbounds i8* %buf, i64 6
  %24 = load i8* %23, align 1, !tbaa !1
  store i8 %24, i8* %21, align 1, !tbaa !1
  store i8 %22, i8* %23, align 1, !tbaa !1
  ret void
}

; Function Attrs: nounwind uwtable
define void @aes_mixColumns(i8* nocapture %buf) #1 {
  br label %1

; <label>:1                                       ; preds = %0, %1
  %indvars.iv = phi i64 [ 0, %0 ], [ %indvars.iv.next, %1 ]
  %2 = getelementptr inbounds i8* %buf, i64 %indvars.iv
  %3 = load i8* %2, align 1, !tbaa !1
  %4 = or i64 %indvars.iv, 1
  %5 = getelementptr inbounds i8* %buf, i64 %4
  %6 = load i8* %5, align 1, !tbaa !1
  %7 = or i64 %indvars.iv, 2
  %8 = getelementptr inbounds i8* %buf, i64 %7
  %9 = load i8* %8, align 1, !tbaa !1
  %10 = or i64 %indvars.iv, 3
  %11 = getelementptr inbounds i8* %buf, i64 %10
  %12 = load i8* %11, align 1, !tbaa !1
  %13 = xor i8 %6, %3
  %14 = xor i8 %13, %9
  %15 = xor i8 %14, %12
  %16 = tail call zeroext i8 @rj_xtime(i8 zeroext %13) #2
  %17 = xor i8 %16, %3
  %18 = xor i8 %17, %15
  store i8 %18, i8* %2, align 1, !tbaa !1
  %19 = xor i8 %9, %6
  %20 = tail call zeroext i8 @rj_xtime(i8 zeroext %19) #2
  %21 = xor i8 %20, %15
  %22 = load i8* %5, align 1, !tbaa !1
  %23 = xor i8 %21, %22
  store i8 %23, i8* %5, align 1, !tbaa !1
  %24 = xor i8 %12, %9
  %25 = tail call zeroext i8 @rj_xtime(i8 zeroext %24) #2
  %26 = xor i8 %25, %15
  %27 = load i8* %8, align 1, !tbaa !1
  %28 = xor i8 %26, %27
  store i8 %28, i8* %8, align 1, !tbaa !1
  %29 = xor i8 %12, %3
  %30 = tail call zeroext i8 @rj_xtime(i8 zeroext %29) #2
  %31 = xor i8 %30, %15
  %32 = load i8* %11, align 1, !tbaa !1
  %33 = xor i8 %31, %32
  store i8 %33, i8* %11, align 1, !tbaa !1
  %indvars.iv.next = add nuw nsw i64 %indvars.iv, 4
  %34 = trunc i64 %indvars.iv.next to i8
  %35 = icmp ult i8 %34, 16
  br i1 %35, label %1, label %36

; <label>:36                                      ; preds = %1
  ret void
}

; Function Attrs: nounwind uwtable
define zeroext i8 @aes_expandEncKey(i8* %k, i8 zeroext %rc) #1 {
  %1 = getelementptr inbounds i8* %k, i64 29
  %2 = load i8* %1, align 1, !tbaa !1
  %3 = zext i8 %2 to i64
  %4 = getelementptr inbounds [256 x i8]* @sbox, i64 0, i64 %3
  %5 = load i8* %4, align 1, !tbaa !1
  %6 = xor i8 %5, %rc
  %7 = load i8* %k, align 1, !tbaa !1
  %8 = xor i8 %6, %7
  store i8 %8, i8* %k, align 1, !tbaa !1
  %9 = getelementptr inbounds i8* %k, i64 30
  %10 = load i8* %9, align 1, !tbaa !1
  %11 = zext i8 %10 to i64
  %12 = getelementptr inbounds [256 x i8]* @sbox, i64 0, i64 %11
  %13 = load i8* %12, align 1, !tbaa !1
  %14 = getelementptr inbounds i8* %k, i64 1
  %15 = load i8* %14, align 1, !tbaa !1
  %16 = xor i8 %15, %13
  store i8 %16, i8* %14, align 1, !tbaa !1
  %17 = getelementptr inbounds i8* %k, i64 31
  %18 = load i8* %17, align 1, !tbaa !1
  %19 = zext i8 %18 to i64
  %20 = getelementptr inbounds [256 x i8]* @sbox, i64 0, i64 %19
  %21 = load i8* %20, align 1, !tbaa !1
  %22 = getelementptr inbounds i8* %k, i64 2
  %23 = load i8* %22, align 1, !tbaa !1
  %24 = xor i8 %23, %21
  store i8 %24, i8* %22, align 1, !tbaa !1
  %25 = getelementptr inbounds i8* %k, i64 28
  %26 = load i8* %25, align 1, !tbaa !1
  %27 = zext i8 %26 to i64
  %28 = getelementptr inbounds [256 x i8]* @sbox, i64 0, i64 %27
  %29 = load i8* %28, align 1, !tbaa !1
  %30 = getelementptr inbounds i8* %k, i64 3
  %31 = load i8* %30, align 1, !tbaa !1
  %32 = xor i8 %31, %29
  store i8 %32, i8* %30, align 1, !tbaa !1
  %33 = ashr i8 %rc, 7
  %34 = and i8 %33, 27
  br label %35

; <label>:35                                      ; preds = %0, %35
  %indvars.iv5 = phi i64 [ 4, %0 ], [ %indvars.iv.next6, %35 ]
  %36 = phi i32 [ 4, %0 ], [ %68, %35 ]
  %37 = add nsw i32 %36, -4
  %38 = sext i32 %37 to i64
  %39 = getelementptr inbounds i8* %k, i64 %38
  %40 = load i8* %39, align 1, !tbaa !1
  %41 = getelementptr inbounds i8* %k, i64 %indvars.iv5
  %42 = load i8* %41, align 1, !tbaa !1
  %43 = xor i8 %42, %40
  store i8 %43, i8* %41, align 1, !tbaa !1
  %44 = add nsw i32 %36, -3
  %45 = sext i32 %44 to i64
  %46 = getelementptr inbounds i8* %k, i64 %45
  %47 = load i8* %46, align 1, !tbaa !1
  %48 = or i64 %indvars.iv5, 1
  %49 = getelementptr inbounds i8* %k, i64 %48
  %50 = load i8* %49, align 1, !tbaa !1
  %51 = xor i8 %50, %47
  store i8 %51, i8* %49, align 1, !tbaa !1
  %52 = add nsw i32 %36, -2
  %53 = sext i32 %52 to i64
  %54 = getelementptr inbounds i8* %k, i64 %53
  %55 = load i8* %54, align 1, !tbaa !1
  %56 = or i64 %indvars.iv5, 2
  %57 = getelementptr inbounds i8* %k, i64 %56
  %58 = load i8* %57, align 1, !tbaa !1
  %59 = xor i8 %58, %55
  store i8 %59, i8* %57, align 1, !tbaa !1
  %60 = add nsw i32 %36, -1
  %61 = sext i32 %60 to i64
  %62 = getelementptr inbounds i8* %k, i64 %61
  %63 = load i8* %62, align 1, !tbaa !1
  %64 = or i64 %indvars.iv5, 3
  %65 = getelementptr inbounds i8* %k, i64 %64
  %66 = load i8* %65, align 1, !tbaa !1
  %67 = xor i8 %66, %63
  store i8 %67, i8* %65, align 1, !tbaa !1
  %indvars.iv.next6 = add nuw nsw i64 %indvars.iv5, 4
  %68 = trunc i64 %indvars.iv.next6 to i32
  %69 = trunc i64 %indvars.iv.next6 to i8
  %70 = icmp ult i8 %69, 16
  br i1 %70, label %35, label %71

; <label>:71                                      ; preds = %35
  %72 = shl i8 %rc, 1
  %73 = xor i8 %34, %72
  %74 = getelementptr inbounds i8* %k, i64 12
  %75 = load i8* %74, align 1, !tbaa !1
  %76 = zext i8 %75 to i64
  %77 = getelementptr inbounds [256 x i8]* @sbox, i64 0, i64 %76
  %78 = load i8* %77, align 1, !tbaa !1
  %79 = getelementptr inbounds i8* %k, i64 16
  %80 = load i8* %79, align 1, !tbaa !1
  %81 = xor i8 %80, %78
  store i8 %81, i8* %79, align 1, !tbaa !1
  %82 = getelementptr inbounds i8* %k, i64 13
  %83 = load i8* %82, align 1, !tbaa !1
  %84 = zext i8 %83 to i64
  %85 = getelementptr inbounds [256 x i8]* @sbox, i64 0, i64 %84
  %86 = load i8* %85, align 1, !tbaa !1
  %87 = getelementptr inbounds i8* %k, i64 17
  %88 = load i8* %87, align 1, !tbaa !1
  %89 = xor i8 %88, %86
  store i8 %89, i8* %87, align 1, !tbaa !1
  %90 = getelementptr inbounds i8* %k, i64 14
  %91 = load i8* %90, align 1, !tbaa !1
  %92 = zext i8 %91 to i64
  %93 = getelementptr inbounds [256 x i8]* @sbox, i64 0, i64 %92
  %94 = load i8* %93, align 1, !tbaa !1
  %95 = getelementptr inbounds i8* %k, i64 18
  %96 = load i8* %95, align 1, !tbaa !1
  %97 = xor i8 %96, %94
  store i8 %97, i8* %95, align 1, !tbaa !1
  %98 = getelementptr inbounds i8* %k, i64 15
  %99 = load i8* %98, align 1, !tbaa !1
  %100 = zext i8 %99 to i64
  %101 = getelementptr inbounds [256 x i8]* @sbox, i64 0, i64 %100
  %102 = load i8* %101, align 1, !tbaa !1
  %103 = getelementptr inbounds i8* %k, i64 19
  %104 = load i8* %103, align 1, !tbaa !1
  %105 = xor i8 %104, %102
  store i8 %105, i8* %103, align 1, !tbaa !1
  br label %106

; <label>:106                                     ; preds = %71, %106
  %indvars.iv3 = phi i64 [ 20, %71 ], [ %indvars.iv.next4, %106 ]
  %107 = phi i32 [ 20, %71 ], [ %139, %106 ]
  %108 = add nsw i32 %107, -4
  %109 = sext i32 %108 to i64
  %110 = getelementptr inbounds i8* %k, i64 %109
  %111 = load i8* %110, align 1, !tbaa !1
  %112 = getelementptr inbounds i8* %k, i64 %indvars.iv3
  %113 = load i8* %112, align 1, !tbaa !1
  %114 = xor i8 %113, %111
  store i8 %114, i8* %112, align 1, !tbaa !1
  %115 = add nsw i32 %107, -3
  %116 = sext i32 %115 to i64
  %117 = getelementptr inbounds i8* %k, i64 %116
  %118 = load i8* %117, align 1, !tbaa !1
  %119 = or i64 %indvars.iv3, 1
  %120 = getelementptr inbounds i8* %k, i64 %119
  %121 = load i8* %120, align 1, !tbaa !1
  %122 = xor i8 %121, %118
  store i8 %122, i8* %120, align 1, !tbaa !1
  %123 = add nsw i32 %107, -2
  %124 = sext i32 %123 to i64
  %125 = getelementptr inbounds i8* %k, i64 %124
  %126 = load i8* %125, align 1, !tbaa !1
  %127 = or i64 %indvars.iv3, 2
  %128 = getelementptr inbounds i8* %k, i64 %127
  %129 = load i8* %128, align 1, !tbaa !1
  %130 = xor i8 %129, %126
  store i8 %130, i8* %128, align 1, !tbaa !1
  %131 = add nsw i32 %107, -1
  %132 = sext i32 %131 to i64
  %133 = getelementptr inbounds i8* %k, i64 %132
  %134 = load i8* %133, align 1, !tbaa !1
  %135 = or i64 %indvars.iv3, 3
  %136 = getelementptr inbounds i8* %k, i64 %135
  %137 = load i8* %136, align 1, !tbaa !1
  %138 = xor i8 %137, %134
  store i8 %138, i8* %136, align 1, !tbaa !1
  %indvars.iv.next4 = add nuw nsw i64 %indvars.iv3, 4
  %139 = trunc i64 %indvars.iv.next4 to i32
  %140 = trunc i64 %indvars.iv.next4 to i8
  %141 = icmp ult i8 %140, 32
  br i1 %141, label %106, label %142

; <label>:142                                     ; preds = %106
  ret i8 %73
}

; Function Attrs: nounwind uwtable
define void @aes256_encrypt_ecb(%struct.aes256_context* %ctx, i8* nocapture readonly %k, i8* nocapture %buf) #1 {
  br label %2

.preheader:                                       ; preds = %2
  %1 = getelementptr inbounds %struct.aes256_context* %ctx, i64 0, i32 2, i64 0
  br label %7

; <label>:2                                       ; preds = %2, %0
  %indvars.iv = phi i64 [ 0, %0 ], [ %indvars.iv.next, %2 ]
  %3 = getelementptr inbounds i8* %k, i64 %indvars.iv
  %4 = load i8* %3, align 1, !tbaa !1
  %5 = getelementptr inbounds %struct.aes256_context* %ctx, i64 0, i32 2, i64 %indvars.iv
  store i8 %4, i8* %5, align 1, !tbaa !1
  %6 = getelementptr inbounds %struct.aes256_context* %ctx, i64 0, i32 1, i64 %indvars.iv
  store i8 %4, i8* %6, align 1, !tbaa !1
  %indvars.iv.next = add nuw nsw i64 %indvars.iv, 1
  %exitcond6 = icmp eq i64 %indvars.iv.next, 32
  br i1 %exitcond6, label %.preheader, label %2

; <label>:7                                       ; preds = %.preheader, %7
  %i.14 = phi i8 [ 7, %.preheader ], [ %phitmp, %7 ]
  %rcon.03 = phi i8 [ 1, %.preheader ], [ %8, %7 ]
  %8 = tail call zeroext i8 @aes_expandEncKey(i8* %1, i8 zeroext %rcon.03) #2
  %phitmp = add i8 %i.14, -1
  %9 = icmp eq i8 %phitmp, 0
  br i1 %9, label %10, label %7

; <label>:10                                      ; preds = %7
  %11 = getelementptr inbounds %struct.aes256_context* %ctx, i64 0, i32 1, i64 0
  %12 = getelementptr inbounds %struct.aes256_context* %ctx, i64 0, i32 0, i64 0
  tail call void @aes_addRoundKey_cpy(i8* %buf, i8* %11, i8* %12) #2
  tail call void @aes_subBytes(i8* %buf) #2
  tail call void @aes_shiftRows(i8* %buf) #2
  %13 = getelementptr inbounds %struct.aes256_context* %ctx, i64 0, i32 0, i64 16
  br label %14

; <label>:14                                      ; preds = %20, %10
  %i.22 = phi i8 [ 1, %10 ], [ %21, %20 ]
  %rcon.11 = phi i8 [ 1, %10 ], [ %rcon.2, %20 ]
  tail call void @aes_mixColumns(i8* %buf) #2
  %15 = and i8 %i.22, 1
  %16 = icmp eq i8 %15, 0
  br i1 %16, label %18, label %17

; <label>:17                                      ; preds = %14
  tail call void @aes_addRoundKey(i8* %buf, i8* %13) #2
  br label %20

; <label>:18                                      ; preds = %14
  %19 = tail call zeroext i8 @aes_expandEncKey(i8* %12, i8 zeroext %rcon.11) #2
  tail call void @aes_addRoundKey(i8* %buf, i8* %12) #2
  br label %20

; <label>:20                                      ; preds = %17, %18
  %rcon.2 = phi i8 [ %rcon.11, %17 ], [ %19, %18 ]
  %21 = add i8 %i.22, 1
  tail call void @aes_subBytes(i8* %buf) #2
  tail call void @aes_shiftRows(i8* %buf) #2
  %exitcond = icmp eq i8 %21, 14
  br i1 %exitcond, label %22, label %14

; <label>:22                                      ; preds = %20
  %23 = tail call zeroext i8 @aes_expandEncKey(i8* %12, i8 zeroext %rcon.2) #2
  tail call void @aes_addRoundKey(i8* %buf, i8* %12) #2
  ret void
}

attributes #0 = { nounwind readnone uwtable "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nounwind uwtable "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #2 = { nobuiltin }

!llvm.ident = !{!0}

!0 = metadata !{metadata !"clang version 3.4 (tags/RELEASE_34/final)"}
!1 = metadata !{metadata !2, metadata !2, i64 0}
!2 = metadata !{metadata !"omnipotent char", metadata !3, i64 0}
!3 = metadata !{metadata !"Simple C/C++ TBAA"}
