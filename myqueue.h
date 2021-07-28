#include <stdio.h>
#include <stdlib.h>

//定义节点
typedef struct node {
    int pid;
    char* name;
    struct node* last;
    struct node* next;
} node;

//定义队列（保存队首和队尾指针）
typedef struct queue_link {
    node* head;
    node* tail;
} que;

//初始化队列
que* InitQueue();

//入队
void InsertQueue(que* q, int pid, char* name);

//出队(删除队首元素)
void DeleteFirst(que* q, int clean_name);

//出队(删除队尾元素)
void DeleteLast(que* q, int clean_name);

// 打印队列信息
void PrintQueue(que* q);

void DeleteQueue(que* q, int clean_name);
