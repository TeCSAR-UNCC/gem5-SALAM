# gem5-SALAM #

SALAM (System Architecture for LLVM-based Accelerator Modeling) integrated custom accelerator modeling into gem5 via a runtime LLVM interpreter.

## Requirements ##
- LLVM-3.8
- Frontend LLVM compiler for preferred development language (eg. clang for C)
- gem5 dependencies

## Usage ##
Define the computation model of you accelerator in you language of choice, and compile to LLVM IR. Control and dataflow graph optimization (eg. loop unrolling) should be handeled by the compiler. Construct accelerators by associating their LLVM IR with an LLVMInterface and connecting it to the desired CommInterface in the gem5 memory map.

Examples for system-level configuration can be found in configs/common/HWAcc.py.
Accelerator benchmarks and examples can be found in benchmarks. benchmarks/dmatest and benchmarks/streamdmatest contain examples for configuring and using central and streaming DMA devices.
The benchmarks/common folder contains basic drivers and syscalls for baremetal simulation.

Baremetal examples can be running using:
> baremetalarm.sh [benchmark]

Currently timing models are verified for full-system baremetal simulation.

## Full-system OS Simulation ##
Please download the latest version of the Linux Kernel for ARM from the [gem5 ARM kernel page](http://gem5.org/ARM_Kernel).
You will also need the [ARM disk images](http://www.gem5.org/dist/current/arm/) for full system simulation.
Devices operate in the physical memory address space.

## Resources ##
An excellent [tutorial for working with gem5](http://learning.gem5.org/book/index.html#).
