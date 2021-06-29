./tohide
pid=$(ps -ef | grep "tohide" | grep -v grep | awk '{print $2}')
echo $pid
sudo mount /dev/sda1 /proc/$pid
ech