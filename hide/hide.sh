make clean
make
insmod hide.ko
# rmmod hide
dmesg -c >1.log
# lsmod | grep "scan"