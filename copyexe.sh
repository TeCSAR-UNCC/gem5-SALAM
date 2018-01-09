#!/bin/bash
mount -o loop,offset=32256 $M5_PATH/disks/linux-x86.img ./mnt
cd $M5_PATH/tests/test-progs
echo "Copying file: test to img"
cp test $M5_PATH/mnt/home
sleep 1
umount $M5_PATH/mnt
