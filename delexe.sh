#!/bin/bash
mount -o loop,offset=32256 $M5_PATH/disks/linux-x86.img $M5_PATH/mnt
cd $M5_PATH/mnt/home
rm -r test
cd $M5_PATH
sleep 1
umount $M5_PATH/mnt
