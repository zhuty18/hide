gcc tohide.c -o tohide
./tohide &
pid=$(ps -ef | grep "tohide" | grep -v grep | awk '{print $2}')
sudo mount /dev/sda1 /proc/$pid
echo process $pid is hiden!