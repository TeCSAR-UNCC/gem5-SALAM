; ModuleID = 'top.c'
source_filename = "top.c"
target datalayout = "e-m:e-p:32:32-Fi8-i64:64-v128:64:128-a:0:32-n32-S64"
target triple = "armv7-pc-none-eabi"

; Function Attrs: nofree norecurse nounwind
define dso_local void @vadd(i32* nocapture readonly %0, i32* nocapture readonly %1, i32* nocapture %2) local_unnamed_addr #0 !dbg !7 {
  br label %4, !dbg !9

4:                                                ; preds = %4, %3
  %5 = phi i32 [ 0, %3 ], [ %12, %4 ]
  %6 = getelementptr inbounds i32, i32* %0, i32 %5, !dbg !10
  %7 = load i32, i32* %6, align 4, !dbg !10, !tbaa !11
  %8 = getelementptr inbounds i32, i32* %1, i32 %5, !dbg !15
  %9 = load i32, i32* %8, align 4, !dbg !15, !tbaa !11
  %10 = add i32 %9, %7, !dbg !16
  %11 = getelementptr inbounds i32, i32* %2, i32 %5, !dbg !17
  store i32 %10, i32* %11, align 4, !dbg !18, !tbaa !11
  %12 = add nuw nsw i32 %5, 1, !dbg !19
  %13 = icmp eq i32 %12, 256, !dbg !20
  br i1 %13, label %14, label %4, !dbg !9, !llvm.loop !21

14:                                               ; preds = %4
  ret void, !dbg !25
}

; Function Attrs: nofree norecurse nounwind
define dso_local void @top() local_unnamed_addr #0 !dbg !26 {
  call void @vadd(i32* nonnull inttoptr (i32 268566656 to i32*), i32* nonnull inttoptr (i32 268567744 to i32*), i32* nonnull inttoptr (i32 268568832 to i32*)), !dbg !27
  ret void, !dbg !28
}

attributes #0 = { nofree norecurse nounwind "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "frame-pointer"="all" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="generic" "target-features"="+armv7-a,+dsp,+soft-float,+strict-align,-crypto,-d32,-dotprod,-fp-armv8,-fp-armv8d16,-fp-armv8d16sp,-fp-armv8sp,-fp16,-fp16fml,-fp64,-fpregs,-fullfp16,-mve,-mve.fp,-neon,-thumb-mode,-vfp2,-vfp2sp,-vfp3,-vfp3d16,-vfp3d16sp,-vfp3sp,-vfp4,-vfp4d16,-vfp4d16sp,-vfp4sp" "unsafe-fp-math"="false" "use-soft-float"="true" }

!llvm.dbg.cu = !{!0}
!llvm.module.flags = !{!3, !4, !5}
!llvm.ident = !{!6}

!0 = distinct !DICompileUnit(language: DW_LANG_C99, file: !1, producer: "clang version 10.0.0-4ubuntu1~18.04.2 ", isOptimized: true, runtimeVersion: 0, emissionKind: NoDebug, enums: !2, splitDebugInlining: false, nameTableKind: None)
!1 = !DIFile(filename: "top.c", directory: "/home/aditya/gem5-SALAM/benchmarks/test-cases/vadd/hw")
!2 = !{}
!3 = !{i32 2, !"Debug Info Version", i32 3}
!4 = !{i32 1, !"wchar_size", i32 4}
!5 = !{i32 1, !"min_enum_size", i32 4}
!6 = !{!"clang version 10.0.0-4ubuntu1~18.04.2 "}
!7 = distinct !DISubprogram(name: "vadd", scope: !1, file: !1, line: 5, type: !8, scopeLine: 5, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition | DISPFlagOptimized, unit: !0, retainedNodes: !2)
!8 = !DISubroutineType(types: !2)
!9 = !DILocation(line: 9, column: 2, scope: !7)
!10 = !DILocation(line: 10, column: 10, scope: !7)
!11 = !{!12, !12, i64 0}
!12 = !{!"int", !13, i64 0}
!13 = !{!"omnipotent char", !14, i64 0}
!14 = !{!"Simple C/C++ TBAA"}
!15 = !DILocation(line: 10, column: 17, scope: !7)
!16 = !DILocation(line: 10, column: 15, scope: !7)
!17 = !DILocation(line: 10, column: 3, scope: !7)
!18 = !DILocation(line: 10, column: 8, scope: !7)
!19 = !DILocation(line: 9, column: 20, scope: !7)
!20 = !DILocation(line: 9, column: 13, scope: !7)
!21 = distinct !{!21, !9, !22, !23, !24}
!22 = !DILocation(line: 11, column: 2, scope: !7)
!23 = !{!"llvm.loop.vectorize.width", i32 2}
!24 = !{!"llvm.loop.vectorize.enable", i1 true}
!25 = !DILocation(line: 14, column: 1, scope: !7)
!26 = distinct !DISubprogram(name: "top", scope: !1, file: !1, line: 16, type: !8, scopeLine: 16, spFlags: DISPFlagDefinition | DISPFlagOptimized, unit: !0, retainedNodes: !2)
!27 = !DILocation(line: 21, column: 2, scope: !26)
!28 = !DILocation(line: 24, column: 1, scope: !26)
