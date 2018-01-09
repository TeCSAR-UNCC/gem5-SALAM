/*  chardev.h - the header file with the ioctl definitions.
 *
 *  The declarations here have to be in a header file, because
 *  they need to be known both to the kernel module
 *  (in chardev.c) and the process calling ioctl (ioctl.c)
 */

#ifndef SAM_DEV_H
#define SAM_DEV_H

#include <linux/ioctl.h>



/* The major device number. We can't rely on dynamic 
 * registration any more, because ioctls need to know 
 * it. */
#define MAJOR_NUM 100


/* Set the message of the device driver */
#define IOCTL_MMR_WRITE _IOR(MAJOR_NUM, 0, size_t)
/* _IOR means that we're creating an ioctl command 
 * number for passing information from a user process
 * to the kernel module. 
 *
 * The first arguments, MAJOR_NUM, is the major device 
 * number we're using.
 *
 * The second argument is the number of the command 
 * (there could be several with different meanings).
 *
 * The third argument is the type we want to get from 
 * the process to the kernel.
 */

/* Get the message of the device driver */
#define IOCTL_BLOCKING_DEV_ACCESS _IOR(MAJOR_NUM, 1, size_t)
 /* This IOCTL is used for output, to get the message 
  * of the device driver. However, we still need the 
  * buffer to place the message in to be input, 
  * as it is allocated by the process.
  */

#define IOCTL_NONBLOCKING_DEV_ACCESS _IOR(MAJOR_NUM, 2, size_t)

#define IOCTL_DEV_SYNC _IOR(MAJOR_NUM, 3, size_t)

#define IOCTL_POLLING_ACCESS _IOR(MAJOR_NUM, 4, size_t)

/* The name of the device file */
#define DEVICE_FILE_NAME "sam_dev"


#endif

