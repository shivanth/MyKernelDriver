#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/init.h>
#include<linux/fs.h>
#include<linux/slab.h>
#include<linux/types.h>
#include<asm/types.h>


MODULE_LICENSE("Dual BSD/GPL");

int memory_open(struct indode*inode,struct file*filp);

int memory_close(struct inode *inode,struct file * filp);

ssize_t memory_read(struct file * filep,char* buff,size_t count,loff_t* f_pos);

ssize_t memory_write(struct file*filp,char * buff,size_t count,loff_t * f_pos);

void memory_exit();

int memory_init();

struct file_operations memory_fops={
 read:memory_read,
 write:memory_write,
 open:memory_open,
 release_memory_close};
}

int major_number=0;

char *  buffer;

int __init module_start_up(){
  result=register_chrdev(major_number,"Mydevice",&memory_fops);
  if(result<0){
    printk(KERN_ALERT "Error registering device");
    return result;
}
  buffer=kmalloc(1,GFP_KERNEL);
  if(!buffer){
    result=-ENOMEM;
    memory_exit();
    return result;
  }
  memset(buffer,0,1);
  
 

  return 0;
}
void memory_close(){
  unregister_chrev(major_number,"Mydevice");
  if(buffer)
    kfree(buffer);
}
void __exit module_stop(){
  printk(KERN_INFO "Goof Bye world");
}

module_init(module_start_up);
module_exit(module_stop);
