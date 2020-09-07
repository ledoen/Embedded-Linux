//第一步，包含头文件
#include <linux/fs.h>
#include <linux/types.h>
#include <linux/cdev.h>
#include <linux/uacess.h>
#include <linux/module>
#include <linux/kernel>

//第二步，定义及声明相关设备数据
#define DEVICENAME "ledoen_watch"
unsigned int major=222;
unsigned int minor=0;
struct cdev *ledoen_dev;
dev_t dev;
static char ledoenbuf[1024]="read success";

//第三步，实现open/release函数
static int ledoen_open(struct inode *inodep, struct file *filep)
{
    printk("ledoen read success!\n");
    return 0;
}

int ledoen_release(struct inode *inodep, struct file *filep)
{
    return 0;
}

//第四步，实现read/write函数
static ssize_t ledoen_read(struct file *filep, char __user *buf, size_t count, loff_t *offset)
{
    if(copy_to_user(buf, ledoenbuf,1)){
        printk("ledoen copy_to_user fail!\n");
    }
    return 0;
}

ssize_t ledoen_write(struct file *filep, const char __user *buf, size_t count, loff_t *offset)
{
    printk("ledoen write\n");
    return 0;
}

//第五步，指定file_operations成员
static const struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = ledoen_open,
    .release = ledoen_release,
    .read = ledoen_read,
    .write = ledoen_write,
};

//第六步，实现设备初始化函数（获取设备号、注册设备）
static int __init ledoen_init(void)
{
    int a;
    dev = MKDEV(major, minor);
    a = register_chrdev_region(dev, 1, DEVICENAME);
    
    ledoen_dev = cdev_alloc();
    ledoen_dev->owner = THIS_MODULE;
    cdev_init(ledoen_dev, &fops);
    
    cdev_add(ledoen_dev, dev, 1);
    return 0;
}

//第七步，实现设备关闭函数
static void __exit ledoen_exit(void)
{
    cdev_del(ledoen_dev);
    unregister_chrdev_region(dev,1);
}

module_init(ledoen_init);
module_exit(ledoen_exit);
MODULE_LICESENSE("GPL");

//第八步，编译加载模块
    //Makefile 文件内容如下：
obj-m += ledoendev.o
KERDIR = /lib/modules/$(shell uname -r)/build
PWD=$(shell pwd)

modules:
    $(MAKE) -C $(KERDIR) M=$(PWD) modules
    
pc:
    gcc -o ledoendev ledoendev.c
arm:
    arm-linux-gcc -o ledoendev ledoendev.c
    
clean:
    rm -rf *.o *~core *.depend *.cmd *.ko *.mod.c *.tmp_versions
//第九步，在文件系统建立设备节点
sudo mknod /dev/ledoendev c 222 0
