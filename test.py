import os, random, time

TEST_TIME = 10000
right = 0

f = open('res.txt', 'w')
h = 0
for i in range(TEST_TIME):
    p = os.popen("./main")
    output = p.read()
    p.close()
    print(output)
    output = output.split("\n")
    output.remove("")
    t = output[-1].split()
    if len(output) == 2:
        right += 1
    else:
        for j in range(1, len(output) - 1):
            f.write(output[j] + "\n")
    s = "time:" + t[0] + " cpu:" + t[1] + " mem:" + t[2]
    h += float(t[0])
    f.write(s + "\n\n")
    print(i / TEST_TIME)

print(right)
print(h / TEST_TIME)
f.close()