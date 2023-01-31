#!/bin/bash

sudo apt-get install gcc-multilib g++-multilib

sudo chmod u+x cactiStats.sh
sudo chmod u+x cactiSweep.sh
sudo chmod u+x clearStats.sh
sudo chmod u+x getcactiResults.sh

cd ../ext/mcpat/cacti

make clean
make all


echo " "
echo "Testing Install"
echo " "

sleep 1
./cacti -infile cache.cfg

echo "=================================================================="
echo "Usage: ./cactiSweep config.yml bench_name config_name"
echo "bench_name and config_name are only for grouping"
#should make them optional at some point
