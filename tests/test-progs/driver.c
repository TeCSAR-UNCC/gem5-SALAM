/*  driver.c - the process to use ioctl's to control the kernel module
 *
 *  Until now we could have used cat for input and output.  But now
 *  we need to do ioctl's, which require writing our own process. 
 */

/* device specifics, such as ioctl numbers and the 
 * major device file. */
#include "sam_dev.h"    


#include <fcntl.h>      /* open */ 
#include <unistd.h>     /* exit */
#include <sys/ioctl.h>  /* ioctl */
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <err.h>

#define MAP_SIZE 4096UL
#define MAP_MASK (MAP_SIZE - 1)
#define DATA_ADDR 0xFF000000

/* Functions for the ioctl calls */
void
ioctl_mmr_write(int file_desc, size_t address)
{
  int ret_val;

  ret_val = ioctl(file_desc, IOCTL_MMR_WRITE, address);

  if (ret_val < 0) {
    printf ("ioctl_dev_write failed:%d\n", ret_val);
    exit(-1);
  }
}


void
ioctl_blocking_access(int file_desc)
{
  int ret_val;
  size_t address = 0; 

  printf("Attempting blocking device access\n");
  ret_val = ioctl(file_desc, IOCTL_BLOCKING_DEV_ACCESS, address);

  if (ret_val < 0) {
    printf ("ioctl_blocking_access failed:%d\n", ret_val);
    exit(-1);
  }
}

void
ioctl_nonblocking_access(int file_desc)
{
  int ret_val;
  size_t address = 0;
  
  printf("Attempting non-blocking device access\n");
  ret_val = ioctl(file_desc, IOCTL_BLOCKING_DEV_ACCESS, address);

  if (ret_val < 0) {
    printf ("ioctl_nonblocking_access failed:%d\n", ret_val);
    exit(-1);
  }
}

void
ioctl_dev_sync(int file_desc)
{
  int ret_val;
  size_t address = 0;
  
  printf("Attempting device sync\n");
  ret_val = ioctl(file_desc, IOCTL_BLOCKING_DEV_ACCESS, address);

  if (ret_val < 0) {
    printf ("ioctl_dev_sync failed:%d\n", ret_val);
    exit(-1);
  }
}

void
ioctl_polling_access(int file_desc)
{
  int ret_val;
  size_t address = 0;
  
  printf("Attempting polling device access\n");
  ret_val = ioctl(file_desc, IOCTL_POLLING_ACCESS, address);

  if (ret_val < 0) {
    printf ("ioctl_polling_access failed:%d\n", ret_val);
    exit(-1);
  }
}

/* Main - Call the ioctl functions */
int
main()
{
  int file_desc, ret_val;
  
  off_t          data_base = DATA_ADDR;
  void          *mapped_base;
  void          *mapped_data_base;
  int           *pu_data;
  int  volatile  data;
  int            memfd;
  size_t         ldev = (8 * 1);
  unsigned long  mask = (8 * 1)-1;

  file_desc = open(DEVICE_FILE_NAME, 0);
  if (file_desc < 0) {
    printf ("Can't open device file: %s\n", 
            DEVICE_FILE_NAME);
    exit(-1);
  }

  printf("Device File Opened\n");
  
  memfd = open("/dev/mem", O_RDWR | O_SYNC);
  
  mapped_base = mmap(0, MAP_SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, memfd, data_base & ~MAP_MASK);
  if (mapped_base == MAP_FAILED)
      errx(1, "mmap failure");
  mapped_data_base = mapped_base + (data_base & MAP_MASK);
  pu_data = mapped_data_base;
  
  ioctl_mmr_write(file_desc, 0xff000000);
  
  data = 2048;
  
  *pu_data = data;
  
  printf("Data: %d\n", data);
  
  /*ioctl_blocking_access(file_desc);
  
  printf("Data: %d\n", *pu_data);
  
  ioctl_nonblocking_access(file_desc);
  
  printf("I can do things in the mean time\n");
  
  ioctl_dev_sync(file_desc);*/
  
  ioctl_polling_access(file_desc);
  
  printf("Data: %d\n", *pu_data);
  
  munmap(mapped_base, ldev);
  close(memfd);
  //close(file_desc);
  //printf("Device File Closed\n");
}
