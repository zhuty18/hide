# Linux下隐藏进程检测工具

已测试环境

Ubuntu18.04(5.4.0-80-generic)

Ubuntu18.04(4.15.0-38-generic)

RHEL8.4(4.18.0-305.10.2.el8_4.x86_64)

# 使用

1. 安装操作系统下的`linux-headers-$(uname -r)`

2. 编译`make`

3. 运行`./main`检测隐藏进程

# 测试本工具

编译时使用`make debug`，运行`python3 test.py`
