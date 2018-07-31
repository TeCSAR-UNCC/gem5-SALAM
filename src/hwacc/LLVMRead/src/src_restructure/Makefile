CPPFLAGS=-Wall -std=c++11 -c

all: clean instructions registers

registers: registers.cc registers.hh
	g++ ${CPPFLAGS} registers.cc
instructions: instructions.cc instructions.hh
	g++ ${CPPFLAGS} instructions.cc
clean:
	rm -f *.o
