import os
# 比较/proc与ps命令，找到隐藏进程
# 可以找到所有用户隐藏的进程
# 但没能解决内核态劫持的情况

os.system("sudo bash scan.sh")
p=[]
with open("1.log",'r',encoding="utf8") as f:
    k=False
    for line in f:
        if line.__contains__("进程扫描模块开始运行"):
            k=True
        elif line.__contains__("进程扫描模块卸载成功"):
            pass
        elif k:
            p.append(line.split()[-2])


cmd = "ps -e"
process = os.popen(cmd)
output = process.read()
process.close()
output = output.split()
t = int(len(output)/4)
k = []
for i in range(1, t-2):
    k.append(output[i*4])

for i in range(0,len(p)-5):
    if not(p[i] in k):
        print(p[i])
