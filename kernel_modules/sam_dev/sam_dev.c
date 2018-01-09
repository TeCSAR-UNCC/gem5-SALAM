/*  sam_dev.c - Create an input/output character device
 */

#include <linux/kernel.h>   /* We're doing kernel work */
#include <linux/module.h>   /* Specifically, a module */

/* Deal with CONFIG_MODVERSIONS */
#if CONFIG_MODVERSIONS==1
#define MODVERSIONS
#include <linux/modversions.h>
#endif

/* For character devices */

/* The character device definitions are here */
#include <linux/fs.h>

/* Our own ioctl numbers */
#include "sam_dev.h"

#include <asm/io.h> /* for ioremap(), ioread(), and iowrite() */
#include <linux/ioport.h>
#include <asm/uaccess.h>  /* for get_user and put_user */
#include <linux/interrupt.h> /* add interrupt support */
#include <linux/wait.h> /* wait_event() for blocking I/O */
#include <linux/delay.h>
#include <linux/of_device.h>
#include <linux/platform_device.h>

/* In 2.2.3 /usr/include/linux/version.h includes a
 * macro for this, but 2.0.35 doesn't - so I add it
 * here if necessary. */
#ifndef KERNEL_VERSION
#define KERNEL_VERSION(a,b,c) ((a)*65536+(b)*256+(c))
#endif

#define SUCCESS 0


/* Device Declarations ******************************** */


/* The name for our device, as it will appear in
 * /proc/devices */
#define DEVICE_NAME "sam_dev"
#define DEV_IRQ 320

#define MMR_ADDR  0x2F000000
#define MMR_SIZE  0x00000008

/* Is the device open right now? Used to prevent
 * concurent access into the same device */
static int Device_Open = 0;

/* Address of the I/O device */
static void *addr;

static struct resource *mmr_res;

static int intFlag;

static DECLARE_WAIT_QUEUE_HEAD(devQueue);

static uint32_t mmrconfig;

/* This function is called whenever a process attempts
 * to open the device file */
static int device_open(struct inode *inode,
                       struct file *file)
{
  printk ("device_open(%p)\n", file);

  /* We don't want to talk to two processes at the
   * same time */
  if (Device_Open)
    return -EBUSY;

  /* If this was a process, we would have had to be
   * more careful here, because one process might have
   * checked Device_Open right before the other one
   * tried to increment it. However, we're in the
   * kernel, so we're protected against context switches.
   *
   * This is NOT the right attitude to take, because we
   * might be running on an SMP box, but we'll deal with
   * SMP in a later chapter.
   */

  Device_Open++;

  return SUCCESS;
}


/* This function is called when a process closes the
 * device file. It doesn't have a return value because
 * it cannot fail. Regardless of what else happens, you
 * should always be able to close a device (in 2.0, a 2.2
 * device file could be impossible to close).
 */
static int device_release(struct inode *inode,
                          struct file *file)
{
  printk ("device_release(%p,%p)\n", inode, file);

  /* We're now ready for our next caller */
  Device_Open --;

  return 0;
}

static void mmrWrite(struct file *file, size_t mmrVal)
{
    iowrite32(mmrVal, addr + 4);
}

static void configWrite(struct file *file, size_t val)
{
    iowrite32(val, addr);
}

static uint32_t configRead(struct file *file)
{
    return ioread32(addr);
}

/* This function is called whenever a process tries to
 * do an ioctl on our device file. We get two extra
 * parameters (additional to the inode and file
 * structures, which all device functions get): the number
 * of the ioctl called and the parameter given to the
 * ioctl function.
 *
 * If the ioctl is write or read/write (meaning output
 * is returned to the calling process), the ioctl call
 * returns the output of this function.
 */
