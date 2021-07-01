#include <linux/module.h>
#include <linux/list.h>
#include <linux/init.h>
#include <linux/sched.h>

MODULE_LICENSE("Dual BSD/GPL");

static int test_init(void)
{
        struct task_struct *task, *p;
        struct list_head *pos;
        int count=0;

        printk(KERN_ALERT "test module init\n");

        task=&init_task;
        list_for_each(pos, &task->tasks)
        {
                p=list_entry(pos, struct task_struct, tasks);
                count++;
                printk(KERN_ALERT "%s[%d]\n", p->comm, p->pid);
        }
        printk(KERN_ALERT "Total %d tasks\n", count);

        return 0;
}

static void test_exit(void)
{
        printk(KERN_ALERT "test module exit!\n");
}

module_init(test_init);
module_exit(test_exit);