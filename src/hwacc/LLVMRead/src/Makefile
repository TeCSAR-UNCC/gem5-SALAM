CPPFLAGS=-Wall -std=c++11 -c

all: clean basic_block llvm_types instructions registers power_func

llvm_types: llvm_types.cc llvm_types.hh
	g++ ${CPPFLAGS} llvm_types.cc
basic_block: basic_block.cc basic_block.hh
	g++ ${CPPFLAGS} basic_block.cc
registers: registers.cc registers.hh
	g++ ${CPPFLAGS} registers.cc
instructions: instructions.cc instructions.hh
	g++ ${CPPFLAGS} instructions.cc
instructions: power_func.cc power_func.hh
	g++ ${CPPFLAGS} power_func.cc

clean:
	rm -f *.o
