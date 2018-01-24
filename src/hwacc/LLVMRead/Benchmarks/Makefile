BENCHMARKS=\
	aes/aes \
	bfs/bulk \
	bfs/queue \
	fft/strided \
	fft/transpose \
	gemm/ncubed \
	gemm/blocked \
	kmp/kmp \
	md/knn \
	md/grid \
	nw/nw \
	sort/merge \
	sort/radix \
	spmv/crs \
	spmv/ellpack \
	stencil/stencil2d \
	stencil/stencil3d \
	viterbi/viterbi
	
CFLAGS=-O1 -Wall -Wno-unused-label

.PHONY: build clean all

build:
	@( for b in $(BENCHMARKS); do $(MAKE) CFLAGS="$(CFLAGS)" -C $$b; done )
	
clean:
	@( for b in $(BENCHMARKS); do $(MAKE) -C $$b clean || exit ; done )
	
all: clean build
