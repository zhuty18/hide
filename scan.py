import os
# 在用户态比较/proc与ps命令，找到隐藏进程
path = "/proc"
l = os.listdir(path)
p = []
for i in l:
    try:
        int(i)
    except ValueError:
        pass
    else:
        p.append(i)

cmd = "ps -e"
process = os.popen(cmd)
output = process.read()
process.close()
output = output.split()
t = int(len(output)/4)
k = []
for i in range(1, t-2):
    k.append(output[i*4])

for i in p:
    if not(i in k):
        print(i)
