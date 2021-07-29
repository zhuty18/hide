import os, random, time

TEST_TIME = 10000
right = 0

f = open('res.txt', 'w')
cost_time = 0
cost_cpu = 0
cost_mem = 0
for i in range(TEST_TIME):
    # if i !=0:
    #     time.sleep(random.random())
    p = os.popen("./main")
    output = p.read()
    p.close()
    # print(output)
    output = output.split("\n")
    output.remove("")
    t = output[-1].split()
    if len(output) == 2:
        right += 1
    else:
        f.write(str(i) + "\n")
        for j in range(1, len(output) - 1):
            f.write(output[j] + "\n")
        f.write("\n")
    s = "time:" + t[0] + " cpu:" + t[1] + " mem:" + t[2]
    cost_time += float(t[0])
    cost_cpu += float(t[1])
    cost_mem += int(t[2])
    print("\r测试已完成{}%".format(100 * (i + 1) / TEST_TIME), end="\r")

print()
print("测试数：" + str(TEST_TIME))
print("错误率" + str(100 * (TEST_TIME - right) / TEST_TIME) + "%")
print("平均耗时：" + format(cost_time / TEST_TIME, ".6f") + "s")
print("平均占用CPU：" + format(cost_cpu / TEST_TIME, ".2f") + "%")
print("平均占用内存：" + str(cost_mem / TEST_TIME) + "kB")
f.close()
