CPPFLAGS=-Wall -std=c++11 -c

all: clean basic_block compute_node register operations llvm_types

llvm_types: llvm_types.cc llvm_types.hh
	g++ ${CPPFLAGS} llvm_types.cc
basic_block: basic_block.cc basic_block.hh
	g++ ${CPPFLAGS} basic_block.cc
compute_node: compute_node.cc compute_node.hh
	g++ ${CPPFLAGS} compute_node.cc
register: register.cc register.hh
	g++ ${CPPFLAGS} register.cc
operations: operations.cc operations.hh
	g++ ${CPPFLAGS} operations.cc
clean:
	rm -f *.o
