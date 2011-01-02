
#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/init.h>
#include<linux/fs.h>
#include<linux/slab.h>
#include<linux/types.h>
#include<asm/types.h>
#include<asm/uaccess.h>


MODULE_LICENSE("Dual BSD/GPL");



int memory_open(struct inode *inode,struct file*filp);

void  memory_close();

ssize_t memory_read(struct file * filep,char* buff,size_t count,loff_t* f_pos);

ssize_t memory_write(struct file*filp,char * buff,size_t count,loff_t * f_pos);

void memory_exit();

int memory_init();
/*The file ops structure definition for my driver Currently dealing only with open close read add write 
 */
struct file_operations memory_fops={
 read:memory_read,
 write:memory_write,
 open:memory_open,
 release:memory_close
};


int major_number=0;

char *  buffer;

int __init module_start_up(){
  int result=register_chrdev(major_number,"Mydevice",&memory_fops);
  if(result<0){
    printk(KERN_ALERT "Error registering device");
    return result;
}
  buffer=kmalloc(1,GFP_KERNEL);
  if(!buffer){
    result=-ENOMEM;
    memory_close();
    return result;
  }
  memset(buffer,0,1);
  return 0;
}
void memory_close(){
  unregister_chrdev(major_number,"Mydevice");
  if(buffer)
    kfree(buffer);
}
ssize_t memory_read(struct file * filep,char * buff,size_t count,loff_t * f_pos){
  if(!buff)
    return -ENOMEM;//don't ask me why i really need to think about this return value
  int k=copy_to_user(buffer,buff,1);
  return k;
}
ssize_t memory_write(struct file * filep,char * buff,size_t count,loff_t * f_pos){
  if(!buff)
    return -ENOMEM;// again food for thought
  int k=copy_from_user(buffer,buff,1);
  return k;
}

void __exit module_stop(){
  printk(KERN_INFO "Goof Bye world");
}

module_init(module_start_up);
module_exit(module_stop);

