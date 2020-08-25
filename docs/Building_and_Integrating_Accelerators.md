# Building and Integrating Accelerators in gem5-SALAM

## Writing the Accelerator Code

In this example we want to design an accelerator for a Generic Matrix Multiply operation (GEMM). This example has already been created in **benchmarks/sys_validation/gemm** and will be referenced throughout this guide. It contains a folder for accelerator code (hw) and a folder for the host code (sw). 

When creating your accelerator there are a many considerations to be made. 

These include:

- Parallelism Desired
- Desired power consumption
- Target area 
- How to integrate it into the system 
- How to receive data
- Is it going to be coupled in main memory
- Does the accelerator need DMAs
- How to control the accelerator

We are going to design a highly parallel accelerator that is loosely coupled in memory and control. It will also utilize a DMA for memory transfers between the accelerator's scratch-pad memory, and main memory.

Our accelerator code has two main files. **top.c** manages the GEMM accelerator and DMAs. While **gemm.c** includes our algorithm with any compiler optimizations that we want. 

### gemm.c 

We will first start with creating the code for our accelerator. 

In gemm.c there is a GEMM loop application written. To expose parallelism for computation and memory access we fully unroll the innermost loop of the application. gem5-SALAM will natively pipeline the other loop instances for us. To accomplish the loop unrolling we can utilize clang compiler pragmas such as those on line 18 of gemm.c: 

```c
#pragma clang loop unroll(full)
```

Since the GEMM accelerator is going to pull from a static set of memory accesses that are hard coded, the memory addresses associated with each of the matrices for the GEMM operation will be used again in our system design.

The complete code for the GEMM accelerator can be found under **gemm/hw/gemm.c**

### top.c

Now that the accelerator is written, we will write our control mechanism for the accelerator and DMAs. In this instance we want to have the DMAs and accelerator controlled by an additional device to reduce overhead on the CPU. 

In the top.c we begin with a declaration having 3 addresses passed to our Top accelerator. These addresses correspond to locations in the accelerator's Memory Mapped Register (MMR) that will be filled by the host CPU later.

Additionally, we setup a series of static addresses (Lines 8-12) associated with the MMRs of the GEMM accelerator so that the Top accelerator can invoke and control those. 

```c
volatile uint8_t  * GEMMFlags  = (uint8_t *)GEMM;
volatile uint8_t  * DmaFlags   = (uint8_t  *)(DMA);
volatile uint64_t * DmaRdAddr  = (uint64_t *)(DMA+1);
volatile uint64_t * DmaWrAddr  = (uint64_t *)(DMA+9);
volatile uint32_t * DmaCopyLen = (uint32_t *)(DMA+17)
```

We then set the MMRs of the DMA to perform the memory copy between DRAM and the scratchpad memory (Lines 16-28). 

```c
//Transfer Input Matrices
//Transfer M1
*DmaRdAddr  = m1_addr;
*DmaWrAddr  = M1ADDR;
*DmaCopyLen = mat_size;
*DmaFlags   = DEV_INIT;
//Poll DMA for finish
while ((*DmaFlags & DEV_INTR) != DEV_INTR);
//Transfer M2
*DmaRdAddr  = m2_addr;
*DmaWrAddr  = M2ADDR;
*DmaCopyLen = mat_size;
*DmaFlags   = DEV_INIT;
//Poll DMA for finish
while ((*DmaFlags & DEV_INTR) != DEV_INTR);
```

After copying our two input matrices we invoke the GEMM accelerator and wait for it to finish computation (Lines 31-33). 

```c 
//Start the accelerated function
*GEMMFlags = DEV_INIT;
//Poll function for finish
while ((*GEMMFlags & DEV_INTR) != DEV_INTR);
```

After the computation has completed, we write back the resulting matrix from the scratchpad memory to system memory (Lines 36-41).

```c
//Transfer M3
*DmaRdAddr  = M3ADDR;
*DmaWrAddr  = m3_addr;
*DmaCopyLen = mat_size;
*DmaFlags   = DEV_INIT;
//Poll DMA for finish
while ((*DmaFlags & DEV_INTR) != DEV_INTR);
```

The complete code for the Top accelerator can be found in **gemm/hw/top.c**

### INI files

For each of our accelerators we also need to generate an INI file. In each INI file we can define the number of cycles for each IR instruction and provide any limitations on the number of Functional Units (FUs) associated with IR instructions. 

Additionally, there are options for setting the FU clock periods and controls for pipelining of the accelerator. Below is an example with a few IR instructions and their respective cycle counts:

```ini
[CycleCounts]
counter = 1
gep = 0
phi = 0
select = 1
ret = 1
br = 0
switch = 1
indirectbr = 1
invoke = 1
```

