/**
* @file kfifomodule.c
* @brief module definitions for kfifo, hw4 question 4
* @author Andrew Kuklinski
* @date 03/06/2018
**/

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/kthread.h>
#include <linux/kfifo.h>
#include <linux/slab.h>

typedef struct infostruct{
  char * message;
}infostruct;

#define FIFOSIZE 32
static DECLARE_KFIFO(newfifo, infostruct,FIFOSIZE);

static int __init kfifomodule_init(void)
{
  infostruct * info;
  infostruct * newinfo;
  
  printk(KERN_ALERT "kfifomodule ENTER\n");

  info = kmalloc(sizeof(infostruct),GFP_KERNEL);
  newinfo = kmalloc(sizeof(infostruct),GFP_KERNEL);
  
  info->message = "testing 1 2 3\0";
  printk(KERN_INFO " original message: %s",info->message);

  //INIT_KFIFO(newfifo);

  kfifo_in(&newfifo, info,1);


  kfifo_out(&newfifo, newinfo,1);

  printk(KERN_INFO "READ MESSAGE: %s",(newinfo->message));

 
  return 0;
}

static void __exit kfifomodule_exit(void)
{
  kfifo_free(&newfifo);
  printk(KERN_ALERT "kfifomodule EXIT\n");
}

  
module_init(kfifomodule_init);
module_exit(kfifomodule_exit);


MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("writing to a thread via a kfifo");
MODULE_AUTHOR("Andrew Kuklinski");
