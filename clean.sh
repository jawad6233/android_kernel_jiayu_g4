#!/bin/bash
#Stop script if something is broken
set -e

#Export target product
export TARGET_KERNEL_PRODUCT=`cat DEVICE_TREE`

#Kernel part
make mrproper
