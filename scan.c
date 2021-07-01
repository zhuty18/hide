#include <linux/init.h>
#include <linux/init_task.h>
#include <linux/kernel.h>
#include <linux/list.h>
#include <linux/module.h>
#include <linux/sched.h>

//初始化函数
static int __init scan_init(void) {
    struct task_struct *task, *p;
    struct list_head* pos;
    int count = 0;

    printk("内核模块开始运行\n");

    task = &init_task;

    list_for_each(pos, &task->tasks) {
        p = list_entry(pos, struct task_struct, tasks);
        count++;
        printk("state%ld", p->state);  //打印进程状态
        printk("--->%d", p->pid);      //打印进程的ID号
        printk("--->%s\n", p->comm);   //打印进程的名字
    }

    printk("总的进程数为:%d\n", count);

    return 0;
}

//退出和清理函数
static void __exit scan_exit(void) {
    printk("xxx拜拜了, module被卸载了\n");
}

module_init(scan_init);
module_exit(scan_exit);
MODULE_LICENSE("GPL");