Importantly, under the AccConfig section, we set MMR specific details such as the size of the flags register, memory address, interrupt line number, and the accelerator's clock. 

```ini
[AccConfig]
flags_size = 1
config_size = 0
int_num = -1
clock_period = 10
premap_data = 0
data_bases = 0
```

In the Memory section, you can define the scratchpad's memory address, size, response latency and number of ports. Also, if you want the accelerator to verify that memory exists in the scratchpad prior to accessing the scratchpad we can set ready mode to true. 

```ini
[Memory]
addr_range = 0x2f100000
size = 98304
latency = 2ns
ports = 4
ready_mode = True
reset_on_private_read = False
```

Lastly, we need to set the memory address under CommInterface of our MMR for the accelerator as well as the overall size of the MMR that account for all variables that need to be passed, 8 bytes per variable, and flags.

```ini
[CommInterface]
pio_addr = 0x2f000019
pio_size = 1
```

## Constructing the System

We are now going to leverage and modify the example scripts for gem5's full system simulation. In **configs/SALAM/sysValidation.py** we have a modified version of the script located in **configs/examples/fs.py** that is provided with the default gem5. The main difference in our configuration is we invoke our own function that has been added on line 240. This adds **configs/SALAM/validate_acc.py** to the overall system configuration.

### validate_acc.py

#### Configuring the Accelerator Cluster

In order to simplify the organization of accelerator-related resources, we define a accelerator cluster. This accelerator cluster will contain any shared resources between the accelerators as well as the accelerators themselves. It has several functions associated with it that help with attaching accelerators to it and for hooking cluster into the system. 

The *_attach_bridges* function (line 19) connects the accelerator cluster into the larger system, and connects the memory bus to the cluster. This gives devices outside the cluster master access to cluster resources. 

```python
system.acctest._attach_bridges(system, local_range, external_range)
```

We then invoke the *_connect_caches* function (line 20) in order to connect any cache hierarchy that exists in-between the cluster, the memory bus, or l2xbar of the CPU depending on design.  This gives the accelerator cluster master access to resources outside of itself. It also establishes coherency between cluster and other resources via caches. If no caches are needed this will merely attach the cluster to the memory bus without a cache.

```python
system.acctest._connect_caches(system, options, l2coherent=False)
```

These functions are defined in **src/hwacc/AccCluster.py**

### Adding Accelerators to the Cluster

#### Top

First, we are going to create a CommInterface (Line 30) which is the communications portion of our Top accelerator. We will then configure Top and generate its LLVM interface by passing CommInterface, a config file, and an IR file, to AccConfig (Line 31). This will generate the LLVM interface, configure any hardware limitations, and will establish the static Control and Dataflow Graph (CDFG).

We then connect the accelerator to the cluster (Line 32). This will attach the PIO port of the accelerator to the cluster's local bus that is associated with MMRs. 

#### Bench

For our benchmark, we follow the same steps. 

- Create a CommInterface 
- Configure it using AccConfig
- Attach it to the accelerator cluster

This can be seen on lines 35-39.

```bash
# Add the benchmark function
acc = options.accbench
config = hw_path + acc + ".ini"
ir = hw_path + acc + ".ll"
system.acctest.bench = CommInterface(devicename=acc, gic=gic, reset_spm=False)
AccConfig(system.acctest.bench, config, ir)
```

Because we want our Bench accelerator to be managed by the Top accelerator, we connect the PIO directly to the local ports of the Top accelerator. This creates a direct connection, with no additional buses or ports (Line 40). 

We then define a scratchpad memory and configure it using AccSPMConfig, which points to our accelerator's config file (Line 42). 

Lastly we connect scratchpad memory to the cluster (Line 43), this allows for all accelerators in the cluster to access it. 

Lines 46-63 configure different buffer sizes for the DMA. These are optional, but are presented to demonstrate how you can impose additional limitations on the DMA to control how data is transferred.

#### DMA

Finally, we create a NoncoherentDma and attach it to our cluster on lines 66-71. 

```bash
# Add the cluster DMA
system.acctest.dma = NoncoherentDma(pio_addr=0x2FF00000, pio_size=21, gic=gic, int_num=98)
system.acctest.dma.cluster_dma = system.acctest.local_bus.slave
system.acctest.dma.dma = system.acctest.coherency_bus.slave
system.acctest.dma.pio = system.acctest.top.local
system.acctest.dma.max_req_size = max_req_size
system.acctest.dma.buffer_size = buffer_size
```

Please note that NoncoherentDma, by name, does not have any coherency. If coherency is desired in an application, you can simply connect it to a cache in the gem5 system.

The ports attached on lines 67-69 are described below:

