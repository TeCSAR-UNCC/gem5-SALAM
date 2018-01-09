gem5 ARM Bare-Metal Example
===========================

This git repository shows a simple example for an ARM Bare-Metal implementation
with gem5. It can be used as a starting point for bare metal projects with
this simulator. For more information on gem5 please look at:
[gem5.org](http://www.gem5.org/Main_Page).

To install the toolchain on Mac OSX you can yous macports:

``` bash
    sudo port install arm-none-eabi-gcc
    sudo port install arm-none-eabi-gdb
```
or Homebrew:
``` bash
    brew tap PX4/homebrew-px4
    brew update
    brew install gcc-arm-none-eabi
```

on Linux based systems (e.g. Debian):
``` bash
    apt-get install gcc-arm-none-eabi
```

To compile the example software set a proper path to you bare metal toolchain
in the Makefile.

``` bash
    make
```

To run the software in gem5 do the following steps:

``` bash
    export M5_PATH="/path/to/gem5.bare-metal/"
```   
For the simple example:
``` bash
    ./build/ARM/gem5.opt \
    ./configs/example/fs.py \
    --bare-metal \
    --disk-image=/path/to/gem5.bare-metal/common/fake.iso \
    --kernel=/path/to/gem5.bare-metal/simple/main.elf \
    --machine-type=RealView_PBX \
    --dtb-filename=none \
    --mem-size=256MB
```

For the interrupt example:
``` bash
    ./build/ARM/gem5.opt \
    ./configs/example/fs.py \
    --bare-metal \
    --disk-image=/path/to/gem5.bare-metal/common/fake.iso \
    --kernel=/path/to/gem5.bare-metal/interrupt/main.elf \
    --machine-type=RealView_PBX \
    --dtb-filename=none \
    --mem-size=256MB
```

Licence
=======
```
Copyright (c) 2015, University of Kaiserslautern
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are
met:

1. Redistributions of source code must retain the above copyright notice,
   this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its
   contributors may be used to endorse or promote products derived from
   this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER
OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

Authors: Matthias Jung
```
