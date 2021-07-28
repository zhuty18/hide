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
    printk("start scaning!!!\n");

    task = &init_task;

    list_for_each(pos, &task->tasks) {
        p = list_entry(pos, struct task_struct, tasks);
        count++;
        // printk("state%ld", p->state);     //打印进程状态
        printk("%d %s", p->pid, p->comm);  //打印进程的ID号和名字
        // printk("name--->%s\n", p->comm);  //打印进程的名字
    }
    // printk("count: %d\n", count);
    return 0;
}

//退出和清理函数
static void __exit scan_exit(void) {
    printk("scaning completed!!!\n");
}

module_init(scan_init);
module_exit(scan_exit);
MODULE_LICENSE("GPL");