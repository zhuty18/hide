# plan A

改变ps命令

# plan B

劫持Hook系统调用

# plan C

找到要隐藏进程的PID（举例为666）```ps -ef```

找到磁盘编号（我的为/dev/nvme0n1p8）```df```

利用mount命令，通过挂载隐藏进程```sudo mount /dev/nvme0n1p8 /proc/666```

显示
查看是否有被隐藏的进程```cat /proc/mounts```

重新显示进程```sudo umount /dev/nvme0n1p8 /proc/666```

# plan D

ps原理：
（1）调用openat系统函数获取/proc目录的文件句柄
（2）调用系统函数getdents递归获取/proc目录下所有文件信息（包括子目录）
（3）调用open函数打开/proc/进程pid/stat和/proc/进程pid/status,以及/proc/进程pid/cmdline文件开始获取进程信息
（4）然后打印输出

实现：
劫持getdents 等系统调用函数或libc中的readdir 函数等系统调用函数一般来说有3个途径
I、修改内核调用，比如getdents 的源码
II、修改libc库中readdir 函数的源码
III、利用环境变量LD_PRELOAD 或者配置ld.so.preload文件 以使的恶意的动态库先于系统标准库加载，以达到架空系统标准库中相关函数的目的，最终实现对特定进程的隐藏
备注：getdents函数对应的系统调用是sys_getdents()，readdir函数对应的系统调用是proc_pid_readdir()，

防护
I、检查LD_PRELOAD环境变量是否有异常
II、检查ld.so.preload 等配置文件是否有异常
III、自己写个python小工具，直接读取/proc中的内容，对于ps等工具的结果，对不上，则存在被劫持可能
IV、使用sysdig（有开源版，可以监控ps等的调用过程，观察是否有恶意动态库被加载。strace有类似功能）或者prochunter（google 上search）
sysdig proc.name=ps

# plan E

劫持VFS接口层

proc文件系统也是VFS抽象类的一个具体子类，与其它文件系统不同的是，它是存在与内存中的虚拟文件系统，以文件系统的方式为访问内核数据的操作提供接口。在执行ps或ls命令时，proc文件系统会调用readdir函数依次遍历proc目录下的数字目录，而readdir又会调用filldir函数返回改数字目录对应进程的相关信息，因此，只要在filldir函数中增加适当的过滤，就可以实现对进程的隐藏了。上面两个方法要么修改内核，要么新增加内核模块（LKM）。修改内核一般需要重启，增加内核模块不需要，所以增加内核模块更隐蔽一些。

检查

查看内核编译时间```uname -a```
查看内核模块```lsmod```