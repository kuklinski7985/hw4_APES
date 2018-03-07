#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/sched.h>
#include <linux/sched/task.h>
#include <linux/stat.h>
#include <linux/init.h>

static int input_pid = 1;

module_param(input_pid,int,0);
MODULE_PARM_DESC(input_pid, "input pid location to start process tree inspection");

static int __init processtreemodule_init(void)
{
  struct task_struct * task;
  struct task_struct * parent;
  struct list_head children;
  int nicevalue;
  
  printk(KERN_ALERT "Process Tree Evaluation Enter\n");
  
  for(task = current; task != &init_task; task = task->parent)
    {
      parent = task->parent;
      children = task->children;
      nicevalue = task_nice(task);
      printk(KERN_NOTICE "NAME: %s | PID: %d | NICE: %d | STATE: %ld",
	     task->comm, task->pid,nicevalue,task->state);
    }
  nicevalue = task_nice(task);
  printk(KERN_NOTICE "NAME: %s | PID: %d | NICE: %d | STATE: %ld",
                     task->comm, task->pid,nicevalue,task->state);
  
  return 0;
}

static void __exit processtreemodule_exit(void)
{
  printk(KERN_ALERT "Process Tree Evaluation EXIT\n");
}

module_init(processtreemodule_init);
module_exit(processtreemodule_exit);


MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Given a PID, show all parents until PID=1");
MODULE_AUTHOR("Andrew Kuklinski");


//task_struct located /home/andrewkuklinski/linux-4.14.13/include/linux/sched.h
