#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/errno.h>
#include <linux/ioport.h>
#include <asm/io.h>

#define BASE 0xe000
#define SIZE 0x08

int init_module(void)
{
  int t1;
  if ( ! request_region(BASE, SIZE, "ioacc") ) {
    printk( KERN_INFO "unable to get io port at 0x%8X\n", BASE );
    return -ENODEV;
  }

  /* a little test */
  t1 = inl(BASE);
  printk( KERN_INFO "read %d\n", t1 );

  outl(0, BASE);
  t1 = inl(BASE);
  printk( KERN_INFO "read %d\n", t1 );
  return 0;
}

void cleanup_module(void)
{
  release_region(BASE, SIZE);
}