int device_ioctl(
    struct file *file,
    unsigned int ioctl_num,/* The number of the ioctl */
    unsigned long ioctl_param) /* The parameter to it */
{
  printk("ioctl() detected with number: %x\n", ioctl_num);
  /* Switch according to the ioctl called */
  switch (ioctl_num) {
    case IOCTL_MMR_WRITE:
      /* Receive a pointer to a message (in user space)
       * and set that to be the device's message. */

      /* Get the parameter given to ioctl by the process */
      printk("Attempting to write to device MMR\n");
      mmrWrite(file, ioctl_param);
      break;

    case IOCTL_BLOCKING_DEV_ACCESS:
      /* Give the current message to the calling
       * process - the parameter we got is a pointer,
       * fill it. */
      printk("Attempting to access the device\n");
      intFlag = 0;
      //mmrRead(file);
      configWrite(file, 0x0001);
      printk("Sleeping until device is finished writing back\n");
      wait_event_interruptible(devQueue, intFlag != 0);
      break;

    case IOCTL_NONBLOCKING_DEV_ACCESS:
      printk("Attempting to access the device\n");
      intFlag = 0;
      configWrite(file, 0x0001);
      //mmrRead(file);
      break;

    case IOCTL_DEV_SYNC:
      printk("Checking to see if device has finished writing back\n");
      wait_event_interruptible(devQueue, intFlag != 0);
      break;

    case IOCTL_POLLING_ACCESS:
      printk("Attempting to access the device\n");
      configWrite(file, 0x0001);
      while(!(ioread32(addr) & 0x80000000)) {
        udelay(10);
      }
      break;
  }

  return SUCCESS;
}

static irqreturn_t devHandler(int irq, void *dev_id)//, struct pt_regs *regs)
{
    intFlag = 1;
    wake_up_interruptible(&devQueue);
    return IRQ_HANDLED;
}

/* Module Declarations *************************** */


/* This structure will hold the functions to be called
 * when a process does something to the device we
 * created. Since a pointer to this structure is kept in
 * the devices table, it can't be local to
 * init_module. NULL is for unimplemented functions. */
struct file_operations Fops = {
  .unlocked_ioctl = device_ioctl,   /* ioctl */
  .open = device_open,
  .release = device_release  /* a.k.a. close */
};

/* Initialize the module - Register the character device */
int init_module()
{
  int ret_val;

  /* Register the character device (atleast try) */
  ret_val = register_chrdev(MAJOR_NUM,
                                 DEVICE_NAME,
                                 &Fops);

  /* Negative values signify an error */
  if (ret_val < 0) {
    printk ("%s failed with %d\n",
            "Sorry, registering the character device ",
            ret_val);
    return ret_val;
  }

  printk ("%s The major device number is %d.\n",
          "Registeration is a success",
          MAJOR_NUM);
  printk ("If you want to talk to the device driver,\n");
  printk ("you'll have to create a device file. \n");
  printk ("We suggest you use:\n");
  printk ("mknod %s c %d 0\n", DEVICE_FILE_NAME,
          MAJOR_NUM);
  printk ("The device file name is important, because\n");
  printk ("the ioctl program assumes that's the\n");
  printk ("file you'll use.\n");

  printk ("Requesting memory region for device MMR\n");
  mmr_res = request_mem_region(MMR_ADDR, MMR_SIZE, DEVICE_FILE_NAME);

  if(mmr_res && ((mmr_res->start != MMR_ADDR) && (mmr_res->end != (MMR_ADDR + MMR_SIZE))))
  {
    printk ("Could not map memory mapped register for device!\n");
  } else {
    printk ("Registering device address: 0x%08x of size: %d\n", MMR_ADDR, MMR_SIZE);
    addr = ioremap_nocache(MMR_ADDR, MMR_SIZE);
  }
  printk("Attempting to reserve IRQ number: %d\n", DEV_IRQ);
  if(request_irq(DEV_IRQ, devHandler, 0, DEVICE_NAME, NULL)) {
    printk("Device could not be assigned IRQ %i\n", DEV_IRQ);
  }
  intFlag = 0;

  return 0;
}


/* Cleanup - unregister the appropriate file from /proc */
void cleanup_module()
{
  unregister_chrdev(MAJOR_NUM, DEVICE_NAME);

  if((mmr_res->start == MMR_ADDR) && (mmr_res->end == (MMR_ADDR + MMR_SIZE)))
  {
    iounmap(addr);
    release_mem_region(MMR_ADDR, MMR_SIZE);
  }
  free_irq(DEV_IRQ, NULL);
}




