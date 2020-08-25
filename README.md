# gem5-SALAM #

gem5-SALAM (System Architecture for LLVM-based Accelerator Modeling), is a novel system architecture designed to enable LLVM-based modeling and simulation of custom hardware accelerators.

# Requirements

**Please note that Ubuntu 20.04 is not currently supported**

- gem5 dependencies
- LLVM-3.8.1
- Frontend LLVM compiler for preferred development language (eg. clang for C)

# gem5-SALAM Setup

## Dependencies for gem5-SALAM

### Required

#### All Required Dependencies

```bash
sudo apt install build-essential m4 scons python-dev libprotobuf-dev python-protobuf protobuf-compiler libgoogle-perftools-dev
```

#### GCC (The Compiler used)

``` bash
sudo apt install build-essential m4
```

#### SCons (Build Environment)

```bash
sudo apt install scons
```

#### Python 2 (Required Libraries)

```bash
sudo apt install python-dev
```

#### Protobuf (Used for trace playback and generation)

```bash
sudo apt-get install libprotobuf-dev python-protobuf protobuf-compiler libgoogle-perftools-dev
```

### Recommended

#### Visualization

gem5 offers the ability to create a system configuration diagram. In order to be able to create these when using gem5-SALAM, the following packages are required:

- python-pydot
- python-pydot-ng
- graphviz

The following command can be run in Ubuntu to install the required dependencies:

``` bash
sudo apt install python-pydot python-pydot-ng graphviz
```

#### ARM GCC Cross Compiler

```bash
sudo apt-get install gcc-arm-none-eabi
```

## LLVM/Clang Setup 

To build LLVM, you need to have CMake installed. If you don't already have it installed, you can easily install it from the Ubuntu repositories. 

```bash
sudo apt install cmake
```

First, clone the LLVM source from github.

```bash
git clone https://github.com/llvm/llvm-project.git
```

Since we are going to be using LLVM 3.8.1, we want to checkout the LLVM 3.8.x branch. 

```bash
cd llvm-project/
git checkout release/3.8.x
```

Copy clang into the LLVM tools directory so it is build with it

```bash
cp -r clang llvm/tools
```

Next, make your build directory

```bash
mkdir build
cd build
```

Now that the build directory has been made, we can tell CMake to configure LLVM to be built with the options needed. 

Below are some recommended CMake flags: 

- DCMAKE_INSTALL_PREFIX: Changes your install directory to from /usr/local to a user defined value. This makes uninstalling and changing LLVM versions significantly easier.

- DLLVM_ENABLE_ASSERTIONS: Enables code assertions, which helps with debugging

- BUILD_SHARED_LIBS: Builds each LLVM component as a shared library. Helps reduce disk space used.

```bash
cmake -DCMAKE_INSTALL_PREFIX=<your_install_dir> -DBUILD_SHARED_LIBS=true - DLLVM_ENABLE_ASSERTIONS=true -G "Unix Makefiles" ../llvm
```

Now that the build environment is setup, you can go ahead and run make

```bash
make -j`nproc`
```

After the project is finished compiling, install the binaries to your defined directory. 

```bash
make install
```

Finally, update your alternatives to point to your compiled binaries. A script to do this has been provided. It currently assumes that the path to the binaries is /home/<username>/LLVM-3.8/

```bash
./update-alternatives.sh
```

# Building gem5-SALAM

Once you have successfully installed all of the necessary dependencies, you can go ahead and clone the gem5-SALAM repository to begin working with it.

```bash
git clone https://github.com/TeCSAR-UNCC/gem5-SALAM
```

When building gem5-SALAM, there are multiple different binary types that can be created. Just like in gem5 the options are debug, opt, fast, prof, and perf. 

Below are the bash commands you would use to build the opt or debug binary. 

```bash
scons build/ARM/gem5.opt -j`nproc`
```

```bash
scons build/ARM/gem5.debug -j`nproc`
```

For more information regarding the binary types, and other build information refer to the gem5 build documentation [here](http://learning.gem5.org/book/part1/building.html).

# Using gem5-SALAM

To use gem5-SALAM you need to define the computation model of you accelerator in your language of choice,and compile it to LLVM IR. Any control and dataflow graph optimization (eg. loop unrolling) should be handled by the compiler. You can construct accelerators by associating their LLVM IR with an LLVMInterface and connecting it to the desired CommInterface in the gem5 memory map.

Below are some resources in the gem5-SALAM directory that can be used when getting started:

- Examples for system-level configuration can be found in **configs/common/HWAcc.py**.
- Accelerator benchmarks and examples can be found in the **benchmarks** directory. 
- The **benchmarks/common** directory contains basic drivers and syscalls for baremetal simulation.
- **benchmarks/sys_validation** contains examples for configuring and using gem5-SALAM with different algorithms.

## System Validation Examples

The system validation examples under **benchmarks/sys_validation** are good examples for how you interface with the gem5-SALAM simulation objects.

In order to use the system validation benchmarks, it is required to have the ARM GCC cross-compiler installed. If you didn't already install it when you setup the dependencies, you can install it in Ubuntu by running the below command:

```bash
sudo apt-get install gcc-arm-none-eabi
```

**systemValidation.sh** requires an environment variable named **M5_PATH** to be set. You will want to point it to your gem5-SALAM path as shown below. 

```bash
export M5_PATH=/path/to/gem5-SALAM
```

Next, compile your desired example. 

```bash
cd $M5_PATH/benchmarks/sys_validation/[benchmark]
make
```

Finally, you can run any of the benchmarks you have compiled by running the system validation script.

```bash
./systemValidation.sh -b [benchmark]
```

If you would like to see the gem5-SALAM command created by the shell file you would just need to inspect the **RUN_SCRIPT** variable in the shell file. 

# Resources

## gem5 Documentation

https://www.gem5.org/documentation/

## gem5 Tutorial

The gem5 documentation has a [tutorial for working with gem5](http://learning.gem5.org/book/index.html#) that will help get you started with the basics of creating your own sim objects.

## Building and Integrating Accelerators in gem5-SALAM

We have written a guide on how to create the GEMM system validation example. This will help you get started with creating your own benchmarks and systems. It can be viewed [here](https://github.com/clonetrooper67/gem5-SALAM-README/blob/master/Building_and_Integrating_Accelerators.md).

## SALAM Object Overview

The [SALAM Object Overview](https://github.com/clonetrooper67/gem5-SALAM-README/blob/master/SALAM_Object_Overview.md) covers what various Sim Objects in gem5-SALAM are and their purpose.

## Full-system OS Simulation ##

Please download the latest version of the Linux Kernel for ARM from the [gem5 ARM kernel page](http://gem5.org/ARM_Kernel).
You will also need the [ARM disk images](http://www.gem5.org/dist/current/arm/) for full system simulation.
Devices operate in the physical memory address space.