- cluster_dma: This port allows for master access within cluster. This connects it to the cluster local_bus which gives it access to the scratchpad memory  
- dma: This port provides master access to the overall system. This is achieved by connecting the port to the coherency bus.
- pio: This port is associated with the MMR and gives other devices control of the DMA. In this example, the PIO port is connected to the Top accelerator since it is the only device interacting with the DMA.

This configuration code for the System Validation benchmarks is located in **configs/SALAM/validate_acc.py**  

## Writing the Host Code

For this application, we are using a bare metal kernel. This means that we will have a load file, assembly file,  and must generate ELF files for execution.

In our boot code, we setup an Interrupt Service Routine (ISR) in isr.c to interact with our Top accelerator.

### main.cpp

In our main software file we start by creating and filling out our matrices for the GEMM operation. This is accomplished with the genData function defined in bench.h. We then pass the addresses of those matrices to the accelerator via its MMRs and invoke the accelerator (Lines 36-40). 

```c
*val_a = (uint32_t)(void *)m1;
*val_b = (uint32_t)(void *)m2;
*val_c = (uint32_t)(void *)m3;
// printf("%d\n", *top);
*top = 0x01;
```

Since this is an interrupting system it would be possible to perform other operations while the device is not finished, however; in this example there is nothing to do until our task is completed. 

Once the accelerator is done, we will read back the results for evaluation. The verification for the results can be seen in lines 44-75. We will also utilize the m5_dump_stats function to tell gem5 to output statistics it has been tracking, and m5_exit to close out the simulation. The statistics associated with the accelerator will automatically dump to stdout when the it finishes executing.

Please note that a run script is provided for all of the System Validation benchmarks. It is located at **gem5-SALAM/systemValidation.sh**. Usage instructions for this script are provided in the README.

## Compiling the Benchmark

Once the two accelerators have been written, you will want to invoke the clang compiler to generate the LLVM Intermediate Representation  (IR).

An example Makefile has also been provided with the GEMM benchmark. In the  Makefile we compile our accelerators to IR, then we pass that through the LLVM optimizer with Level 1 optimizations, and disable the subsequent object file to get the human readable IR.

```bash
KERN=gemm
TARGET=$(KERN).ll
%.o : %.c
	clang -emit-llvm -c -o $@ $<
%.bc : %.o
	opt -O1 -mem2reg -simplifycfg -loops -loop-unroll -o $@ $<
%.ll : %.bc
	llvm-dis -o $@ $<
build : $(TARGET)
```

Finally, to compile your host code a similar Makefile is provided. This Makefile also generates the ELF file that is necessary to run the benchmark. 

```bash
include ../../../common/Makefile

OBJS = boot.o ../../../common/syscalls.o main.o isr.o

main.elf: $(OBJS) $(LNK_SCRIPT) Makefile
        $(CC) $(LNK_FILE_OPT) -o $@ $(OBJS) $(LNK_OPT)

boot.o: Makefile
        $(CPP) boot.s $(CFLAGS) | $(AS) $(ASFLAGS) -o boot.o
```

 Simply run both Makefiles and the benchmark will be ready to run.

## Benchmark Output

This section concerns the output of a benchmark after is has already been run. The system validation script will be used. You can refer to [here](https://github.com/clonetrooper67/gem5-SALAM-README/blob/master/README.md#system-validation-examples) in the README for a usage guide. 

In gem5-SALAM when a benchmark is run there are a few items that are of interest. 

- The system's terminal output
- Statistics
- System Configuration Diagram

Once the GEMM benchmark is run the system validation script places the system's output in **BM_ARM_OUT/sys_validation/gemm**

In **stats.txt** you are able to see various gem5 stats such as memory usage, time the simulation ran, etc. This is useful for performing any analysis on the system you are working with. 

```bash
---------- Begin Simulation Statistics ----------
host_inst_rate                     106061 # Simulator instruction rate (inst/s)
host_mem_usage                     5712112 # Number of bytes of host memory used
host_op_rate                       119835 # Simulator op (including micro ops) rate (op/s)
host_seconds                       273.03 # Real time elapsed on the host
host_tick_rate                     840130224 # Simulator tick rate (ticks/s)
```

If you have installed the [optional graphviz dependencies](https://github.com/clonetrooper67/gem5-SALAM-README#visualization) gem5-SALAM will generate a system configuration diagram when run. In our case the GEMM accelerator's diagram is stored in **config.dot** and should look like below: 

<p align="center"> 
    <img width="359" height="390" src="https://github.com/clonetrooper67/gem5-SALAM-README/blob/master/GEMM_ACC.png"
</p>


**system.terminal** contains the simulated system's terminal output. This can be useful for viewing any issues or outputs that the actual simulation might have. An example from the GEMM benchmark is shown below:

```bash
Generating data
Data generated
Interrupt
Interrupt finished
Job complete
Checking result
Running bench on CPU
Comparing CPU run to accelerated run
Check Passed
```

