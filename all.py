import os, time
# 比较内核态进程列表与ps命令，找到隐藏进程
# 可以解决内核态劫持的情况
cost = time.time()
# 读取ps命令输出
cmd = "ps -e"
process = os.popen(cmd)
output = process.read()
process.close()
output = output.split()
# 第一行是PID YYD等列名，最后两行是这里使用的sh和ps命令
t = int(len(output) / 4)
k = []
for i in range(1, t - 2):
    k.append((output[i * 4], output[i * 4 + 3]))

# 在内核态遍历进程
os.system("sudo insmod scan.ko")
os.system("sudo rmmod scan")
cmd = "dmesg -c"
process = os.popen(cmd)
output = process.read()
process.close()
f = output.split('\n')
f.remove('')
p = []
h = False
for line in f:
    if line.__contains__("start scaning!!!"):
        h = True
    elif line.__contains__("scaning completed!!!"):
        pass
    elif h:
        t = line.split()
        p.append((t[-2], t[-1]))
# 末尾有装载模块读取进程列表时的sh, sudo, insmod命令，需要过滤掉
filter = ['insmod', 'sudo', 'sh']
fi = {}
for i in range(-1, -len(p), -1):
    if p[i][1] in filter:
        fi[i + len(p)] = True
        filter.remove(p[i][1])
        if len(filter) == 0:
            break

# 检测隐藏进程
hiding = []


# 因为内核态和ps命令的kworker进程输出的进程名不同，进行核查
def legal_kworker(n):
    for i in range(0, len(k)):
        if k[i][0] == n[0] and k[i][1].startswith(n[1]):
            return True
    return False


for i in range(0, len(p)):
    if not (((p[i] in k) or fi.get(i, False)) or legal_kworker(p[i])):
        print(p[i])
        hiding.append(p[i])

# 检查是否为误差
for i in hiding:
    cmd = "ps -e | grep \"" + i[1] + "\""
    process = os.popen(cmd)
    output = process.read()
    process.close()
    output = output.split()
    for h in output:
        h = h.split()
        k = (h[0], h[3])
        if k == i:
            hiding.remove(i)

print(time.time() - cost)
# 输出结果
if hiding == []:
    print("当前没有隐藏的进程！")
else:
    k = input("输入Y终止进程！")
    if k == 'y' or k == 'Y':
        for i in range(0, len(hiding)):
            os.system("sudo kill " + hiding[i][0])
