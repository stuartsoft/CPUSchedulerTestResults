#!/bin/bash

# place the linux source folder in your home directory
# place this script in your home directory next to the linux folder

#begin building
cd ~/linux-4.2.6
make mrproper
zcat /proc/config.gz > .config

#setup build using menu
make menuconfig
#build using 4 concurrent threads
make -j4

#build should now be done!
