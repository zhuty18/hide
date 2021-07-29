#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "myqueue.h"
#ifdef SUPER
#include "mystat.h"
#endif
#define BUF_SIZE 1024
char buf[BUF_SIZE];

void AddProc(que* q, int start) {
    int pid;
    sscanf(&buf[start], "%d", &pid);
    int l = 0;
    int len = strlen(buf);
    int i;
    for (i = len - 2; i >= 0; i--) {
        if (buf[i] == ' ') {
            l = len - i - 1;
            break;
        }
    }
    i += 1;
    char* name = (char*)malloc(sizeof(char) * l);
    for (int j = 0; j < l; j++) {
        if (j == l - 1) {
            name[j] = '\0';
        } else {
            name[j] = buf[i + j];
        }
    }
    InsertQueue(q, pid, name);
}

void CleanEnd(que* q, char* target) {
    node* n = q->tail;
    if (strcmp(n->name, target) == 0) {
        DeleteLast(q, 1);
        return;
    } else {
        while (n = n->last) {
            if (strcmp(n->name, target) == 0) {
                n->last->next = n->next;
                n->next->last = n->last;
                free(n->name);
                free(n);
                return;
            }
        }
    }
    // RedHat操作系统内不会有sh进程
    // printf("clean fail %s\n",target);
}

int CheckProc(int pid, char* name) {
    int rtn = 0;
    char cmd[64];
    int l = 13;
    strcpy(cmd, "ps -e | grep \"");
    strcpy(&cmd[l + 1], name);
    l += strlen(name);
    cmd[l + 1] = '\"';
    cmd[l + 2] = '\0';
    FILE* p_file = popen(cmd, "r");
    while (fgets(buf, BUF_SIZE, p_file) != NULL) {
        int p;
        sscanf(buf, "%d", &p);
        if (p == pid) {
            rtn = 1;
            break;
        }
    }
    pclose(p_file);
    return rtn;
}

int main() {
#ifdef SUPER
    unsigned long totalcputime1, totalcputime2;
    unsigned long procputime1, procputime2;
    totalcputime1 = get_cpu_total_occupy();
    procputime1 = get_cpu_proc_occupy(getpid());
#endif
    clock_t start = clock();

    que* que_usr;
    que_usr = InitQueue();
    FILE* p_file = popen("ps -e", "r");
    int ok = 0;
    while (fgets(buf, BUF_SIZE, p_file) != NULL) {
        if (ok == 0) {
            ok = 1;
        } else {
            AddProc(que_usr, 0);
        }
    }
    pclose(p_file);
    // 删除最后两个sh和ps进程
    CleanEnd(que_usr, "ps");
    CleanEnd(que_usr, "sh");

    que* que_knl;
    que_knl = InitQueue();
    system("sudo insmod scan.ko");
    system("sudo rmmod scan");
    p_file = popen("sudo dmesg -c", "r");
    ok = 0;
    while (fgets(buf, BUF_SIZE, p_file) != NULL) {
        if (ok == 0) {
            for (int i = 0; buf[i] != '\0'; i++) {
                if (buf[i] == ']') {
                    if (strcmp(&buf[i + 2], "start scaning!!!\n") == 0) {
                        ok = 1;
                    }
                    break;
                }
            }
        } else {
            for (int i = 0; buf[i] != '\0'; i++) {
                if (buf[i] == ']') {
                    if (strcmp(&buf[i + 2], "scaning completed!!!\n") == 0) {
                        ok = 0;
                        break;
                    } else {
                        AddProc(que_knl, i + 1);
                        break;
                    }
                }
            }
        }
    }
    pclose(p_file);
    CleanEnd(que_knl, "insmod");
    CleanEnd(que_knl, "sudo");
    CleanEnd(que_knl, "sh");

    que* hide;
    hide = InitQueue();
    node* n = que_knl->head;
    node* u = que_usr->head;
    while (n) {
        int h = 0;
        if (u) {
            h = u->pid;
        }
        if (h == n->pid) {
            //内核扫描进程在用户态获取的结果中
            n = n->next;
            u = u->next;
        } else if ((h > n->pid) || (u == NULL)) {
            //用户态获取的进程晚于内核扫描进程建立，或用户态进程列表已经遍历完成
            if (CheckProc(n->pid, n->name) == 0) {
                //检查该进程是否存活
                InsertQueue(hide, n->pid, n->name);
                n = n->next;
            } else {
                n = n->next;
            }
        } else if (h < n->pid) {
            //用户态获取的进程先于内核扫描进程建立，即该进程已死亡
            u = u->next;
        }
    }
#ifdef SUPER
    clock_t end = clock();
#endif
    PrintQueue(hide);
    sleep(1);
#ifdef SUPER
    double time = (double)(end - start) / CLOCKS_PER_SEC;
    int pid = getpid();
    totalcputime2 = get_cpu_total_occupy();
    procputime2 = get_cpu_proc_occupy(pid);
    double cpu = 0.0;
    if (0 != totalcputime2 - totalcputime1) {
        cpu = 100.0 * (procputime2 - procputime1) /
              (totalcputime2 - totalcputime1);
    }
    // printf("%ld,%ld\n",procputime2-procputime1,totalcputime2-totalcputime1);
    int mem = get_proc_mem(pid);
    printf("%f %.16f %d\n", time, cpu, mem);
#endif

    //释放内存
    DeleteQueue(hide, 0);
    DeleteQueue(que_usr, 1);
    DeleteQueue(que_knl, 1);
    return 0;
}
