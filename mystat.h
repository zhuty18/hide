#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define VMRSS_LINE 17
#define VMSIZE_LINE 13
#define PROCESS_ITEM 14

typedef struct {
    unsigned long user;
    unsigned long nice;
    unsigned long system;
    unsigned long idle;
} Total_Cpu_Occupy_t;

typedef struct {
    unsigned int pid;
    unsigned long utime;   // user time
    unsigned long stime;   // kernel time
    unsigned long cutime;  // all user time
    unsigned long cstime;  // all dead time
} Proc_Cpu_Occupy_t;

//获取CPU占用率
double get_proc_cpu(unsigned int pid);

//获取进程占用内存
unsigned int get_proc_mem(unsigned int pid);
