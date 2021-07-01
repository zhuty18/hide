import os
# 比较内核态进程列表与ps命令，找到隐藏进程
# 可以解决内核态劫持的情况

os.system("sudo bash scan.sh")
p = []
with open("1.log", 'r', encoding="utf8") as f:
    k = False
    for line in f:
        if line.__contains__("进程扫描模块开始运行"):
            k = True
        elif line.__contains__("进程扫描模块卸载成功"):
            pass
        elif k:
            p.append(line.split()[-2])


cmd = "ps -e"
process = os.popen(cmd)
output = process.read()
process.close()
output = output.split()
# 第一行是PID YYD等列名，最后两行是这里使用的sh和ps命令
t = int(len(output)/4)
k = []
for i in range(1, t-2):
    k.append(output[i*4])

# 后四行是装载模块读取进程列表时的sh, sudo, bash, insmod命令
for i in range(0, len(p)-4):
    if not(p[i] in k):
        print(p[i])
