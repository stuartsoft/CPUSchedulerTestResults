#!/bin/bash

# place the linux source folder in your home directory
# place this script in your home directory next to the linux folder

#This script only works if started under root!

if ! [ $(id -u) = 0 ]; then
	echo "Please run as root"
	exit 1
fi

cd /home/stuart/linux-4.2.6

#remove old kernel
rm /boot/System.map-4.2.6-STU
rm /boot/initramfs-4.2.6-STU.img
rm /boot/vmlinuz-4.2.6-STU
#grub-mkconfig -o /boot/grub/grub.cfg

make modules_install
#copy kernel and system map into /boot
cp -v arch/x86/boot/bzImage /boot/vmlinuz-4.2.6-STU
mkinitcpio -k 4.2.6-STU -c /etc/mkinitcpio.conf -g /boot/initramfs-4.2.6-STU.img
cp System.map /boot/System.map-4.2.6-STU
ln -sf /boot/System.map-4.2.6-STU /boot/System.map

#update grub with boot entry for new kernel
grub-mkconfig -o /boot/grub/grub.cfg

