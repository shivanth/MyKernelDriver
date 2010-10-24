#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/init.h>
#include<linux/fs.h>
#include<linux/slab.h>

int __init module_start_up(){
  printk(KERN_INFO "Hello world");
  return 0;
}

void __exit module_stop(){
  printk(KERN_INFO "Goof Bye world");
}

module_init(module_start_up);
module_exit(module_stop);
