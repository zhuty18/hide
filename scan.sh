make clean
make
touch /tmp/sessions.txt
sudo insmod scan.ko
sudo rmmod scan
sudo dmesg -c >1.log
# lsmod | grep "scan"