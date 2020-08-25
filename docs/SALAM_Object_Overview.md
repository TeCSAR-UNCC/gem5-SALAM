# SALAM Object Overview

All source code for these objects are stored in **src/hwacc**

## CommInterface

The communications interface is the base gem5 component of an accelerator in gem5-SALAM. It provides programmability as well as system memory access to the accelerator. It also provides mechanisms for synchronization, including memory interrupt lines. 

The memory map for the CommInterface object is broken into three sections:

- Flags: Provides runtime status information and switches for invoking the interface
- Config: Currently has no function, but is reserved for a future version 
- Variables: Addresses for runtime variables or values that will be pulled upon invocation. 

### Ports: 

- PIO: Connects to MMRs and provides external devices the ability to program the CommInterface.
- Local Ports: Provides access to other devices within an accelerator's local cluster .
- ACP Ports: Provides access to devices outside of the accelerator's local cluster.

- Stream Ports: Implements an AXI-stream like paradigm that limits read and write to data availability. It can be used in producer-consumer schemes with other devices using StreamBuffers or StreamDMAs. 
- SPM Ports: Provides access to scratchpads using the additional synchronization controls provided by the scratchpad memory.

## LLVMInterface

The LLVM Interface represents the data path of the accelerator. It is what parses the LLVM IR file to generate the hardware data path and then generates and executes the LLVM Control and Data Flow Graph (CDFG) using runtime data provided by the CommInterface. 

## AccCluster

This is an optional simulation object useful for organizing accelerators and resources shared between accelerators. AccCluster provides utilities for connecting accelerators and other shared resources into the larger system. 

## ScratchpadMemory

ScratchpadMemory is a custom fast-access memory for accelerators. It includes access synchronization mechanisms such as ready mode. When access synchronization is activated accelerators will not be able to access data that. Furthermore, additional controls will be placed on reads and writes to the scratchpad in order to implement various sync mechanisms.

## NoncoherentDma

NoncohherentDma provides a memory-to-memory transfer. This is useful for copying data to and from system memory and scratchpads. The MMR layout of the NoncoherentDma is described in noncoherent_dma.hh.

## StreamDma

The StreamDma provides DMA access between traditional memory objects, scratchpads, an AXI-stream like interface. It also supports auto-play features commonly found in video DMAs. The memory map is defined in stream_dma.hh 

## StreamBuffer

StreamBuffer is a small FIFO buffer that enables AXI-Stream like communication between devices. 

