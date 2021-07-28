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

// 打印队列信息
void PrintQueue(que* q) {
    if (q->head == NULL) {
        printf("当前没有隐藏的进程！\n");
        return;
    } else {
        printf("隐藏进程如下：\n");
        for (node* n = q->head; n; n = n->next) {
            printf("%d %s\n", n->pid, n->name);
        }
    }
}

void DeleteQueue(que* q, int clean_name) {
    while (q->head) {
        DeleteFirst(q, clean_name);
    }
    free(q);
}
