#include <stdio.h>
#include <stdint.h>
#include <sys/mman.h>
#include <sys/fcntl.h>
#include <err.h>
#include <stdlib.h>

#define MAP_SIZE 4096UL
#define MAP_MASK (MAP_SIZE - 1)

int main(void)
{
    off_t          dev_base = 0x100000000;
    size_t         ldev = (8 * 1);
    unsigned long  mask = (8 * 1)-1;
    int           *pu;
    void          *mapped_base;
    void          *mapped_dev_base;
    int  volatile *pcid;
    int  volatile  cid;
    int            memfd;
    
    off_t          data_base = 0xFF000000;
    void          *mapped_base_2;
    void          *mapped_data_base;
    int           *pu_data;
    int  volatile *pcid_data;
    int  volatile  cid_data;
    int  volatile  data;

    memfd = open("/dev/mem", O_RDWR | O_SYNC);
    
    mapped_base = mmap(0, MAP_SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, memfd, dev_base & ~MAP_MASK);
    if (mapped_base == MAP_FAILED)
        errx(1, "mmap failure");
    mapped_dev_base = mapped_base + (dev_base & MAP_MASK);
    pu = mapped_dev_base;
    
    mapped_base_2 = mmap(0, MAP_SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, memfd, data_base & ~MAP_MASK);
    if (mapped_base_2 == MAP_FAILED)
        errx(1, "mmap failure");
    mapped_data_base = mapped_base_2 + (data_base & MAP_MASK);
    pu_data = mapped_data_base;

    printf("pu    = %08p\n", pu);
    printf("pu_data = %08p\n", pu_data);
    cid = 0xFF000000;
    data = 0x88888888;
    *pu_data = data;
    printf("Data: 0x%08x\n", *pu_data);
    *pu = cid;
    cid_data = *pu;
    
    usleep(1000);
    
    printf("Modified data?: 0x%08x\n", *pu_data);
        
    munmap(mapped_base, ldev);
    munmap(mapped_base_2, ldev);
    close(memfd);

    return (EXIT_SUCCESS);
}
