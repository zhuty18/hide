make clean
make
insmod scan.ko
rmmod scan
dmesg -c >1.log
# lsmod | grep "scan"