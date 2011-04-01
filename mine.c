#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/init.h>
#include<linux/fs.h>
#include<linux/slab.h>
#include<linux/types.h>
#include<asm/types.h>
#include<asm/uaccess.h>
#include<linux/cdev.h>

MODULE_LICENSE("Dual BSD/GPL");

#define MAJOR_NO 12



int memory_open(struct inode *inode,struct file*filp);
void  memory_close();
ssize_t memory_read(struct file * filep,char* buff,size_t count,loff_t* f_pos);
ssize_t memory_write(struct file*filp,char * buff,size_t count,loff_t * f_pos);

void memory_exit();

int memory_init();
/*The file ops structure definition for my driver Currently dealing only with open close read and write 
 */
struct file_operations memory_fops={
 read:memory_read,
 write:memory_write,
 open:memory_open,
 release:memory_close
};

struct mydev {
  dev_t devno;
  int maximum;
  char * buffer;
  struct cdev mycdev;
  size_t size;
}mydev;

int __init module_start_up(){
  mydev.devno=MKDEV(MAJOR_NO,12);
  cdev_init(&(mydev.mycdev),&memory_fops);
  int result=cdev_add(&(mydev.mycdev),mydev.devno,12);
  if(result<0){
    printk(KERN_ALERT "Error registering device");
    return result;
}
  mydev.maximum=1024*1024;
  mydev.size=0;
  mydev.buffer=kmalloc(mydev.maximum,GFP_KERNEL);
  if(!mydev.buffer){
    result=-ENOMEM;
    memory_close();
    return result;
  }
  memset(mydev.buffer,0,mydev.maximum);
  return 0;
}
void memory_close(){
  cdev_del(&(mydev.mycdev));
  if(mydev.buffer)
    kfree(mydev.buffer);
}
ssize_t memory_read(struct file * filep,char * buff,size_t count,loff_t * f_pos){
  int copysize;
  if(buff)
    return -ENOMEM;//don't ask me why i really need to think about this return value
  if(mydev.size>count )
     copysize=count;
  else 
    copysize=mydev.size;
  int k=copy_to_user(mydev.buffer,buff,copysize);
  return k;
}
ssize_t memory_write(struct file * filep,char * buff,size_t count,loff_t * f_pos){
  if(!buff)
    return -ENOMEM;// again food for thought
  if(count+mydev.size>mydev.maximum)
    return -ENOMEM;
  int k=copy_from_user(mydev.buffer,buff,count);
  return k;
}
int memory_open(struct inode * inode ,struct  file * filep){
  return 0;
}
void __exit module_stop(){
  printk(KERN_INFO "Goof Bye world");
  memory_close();
}

module_init(module_start_up);
module_exit(module_stop);

