#include "hwacc/register_bank.hh"

#include <sys/mman.h>
#include <sys/types.h>
#include <sys/user.h>

#include <stdio.h>
#include <stdlib.h>
#include <iomanip>

#include "base/trace.hh"
#include "mem/packet.hh"
#include "mem/packet_access.hh"
#include "sim/system.hh"
#include "debug/Drain.hh"

using namespace std;

#include "debug/MemoryAccess.hh"

RegisterBank::RegisterBank(const RegisterBankParams &p) :
    AbstractMemory(p),
    deltaTime(p.delta_time),
    deltaEvent([this]{ delta(); }, name()) {
        // Setup of the delta memory container
        int shm_fd = -1;
        int map_flags = MAP_ANON | MAP_PRIVATE;

        deltaAddr = (uint8_t *)mmap(NULL, range.size(),
                                    PROT_READ | PROT_WRITE,
                                    map_flags, shm_fd, 0);
        if (deltaAddr == (uint8_t *)MAP_FAILED) {
            perror("mmap");
            fatal("Could not mmap %d bytes for range %s!\n", range.size(),
                range.to_string());
        }
    }