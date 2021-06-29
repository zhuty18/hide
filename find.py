import os
import re

cmd="cat /proc/mounts"

process = os.popen(cmd)
output = process.read()
process.close()
# print(output)

output=output.split()
l=len(output)
l=int(l/6)
t="/proc/\d{1,6}"
hashide=False
hide=[]
for i in range(0,l):
    an=re.search(t,output[i*6+1])
    if an:
        hide.append((output[i*6],output[i*6+1]))
        hashide=True

if hashide:
    os.system("ps")
    print("有隐藏的进程！")
    print(hide)
    k=input("输入Y显示进程！")
    if k=='y' or k=='Y':
        for i in range(0,len(hide)):
            print("sudo umount "+hide[i][0]+" "+hide[i][1])
            os.system("sudo umount "+hide[i][0]+" "+hide[i][1])
        os.system("ps")
else:
    print("当前没有被隐藏的进程！")