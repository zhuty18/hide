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
            t = line.split()
            p.append((t[-2], t[-1]))

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

hiding = []


def legal_kworker(n):
    for i in range(0, len(k)):
        if k[i][0] == n[0] and k[i][1].startswith(n[1]):
            return True
    return False


# 后四行是装载模块读取进程列表时的sh, sudo, bash, insmod命令
filter = ['insmod', 'bash', 'sudo', 'sh']
fi = {}
for i in range(-1, -len(p), -1):
    if p[i][1] in filter:
        fi[i + len(p)] = True
        filter.remove(p[i][1])
        if len(filter) == 0:
            break

for i in range(0, len(p)):
    if not (((p[i] in k) or fi.get(i, False)) or legal_kworker(p[i])):
        print(p[i])
        hiding.append(p[i])

if hiding == []:
    print("当前没有隐藏的进程！")
else:
    k = input("输入Y终止进程！")
    if k == 'y' or k == 'Y':
        for i in range(0, len(hiding)):
            os.system("sudo kill " + hiding[i][0])
