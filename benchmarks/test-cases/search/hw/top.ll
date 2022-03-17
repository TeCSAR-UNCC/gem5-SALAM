; ModuleID = 'top.c'
source_filename = "top.c"
target datalayout = "e-m:e-p:32:32-Fi8-i64:64-v128:64:128-a:0:32-n32-S64"
target triple = "armv7-pc-none-eabi"

%struct.BTree = type { [7 x i32], [8 x %struct.BTree*], i32, i32 }

; Function Attrs: nofree norecurse nounwind
define dso_local void @top(i32* nocapture readonly %0, i32* nocapture %1, i32* nocapture readonly %2) local_unnamed_addr #0 {
  %4 = bitcast i32* %2 to %struct.BTree**
  %5 = load %struct.BTree*, %struct.BTree** %4, align 4, !tbaa !3
  %6 = load i32, i32* %0, align 4, !tbaa !3
  %7 = getelementptr inbounds %struct.BTree, %struct.BTree* %5, i32 0, i32 2
  %8 = load i32, i32* %7, align 4, !tbaa !7
  %9 = icmp eq i32 %8, 0
  br i1 %9, label %10, label %15

10:                                               ; preds = %3, %48
  %11 = phi %struct.BTree* [ %49, %48 ], [ %5, %3 ]
  %12 = getelementptr inbounds %struct.BTree, %struct.BTree* %11, i32 0, i32 3
  %13 = load i32, i32* %12, align 4, !tbaa !9
  %14 = icmp sgt i32 %13, 0
  br i1 %14, label %20, label %48

15:                                               ; preds = %48, %3
  %16 = phi %struct.BTree* [ %5, %3 ], [ %49, %48 ]
  %17 = getelementptr inbounds %struct.BTree, %struct.BTree* %16, i32 0, i32 3
  %18 = load i32, i32* %17, align 4, !tbaa !9
  %19 = icmp sgt i32 %18, 0
  br i1 %19, label %56, label %63

20:                                               ; preds = %10, %42
  %21 = phi i32 [ %46, %42 ], [ %13, %10 ]
  %22 = phi i32 [ %44, %42 ], [ 0, %10 ]
  %23 = phi %struct.BTree* [ %43, %42 ], [ %11, %10 ]
  %24 = getelementptr inbounds %struct.BTree, %struct.BTree* %23, i32 0, i32 0, i32 %22
  %25 = load i32, i32* %24, align 4, !tbaa !3
  %26 = icmp ult i32 %6, %25
  br i1 %26, label %27, label %30

27:                                               ; preds = %20
  %28 = getelementptr inbounds %struct.BTree, %struct.BTree* %23, i32 0, i32 1, i32 %22
  %29 = load %struct.BTree*, %struct.BTree** %28, align 4, !tbaa !10
  br label %48

30:                                               ; preds = %20
  %31 = icmp eq i32 %6, %25
  br i1 %31, label %32, label %35

32:                                               ; preds = %30
  store i32 1, i32* %1, align 4, !tbaa !3
  %33 = getelementptr inbounds %struct.BTree, %struct.BTree* %23, i32 0, i32 1, i32 %22
  %34 = load %struct.BTree*, %struct.BTree** %33, align 4, !tbaa !10
  br label %48

35:                                               ; preds = %30
  %36 = add nsw i32 %21, -1
  %37 = icmp eq i32 %22, %36
  br i1 %37, label %38, label %42

38:                                               ; preds = %35
  %39 = add nuw nsw i32 %22, 1
  %40 = getelementptr inbounds %struct.BTree, %struct.BTree* %23, i32 0, i32 1, i32 %39
  %41 = load %struct.BTree*, %struct.BTree** %40, align 4, !tbaa !10
  br label %42

42:                                               ; preds = %35, %38
  %43 = phi %struct.BTree* [ %41, %38 ], [ %23, %35 ]
  %44 = add nuw nsw i32 %22, 1
  %45 = getelementptr inbounds %struct.BTree, %struct.BTree* %43, i32 0, i32 3
  %46 = load i32, i32* %45, align 4, !tbaa !9
  %47 = icmp slt i32 %44, %46
  br i1 %47, label %20, label %48

48:                                               ; preds = %42, %10, %32, %27
  %49 = phi %struct.BTree* [ %29, %27 ], [ %34, %32 ], [ %11, %10 ], [ %43, %42 ]
  %50 = getelementptr inbounds %struct.BTree, %struct.BTree* %49, i32 0, i32 2
  %51 = load i32, i32* %50, align 4, !tbaa !7
  %52 = icmp eq i32 %51, 0
  br i1 %52, label %10, label %15

53:                                               ; preds = %56
  %54 = load i32, i32* %17, align 4, !tbaa !9
  %55 = icmp slt i32 %61, %54
  br i1 %55, label %56, label %63

56:                                               ; preds = %15, %53
  %57 = phi i32 [ %61, %53 ], [ 0, %15 ]
  %58 = getelementptr inbounds %struct.BTree, %struct.BTree* %16, i32 0, i32 0, i32 %57
  %59 = load i32, i32* %58, align 4, !tbaa !3
  %60 = icmp eq i32 %6, %59
  %61 = add nuw nsw i32 %57, 1
  br i1 %60, label %62, label %53

62:                                               ; preds = %56
  store i32 1, i32* %1, align 4, !tbaa !3
  br label %63

63:                                               ; preds = %53, %15, %62
  ret void
}

attributes #0 = { nofree norecurse nounwind "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "frame-pointer"="all" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="generic" "target-features"="+armv7-a,+dsp,+soft-float,+strict-align,-crypto,-d32,-dotprod,-fp-armv8,-fp-armv8d16,-fp-armv8d16sp,-fp-armv8sp,-fp16,-fp16fml,-fp64,-fpregs,-fullfp16,-mve,-mve.fp,-neon,-thumb-mode,-vfp2,-vfp2sp,-vfp3,-vfp3d16,-vfp3d16sp,-vfp3sp,-vfp4,-vfp4d16,-vfp4d16sp,-vfp4sp" "unsafe-fp-math"="false" "use-soft-float"="true" }

!llvm.module.flags = !{!0, !1}
!llvm.ident = !{!2}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 1, !"min_enum_size", i32 4}
!2 = !{!"clang version 10.0.0-4ubuntu1~18.04.2 "}
!3 = !{!4, !4, i64 0}
!4 = !{!"int", !5, i64 0}
!5 = !{!"omnipotent char", !6, i64 0}
!6 = !{!"Simple C/C++ TBAA"}
!7 = !{!8, !4, i64 60}
!8 = !{!"BTree", !5, i64 0, !5, i64 28, !4, i64 60, !4, i64 64}
!9 = !{!8, !4, i64 64}
!10 = !{!11, !11, i64 0}
!11 = !{!"any pointer", !5, i64 0}
