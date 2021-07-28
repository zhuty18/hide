#include "mystat.h"

//获取第N项开始的指针
const char* get_items(const char* buffer, unsigned int item) {
    const char* p = buffer;

    int len = strlen(buffer);
    int count = 0;

    for (int i = 0; i < len; i++) {
        if (' ' == *p) {
            count++;
            if (count == item - 1) {
                p++;
                break;
            }
        }
        p++;
    }

    return p;
}

//获取总的CPU时间
unsigned long get_cpu_total_occupy() {
    FILE* fd;
    char buff[1024] = {0};
    Total_Cpu_Occupy_t t;

    fd = fopen("/proc/stat", "r");
    if (NULL == fd) {
        return 0;
    }

    fgets(buff, sizeof(buff), fd);
    char name[64] = {0};
    sscanf(buff, "%s %ld %ld %ld %ld", name, &t.user, &t.nice, &t.system,
           &t.idle);
    fclose(fd);

    return (t.user + t.nice + t.system + t.idle);
}

//获取进程的CPU时间
unsigned long get_cpu_proc_occupy(unsigned int pid) {
    char file_name[64] = {0};
    Proc_Cpu_Occupy_t t;
    FILE* fd;
    char line_buff[1024] = {0};
    sprintf(file_name, "/proc/%d/stat", pid);

    fd = fopen(file_name, "r");
    if (NULL == fd) {
        return 0;
    }

    fgets(line_buff, sizeof(line_buff), fd);

    sscanf(line_buff, "%u", &t.pid);
    const char* q = get_items(line_buff, PROCESS_ITEM);
    sscanf(q, "%ld %ld %ld %ld", &t.utime, &t.stime, &t.cutime, &t.cstime);
    fclose(fd);

    return (t.utime + t.stime + t.cutime + t.cstime);
}


//获取进程占用内存
unsigned int get_proc_mem(unsigned int pid) {
    char file_name[64] = {0};
    FILE* fd;
    char line_buff[512] = {0};
    sprintf(file_name, "/proc/%d/status", pid);

    fd = fopen(file_name, "r");
    if (NULL == fd) {
        return 0;
    }

    char name[64];
    int vmrss;
    for (int i = 0; i < VMRSS_LINE - 1; i++) {
        fgets(line_buff, sizeof(line_buff), fd);
    }

    fgets(line_buff, sizeof(line_buff), fd);
    sscanf(line_buff, "%s %d", name, &vmrss);
    fclose(fd);

    return vmrss;
}
