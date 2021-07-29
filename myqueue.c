#include "myqueue.h"
//初始化队列
que* InitQueue() {
    que* q = (que*)malloc(sizeof(que));
    q->head = q->tail = NULL;
    return q;
}

//入队
void InsertQueue(que* q, int pid, char* name) {
    node* n = (node*)malloc(sizeof(node));
    n->pid = pid;
    n->name = name;
    n->next = NULL;  //尾插法，插入元素指向空
    n->last = q->tail;
    if (q->tail == NULL) {
        q->head = n;
        q->tail = n;
    } else {
        q->tail->next = n;  //让n成为当前的尾部节点下一节点
        q->tail = n;        //尾部指针指向n
    }
}

//出队(删除队首元素)
void DeleteFirst(que* q, int clean_name) {
    node* n = q->head;
    if (q->head == NULL)  //判断队列是否为空
        return;
    if (q->head == q->tail)  //是否只有一个元素
    {
        q->head = NULL;
        q->tail = NULL;
    } else {
        q->head = q->head->next;
        q->head->last = NULL;
    }
    if (clean_name) {
        free(n->name);
    }
    free(n);
}

//出队(删除队尾元素)
void DeleteLast(que* q, int clean_name) {
    node* n = q->tail;
    if (q->tail == NULL)  //判断队列是否为空
        return;
    if (q->head == q->tail)  //是否只有一个元素
    {
        q->head = NULL;
        q->tail = NULL;
    } else {
        q->tail = q->tail->last;
        q->tail->next = NULL;
    }
    if (clean_name) {
        free(n->name);
    }
    free(n);
}
char hidden_user[] = "the user is hidden!";
info* GetInfo(pid_t pid) {
    info* rtn = (info*)malloc(sizeof(info));
    uid_t uid;

    char file_name[64] = {0};
    FILE* fd;
    char line_buff[512] = {0};
    sprintf(file_name, "/proc/%d/status", pid);

    fd = fopen(file_name, "r");
    if (NULL == fd) {
        rtn->ppid = 0;
        rtn->user = hidden_user;
    } else {
        char name[64];
        for (int i = 0; i < 6; i++) {
            fgets(line_buff, sizeof(line_buff), fd);
        }
        fgets(line_buff, sizeof(line_buff), fd);
        sscanf(line_buff, "%s %d", name, &rtn->ppid);
        fgets(line_buff, sizeof(line_buff), fd);
        fgets(line_buff, sizeof(line_buff), fd);
        sscanf(line_buff, "%s %d", name, &uid);
        fclose(fd);

        struct passwd* pwd;
        pwd = getpwuid(uid);
        rtn->user = pwd->pw_name;
    }
    return rtn;
}

// 打印队列信息
void PrintQueue(que* q) {
    node* n = q->head;
    int hide = 0;
    while (n) {
        char path[64];
        sprintf(path, "/proc/%d", n->pid);
        if (access(path, F_OK) == 0) {
            info* info = GetInfo(n->pid);
            if (hide == 0) {
                printf("隐藏进程如下：\n");
            }
            printf("pid: %d, 父进程pid: %d, 进程名: %s, 用户: %s\n", n->pid,
                   info->ppid, n->name, info->user);
            free(info);
            hide++;
        }
        n = n->next;
    }
    if (hide == 0) {
        printf("当前没有隐藏的进程！\n");
    }
}

void DeleteQueue(que* q, int clean_name) {
    while (q->head) {
        DeleteFirst(q, clean_name);
    }
    free(q);
}
